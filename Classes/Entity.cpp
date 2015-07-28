//
// Entity.cpp
// DotWar
//
// Created by ryutamaki on 2015/07/15.
//
//

#include "Entity.h"

#include "ColorConstants.h"

USING_NS_CC;
using namespace cocostudio::timeline;

#pragma mark - Initilalizer

bool Entity::init()
{
    if (! Node::init()) {
        return false;
    }

    this->stateMachine = std::unique_ptr<EntityStateMachine>(new EntityStateMachine());
    this->stateMachine->setDelegate(this);
    this->synchronizer = std::unique_ptr<EntitySynchronizer>(new EntitySynchronizer());
    this->velocity = Vec2::ZERO;
    this->setupAttackMap();

    return true;
}

#pragma mark Accessor

int Entity::getHp()
{
    return this->hp;
}

void Entity::setHp(int hp)
{
    this->hp = MAX(hp, 0);

    this->setBodyColorByCurrentHp();

    if (this->hp <= 0) {
        this->deactivate();
    }
}

Vec2 Entity::getVelocity()
{
    return this->velocity;
}

std::string Entity::getCurrentAttackName()
{
    return this->currentAttackName;
}

std::vector<std::string> Entity::getAttackNameList()
{
    std::vector<std::string> attackNameList;

    for (std::map<std::string, AttackParams>::iterator it = this->attackMap.begin(); it != this->attackMap.end(); ++it) {
        attackNameList.push_back(it->first);
    }

    return attackNameList;
}

AttackParams Entity::getAttackParamsByName(std::string attackName)
{
    auto itr = this->attackMap.find(attackName);

    if (itr != this->attackMap.end()) {
        return this->attackMap[attackName];
    }

    CCASSERT(false, "Undefined attack is selected");
}

Rect Entity::getBodyRect()
{
    Size size = this->getBodySize();
    Vec2 position = this->getPosition();

    Rect returnRect = Rect(
            position.x - size.width * 0.5f,
            position.y - size.height * 0.5f,
            size.width, size.height
        );
    return returnRect;
}

Rect Entity::getBodyRectInWorldSpace()
{
    Rect returnRect;
    Vector<Node*> children = this->getChildren();

    for (int i = 0, last = (int)children.size(); i < last; ++i) {
        Sprite* sprite = dynamic_cast<Sprite*>(children.at(i));

        if (! sprite || ! sprite->isVisible()) {
            continue;
        }

        Size size = sprite->getContentSize();
        Vec2 bodyPosition = sprite->getPosition();
        Vec2 bodyPositionInWorld = this->convertToWorldSpace(bodyPosition);

        Rect spriteRect = Rect(
                bodyPositionInWorld.x - size.width * sprite->getAnchorPoint().x,
                bodyPositionInWorld.y - size.height * sprite->getAnchorPoint().y,
                size.width, size.height
            );

        if (returnRect.equals(Rect::ZERO)) {
            returnRect = spriteRect;
        } else {
            returnRect.merge(spriteRect);
        }
    }

    return returnRect;
}

std::string Entity::getIdentifier()
{
    return this->identifier;
}

void Entity::setIdentifier(std::string identifier)
{
    this->identifier = identifier;
}

Size Entity::getBodySize()
{
    Sprite* body = this->getChildByName<Sprite*>("Body");
    return body->getContentSize();
}

bool Entity::getIsDead()
{
    return this->isDead;
}

JSONPacker::EntityState Entity::currentEntityState()
{
    JSONPacker::EntityState entityState;
    entityState.identifier = this->identifier;
    entityState.hp = this->getHp();
    entityState.position = this->getPosition();
    entityState.moveState = this->stateMachine->getMoveState();
    entityState.attackState = this->stateMachine->getAttackState();

    entityState.damage.identifier = "";
    entityState.damage.volume = 0;

    return entityState;
}

#pragma mark Game logic

void Entity::activate()
{
    this->isDead = false;
    this->scheduleUpdate();
    this->runAction(this->timeline);
}

void Entity::deactivate()
{
    this->isDead = true;
    this->stateMachine->move(EntityMoveState::NONE);
    this->stateMachine->cancelAttack();
    this->unscheduleUpdate();
    this->stopAllActions();
}

void Entity::attack(const std::string attackName)
{
    if (! this->stateMachine->canAttack()) {
        return;
    }

    if (this->getIsDead()) {
        return;
    }

    this->timeline->play(attackName, false);
    this->timeline->setFrameEventCallFunc([this, attackName](Frame* frame) {
        EventFrame* frameEvent = dynamic_cast<EventFrame*>(frame);
        auto eventName = frameEvent->getEvent();

        // log("---- %s ----", eventName.c_str());
        if (eventName == "Ready") {
            this->stateMachine->readyToAttack();
            this->currentAttackName = attackName;
        } else if (eventName == "Attack") {
            this->stateMachine->startToAttack();
        } else if (eventName == "Cooldown") {
            this->stateMachine->coolDownAttaking();
        } else if (eventName == "Finish") {
            this->stateMachine->finishAttaking();
            this->currentAttackName = "";
        }
    });

    // override point
}

void Entity::receiveDamage(const int damage)
{
    if (this->getIsDead()) {
        return;
    }
    // std::string animationName = "Damaged";
    // this->timeline->play(animationName, false);

    this->setHp(this->getHp() - damage);

    ParticleSystemQuad* particle = ParticleSystemQuad::create("Damage.plist");
    particle->setScaleX(-1.0f);
    particle->setPosition(Vec2(0.0f, 0.0f));
    this->addChild(particle);

    // override point
}

#pragma mark EntityStateMachineDelegate

void Entity::willStateChange(EntityMoveState moveState, EntityAttackState attackState)
{
}

void Entity::didStateChanged(EntityMoveState newMoveState, EntityAttackState newAttackState)
{
    if (this->getIsDead()) {
        return;
    }

    JSONPacker::EntityState currentEntityState = this->currentEntityState();
    this->synchronizer->sendData(currentEntityState);
}

#pragma mark - Protected method

#pragma mark View lifecycle

void Entity::onEnter()
{
    Node::onEnter();

    Vector<Node*> children = this->getChildren();

    for (int i = 0, last = (int)children.size(); i < last; ++i) {
        Sprite* sprite = dynamic_cast<Sprite*>(children.at(i));

        if (! sprite) {
            continue;
        }
        sprite->setCascadeColorEnabled(false);
    }
}

void Entity::onExit()
{
    Node::onExit();

    this->deactivate();
}

void Entity::setupAttackMap()
{
    // override point
}

#pragma mark Game logic

void Entity::setBodyColorByCurrentHp()
{
    Vector<Node*> children = this->getChildren();

    for (int i = 0, last = (int)children.size(); i < last; ++i) {
        Sprite* sprite = dynamic_cast<Sprite*>(children.at(i));

        if (! sprite) {
            continue;
        }

        if (this->initialHp * 0.75 < this->hp) {
            sprite->setColor(Color3B(DARK_BLUE));
        } else if (this->initialHp * 0.5 < this->hp && this->hp <= this->initialHp * 0.75) {
            sprite->setColor(Color3B(LIGHT_BLUE));
        } else if (this->initialHp * 0.25 < this->hp && this->hp <= this->initialHp * 0.5) {
            sprite->setColor(Color3B(LIGHT_RED));
        } else if (this->hp <= this->initialHp * 0.25f) {
            sprite->setColor(Color3B(DARK_RED));
        }
    }
}

void Entity::update(float dt)
{
    Node::update(dt);

    EntityMoveState currentMoveState = this->stateMachine->getMoveState();
    Vec2 direction = EntityHelper::directionFromMoveState(currentMoveState);
    this->velocity = this->velocityFactor * direction * dt;
    this->setRotation(EntityHelper::rotationFromMoveState(currentMoveState, this->getRotation()));
}

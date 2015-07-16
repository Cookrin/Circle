//
//  Entity.cpp
//  DotWar
//
//  Created by ryutamaki on 2015/07/15.
//
//

#include "Entity.h"

USING_NS_CC;

#pragma mark - Initilalizer

bool Entity::init()
{
    if (!Node::init())
    {
        return false;
    }

    this->hp = 100;
    this->moveState = CharacterMoveState::NONE;
    this->velocity = Vec2(0.0f, 0.0f);

    return true;
}

#pragma mark Accessor

int Entity::getHp()
{
    return this->hp;
}

void Entity::setMoveState(CharacterMoveState moveState)
{
    this->moveState = moveState;
}

#pragma mark Game logic

void Entity::attack()
{
    // override point
}

void Entity::receiveDamage(int damage, Vec2 knockback)
{
    this->runAction(Sequence::create(ScaleTo::create(0.1f, 0.9f), ScaleTo::create(0.1f, 1.0f), NULL));
    this->hp -= damage;
    this->setPosition(this->getPosition() + knockback);

    // override point
}

bool Entity::isDead()
{
    if (this->getHp() < 0)
    {
        return true;
    }
    return false;
}

#pragma mark - Protected method

#pragma mark View lifecycle

void Entity::onEnter()
{
    Node::onEnter();
}

void Entity::onExit()
{
    Node::onExit();
}

#pragma mark Utility methods

Vec2 Entity::directionFromMoveState(CharacterMoveState moveState)
{
    switch (this->moveState) {
        case CharacterMoveState::UP_RIGHT:
            return Vec2(1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f));
        case CharacterMoveState::UP:
            return Vec2(0.0f, 1.0f);
        case CharacterMoveState::UP_LEFT:
            return Vec2(- 1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f));
        case CharacterMoveState::LEFT:
            return Vec2(- 1.0f, 0.0f);
        case CharacterMoveState::DOWN_LEFT:
            return Vec2(- 1.0f / sqrt(2.0f), - 1.0f / sqrt(2.0f));
        case CharacterMoveState::DOWN:
            return Vec2(0.0f, - 1.0f);
        case CharacterMoveState::DOWN_RIGHT:
            return Vec2(1.0f / sqrt(2.0f), - 1.0f / sqrt(2.0f));
        case CharacterMoveState::RIGHT:
            return Vec2(1.0f, 0.0f);
        case CharacterMoveState::NONE:
            return Vec2(0.0f, 0.0f);
        default:
            CCASSERT(false, "Undefined moveState are passed");
            break;
    }
}

CharacterMoveState Entity::moveStateFromStartPositionAndEndPosition(cocos2d::Vec2 startPosition, cocos2d::Vec2 endPosition)
{
    Vec2 direction = endPosition - startPosition;
    float yDifference = endPosition.y - startPosition.y;
    float radiun = Vec2::angle(direction, Vec2(1.0f, 0.0f));
    float degree = CC_RADIANS_TO_DEGREES(radiun);
    degree = yDifference > 0 ? degree : 360.0f - degree;

    if (22.5 <= degree && degree < 67.5)
        return CharacterMoveState::UP_RIGHT;
    else if (67.5 <= degree && degree < 112.5)
        return CharacterMoveState::UP;
    else if (112.5 <= degree && degree < 157.5)
        return CharacterMoveState::UP_LEFT;
    else if (157.5 <= degree && degree < 202.5)
        return CharacterMoveState::LEFT;
    else if (202.5 <= degree && degree < 247.5)
        return CharacterMoveState::DOWN_LEFT;
    else if (247.5 <= degree && degree < 292.5)
        return CharacterMoveState::DOWN;
    else if (292.5 <= degree && degree < 337.5)
        return CharacterMoveState::DOWN_RIGHT;
    else
        return CharacterMoveState::RIGHT;
}
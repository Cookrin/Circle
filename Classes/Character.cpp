//
//  Character.cpp
//  DotWar
//
//  Created by ryutamaki on 2015/07/15.
//
//

#include "Character.h"

#pragma mark - Public methods

#pragma mark Initializer

bool Character::init() {
    if (!Entity::init()) {
        return false;
    }

    // load the character animation timeline
    this->timeline = CSLoader::createTimeline("Character.csb");
    // retain the character animation timeline so it doesn't get deallocated
    this->timeline->retain();

    return true;
}

#pragma mark Accessor

void Character::setMoveStateByStartPositionAndCurrentPosition(cocos2d::Vec2 startPosition, cocos2d::Vec2 currentPosition)
{
    EntityMoveState moveState = this->moveStateFromStartPositionAndEndPosition(startPosition, currentPosition);
    if (moveState == this->stateMachine->getMoveState())
        return;

    this->stateMachine->startMoving(moveState);
}

#pragma mark Game logic

#pragma mark - Prvate methods

#pragma mark View lifecycle

void Character::onEnter()
{
    Entity::onEnter();
}

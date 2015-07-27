//
// StageButton.cpp
// DotWar
//
// Created by ryutamaki on 2015/07/26.
//
//

#include "StageButton.h"

#pragma mark - Public methods

#pragma mark Initializer

bool StageButton::init()
{
    if (! Node::init()) {
        return false;
    }

    // load the character animation timeline
    this->timeline = CSLoader::createTimeline("StageButton.csb");
    // retain the character animation timeline so it doesn't get deallocated
    this->timeline->retain();

    return true;
}

#pragma mark Accessor

void StageButton::setEntityType(EntityType entityType)
{
    this->entityType = entityType;
}

EntityType StageButton::getEntityType()
{
    return this->entityType;
}

#pragma mark - Private methods

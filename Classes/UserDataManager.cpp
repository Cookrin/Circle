//
// UserDataManager.cpp
// DotWar
//
// Created by ryutamaki on 2015/07/28.
//
//

#include "UserDataManager.h"

USING_NS_CC;

#pragma mark - Public methods

static UserDataManager* sharedUserDataManager;
UserDataManager* UserDataManager::getInstance()
{
    if (! sharedUserDataManager) {
        sharedUserDataManager = new UserDataManager();
    }

    return sharedUserDataManager;
}

#pragma mark Constructor and Destructor

UserDataManager::UserDataManager()
{
}

UserDataManager::~UserDataManager()
{
}

void UserDataManager::load()
{
    std::string path = this->getFilePath();

    if (! FileUtils::getInstance()->isFileExist(path)) {
        this->userData = ValueMap();
    } else {
        this->userData = FileUtils::getInstance()->getValueMapFromFile(path);
    }
}

bool UserDataManager::save()
{
    std::string path = this->getFilePath();

    if (FileUtils::getInstance()->writeToFile(this->userData, path)) {
        CCLOG("Save Success");
        return true;
    } else {
        CCLOG("Save Failed");
        return false;
    }
}

#pragma mark Accessor

#pragma mark Coin

void UserDataManager::setCoinCount(int coinCount)
{
    this->userData[USER_DATA_COIN_COUNT] = coinCount;

    this->save();
}

int UserDataManager::getCoinCount()
{
    this->load();

    if (this->userData.find(USER_DATA_COIN_COUNT) == this->userData.end()) {
        int initialCoinCount = 0;
        return initialCoinCount;
    }

    int coinCount = this->userData[USER_DATA_COIN_COUNT].asInt();
    return coinCount;
}

#pragma mark High score

void UserDataManager::setHighScoreByEntityType(int highScore, EntityType entityType)
{
    int currentHighScore = this->getHighScoreByEntityType(entityType);

    if (highScore < currentHighScore) {
        return;
    }

    ValueMap highScoreMap;

    if (this->userData.find(USER_DATA_HIGHSCORE) == this->userData.end()) {
        highScoreMap = ValueMap();
    } else {
        highScoreMap = this->userData[USER_DATA_HIGHSCORE].asValueMap();
    }

    std::string entityTypeString = this->getEntityTypeStringByEntityType(entityType);
    highScoreMap[entityTypeString] = highScore;
    this->userData[USER_DATA_HIGHSCORE] = highScoreMap;

    this->save();
}

int UserDataManager::getHighScoreByEntityType(EntityType entityType)
{
    this->load();

    if (this->userData.find(USER_DATA_HIGHSCORE) == this->userData.end()) {
        int initialHighScore = 0;
        return initialHighScore;
    }

    std::string entityTypeString = this->getEntityTypeStringByEntityType(entityType);
    ValueMap highScoreMap = this->userData[USER_DATA_HIGHSCORE].asValueMap();
    int highScore = highScoreMap[entityTypeString].asInt();

    return highScore;
}

#pragma mark Entity parameter

void UserDataManager::setEntityLevelParameter(EntityType entityType, EntityLevelParameter entityLevelParameter)
{
    ValueMap entityListParameterMap;

    if (this->userData.find(USER_DATA_ENTITY_PARAMETER) == this->userData.end()) {
        entityListParameterMap = ValueMap();
    } else {
        entityListParameterMap = this->userData[USER_DATA_ENTITY_PARAMETER].asValueMap();
    }

    std::string entityTypeString = this->getEntityTypeStringByEntityType(entityType);
    ValueMap entityParameterMap;

    if (entityListParameterMap.find(entityTypeString) == entityListParameterMap.end()) {
        entityParameterMap = ValueMap();
    } else {
        entityParameterMap = entityListParameterMap[entityTypeString].asValueMap();
    }

    entityParameterMap["levelHp"] = entityLevelParameter.levelHp;
    entityParameterMap["levelAttack"] = entityLevelParameter.levelAttack;
    entityParameterMap["levelSpeed"] = entityLevelParameter.levelSpeed;

    entityListParameterMap[entityTypeString] = entityParameterMap;
    this->userData[USER_DATA_ENTITY_PARAMETER] = entityListParameterMap;

    this->save();
}

EntityLevelParameter UserDataManager::getEntityLevelParameter(EntityType entityType)
{
    this->load();

    // Return initial params if there are no data for entity parameters
    if (this->userData.find(USER_DATA_ENTITY_PARAMETER) == this->userData.end()) {
        EntityLevelParameter levelParameter = ENTITY_INITIAL_LEVEL_PARAMETER.at(entityType);
        return levelParameter;
    }

    std::string entityTypeString = std::to_string((int)entityType);
    ValueMap entityListParameterMap = this->userData[USER_DATA_ENTITY_PARAMETER].asValueMap();

    // Return initial params if there are no data for a selected entity
    if (entityListParameterMap.find(entityTypeString) == this->userData.end()) {
        EntityLevelParameter levelParameter = ENTITY_INITIAL_LEVEL_PARAMETER.at(entityType);
        return levelParameter;
    }

    ValueMap data = entityListParameterMap[entityTypeString].asValueMap();
    return EntityLevelParameter {
               data["levelHp"].asInt(),
               data["levelAttack"].asInt(),
               data["levelSpeed"].asInt(),
    };
}

#pragma mark - Private methods

#pragma mark Utility

std::string UserDataManager::getFilePath()
{
    return FileUtils::getInstance()->getWritablePath() + "user_data";
}

std::string UserDataManager::getEntityTypeStringByEntityType(EntityType entityType)
{
    std::string entityTypeString = std::to_string((int)entityType);
    return entityTypeString;
}
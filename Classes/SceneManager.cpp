//
// SceneManager.cpp
// Tetrominos
//
// Created by ryutamaki on 2015/07/01.
//
//

#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"

using namespace cocos2d;

static SceneManager* sharedSceneManager;

SceneManager* SceneManager::getInstance()
{
    if (! sharedSceneManager) {
        sharedSceneManager = new (std::nothrow)SceneManager();
    }

    return sharedSceneManager;
}

SceneManager::SceneManager()
{
    this->networkingWrapper = std::unique_ptr<NetworkingWrapper>(new NetworkingWrapper());
    this->networkingWrapper->setDelegate(this);

    this->gameScene = nullptr;
}

SceneManager::~SceneManager()
{
}

#pragma mark - Public methods

void SceneManager::enterGameScene(bool networked)
{
    Scene* scene = Scene::create();
    this->gameScene = GameScene::create();
    this->gameScene->setNetworkedSession(networked);

    scene->addChild(this->gameScene);
    Director::getInstance()->pushScene(scene);
}

void SceneManager::exitGameScene()
{
    if (gameScene) {
        auto menuScene = MenuScene::createScene();
        Director::getInstance()->replaceScene(menuScene);
        this->gameScene = nullptr;
        this->networkingWrapper->disconnect();
    }
}

void SceneManager::showPeerList()
{
    this->networkingWrapper->showPeerList();
}

void SceneManager::receiveMultiplayerInvitations()
{
    this->networkingWrapper->startAdvertisingAvailability();
}

void SceneManager::sendData(const void* data, unsigned long length)
{
    this->networkingWrapper->sendData(data, length);
}

std::vector<std::string> SceneManager::getPeerNameList()
{
    return this->networkingWrapper->getPeerList();
}

std::string SceneManager::getMyName()
{
    return this->networkingWrapper->getMyPeerId();
}

std::string SceneManager::getHostUserName()
{
    std::vector<std::string> peerList = this->getPeerNameList();
    peerList.push_back(this->getMyName());
    std::sort(peerList.begin(), peerList.end());
    return peerList[0];
}

std::string SceneManager::getUniqueIdentifier()
{
    std::string uniqueIdentifier = this->networkingWrapper->getUniqueIdentifier();
    return uniqueIdentifier;
}

bool SceneManager::isHost()
{
    if (this->getMyName().compare(this->getHostUserName()) == 0) {
        return true;
    }
    return false;
}

#pragma mark - Private methods

void SceneManager::receivedData(const void* data, unsigned long length)
{
    if (this->gameScene) {
        this->gameScene->receivedData(data, length);
    }
}

void SceneManager::stateChanged(ConnectionState state)
{
    switch (state) {
        case ConnectionState::CONNECTING:
            CCLOG("Connecting...");
            break;

        case ConnectionState::NOT_CONNECTED:
            CCLOG("Not connected...");
            break;

        case ConnectionState::CONNECTED:
            CCLOG("Connected...");

            if (! this->gameScene) {
                this->networkingWrapper->stopAdvertisingAvailability();
                this->enterGameScene(true);
            }
            break;

        default:
            break;
    }
}

#include "AppDelegate.h"
#include "MenuScene.h"
#include "UserDataManager.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

// if you want a different context,just modify the value of glContextAttrs
// it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributions,now can only set six attributions:
    // red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if (! glview) {
        glview = GLViewImpl::createWithRect("DotWar", Rect(0, 0, 960, 640));
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(960, 640, ResolutionPolicy::FIXED_HEIGHT);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // resolution settings
    std::vector<std::string> searchResolutionsOrder(1);

    cocos2d::Size targetSize = glview->getFrameSize();

    if (targetSize.height < 481.0f) {
        searchResolutionsOrder[0] = "resources-1x";
    } else if (targetSize.height < 1137.0f) {
        searchResolutionsOrder[0] = "resources-2x";
    } else if (targetSize.height < 2047.0f) {
        searchResolutionsOrder[0] = "resources-3x";
    } else {
        searchResolutionsOrder[0] = "resources-4x";
    }

    FileUtils::getInstance()->setSearchResolutionsOrder(searchResolutionsOrder);

    // create a scene. it's an autorelease object
    auto scene = MenuScene::createScene();

    // run
    director->runWithScene(scene);

    // load user data
    UserDataManager::getInstance()->load();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

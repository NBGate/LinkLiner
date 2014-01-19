//
//  LinkLinkerAppDelegate.cpp
//  LinkLinker
//
//  Created by jun on 13-8-11.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "Config.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <View/AudioManager.h>
#include <View/MenuScene.h>
#include <Model/Record.h>

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCSize designSize = CCSizeMake(DESIGNED_WIDTH, DESIGNED_HEIGHT);
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);

    std::vector<std::string> searchPaths;
    searchPaths.push_back("fonts");
    searchPaths.push_back("music");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
    cacher->addSpriteFramesWithFile("link.plist");

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    Record::instance()->load();
    // create a scene. it's an autorelease object
    //CCScene *pScene = CCScene::create();
    //GameLayer *pLayer = GameLayer::create();
    //pLayer->init();
    //pScene->addChild(pLayer);
    CCScene *pScene = Menu::scene();
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

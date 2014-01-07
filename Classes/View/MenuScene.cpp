
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Menu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Menu *layer = Menu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

Menu::~Menu()
{

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

Menu::Menu()
:isEnter(false)
{
}

// on "init" you need to initialize your instance
bool Menu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName("bg.png");
    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    pSprite->setAnchorPoint(CCPoint(0.5, 0.5));
    pSprite->setRotation(90);
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    CCLabelTTF* label = CCLabelTTF::create("连连看", "Marker Felt", 48);
	label->setPosition(  ccp(size.width /2 , size.height/2 + 80) );
    this->addChild(label, 1);
    CCActionInterval* blinkAtion = CCBlink::create(1, 1);
	CCSprite* button = CCSprite::create("play.png");
    button->setPosition(  ccp(size.width/2, size.height/2) );
    button->runAction(blinkAtion);
    this->addChild(button, 2);
	this->setTouchEnabled(true);
    
    return true;
}

bool Menu::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //CCActionInterval* actionTo = CCScaleTo::create(2.0f, 0.5f);
    //CCActionInterval* actionBy = CCScaleTo::create(2.0f, 1.0f, 10.0f);
    //CCActionInterval* actionBy2 = CCScaleTo::create(2.0f, 5.0f, 1.0f);
    //this->getChildByTag(2)->runAction(CCSequence::create(actionBy, actionBy->reverse(), NULL));
    
    if(!this->isEnter){
        this->scheduleOnce(schedule_selector(Menu::playGame), 0);
        this->isEnter = true;
    }
    return true;
}
void Menu::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void Menu::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void Menu::playGame(CCObject* pSender)
{
    CCScene *pScene = CCScene::create();
    GameLayer *pLayer = GameLayer::create();
    pScene->addChild(pLayer);
    CCTransitionFade* transitionScene = CCTransitionFade::create(1.0, pScene,ccWHITE);
    CCDirector::sharedDirector()->replaceScene(transitionScene);
}

void Menu::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}





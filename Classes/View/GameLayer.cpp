#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include <View/GridNode.h>
#include <Controller/GameLogic.h>
#include <Model/MapManager.h>

#define OFFSET_X							210
#define OFFSET_Y							380
#define SIZE_W								38
#define SIZE_H								38
#define TOTAL_ROWS							7
#define TOTAL_COlS                          10
#define TOTAL_IMG							16

GameLayer::GameLayer() {
    m_logic = new GameLogic();
}

GameLayer::~GameLayer() {
    if (m_logic)
        delete m_logic;
}

bool GameLayer::init() {
    bool bRet = false;
    do {
        CCLayer::init();
        initView();
        initSound();
        m_gridNodeArray = CCNode::create();
        this->addChild(m_gridNodeArray);
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);
        scheduleUpdate();
        bRet = true;
    } while (0);

    return bRet;
}

void GameLayer::initView() {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* bgSprite = CCSprite::create("bg.png");
    bgSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(bgSprite);
}


void GameLayer::initSound() {
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("back2.mp3", true);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    CCArray* grids = m_gridNodeArray->getChildren();
    CCObject* obj = NULL;
    CCARRAY_FOREACH(grids, obj) {
        GridNode* grid = (GridNode*)obj;
        if (grid->boundingBox().containsPoint(location)) {
            CCLog("ccTouchBegan::gtag %d\n", grid->getTag());
            m_logic->touchGrid(grid->getTag());
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("choose.wav");
            CCSprite* curSprite = (CCSprite *)grid->getChildByTag(ImageTag);
            CCLog("ccTouchBegn::curSprite::%d", curSprite->getTag());
            curSprite->setScale(0.6f);
            break;
        }
    }
    return true;
}

void GameLayer::registerWithTouchDispatcher() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void GameLayer::update(float delta) {
    m_logic->update(delta);
    updateGridNode();
}

void GameLayer::updateGridNode() {
    m_gridNodeArray->removeAllChildren();
    MapManager* map = m_logic->currentMap();
    if (map == NULL)
        return;
    const MapManager::GridArray& grids = map->getGrids();
    int cnt = grids.size();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    for (int i = 0; i < cnt; i++) {
        Grid::Status status = grids[i]->status;
        if (status == Grid::Normal || status == Grid::Select) {
            GridNode* node = GridNode::create();
            node->setTag(grids[i]->id);
            CCSprite *sprite = CCSprite::create(this->imageFilename(grids[i]->imageId)->getCString());
            sprite->setScale(0.5f);
            if (status == Grid::Select) {
                sprite->setScale(0.6f);
            }
            sprite->setPosition(CCPoint(0,0));
            node->setContentSize(sprite->boundingBox().size);
            int x = origin.x  + OFFSET_X + grids[i]->col*SIZE_W;
            int y = origin.y  + OFFSET_Y - (grids[i]->row+1)*SIZE_H;
            sprite->setAnchorPoint(ccp(0, 0));
            node->setAnchorPoint(ccp(0, 0));
            node->setPosition(x, y);
            node->addChild(sprite, 0, ImageTag);
            m_gridNodeArray->addChild(node);
            
        } else if (status == Grid::Select) {
            
        }
    }
}

cocos2d::CCString* GameLayer::imageFilename(int imageId) {
    if (imageId >= 1 && imageId <= TOTAL_IMG){
        return cocos2d::CCString::createWithFormat("%d.png",imageId);
    }else{
        return NULL;
    }
}










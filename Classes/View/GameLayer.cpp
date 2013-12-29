#include "GameLayer.h"
#include <View/GridNode.h>
#include <Controller/GameLogic.h>
#include <Model/MapManager.h>

#define OFFSET_X							20
#define OFFSET_Y							400
#define SIZE_W								40
#define SIZE_H								40
#define TOTAL_X								7
#define TOTAL_Y                             10
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
        CCLog("GameLayer::init");
        m_gridNodeArray = CCNode::create();
        this->addChild(m_gridNodeArray);
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);
        scheduleUpdate();
        bRet = true;
    } while (0);

    return bRet;
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    //CCLog("ccTouchBegan");
    CCArray* grids = m_gridNodeArray->getChildren();
    CCObject* obj = NULL;
    CCARRAY_FOREACH(grids, obj) {
        GridNode* grid = (GridNode*)obj;
//        CCLog("b %0.2f %0.2f",grid->boundingBox().size.width, grid->boundingBox().size.height);
//        CCLog("origin %0.2f %0.2f",grid->boundingBox().origin.x, grid->boundingBox().origin.y);
//        CCLog("location %0.2f %0.2f", location.x, location.y);
        if (grid->boundingBox().containsPoint(location)) {
            //CCLog("ccTouchBegan");//
            CCLog("gtag %d", grid->getTag());
            m_logic->touchGrid(grid->getTag());
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
    //CCLog("GameLayer::updateGridNode");
    m_gridNodeArray->removeAllChildren();
    MapManager* map = m_logic->currentMap();
    if (map == NULL)
        return;
    const MapManager::GridArray& grids = map->getGrids();
    int cnt = grids.size();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    for (int i = 0; i < cnt; i++) {
        Grid::Status status = grids[i]->status;
        if (status == Grid::Normal) {
            GridNode* node = GridNode::create();
            node->setTag(grids[i]->id);
            CCSprite *sprite = CCSprite::create(this->imageFilename(grids[i]->imageId)->getCString());
            sprite->setScale(0.5f);
            sprite->setPosition(CCPoint(0,0));
            node->setContentSize(sprite->boundingBox().size);
            //node->setAnchorPoint(CCPoint(0.5,0.5));
            
            int x = origin.x  + OFFSET_X + grids[i]->col*SIZE_W;
            int y = origin.y  + OFFSET_Y - (grids[i]->row+1)*SIZE_H;
            CCLog("x: %d, y: %d", x, y);
            node->setPosition(x, y);
            
            
            /*ccp(origin.x + OFFSET_X + (SIZE_W / 2) + SIZE_W * grids[i]->col,                                                     origin.y +   480 - (SIZE_H / 2) - SIZE_H *grids[i]->row - OFFSET_Y));*/
                              
            node->addChild(sprite);
            
            m_gridNodeArray->addChild(node);
        }
    }
}

cocos2d::CCString* GameLayer::imageFilename(int imageId) {
    return cocos2d::CCString::createWithFormat("%d.png", imageId);
}










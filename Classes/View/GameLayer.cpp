#include "GameLayer.h"
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
        CCLog("b %0.2f %0.2f",grid->boundingBox().size.width, grid->boundingBox().size.height);
        CCLog("origin %0.2f %0.2f",grid->boundingBox().origin.x, grid->boundingBox().origin.y);
        CCLog("location %0.2f %0.2f", location.x, location.y);
        //CCPoint touchPoint = pTouch->getLocationInView();
        //touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
        //touchPoint = convertTouchToNodeSpace(pTouch);
        //CCLog("touchPoint %0.2f %0.2f", touchPoint.x, touchPoint.y);
        if (grid->boundingBox().containsPoint(location)) {
            
        //if (CCRect::containsPoint()) {
            //CCLog("ccTouchBegan");//
            CCLog("ccTouchBegan::gtag %d\n", grid->getTag());
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
            //CCLog("%.2f,%.2f", origin.x, origin.y);
            int x = origin.x  + OFFSET_X + grids[i]->col*SIZE_W;
            int y = origin.y  + OFFSET_Y - (grids[i]->row+1)*SIZE_H;
            //CCLog("id=%d:x=%d,y=%d;row=%d,col=%d,bx=%.2f,by=%.2f", i+1, x, y,grids[i]->row,grids[i]->col,node->boundingBox().size.width, node->boundingBox().size.height);
            //CCLog("b %0.2f %0.2f",node->boundingBox().size.width, node->boundingBox().size.height);
            //CCLog("origin %0.2f %0.2f",node->boundingBox().origin.x, node->boundingBox().origin.y);
            //CCNode::m_bIgnoreAnchorPointForPosition=false;
            node->setAnchorPoint(ccp(0, 0));
            sprite->setAnchorPoint(ccp(0,0));
            node->setPosition(x, y);
            node->addChild(sprite);
            
            m_gridNodeArray->addChild(node);
        }
    }
}

cocos2d::CCString* GameLayer::imageFilename(int imageId) {
    return cocos2d::CCString::createWithFormat("%d.png", imageId);
}










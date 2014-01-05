#include <Config.h>
#include "GameLayer.h"
#include "GridNode.h"
#include <SimpleAudioEngine.h>
#include <Controller/GameLogic.h>
#include <Model/MapManager.h>

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
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);
        scheduleUpdate();
        bRet = true;
    } while (0);

    return bRet;
}

void GameLayer::initView() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCSprite* bgSprite = CCSprite::createWithSpriteFrameName("bg.png");
    bgSprite->setPosition(CCPoint(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    bgSprite->setAnchorPoint(CCPoint(0.5, 0.5));
    bgSprite->setRotation(90);
    this->addChild(bgSprite);

    for (int i = 0; i < TOTAL_ROW; i++) {
        for (int j = 0; j < TOTAL_COl; j++) {
            GridNode* node = GridNode::create();
            node->setAnchorPoint(CCPoint(0.5, 0.5));
            float x = origin.x + GRID_ORIGIN_X + j * GRID_SPACING;
            float y = origin.y + GRID_ORIGIN_Y + i * GRID_SPACING;
            node->setPosition(x, y);
            this->addChild(node);
            m_gridNodeArray[i * TOTAL_COl + j] = node;
        }
    }
}


void GameLayer::initSound() {
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("back2.mp3", true);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    for (int i = 0; i < TOTAL_ROW * TOTAL_COl; i++) {
        GridNode* grid = m_gridNodeArray[i];
        if (grid->isVisible() && grid->boundingBox().containsPoint(location)) {
            m_logic->touchGrid(grid->getTag());
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("choose.wav");
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
    MapManager* map = m_logic->currentMap();
    if (map == NULL)
        return;

    const MapManager::GridArray& grids = map->getGrids();
    MapManager::GridArray::const_iterator pos = grids.begin();
    for (; pos != grids.end(); ++pos) {
        Grid* grid = pos->second;
        //int status = grid->status;
        int index = grid->row * TOTAL_COl + grid->col;
        m_gridNodeArray[index]->updateGrid(pos->second);
    }
}


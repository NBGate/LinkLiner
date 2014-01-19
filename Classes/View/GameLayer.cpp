#include <Config.h>
#include "GameLayer.h"
#include "GridNode.h"
#include "Effect.h"
#include "ShowNumberNode.h"
#include <Controller/GameLogic.h>
#include <Model/MapManager.h>
#include <Utils/AudioManager.h>

GameLayer::GameLayer() {
    m_logic = new GameLogic(this);
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
    this->addChild(bgSprite, -1000);

    for (int i = 0; i < TOTAL_ROW; i++) {
        for (int j = 0; j < TOTAL_COl; j++) {
            GridNode* node = GridNode::create();
            node->setAnchorPoint(CCPoint(0.5, 0.5));
            float x = origin.x + GRID_ORIGIN_X + j * GRID_SPACING;
            float y = origin.y + GRID_ORIGIN_Y + i * GRID_SPACING;
            node->setPosition(x, y);
            this->addChild(node, 100);
            m_gridNodeArray[i * TOTAL_COl + j] = node;
        }
    }
    
    m_score = ShowNumberNode::createShowNumberNode("numbers.png", INT_MAX, 22, 30);
    m_score->f_ShowNumber(0);
    m_score->setPosition(800, 610);
    this->addChild(m_score, 100);
    
    m_time = ShowNumberNode::createShowNumberNode("numbers.png", INT_MAX, 22, 30);
    m_time->f_ShowNumber(60);
    m_time->setPosition(100, 610);
    this->addChild(m_time, 100);
    
}


void GameLayer::initSound() {
    AudioManager::instance()->setBackgroundMusicVolume(0.3f);
    AudioManager::instance()->playBackgroundMusic("back2.mp3");
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    for (int i = 0; i < TOTAL_ROW * TOTAL_COl; i++) {
        GridNode* grid = m_gridNodeArray[i];
        if (grid->isVisible() && grid->boundingBox().containsPoint(location)) {
            m_logic->touchGrid(grid->getTag());
            AudioManager::instance()->playEffect("choose.wav");
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
    int score = map->getScore();
    //CCLog("score: %d", score);
    m_score->f_ShowNumber(score);
    int time = map->getTime();
    m_time->f_ShowNumber(time);
}

void GameLayer::linkEffect(const MatchGrid* match) {
    MapManager* map = m_logic->currentMap();
    if (map == NULL || match == NULL)
        return;

    int end = match->path.size();
    std::vector<CCPoint> posArray;
    for (int i = 0; i < end; i++) {
        Grid* grid = map->getGrid(match->path[i]);
        posArray.push_back(m_gridNodeArray[grid->row * TOTAL_COl+grid->col]->getPosition());
    }
    Effect::instance()->linkEffect(this, posArray, callfunc_selector(GameLayer::linkEffectCallback));
}

void GameLayer::linkEffectCallback() {
    m_logic->onLinkEffectEnd();
}

void GameLayer::explodeEffect(const MatchGrid* match) {
    MapManager* map = m_logic->currentMap();
    if (map == NULL || match == NULL)
        return;

    Grid* grid = map->getGrid(match->head);
    GridNode* node = m_gridNodeArray[grid->row * TOTAL_COl+grid->col];
    Effect::instance()->explodeEffect(this, node->getPosition());
    grid = map->getGrid(match->tail);
    node = m_gridNodeArray[grid->row * TOTAL_COl+grid->col];
    Effect::instance()->explodeEffect(this, node->getPosition());
}


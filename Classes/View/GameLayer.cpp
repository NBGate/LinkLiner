#include <Config.h>
#include "GameLayer.h"
#include "GridNode.h"
#include "Effect.h"
#include "ShowNumberNode.h"
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
    
    m_showNumberNode = ShowNumberNode::createShowNumberNode("numbers.png", 100000, 22, 30);
    m_showNumberNode->f_ShowNumber(1000);
    m_showNumberNode->setPosition(800, 610);
    this->addChild(m_showNumberNode, 100);
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
    linkEffect();
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
    CCLog("score: %d", score);
    m_showNumberNode->f_ShowNumber(score);
}

void GameLayer::linkEffectCallback() {
}

void GameLayer::linkEffect() {
    MapManager* map = m_logic->currentMap();
    if (map == NULL)
        return;

    MapManager::Match m = map->getMatch();
    while (m.path.size() > 0) {
        int end = m.path.size();
        std::vector<CCPoint> posArray;
        for (int i = 0; i < end; i++) {
            Grid* grid = map->getGrid(m.path[i]);
            posArray.push_back(m_gridNodeArray[grid->row * TOTAL_COl+grid->col]->getPosition());
        }
        Effect::instance()->linkEffect(this, posArray);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("link.mp3", false);

        m = map->getMatch();
    }
}


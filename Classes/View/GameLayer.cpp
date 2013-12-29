#include "GameLayer.h"
#include <View/GridNode.h>
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

        m_gridNodeArray = CCNode::create();
        this->addChild(m_gridNodeArray);
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);
        bRet = true;
    } while (0);

    return bRet;
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();

    CCArray* grids = m_gridNodeArray->getChildren();
    CCObject* obj = NULL;
    CCARRAY_FOREACH(grids, obj) {
        GridNode* grid = (GridNode*)obj;
        if (grid->boundingBox().containsPoint(location)) {
            m_logic->touchGrid(grid->getTag());
            break;
        }
    }
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
    for (int i = 0; i < cnt; i++) {
        GridNode* node = GridNode::create();
        node->setTag(grids[i]->id);
        m_gridNodeArray->addChild(node);
    }
}

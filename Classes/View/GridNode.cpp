#include <Config.h>
#include "GridNode.h"
#include <Model/MapManager.h>

#define SELECT_ACTION_TAG   0

GridNode::GridNode() {
}

GridNode::~GridNode() {
}

bool GridNode::init() {
    bool bRet = false;
    do {
        CCNode::init();
        this->setContentSize(CCSize(GRID_WIDTH, GRID_HEIGHT));

        m_gridSprite = CCSprite::create();
        m_gridSprite->setAnchorPoint(CCPoint(0, 0));
        m_gridSprite->setPosition(CCPoint(0, 0));
        this->addChild(m_gridSprite);

        m_selectAnimatoin = CCAnimation::create();
        CCString* frame;
        for (int i = 0; i < 5; i++) {
            frame = CCString::createWithFormat("select_frame_%d.png", i);
            m_selectAnimatoin->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frame->getCString()));
        }
        m_selectAnimatoin->setDelayPerUnit(0.5f / 5.0f);
        m_selectAnimatoin->setRestoreOriginalFrame(true);
        m_selectAnimatoin->setLoops(true);
        m_selectAnimatoin->retain();

        bRet = true;
    } while (0);

    return bRet;
}

void GridNode::updateGrid(Grid* grid) {
    m_grid = grid;
    if (m_grid == NULL) {
        this->setVisible(false);
        return;
    }
    this->setVisible(true);
    setTag(m_grid->id);
    m_gridSprite->initWithSpriteFrameName(CCString::createWithFormat("%d.png",m_grid->id)->getCString());
    if (m_grid->status & Grid::Select) {
        showSelectAnimation();
    }
    else {
        hideSelectAnimation();
    }
}

void GridNode::showSelectAnimation() {
    CCAnimate *action = CCAnimate::create(m_selectAnimatoin);
    action->setTag(SELECT_ACTION_TAG);
    m_gridSprite->runAction(action);
}

void GridNode::hideSelectAnimation() {
    m_gridSprite->stopActionByTag(SELECT_ACTION_TAG);
}

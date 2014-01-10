#include <Config.h>
#include "GridNode.h"
#include <Model/MapManager.h>

#define SELECT_ACTION_TAG   10

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
        m_gridSprite->setAnchorPoint(CCPoint(0.5, 0.5));
        m_gridSprite->setPosition(CCPoint(GRID_WIDTH / 2, GRID_HEIGHT / 2));
        this->addChild(m_gridSprite, 10);

        m_animSprite = CCSprite::create();
        m_animSprite->setAnchorPoint(CCPoint(0.5, 0.5));
        m_animSprite->setPosition(CCPoint(GRID_WIDTH / 2, GRID_HEIGHT / 2));
        m_animSprite->setVisible(false);
        this->addChild(m_animSprite, 0);

        CCAnimation* selectAnimatoin = CCAnimation::create();
        CCString* frame;
        for (int i = 0; i < 5; i++) {
            frame = CCString::createWithFormat("select_frame_%d.png", i);
            selectAnimatoin->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frame->getCString()));
        }
        selectAnimatoin->setDelayPerUnit(0.5f / 5.0f);
        selectAnimatoin->setLoops(true);
        m_selectAnimate = CCAnimate::create(selectAnimatoin);
        m_selectAnimate->setTag(SELECT_ACTION_TAG);
        m_selectAnimate->retain();

        bRet = true;
    } while (0);

    return bRet;
}

void GridNode::updateGrid(Grid* grid) {
    if (grid == NULL) {
        m_grid = NULL;
        this->setVisible(false);
        return;
    }

    if (m_grid != grid) {
        m_grid = grid;
        setTag(m_grid->id);
        if (m_grid->visible())
            m_gridSprite->initWithSpriteFrameName(CCString::createWithFormat("%d.png", m_grid->imageId)->getCString());
    }

    this->setVisible(m_grid->visible());
    if ((m_grid->status & Grid::Select)) {
        showSelectAnimation();
    }
    else {
        hideSelectAnimation();
    }
}

void GridNode::showSelectAnimation() {
    if (m_animSprite->isVisible())
        return;

    CCLog("showSelectAnimation");
    m_animSprite->setVisible(true);
    m_animSprite->runAction(m_selectAnimate);
    // TODO 选择使用统一的背景颜色，加圈为提示配对
    //ccColor3B selectColor;
    //selectColor.r = 255, selectColor.g = 0, selectColor.b = 0;
    //m_gridSprite->setColor(selectColor);
}

void GridNode::hideSelectAnimation() {
    if (m_animSprite->isVisible() == false)
        return;

    m_animSprite->stopActionByTag(SELECT_ACTION_TAG);
    m_animSprite->setVisible(false);
    
}

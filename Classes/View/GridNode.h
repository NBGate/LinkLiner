#ifndef __GridNode_H_
#define __GridNode_H_

#include "cocos2d.h"

USING_NS_CC;

struct Grid;

class GridNode : public CCNode {
public:
    GridNode();
    ~GridNode();
    virtual bool init();
    CREATE_FUNC(GridNode);

    void updateGrid(Grid* grid);

private:
    void showSelectAnimation();
    void hideSelectAnimation();
private:
    Grid*           m_grid;
    CCSprite*       m_gridSprite;
    CCSprite*       m_animSprite;
    CCAnimate*      m_selectAnimate;
};

#endif // __GridNode_H_

#ifndef __GridNode_H_
#define __GridNode_H_

#include "cocos2d.h"

USING_NS_CC;

class GameLogic;
struct Grid;

class GridNode : public CCNode {
public:
    GridNode();
    ~GridNode();
    virtual bool init();
    CREATE_FUNC(GridNode);
};

#endif // __GridNode_H_

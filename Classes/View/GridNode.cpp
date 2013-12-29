#include "GridNode.h"
#include "GridNode.h"

GridNode::GridNode() {
}

GridNode::~GridNode() {
}

bool GridNode::init() {
    bool bRet = false;
    do {
        CCNode::init();

        bRet = true;
    } while (0);

    return bRet;
}


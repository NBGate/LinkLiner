#ifndef __GameLayer_H_
#define __GameLayer_H_

#include "cocos2d.h"

USING_NS_CC;

class GameLogic;

class GameLayer : public CCLayer {
public:
    enum NodeTags {
        ImageTag = 0x00000000,
    };
public:
    GameLayer();
    ~GameLayer();
    virtual bool init();
    CREATE_FUNC(GameLayer);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();

    virtual void update(float delta);

    void timesUp() { }
    
private:
    void updateGridNode();
    cocos2d::CCString* imageFilename(int);
    void initSound();
    void initView();
    
private:
    CCNode* m_gridNodeArray;
    GameLogic* m_logic;
};

#endif // __GameLayer_H_

#ifndef __GameLayer_H_
#define __GameLayer_H_

#include <Config.h>
#include <cocos2d.h>

USING_NS_CC;

class GameLogic;
class GridNode;
class ShowNumberNode;

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
    void linkEffectCallback();

private:
    void updateGridNode();
    void initSound();
    void initView();

    void linkEffect();
private:
    GridNode*   m_gridNodeArray[TOTAL_ROW*TOTAL_COl];
    GameLogic*  m_logic;
    ShowNumberNode* m_score;
    ShowNumberNode* m_time;
};

#endif // __GameLayer_H_

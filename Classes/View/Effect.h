#ifndef __Effect_H_
#define __Effect_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class Effect {
public:
    static Effect* instance();
    ~Effect() { }

    void linkEffect(CCNode* target, const vector<CCPoint>& posArray, SEL_CallFunc callback = NULL);
    void explodeEffect(CCNode* target, const CCPoint& position, SEL_CallFunc callback = NULL);

private:
    CCNode* linkEffectNode(CCPoint p1, CCPoint p2);
private:
    Effect() { };
    Effect(const Effect&) { };

    static Effect* m_instance;
};

#endif // __Effect_H_

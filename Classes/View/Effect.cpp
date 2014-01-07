#include <Config.h>
#include "Effect.h"

#define LINK_EFFECT_LENGTH  100

Effect* Effect::m_instance = NULL;

float abs(float a) {
    return a > 0 ? a : -a;
}

Effect* Effect::instance() {
    if (m_instance == NULL) {
        m_instance = new Effect();
    }

    return m_instance;
}

void Effect::linkEffect(CCNode* target, const vector<CCPoint>& posArray) {
    int size = posArray.size();
    if (target == NULL || size <= 1)
        return;

    //CCNode* top = CCNode::create();
    for (int i = 0; i < size - 1; i++) {
        target->addChild(linkEffectNode(posArray[i], posArray[i+1]));
    }
}

CCNode* Effect::linkEffectNode(CCPoint p1, CCPoint p2) {
    CCSprite* sprite = CCSprite::create();
    sprite->setAnchorPoint(CCPoint(0.5, 0.5));
    sprite->setPosition(CCPoint(p1.x + (p2.x - p1.x) / 2, p1.y + (p2.y - p1.y) / 2));
    if (p1.x == p2.x) {
        sprite->setRotation(90);
        sprite->setScaleX(abs((p2.y - p1.y) * 1.0f / LINK_EFFECT_LENGTH));
    }
    else {
        sprite->setScaleX(abs((p2.x - p1.x) * 1.0f / LINK_EFFECT_LENGTH));
    }

    CCAnimation* animation = CCAnimation::create();
    CCString* frameName;
    for (int i = 0; i < 2; i++) {
        frameName = CCString::createWithFormat("linkLine_%d.png", i+1);
        animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()
                ->spriteFrameByName(frameName->getCString()));
    }
    animation->setDelayPerUnit(0.4f / 2.0f);
    CCAnimate* animate = CCAnimate::create(animation);
    CCAction* removeAction = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(animate, removeAction, NULL));
    return sprite;
}

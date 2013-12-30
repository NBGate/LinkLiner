#include "Utils.h"
#include <cocos2d.h>

string writablePath() {
    return cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
}


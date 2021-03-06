#ifndef __LinkLinker__NumberSprite__
#define __LinkLinker__NumberSprite__

#include "cocos2d.h"
USING_NS_CC;
class  NumberSprite : public CCSprite
{
public:
    /*
     创建一个数字精灵
     参数:
            pTexture: 包含所有数字图片的图片
            width; 单个数字显示的图片宽度
            lenght; 数字显示高度
    */
    static NumberSprite *CreateNumberSprite(CCTexture2D *pTexture, float width, float lenght);

    /*
     直接显示数字
     参数：
        num 是显示的数字图片序号
        isanimation：是否显示动画
     */
    void f_ShowNumber(int num);
    
    void myInit(float width, float lenght);

    
private:
    /** 保存输入的图片宽高，用于显示滚动数字或是直接显示数字*/
    //保存显示图片宽度
    float m_width;
    //保存图片高度
    float m_lenght;


   };

#endif 

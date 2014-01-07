#ifndef __LinkLinker__ShowNumberNode__
#define __LinkLinker__ShowNumberNode__

#include "cocos2d.h"
USING_NS_CC;

#define SCROLLTIME 0.2f
class ShowNumberNode : public CCNode
{
public:
    /*
        创建一个showNumberNode节点
    参数：
        path: 文件名
        numberMAX：数字显示的最大值
        showwidth: 每个数字图片的宽度
        showheight:每个数字图片的高度

    */
    static ShowNumberNode *createShowNumberNode(const char *path, int numberMAX, float showwidth, float showheight);

    /*同上*/
    bool Init(const char *path, int numberMAX, float showwidth, float showheight);

    
    // 修改显示数字,直接显示数字无滚动
    void f_ShowNumber(int number);
    


private: 
    //添加数字精灵到节点,并进行排列
    void f_addSprite();

    //判断输入数字的位数，比如：11就是2位，333就是3位
    int f_NumberLength(int number);

    //返回num从右边开始第k位数字的值,比如：654321,第3就是3，也就是百位上的数字
    int f_GetNumber(int num, int k);

private:
   
    //用于保存数字最大值
    int m_numberMax;

    //计算数字最大位数
    int m_numberLength;

    //图片宽度
    float m_showWidth;

    //图片的高度
    float m_showHeight;

    CCSpriteBatchNode* m_batchNode ;

};


#endif

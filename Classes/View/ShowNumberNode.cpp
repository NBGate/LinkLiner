#include "ShowNumberNode.h"
#include "NumberSprite.h"


ShowNumberNode *ShowNumberNode::createShowNumberNode(const char *path, int numberMAX, float showwidth, float showheight)
{
    ShowNumberNode *pobSprite = new ShowNumberNode();

    if (pobSprite )
    {

        pobSprite->Init(path , numberMAX, showwidth, showheight);
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;

}
bool ShowNumberNode::Init(const char *path, int numberMAX, float showwidth, float showheight)
{
    //初始化参数
    this->m_numberMax = numberMAX;
    this->m_showWidth = showwidth;
    this->m_showHeight = showheight;

    //计算数字长度
    m_numberLength = f_NumberLength(numberMAX);

    //
    m_batchNode = CCSpriteBatchNode::create(path);

    //将numbersprite 添加进节点
    f_addSprite();

    return true;

}

/*添加数字精灵到节点,并进行排列*/
void ShowNumberNode::f_addSprite() {

    for (int i = 0; i < m_numberLength; i++) {
        NumberSprite *nsp  = NumberSprite::CreateNumberSprite(m_batchNode->getTexture(), m_showWidth, m_showHeight);

        nsp->setPosition(ccp(- m_showHeight * i, 0));

        m_batchNode->addChild(nsp, 0, i);

    }

    this->addChild(m_batchNode);
}

// 修改显示数字
void ShowNumberNode::f_ShowNumber(int number)
{
   
    if (number < 0 || number > m_numberMax)
        return;

    int numlength = f_NumberLength(number);


    for (int i = 0; i < m_numberLength; i++)
    {
        int num = f_GetNumber(number, i + 1);

        NumberSprite *nsp =  (NumberSprite * ) m_batchNode->getChildByTag(i);

        //将超出数字的隐藏，比如：最大值是999，现在需要显示33，那就是隐藏百位上的0
        if (i >= numlength)
        {
            //TODO :
            nsp->setVisible(false);
           //break;
        }
        else
        {
            nsp->setVisible(true);
            nsp->f_ShowNumber(num);
        }
    }
}

//判断输入数字的位数，比如：11就是2位，333就是3位
int ShowNumberNode::f_NumberLength(int number)
{
    if(number == 0 )
        return  1;
    int flag = 0;
    while (number)
    {
        number /= 10;
        flag++;
    }
    return flag;

}

//返回num从右边开始第k位数字的值
int ShowNumberNode::f_GetNumber(int num, int k)
{
    while (k > 1)
    {
        num = num / 10;
        k--;
    }
    return num % 10 ;
}
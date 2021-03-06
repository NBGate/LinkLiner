#include "NumberSprite.h"

void NumberSprite::myInit(float width, float lenght)
{
    m_width     =   width;
    m_lenght    =   lenght;
}

NumberSprite *NumberSprite::CreateNumberSprite(CCTexture2D *pTexture, float width, float lenght)
{
    NumberSprite *pobSprite = new NumberSprite();

    if (pobSprite && pobSprite->initWithTexture(pTexture)  )
    {
        pobSprite->setVisible(false);
        pobSprite->myInit(width , lenght);
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void NumberSprite::f_ShowNumber(int num)
{
   
    this->setVisible(true)  ;
    this->setTextureRect(CCRectMake(m_width * num, 0, m_width, m_lenght));
}

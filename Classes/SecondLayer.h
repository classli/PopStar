#ifndef _SecondLayer_H_
#define _SecondLayer_H_
#include "cocos2d.h"
USING_NS_CC;

class SecondLayer:public Sprite
{
public:
	CREATE_FUNC(SecondLayer);
	virtual bool init();
	Vec2 pp;
};

#endif 
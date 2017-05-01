#include "SecondLayer.h"

bool SecondLayer::init(){
	if (!Sprite::init())
	{
		return false;
	}
	pp=Vec2(0,0);
	return true;
}

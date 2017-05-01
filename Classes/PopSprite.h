#ifndef _POPSPRITE_H_
#define _POPSPRITE_H_

#include "cocos2d.h"
USING_NS_CC;

class PopSprite:public Sprite
{
public:
	static PopSprite* CreatePopSprite(Layer *he,std::string name,int wight,int height,float popX,float popY);
	virtual bool init();
	CREATE_FUNC(PopSprite);

	int getNum();
	void setNum(int num);
	//获取坐标

	//获取位置
	int getX();
	int getY();
	void setX(int x);
	void setY(int y);
	void Remove();
	void onEnter();
	void CreatParticle();
	void setStartNum(int num);

private:
	//颜色
	int number;//标志数字
	int x;//坐标
	int y;
	Vec2 PointVec2;
	int StartNum;
	Layer *helloLayer;
	void enemyInit(Layer *he,std::string name,int wight,int height,float popX,float popY);
};



#endif
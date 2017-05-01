#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PopSprite.h"
#include "SecondLayer.h"
#define  ROW 12//矩阵大小
#define  COL 10

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void autoCreatePopstar(Size size);//自动创建星星

	void addPop(std::string str,int log,int j,int i,Size size,int num);//添加pop

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	PopSprite* getPointSprite(Size size,Point point);//获取触摸点位置


private:
	PopSprite *pop[ROW][COL];//用于存放每个POP对象

	SpriteBatchNode *batchNode0;
	SpriteBatchNode *batchNode1;
	SpriteBatchNode *batchNode2;
	SpriteBatchNode *batchNode3;
	SpriteBatchNode *batchNode4;
	SpriteBatchNode *batchNode5;

	void checkPop(PopSprite* chosePop);//查找相关的星星
	void myThread();//消除的线程
	void MoveUD(int *a);//星星的上下移动
	void MoveLR();//星星的左右移动
	bool isGameOver();
	void JumpThread();

	Array *array;//存放找到的星星
	int lon;//精灵的大小
	int threadId;//消除星星的ID
	std::mutex mutex;//同步硕
	int arrayCount;//找到的星星个数
	bool GameOver;
	Size visibleSize;
};

#endif // __HELLOWORLD_SCENE_H__

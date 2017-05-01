#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PopSprite.h"
#include "SecondLayer.h"
#define  ROW 12//�����С
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

	void autoCreatePopstar(Size size);//�Զ���������

	void addPop(std::string str,int log,int j,int i,Size size,int num);//���pop

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	PopSprite* getPointSprite(Size size,Point point);//��ȡ������λ��


private:
	PopSprite *pop[ROW][COL];//���ڴ��ÿ��POP����

	SpriteBatchNode *batchNode0;
	SpriteBatchNode *batchNode1;
	SpriteBatchNode *batchNode2;
	SpriteBatchNode *batchNode3;
	SpriteBatchNode *batchNode4;
	SpriteBatchNode *batchNode5;

	void checkPop(PopSprite* chosePop);//������ص�����
	void myThread();//�������߳�
	void MoveUD(int *a);//���ǵ������ƶ�
	void MoveLR();//���ǵ������ƶ�
	bool isGameOver();
	void JumpThread();

	Array *array;//����ҵ�������
	int lon;//����Ĵ�С
	int threadId;//�������ǵ�ID
	std::mutex mutex;//ͬ��˶
	int arrayCount;//�ҵ������Ǹ���
	bool GameOver;
	Size visibleSize;
};

#endif // __HELLOWORLD_SCENE_H__

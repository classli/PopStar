#include "HelloWorldScene.h"
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	#include <unistd.h>
#endif
USING_NS_CC;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    // add layer as a child to scene

    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();

	//������Ⱦ����
	batchNode0=SpriteBatchNode::create("blue.png");
	batchNode1=SpriteBatchNode::create("green.png");
	batchNode2=SpriteBatchNode::create("orange.png");
	batchNode3=SpriteBatchNode::create("purple.png");
	batchNode4=SpriteBatchNode::create("red.png");
	batchNode5=SpriteBatchNode::create("particle_texture.png");

	//������Ϸ����
	auto BJ=Sprite::create("bg_mainscene.jpg");
	BJ->setPosition(visibleSize/2);
 	addChild(BJ);

	//�Զ�����POP
 	autoCreatePopstar(visibleSize);

	addChild(batchNode0);
	addChild(batchNode1);
	addChild(batchNode2);
	addChild(batchNode3);
	addChild(batchNode4);
	addChild(batchNode5);

	//���������߳�ID��ÿ������ID��ͬ��ʹ��ֿ�����
	threadId=-1;
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	auto listenerKey=EventListenerKeyboard::create();
	listenerKey->onKeyReleased=[this](EventKeyboard::KeyCode code, Event* e){
		menuCloseCallback();
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKey,this);
	GameOver=false;

    return true;
}

//�Զ���������
void HelloWorld::autoCreatePopstar(Size size)
{
	lon=(size.width-20)/10;//������
	srand(time(NULL));//�������
	for (int i=0;i<ROW;i++)
	{
		for (int j=0;j<COL;j++)
		{
			int num=rand()%5;
			switch (num)
			{
			case 0:
				addPop("blue.png",lon,j,i,size,num);
				break;
			case 1:
				addPop("green.png",lon,j,i,size,num);
				break;
			case 2:
				addPop("orange.png",lon,j,i,size,num);
				break;
			case 3:
				addPop("purple.png",lon,j,i,size,num);
				break;
			case 4:
				addPop("red.png",lon,j,i,size,num);
				break;
			default:
				break;
			}
		}
	}
	
}
//������Ӧ
bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	array=Array::create();//��ʼ�����飬����ҵ�������
	
	Point pp=touch->getLocation();
	Size size=Director::getInstance()->getVisibleSize();

	PopSprite* chosePop=getPointSprite(size,pp);//�Ӵ�������ѡȡ������

	if(chosePop!=NULL&&chosePop->getNum()>-1){
		checkPop(chosePop);//�ҳ�����������ǣ���������
	}

	if(array->count()>1){
		arrayCount=array->count();//���������Ǹ�����������������߳�
		PopSprite* pp_array;

 		if (threadId==-5)threadId=-1;//�߳�ID�����ڲ�ͬ���߳����ֲ�ͬ������
 		else threadId--;//Ҫ���������������ֱ�־��С��0

		for (auto i=0;i<array->count();i++)
		{
			pp_array=(PopSprite*)array->getObjectAtIndex(i);
			int tempNum=pp_array->getNum();
			pp_array->setNum(threadId);//�ı�ѡ�����ǵı�־��������������ָ������Ƿ���Ҫ����
		}
		std::thread tB(&HelloWorld::JumpThread,this);
		tB.join();
		std::thread tA(&HelloWorld::myThread,this);//����һ�����߳�
		tA.detach();//���ȴ��߳̽���
	}
	if (GameOver==true)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1,HelloWorld::createScene()));
	}
	return true; 
}

//������ص�����
void HelloWorld::checkPop(PopSprite* chosePop)
{
	int Direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//��������
	int NewFlag=0;//����������־��һ�����µ�Ԫ�ؼ���ͼ��������������˳�
	for (int i=0;i<4;i++)
	{
		int x=chosePop->getX()+Direction[i][0];
		int y=chosePop->getY()+Direction[i][1];//��ѰĿ�������
		if (x>=0&&x<COL&&y>=0&&y<ROW)
		{
			PopSprite *poptem=pop[y][x];
			if(poptem!=NULL&&poptem->getNum()==chosePop->getNum()&&!array->containsObject(poptem))//�ж�����
			{
				array->addObject(poptem);
				NewFlag=1;
			}
		}
	}
	if(NewFlag)//�ٴε���
	{
		PopSprite *pop_Recursive;
		for(int i=0;i<array->count();i++){
			pop_Recursive=(PopSprite *)array->getObjectAtIndex(i);
			if (pop_Recursive!=NULL&&pop_Recursive->getNum()>-1)
			{
				checkPop(pop_Recursive);
			}
		}	
		return;
	}
	else
	{
		return;
	}
}


//��ȡ������λ��
PopSprite* HelloWorld::getPointSprite(Size size,Point point)
{
	
	int x=(point.x-32+20)/lon;
	int y=(point.y-size.height/7+20)/lon;
	if (x>=0&&x<COL&&y>=0&&y<ROW)
	{
		return pop[y][x];
	}
	else
	{
		return NULL;
	}
	
}

//���pop
void HelloWorld::addPop(std::string str,int log,int j,int i,Size size,int num)
{
	PopSprite *po=PopSprite::CreatePopSprite(this,str,log,log,log*j+32,log*i+size.height/7);//pop�Ĵ�������

	po->setX(j);//����ÿ�����ǵ�����
	po->setY(i);
	po->setNum(num);//�������ֱ�־
	po->setStartNum(num);
	switch (num)//��������ӵ���ͬ��BatchNode��
	{
	case 0:
		batchNode0->addChild(po);
		break;
	case 1:
		batchNode1->addChild(po);
		break;
	case 2:
		batchNode2->addChild(po);
		break;
	case 3:
		batchNode3->addChild(po);
		break;
	case 4:
	 	batchNode4->addChild(po);
		break;
	default:
		break;
	}
	pop[i][j]=po;
}

//�������߳�
void HelloWorld::myThread()
{   
	int _threadId=threadId;//����һЩȫ�ֱ�����ʹ��ֲ��������������̸߳���
	int _arrCount=arrayCount;

	int num[COL]={0};//�����ж�ÿ���Ƿ�������������
	//����ÿ������
	for (int i=0;i<ROW;i++)
	{
		if(_arrCount<0)break;//��������
		for (int j=0;j<COL;j++)
		{
			if(pop[i][j]!=NULL&&pop[i][j]->getNum()==_threadId)
			{       
				    pop[i][j]->CreatParticle();
					mutex.lock();//����
					pop[i][j]->Remove();//ɾ��
					pop[i][j]=NULL;
					mutex.unlock();
					#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
						usleep(50000);//Linuxϵͳ���ôκ���
					#else
						Sleep(50);
					#endif
					_arrCount--;
					num[j]++;
			}
		}
	}
	//����
	MoveUD(num);
	//����
	MoveLR();
	//�ж��Ƿ����
	GameOver=isGameOver();
}

//���ǵ������ƶ�
void HelloWorld::MoveUD(int *a){
	JumpBy *jum;

	for (int i=0;i<COL;i++)
	{
		if(a[i]==0)continue;
		int moveCount=0;//�ƶ��ĸ�������POP�յĸ����������ѱ������Ķ���
		int numCol=0;
		for (int j=0;j<ROW;j++)
		{
			if (pop[j][i]==NULL)
			{
				moveCount++;
			}
			else if(moveCount!=0)
			{
				jum=JumpBy::create(0.2f,Vec2(0,-lon*moveCount),30,1);//����
				pop[j][i]->runAction(jum);
				mutex.lock();
				pop[j-moveCount][i]=pop[j][i];//��������
				pop[j-moveCount][i]->setX(i);//�ı��¶��������
				pop[j-moveCount][i]->setY(j-moveCount);
				pop[j][i]=NULL;
				mutex.unlock();
			}
		}
	}
}

//������
void HelloWorld::MoveLR()
{
	JumpBy *jum;
	int numCol=0;
	for (int i=0;i<COL;i++)
	{
		int num_null=0;
		for (int k=0;k<ROW;k++)
		{
			if(pop[k][i]==NULL)num_null++;
		}
		if(num_null==ROW){
			numCol++;
		}
		else if(numCol!=0)
		{
			for (int j=0;j<ROW;j++)
			{	
				jum=JumpBy::create(0.2f,Vec2(-lon*numCol,0),0,1);//����		
				if(pop[j][i]==NULL)continue;
				pop[j][i]->runAction(jum);
				mutex.lock();
				pop[j][i-numCol]=pop[j][i];//��������
				pop[j][i-numCol]->setX(i-numCol);//�ı��¶��������
				pop[j][i-numCol]->setY(j);
				pop[j][i]=NULL;
				mutex.unlock();
			}
		}
	}
}


bool HelloWorld::isGameOver(){
	bool GameOver=true;
	for (int i=0;i<ROW;i++)
	{
		for (int j=0;j<COL;j++)
		{
			if(pop[i][j]!=NULL){
				int Direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//��������
				for (int k=0;k<4;k++)
				{
					int x=pop[i][j]->getX()+Direction[k][0];
					int y=pop[i][j]->getY()+Direction[k][1];//��ѰĿ�������
					if (x>=0&&x<COL&&y>=0&&y<ROW)
					{
						PopSprite *poptem=pop[y][x];
						if(poptem!=NULL&&poptem->getNum()==pop[i][j]->getNum())//�ж�����
						{
							GameOver=false;
							break;
						}
					}
				}
			}
			if(GameOver==false)break;
		}
		if(GameOver==false)break;
	}
	return GameOver;
}

void HelloWorld::JumpThread(){

	PopSprite* pp_array;
	JumpBy *jum;
	for (auto i=0;i<array->count();i++)
	{
		jum=JumpBy::create(0.2f,Vec2(0,0),8,1);//����
		pp_array=(PopSprite*)array->getObjectAtIndex(i);
		pp_array->runAction(jum);
	}
}

void HelloWorld::menuCloseCallback()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

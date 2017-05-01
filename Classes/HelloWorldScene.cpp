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

	//减少渲染次数
	batchNode0=SpriteBatchNode::create("blue.png");
	batchNode1=SpriteBatchNode::create("green.png");
	batchNode2=SpriteBatchNode::create("orange.png");
	batchNode3=SpriteBatchNode::create("purple.png");
	batchNode4=SpriteBatchNode::create("red.png");
	batchNode5=SpriteBatchNode::create("particle_texture.png");

	//创建游戏背景
	auto BJ=Sprite::create("bg_mainscene.jpg");
	BJ->setPosition(visibleSize/2);
 	addChild(BJ);

	//自动创建POP
 	autoCreatePopstar(visibleSize);

	addChild(batchNode0);
	addChild(batchNode1);
	addChild(batchNode2);
	addChild(batchNode3);
	addChild(batchNode4);
	addChild(batchNode5);

	//消除星星线程ID，每次消除ID不同，使其分开消除
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

//自动创建星星
void HelloWorld::autoCreatePopstar(Size size)
{
	lon=(size.width-20)/10;//精灵宽度
	srand(time(NULL));//随机种子
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
//触摸响应
bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	array=Array::create();//初始化数组，存放找到的星星
	
	Point pp=touch->getLocation();
	Size size=Director::getInstance()->getVisibleSize();

	PopSprite* chosePop=getPointSprite(size,pp);//从触摸点获得选取的星星

	if(chosePop!=NULL&&chosePop->getNum()>-1){
		checkPop(chosePop);//找出相关联的星星，迭代处理
	}

	if(array->count()>1){
		arrayCount=array->count();//关联的星星个数，用于提早结束线程
		PopSprite* pp_array;

 		if (threadId==-5)threadId=-1;//线程ID，用于不同的线程区分不同的星星
 		else threadId--;//要想消除的星星数字标志都小于0

		for (auto i=0;i<array->count();i++)
		{
			pp_array=(PopSprite*)array->getObjectAtIndex(i);
			int tempNum=pp_array->getNum();
			pp_array->setNum(threadId);//改变选出星星的标志数，用于相乘区分该星星是否需要消除
		}
		std::thread tB(&HelloWorld::JumpThread,this);
		tB.join();
		std::thread tA(&HelloWorld::myThread,this);//创建一个新线程
		tA.detach();//不等待线程结束
	}
	if (GameOver==true)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1,HelloWorld::createScene()));
	}
	return true; 
}

//查找相关的星星
void HelloWorld::checkPop(PopSprite* chosePop)
{
	int Direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//搜索方向
	int NewFlag=0;//迭代结束标志，一旦有新的元素加入就继续迭代，否则退出
	for (int i=0;i<4;i++)
	{
		int x=chosePop->getX()+Direction[i][0];
		int y=chosePop->getY()+Direction[i][1];//搜寻目标的坐标
		if (x>=0&&x<COL&&y>=0&&y<ROW)
		{
			PopSprite *poptem=pop[y][x];
			if(poptem!=NULL&&poptem->getNum()==chosePop->getNum()&&!array->containsObject(poptem))//判断条件
			{
				array->addObject(poptem);
				NewFlag=1;
			}
		}
	}
	if(NewFlag)//再次迭代
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


//获取触摸点位置
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

//添加pop
void HelloWorld::addPop(std::string str,int log,int j,int i,Size size,int num)
{
	PopSprite *po=PopSprite::CreatePopSprite(this,str,log,log,log*j+32,log*i+size.height/7);//pop的创建函数

	po->setX(j);//设置每个星星的坐标
	po->setY(i);
	po->setNum(num);//设置数字标志
	po->setStartNum(num);
	switch (num)//将精灵添加到不同的BatchNode中
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

//消除的线程
void HelloWorld::myThread()
{   
	int _threadId=threadId;//保存一些全局变量，使其局部化，不受其他线程干扰
	int _arrCount=arrayCount;

	int num[COL]={0};//由于判断每列是否有消除的星星
	//遍历每个星星
	for (int i=0;i<ROW;i++)
	{
		if(_arrCount<0)break;//无需消除
		for (int j=0;j<COL;j++)
		{
			if(pop[i][j]!=NULL&&pop[i][j]->getNum()==_threadId)
			{       
				    pop[i][j]->CreatParticle();
					mutex.lock();//上锁
					pop[i][j]->Remove();//删除
					pop[i][j]=NULL;
					mutex.unlock();
					#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
						usleep(50000);//Linux系统采用次函数
					#else
						Sleep(50);
					#endif
					_arrCount--;
					num[j]++;
			}
		}
	}
	//下移
	MoveUD(num);
	//左移
	MoveLR();
	//判断是否结束
	GameOver=isGameOver();
}

//星星的上下移动
void HelloWorld::MoveUD(int *a){
	JumpBy *jum;

	for (int i=0;i<COL;i++)
	{
		if(a[i]==0)continue;
		int moveCount=0;//移动的格数，即POP空的个数，代表已被消除的对象
		int numCol=0;
		for (int j=0;j<ROW;j++)
		{
			if (pop[j][i]==NULL)
			{
				moveCount++;
			}
			else if(moveCount!=0)
			{
				jum=JumpBy::create(0.2f,Vec2(0,-lon*moveCount),30,1);//跳动
				pop[j][i]->runAction(jum);
				mutex.lock();
				pop[j-moveCount][i]=pop[j][i];//对象下移
				pop[j-moveCount][i]->setX(i);//改变新对象的坐标
				pop[j-moveCount][i]->setY(j-moveCount);
				pop[j][i]=NULL;
				mutex.unlock();
			}
		}
	}
}

//向左移
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
				jum=JumpBy::create(0.2f,Vec2(-lon*numCol,0),0,1);//跳动		
				if(pop[j][i]==NULL)continue;
				pop[j][i]->runAction(jum);
				mutex.lock();
				pop[j][i-numCol]=pop[j][i];//对象下移
				pop[j][i-numCol]->setX(i-numCol);//改变新对象的坐标
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
				int Direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//搜索方向
				for (int k=0;k<4;k++)
				{
					int x=pop[i][j]->getX()+Direction[k][0];
					int y=pop[i][j]->getY()+Direction[k][1];//搜寻目标的坐标
					if (x>=0&&x<COL&&y>=0&&y<ROW)
					{
						PopSprite *poptem=pop[y][x];
						if(poptem!=NULL&&poptem->getNum()==pop[i][j]->getNum())//判断条件
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
		jum=JumpBy::create(0.2f,Vec2(0,0),8,1);//跳动
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

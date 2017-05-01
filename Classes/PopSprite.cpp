#include "PopSprite.h"


PopSprite* PopSprite::CreatePopSprite(Layer *he,std::string name,int wight,int height,float popX,float popY){
	PopSprite *enemy=new PopSprite();
	if(enemy&&enemy->init()){
		enemy->autorelease();
		enemy->enemyInit(he,name,wight,height,popX,popY);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}

void PopSprite::onEnter()
{
	Sprite::onEnter();
}
bool PopSprite::init(){
	if(!Sprite::init()){
		return false;
	}
	onEnter();
	return true;
}

//精灵的属性设置
void PopSprite::enemyInit(Layer *he,std::string name,int wight,int height,float popX,float popY){
	x=0;
	y=0;
	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(name);
	setTexture(texture);
	setTextureRect(Rect(0,0,wight,height));
	setPosition(Point(popX,popY));
	PointVec2=Vec2(popX,popY);
	helloLayer=he;
}


int PopSprite::getNum()
{
	return number;
}

void PopSprite::setNum(int num)
{
	number=num;
	
}

void PopSprite::setStartNum(int num)
{
	StartNum=num;
}

int PopSprite::getX()
{
	return x;
}
int PopSprite::getY()
{
	return y;
}
void PopSprite::setX(int x)
{
	this->x=x;
}
void PopSprite::setY(int y)
{
	this->y=y;
}

void PopSprite::Remove(){
	removeFromParent();
}

void PopSprite::CreatParticle(){
	ParticleSystem *particleSystem=ParticleExplosion::create();
	particleSystem->setTexture(TextureCache::sharedTextureCache()->addImage("particle_texture.png"));
	particleSystem->setTotalParticles(7);

	particleSystem->setSpeed(250);
	particleSystem->setGravity(Vec2(0,-300));
	particleSystem->setStartSize(17);
	particleSystem->setStartSizeVar(0);
	particleSystem->setEndSize(17);
	particleSystem->setEndSizeVar(0);

	particleSystem->setPosition(this->getPosition());
	particleSystem->setAngle(90);
	particleSystem->setAngleVar(90);
	particleSystem->setLife(1.0f);
	particleSystem->setLifeVar(0.2f);
	particleSystem->setDuration(0.01f);
	particleSystem->setEmissionRate(700);

	switch (StartNum)
	{
	case 0:
		particleSystem->setStartColor(Color4F(41/255.0f,194/255.0f,252/255.0f,255/255.0f));
		particleSystem->setStartColorVar(Color4F(0,0,0,0));
		particleSystem->setEndColor(Color4F(41/255.0f,194/255.0f,252/255.0f,255/255.0f));
		particleSystem->setEndColorVar(Color4F(0,0,0,0));
		break;
	case 1:
		particleSystem->setStartColor(Color4F(84/255.0f,208/255.0f,59/255.0f,255/255.0f));
		particleSystem->setEndColor(Color4F(84/255.0f,208/255.0f,59/255.0f,255/255.0f));
		particleSystem->setStartColorVar(Color4F(0,0,0,0));
		particleSystem->setEndColorVar(Color4F(0,0,0,0));
		break;
	case 2:
		particleSystem->setStartColor(Color4F(248/255.0f,221/255.0f,30/255.0f,255/255.0f));
		particleSystem->setEndColor(Color4F(248/255.0f,221/255.0f,30/255.0f,255/255.0f));
		particleSystem->setStartColorVar(Color4F(0,0,0,0));
		particleSystem->setEndColorVar(Color4F(0,0,0,0));
		break;
	case 3:
		particleSystem->setStartColor(Color4F(150/255.0f,23/255.0f,225/255.0f,255/255.0f));
		particleSystem->setEndColor(Color4F(150/255.0f,23/255.0f,225/255.0f,255/255.0f));
		particleSystem->setStartColorVar(Color4F(0,0,0,0));
		particleSystem->setEndColorVar(Color4F(0,0,0,0));
		break;
	case 4:
		particleSystem->setStartColor(Color4F(250/255.0f,33/255.0f,92/255.0f,255/255.0f));
		particleSystem->setEndColor(Color4F(250/255.0f,33/255.0f,92/255.0f,255/255.0f));
		particleSystem->setStartColorVar(Color4F(0,0,0,0));
		particleSystem->setEndColorVar(Color4F(0,0,0,0));
		break;
	default:
		break;
	}

	particleSystem->setAutoRemoveOnFinish(true);
	helloLayer->addChild(particleSystem);
}

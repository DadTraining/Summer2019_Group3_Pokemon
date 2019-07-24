
#include "City.h"
#include "ResourceManager.h"
#include "SimpleAudioEngine.h"
#include "Buttons.h"
#include "Cave.h"
#include "Route1.h"
#include "Lake.h"
#include "PokemonCenter.h"
#include "Route2.h"

USING_NS_CC;
Size CityvisibleSize;
Size CitytileMapSize;

PhysicsBody* Citybody, *CitygateWay;
Camera *Citycamera;
int City::previousScene = 0;


Scene* City::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = City::create();
	scene->addChild(layer);
	Citycamera = scene->getDefaultCamera();
	return scene;
}


// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool City::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CityvisibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto map = TMXTiledMap::create("res/Map/City.tmx");
	CitytileMapSize = map->getContentSize();
	addChild(map);

	auto mPhysicsLayer = map->getLayer("physics");
	Size layerSize = mPhysicsLayer->getLayerSize();
	for (int i = 0; i < layerSize.width; i++)
	{
		for (int j = 0; j < layerSize.height; j++)
		{
			auto tileSet = mPhysicsLayer->getTileAt(Vec2(i, j));
			if (tileSet != NULL)
			{
				auto physics = PhysicsBody::createBox(tileSet->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
				physics->setCollisionBitmask(13);
				physics->setContactTestBitmask(true);
				physics->setDynamic(false);
				physics->setGravityEnable(false);
				physics->setMass(12);
				tileSet->setPhysicsBody(physics);
			}
		}
	}

	InitObject();

	Button *up = Buttons::getIntance()->GetButtonUp();
	Button *right = Buttons::getIntance()->GetButtonRight();
	Button *left = Buttons::getIntance()->GetButtonLeft();
	Button *down = Buttons::getIntance()->GetButtonDown();
	up->retain();
	up->removeFromParent();
	up->release();
	right->retain();
	right->removeFromParent();
	right->release();
	left->retain();
	left->removeFromParent();
	left->release();
	down->retain();
	down->removeFromParent();
	down->release();
	addChild(up, 100);
	addChild(right, 100);
	addChild(left, 100);
	addChild(down, 100);

	Buttons::getIntance()->ButtonListener(this->mPlayer);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(City::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();
	return true;
}

bool City::onContactBegin(PhysicsContact& contact)

{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if (a->getCollisionBitmask() == 15 && b->getCollisionBitmask() == 17
		|| a->getCollisionBitmask() == 17 && b->getCollisionBitmask() == 15)
	{
		Director::getInstance()->getRunningScene()->pause();
		Route1::previousScene = 1;
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Route1::createScene()));
	}
	else if (a->getCollisionBitmask() == 19 && b->getCollisionBitmask() == 15
		|| a->getCollisionBitmask() == 15 && b->getCollisionBitmask() == 19)
	{
		Director::getInstance()->getRunningScene()->pause();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, PokemonCenter::createScene()));
	}
	else if (a->getCollisionBitmask() == 21 && b->getCollisionBitmask() == 15
		|| a->getCollisionBitmask() == 15 && b->getCollisionBitmask() == 21)
	{
		Director::getInstance()->getRunningScene()->pause();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Cave::createScene()));
	}
	else if (a->getCollisionBitmask() == 23 && b->getCollisionBitmask() == 15
		|| a->getCollisionBitmask() == 15 && b->getCollisionBitmask() == 23)
	{
		Director::getInstance()->getRunningScene()->pause();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Lake::createScene()));
	}
	else if (a->getCollisionBitmask() == 25 && b->getCollisionBitmask() == 15
		|| a->getCollisionBitmask() == 15 && b->getCollisionBitmask() == 25)
	{
		Director::getInstance()->getRunningScene()->pause();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Route2::createScene()));
	}

	return true;

}

void City::InitObject()
{
	auto map = TMXTiledMap::create("res/Map/City.tmx");
	auto m_objectGroup = map->getObjectGroup("Object");
	auto objects = m_objectGroup->getObjects();
	for (int i = 0; i < objects.size(); i++) {
		auto object = objects.at(i);
		auto properties = object.asValueMap();
		float posX = properties.at("x").asFloat();
		float posY = properties.at("y").asFloat();
		int type = object.asValueMap().at("type").asInt();
		if (type == 1) {
			int preScene = object.asValueMap().at("pre").asInt();
			if (preScene == previousScene) {
				mPlayer = new Trainer(this);
				if (preScene==1)
				{
					mPlayer->GetSpriteFront()->setTexture("res/Trainer/walkdown/1.png");
				}
				else if (preScene == 2)
				{
					mPlayer->GetSpriteFront()->setTexture("res/Trainer/walkright/1.png");
				}
				else if (preScene == 3)
				{
					mPlayer->GetSpriteFront()->setTexture("res/Trainer/walkleft/1.png");
				}
				else if (preScene == 4)
				{
					mPlayer->GetSpriteFront()->setTexture("res/Trainer/walkdown/1.png");
				}
				mPlayer->GetSpriteFront()->setPosition(Vec2(posX, posY));
				Citybody = PhysicsBody::createBox(mPlayer->GetSpriteFront()->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
				Citybody->setCollisionBitmask(15);
				Citybody->setMass(14);
				Citybody->setContactTestBitmask(true);
				Citybody->setDynamic(true);
				Citybody->setRotationEnable(false);
				Citybody->setGravityEnable(false);
				mPlayer->GetSpriteFront()->setPhysicsBody(Citybody);
			}
			else continue;
		}
		else if (type == 2)
		{
			mGateWay = Sprite::create("res/walkup.png");
			mGateWay->setPosition(Vec2(posX, posY));
			CitygateWay = PhysicsBody::createBox(mGateWay->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			CitygateWay->setCollisionBitmask(17);
			CitygateWay->setMass(16);
			CitygateWay->setContactTestBitmask(true);
			CitygateWay->setDynamic(false);
			CitygateWay->setGravityEnable(false);
			mGateWay->setPhysicsBody(CitygateWay);
			mGateWay->setVisible(false);
			this->addChild(mGateWay, 10);
		}
		else if (type == 3)
		{
			mGateWay = Sprite::create("res/walkup.png");
			mGateWay->setPosition(Vec2(posX, posY));
			CitygateWay = PhysicsBody::createBox(mGateWay->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			CitygateWay->setCollisionBitmask(19);
			CitygateWay->setMass(18);
			CitygateWay->setContactTestBitmask(true);
			CitygateWay->setDynamic(false);
			CitygateWay->setGravityEnable(false);
			mGateWay->setPhysicsBody(CitygateWay);
			mGateWay->setVisible(false);
			this->addChild(mGateWay, 10);
		}
		else if (type == 4)
		{
			mGateWay = Sprite::create("res/walkup.png");
			mGateWay->setPosition(Vec2(posX, posY));
			CitygateWay = PhysicsBody::createBox(mGateWay->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			CitygateWay->setCollisionBitmask(21);
			CitygateWay->setMass(20);
			CitygateWay->setContactTestBitmask(true);
			CitygateWay->setDynamic(false);
			CitygateWay->setGravityEnable(false);
			mGateWay->setPhysicsBody(CitygateWay);
			mGateWay->setVisible(false);
			this->addChild(mGateWay, 10);
		}
		else if (type == 5)
		{
			mGateWay = Sprite::create("res/walkup.png");
			mGateWay->setPosition(Vec2(posX, posY));
			CitygateWay = PhysicsBody::createBox(mGateWay->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			CitygateWay->setCollisionBitmask(23);
			CitygateWay->setMass(22);
			CitygateWay->setContactTestBitmask(true);
			CitygateWay->setDynamic(false);
			CitygateWay->setGravityEnable(false);
			mGateWay->setPhysicsBody(CitygateWay);
			mGateWay->setVisible(false);
			this->addChild(mGateWay, 10);
		}
		else if (type == 6)
		{
			mGateWay = Sprite::create("res/walkup.png");
			mGateWay->setPosition(Vec2(posX, posY));
			CitygateWay = PhysicsBody::createBox(mGateWay->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			CitygateWay->setCollisionBitmask(25);
			CitygateWay->setMass(24);
			CitygateWay->setContactTestBitmask(true);
			CitygateWay->setDynamic(false);
			CitygateWay->setGravityEnable(false);
			mGateWay->setPhysicsBody(CitygateWay);
			mGateWay->setVisible(false);
			this->addChild(mGateWay, 10);
		}
	}

}

void City::updateCamera() {
	if (CityvisibleSize.width >= CitytileMapSize.width) {
		if (CityvisibleSize.height >= CitytileMapSize.height) {
			Citycamera->setPosition(CitytileMapSize / 2);
		}
		else
		{
			if (abs(mPlayer->GetSpriteFront()->getPosition().y - CitytileMapSize.height / 2)>abs(CitytileMapSize.height / 2 - CityvisibleSize.height / 2)) {
				Citycamera->setPosition(CitytileMapSize.width / 2, (mPlayer->GetSpriteFront()->getPosition().y >Citycamera->getPosition().y) ? (CitytileMapSize.height - CityvisibleSize.height / 2) : CityvisibleSize.height / 2);
			}
			else {
				Citycamera->setPosition(CitytileMapSize.width / 2, mPlayer->GetSpriteFront()->getPosition().y);
			}
		}
	}
	else {
		if (CityvisibleSize.height >= CitytileMapSize.height) {
			if (abs(mPlayer->GetSpriteFront()->getPosition().x - CitytileMapSize.width / 2)>abs(CitytileMapSize.width / 2 - CityvisibleSize.width / 2)) {
				Citycamera->setPosition((mPlayer->GetSpriteFront()->getPosition().y >Citycamera->getPosition().y) ? (CitytileMapSize.width - CityvisibleSize.width / 2) : CityvisibleSize.width / 2, CitytileMapSize.height / 2);
			}
			else {
				Citycamera->setPosition(mPlayer->GetSpriteFront()->getPosition().x, CitytileMapSize.height / 2);
			}
		}
		else {
			if (abs(mPlayer->GetSpriteFront()->getPosition().x - CitytileMapSize.width / 2)>abs(CitytileMapSize.width / 2 - CityvisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - CitytileMapSize.height / 2)>abs(CitytileMapSize.height / 2 - CityvisibleSize.height / 2)) {
				Citycamera->setPosition((mPlayer->GetSpriteFront()->getPosition().x >Citycamera->getPosition().x) ? (CitytileMapSize.width - CityvisibleSize.width / 2) : CityvisibleSize.width / 2, (mPlayer->GetSpriteFront()->getPosition().y >Citycamera->getPosition().y) ? (CitytileMapSize.height - CityvisibleSize.height / 2) : CityvisibleSize.height / 2);
			}
			else if (abs(mPlayer->GetSpriteFront()->getPosition().x - CitytileMapSize.width / 2)>abs(CitytileMapSize.width / 2 - CityvisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - CitytileMapSize.height / 2)<abs(CitytileMapSize.height / 2 - CityvisibleSize.height / 2)) {
				Citycamera->setPosition((mPlayer->GetSpriteFront()->getPosition().x >Citycamera->getPosition().x) ? (CitytileMapSize.width - CityvisibleSize.width / 2) : CityvisibleSize.width / 2, mPlayer->GetSpriteFront()->getPosition().y);
			}
			else if (abs(mPlayer->GetSpriteFront()->getPosition().x - CitytileMapSize.width / 2)<abs(CitytileMapSize.width / 2 - CityvisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - CitytileMapSize.height / 2)>abs(CitytileMapSize.height / 2 - CityvisibleSize.height / 2)) {
				Citycamera->setPosition(mPlayer->GetSpriteFront()->getPosition().x, (mPlayer->GetSpriteFront()->getPosition().y >Citycamera->getPosition().y) ? (CitytileMapSize.height - CityvisibleSize.height / 2) : CityvisibleSize.height / 2);
			}
			else {
				Citycamera->setPosition(mPlayer->GetSpriteFront()->getPosition());
			}
		}
	}

}

void City::update(float dt) {
	updateCamera();
	Buttons::getIntance()->UpdateButton(Citycamera->getPosition().x - 200, Citycamera->getPosition().y - 100);
}
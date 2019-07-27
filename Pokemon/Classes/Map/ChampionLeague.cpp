#include "ChampionLeague.h"
#include "ResourceManager.h"
#include "SimpleAudioEngine.h"
#include "Buttons.h"
#include "Model.h"

using namespace CocosDenshion;
USING_NS_CC;

Size leagueVisibleSize;
Size leagueTileMapSize;

PhysicsBody* leagueBody;
Camera *leagueCamera;

Scene * League::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = League::create();
	scene->addChild(layer);
	leagueCamera = scene->getDefaultCamera();
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool League::init()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("res/Sound/ChampionLeague.mp3", true);
	if (!Layer::init())
	{
		return false;
	}

	leagueVisibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto map = TMXTiledMap::create("res/Map/League.tmx");
	leagueTileMapSize = map->getContentSize();
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
				physics->setCollisionBitmask(Model::BITMASK_WORLD);
				physics->setContactTestBitmask(true);
				physics->setDynamic(false);
				physics->setGravityEnable(false);
				tileSet->setPhysicsBody(physics);
			}
		}
	}

	InitObject();

	Button *up = Buttons::GetIntance()->GetButtonUp();
	Button *right = Buttons::GetIntance()->GetButtonRight();
	Button *left = Buttons::GetIntance()->GetButtonLeft();
	Button *down = Buttons::GetIntance()->GetButtonDown();
	addChild(up, 100);
	addChild(right, 100);
	addChild(left, 100);
	addChild(down, 100);


	Buttons::GetIntance()->ButtonListener(this->mPlayer);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(League::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();
	return true;
}

bool League::onContactBegin(PhysicsContact & contact)
{
	return true;
}

void League::InitObject()
{
	auto map = TMXTiledMap::create("res/Map/League.tmx");
	auto m_objectGroup = map->getObjectGroup("Object");
	auto objects = m_objectGroup->getObjects();
	for (int i = 0; i < objects.size(); i++) {
		auto object = objects.at(i);
		auto properties = object.asValueMap();
		float posX = properties.at("x").asFloat();
		float posY = properties.at("y").asFloat();
		int type = object.asValueMap().at("type").asInt();
		if (type == Model::MODLE_TYPE_MAIN_CHARACTER) {
			mPlayer = new Trainer(this);
			mPlayer->GetSpriteFront()->setTexture("res/Trainer/walkleft/1.png");
			mPlayer->GetSpriteFront()->setPosition(Vec2(posX, posY));
			leagueBody = PhysicsBody::createBox(mPlayer->GetSpriteFront()->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			leagueBody->setCollisionBitmask(Model::BITMASK_PLAYER);
			leagueBody->setContactTestBitmask(true);
			leagueBody->setDynamic(true);
			leagueBody->setRotationEnable(false);
			leagueBody->setGravityEnable(false);
			mPlayer->GetSpriteFront()->setPhysicsBody(leagueBody);
		}
	}
}

void League::UpdateCamera() {
	if (leagueVisibleSize.width >= leagueTileMapSize.width) {
		if (leagueVisibleSize.height >= leagueTileMapSize.height) {
			leagueCamera->setPosition(leagueTileMapSize / 2);
		}
		else
		{
			if (abs(mPlayer->GetSpriteFront()->getPosition().y - leagueTileMapSize.height / 2)>abs(leagueTileMapSize.height / 2 - leagueVisibleSize.height / 2)) {
				leagueCamera->setPosition(leagueTileMapSize.width / 2, (mPlayer->GetSpriteFront()->getPosition().y >leagueCamera->getPosition().y) ? (leagueTileMapSize.height - leagueVisibleSize.height / 2) : leagueVisibleSize.height / 2);
			}
			else {
				leagueCamera->setPosition(leagueTileMapSize.width / 2, mPlayer->GetSpriteFront()->getPosition().y);
			}
		}
	}
	else {
		if (leagueVisibleSize.height >= leagueTileMapSize.height) {
			if (abs(mPlayer->GetSpriteFront()->getPosition().x - leagueTileMapSize.width / 2)>abs(leagueTileMapSize.width / 2 - leagueVisibleSize.width / 2)) {
				leagueCamera->setPosition((mPlayer->GetSpriteFront()->getPosition().y >leagueCamera->getPosition().y) ? (leagueTileMapSize.width - leagueVisibleSize.width / 2) : leagueVisibleSize.width / 2, leagueTileMapSize.height / 2);
			}
			else {
				leagueCamera->setPosition(mPlayer->GetSpriteFront()->getPosition().x, leagueTileMapSize.height / 2);
			}
		}
		else {
			if (abs(mPlayer->GetSpriteFront()->getPosition().x - leagueTileMapSize.width / 2)>abs(leagueTileMapSize.width / 2 - leagueVisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - leagueTileMapSize.height / 2)>abs(leagueTileMapSize.height / 2 - leagueVisibleSize.height / 2)) {
				leagueCamera->setPosition((mPlayer->GetSpriteFront()->getPosition().x >leagueCamera->getPosition().x) ? (leagueTileMapSize.width - leagueVisibleSize.width / 2) : leagueVisibleSize.width / 2, (mPlayer->GetSpriteFront()->getPosition().y >leagueCamera->getPosition().y) ? (leagueTileMapSize.height - leagueVisibleSize.height / 2) : leagueVisibleSize.height / 2);
			}
			else if (abs(mPlayer->GetSpriteFront()->getPosition().x - leagueTileMapSize.width / 2)>abs(leagueTileMapSize.width / 2 - leagueVisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - leagueTileMapSize.height / 2)<abs(leagueTileMapSize.height / 2 - leagueVisibleSize.height / 2)) {
				leagueCamera->setPosition((mPlayer->GetSpriteFront()->getPosition().x >leagueCamera->getPosition().x) ? (leagueTileMapSize.width - leagueVisibleSize.width / 2) : leagueVisibleSize.width / 2, mPlayer->GetSpriteFront()->getPosition().y);
			}
			else if (abs(mPlayer->GetSpriteFront()->getPosition().x - leagueTileMapSize.width / 2)<abs(leagueTileMapSize.width / 2 - leagueVisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - leagueTileMapSize.height / 2)>abs(leagueTileMapSize.height / 2 - leagueVisibleSize.height / 2)) {
				leagueCamera->setPosition(mPlayer->GetSpriteFront()->getPosition().x, (mPlayer->GetSpriteFront()->getPosition().y >leagueCamera->getPosition().y) ? (leagueTileMapSize.height - leagueVisibleSize.height / 2) : leagueVisibleSize.height / 2);
			}
			else {
				leagueCamera->setPosition(mPlayer->GetSpriteFront()->getPosition());
			}
		}
	}
}

void League::update(float dt)
{
	UpdateCamera();
	Buttons::GetIntance()->UpdateButton(leagueCamera->getPosition().x - 200, leagueCamera->getPosition().y - 100);
}

#include "VictoryRoad.h"
#include "ResourceManager.h"
#include "SimpleAudioEngine.h"
#include "Buttons.h"
#include "Route2.h"
#include "Model.h"
#include "ChampionLeague.h"
#include "Popup.h"

using namespace CocosDenshion;
Size roadVisibleSize;
Size roadTileMapSize;
Layer *layer_UI_Road;
PhysicsBody* roadBody, *roadGateWay;
Camera *roadCamera, *cameraUIRoad;

Scene* Road::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = Road::create();
	scene->addChild(layer);
	roadCamera = scene->getDefaultCamera();
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Road::init()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("res/Sound/VictoryRoad.mp3", true);
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	roadVisibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto map = TMXTiledMap::create("res/Map/Road.tmx");
	roadTileMapSize = map->getContentSize();
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
	Button *bag = Buttons::GetIntance()->GetButtonBag();
	Button *tips = Buttons::GetIntance()->GetButtonTips();

	layer_UI_Road = Layer::create();
	layer_UI_Road->setScale(roadVisibleSize.width / layer_UI_Road->getContentSize().width, roadVisibleSize.height / layer_UI_Road->getContentSize().height);
	up->setPosition(Vec2(roadVisibleSize.width / 7.2, roadVisibleSize.height / 3.6));
	bag->setPosition(Vec2(roadVisibleSize.width / 1.09, roadVisibleSize.height / 1.09));
	tips->setPosition(Vec2(roadVisibleSize.width / 20, roadVisibleSize.height / 1.09));
	cameraUIRoad = Camera::create();
	cameraUIRoad->setCameraMask(2);
	cameraUIRoad->setCameraFlag(CameraFlag::USER1);
	up->setCameraMask(2);
	bag->setCameraMask(2);
	tips->setCameraMask(2);
	layer_UI_Road->addChild(cameraUIRoad, 2);
	layer_UI_Road->addChild(up);
	layer_UI_Road->addChild(bag);
	layer_UI_Road->addChild(tips);
	this->addChild(layer_UI_Road, 100);

	Buttons::GetIntance()->ButtonListener(this->mPlayer);

	Buttons::GetIntance()->GetButtonBag()->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			Buttons::GetIntance()->GetButtonBag()->setTouchEnabled(false);
			string str = "My bag - Gold: " + to_string(Bag::GetInstance()->GetGold()) + " $";
			UICustom::Popup *popup = UICustom::Popup::createBag(str);
			popup->removeFromParent();
			popup->setAnchorPoint(Vec2(0.5, 0.5));
			popup->setPosition(roadCamera->getPosition().x - popup->getContentSize().width / 2,
				roadCamera->getPosition().y - popup->getContentSize().height / 2);
			this->addChild(popup, 101);
		}
	});

	Buttons::GetIntance()->GetButtonTips()->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			Buttons::GetIntance()->GetButtonTips()->setTouchEnabled(false);
			UICustom::Popup *popup = UICustom::Popup::createAsMessage("Doctor", Model::GetTipsGame());
			popup->removeFromParent();
			popup->setAnchorPoint(Vec2(0.5, 0.5));
			popup->setPosition(roadCamera->getPosition().x - popup->getContentSize().width / 2,
				roadCamera->getPosition().y - popup->getContentSize().height / 2);
			this->addChild(popup, 101);
		}
	});
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Road::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();
	return true;
}

bool Road::onContactBegin(PhysicsContact& contact)

{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_ROAD_GATE_TO_ROUTE2)
		|| a->getCollisionBitmask() == Model::BITMASK_ROAD_GATE_TO_ROUTE2 && b->getCollisionBitmask() == Model::BITMASK_PLAYER)
	{
		Buttons::GetIntance()->Remove();
		Director::getInstance()->getRunningScene()->pause();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Route2::createScene()));
		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("res/Sound/ExitRoom.mp3", false);
	}
	else if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_ROAD_GATE_TO_LEAGUE)
		|| a->getCollisionBitmask() == Model::BITMASK_ROAD_GATE_TO_LEAGUE && b->getCollisionBitmask() == Model::BITMASK_PLAYER)
	{
		Buttons::GetIntance()->Remove();
		Director::getInstance()->getRunningScene()->pause();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, League::createScene()));
		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("res/Sound/ExitRoom.mp3", false);
	}
	else if ((a->getCollisionBitmask() == Model::BITMASK_WORLD && b->getCollisionBitmask() == Model::BITMASK_PLAYER)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_WORLD))
	{
		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("res/Sound/WallBump.mp3", false);
		switch (Buttons::state)
		{
		case 1:
			mPlayer->GetSpriteFront()->stopActionByTag(0);
			mPlayer->GetSpriteFront()->setPositionY(mPlayer->GetSpriteFront()->getPositionY() - 2);
			break;
		case 2:
			mPlayer->GetSpriteFront()->stopActionByTag(6);
			mPlayer->GetSpriteFront()->setPositionX(mPlayer->GetSpriteFront()->getPositionX() - 2);
			break;
		case 3:
			mPlayer->GetSpriteFront()->stopActionByTag(4);
			mPlayer->GetSpriteFront()->setPositionX(mPlayer->GetSpriteFront()->getPositionX() + 2);
			break;
		case 4:
			mPlayer->GetSpriteFront()->stopActionByTag(2);
			mPlayer->GetSpriteFront()->setPositionY(mPlayer->GetSpriteFront()->getPositionY() + 2);
			break;
		default:
			break;
		}
	}
	return true;

}

void Road::InitObject()
{
	auto map = TMXTiledMap::create("res/Map/Road.tmx");
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
			mPlayer->GetSpriteFront()->setPosition(Vec2(posX, posY));
			roadBody = PhysicsBody::createBox(mPlayer->GetSpriteFront()->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			roadBody->setCollisionBitmask(Model::BITMASK_PLAYER);
			roadBody->setContactTestBitmask(true);
			roadBody->setDynamic(true);
			roadBody->setRotationEnable(false);
			roadBody->setGravityEnable(false);
			mPlayer->GetSpriteFront()->setPhysicsBody(roadBody);
		}
		else if (type == Model::MODLE_TYPE_ROAD_GATE_TO_ROUTE2)
		{
			mGateWay = Sprite::create("res/walkup.png");
			mGateWay->setPosition(Vec2(posX, posY));
			roadGateWay = PhysicsBody::createBox(mGateWay->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			roadGateWay->setCollisionBitmask(Model::BITMASK_ROAD_GATE_TO_ROUTE2);
			roadGateWay->setContactTestBitmask(true);
			roadGateWay->setDynamic(false);
			roadGateWay->setGravityEnable(false);
			mGateWay->setPhysicsBody(roadGateWay);
			mGateWay->setVisible(false);
			this->addChild(mGateWay, 10);
		}
		else if (type == Model::MODLE_TYPE_ROAD_GATE_TO_LEAGUE)
		{
			mGateWay = Sprite::create("res/walkup.png");
			mGateWay->setPosition(Vec2(posX, posY));
			roadGateWay = PhysicsBody::createBox(mGateWay->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			roadGateWay->setCollisionBitmask(Model::BITMASK_ROAD_GATE_TO_LEAGUE);
			roadGateWay->setContactTestBitmask(true);
			roadGateWay->setDynamic(false);
			roadGateWay->setGravityEnable(false);
			mGateWay->setPhysicsBody(roadGateWay);
			mGateWay->setVisible(false);
			this->addChild(mGateWay, 10);
		}
	}
}

void Road::UpdateCamera() {
	if (roadVisibleSize.width >= roadTileMapSize.width) {
		if (roadVisibleSize.height >= roadTileMapSize.height) {
			roadCamera->setPosition(roadTileMapSize / 2);
		}
		else
		{
			if (abs(mPlayer->GetSpriteFront()->getPosition().y - roadTileMapSize.height / 2)>abs(roadTileMapSize.height / 2 - roadVisibleSize.height / 2)) {
				roadCamera->setPosition(roadTileMapSize.width / 2, (mPlayer->GetSpriteFront()->getPosition().y >roadCamera->getPosition().y) ? (roadTileMapSize.height - roadVisibleSize.height / 2) : roadVisibleSize.height / 2);
			}
			else {
				roadCamera->setPosition(roadTileMapSize.width / 2, mPlayer->GetSpriteFront()->getPosition().y);
			}
		}
	}
	else {
		if (roadVisibleSize.height >= roadTileMapSize.height) {
			if (abs(mPlayer->GetSpriteFront()->getPosition().x - roadTileMapSize.width / 2)>abs(roadTileMapSize.width / 2 - roadVisibleSize.width / 2)) {
				roadCamera->setPosition((mPlayer->GetSpriteFront()->getPosition().y >roadCamera->getPosition().y) ? (roadTileMapSize.width - roadVisibleSize.width / 2) : roadVisibleSize.width / 2, roadTileMapSize.height / 2);
			}
			else {
				roadCamera->setPosition(mPlayer->GetSpriteFront()->getPosition().x, roadTileMapSize.height / 2);
			}
		}
		else {
			if (abs(mPlayer->GetSpriteFront()->getPosition().x - roadTileMapSize.width / 2)>abs(roadTileMapSize.width / 2 - roadVisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - roadTileMapSize.height / 2)>abs(roadTileMapSize.height / 2 - roadVisibleSize.height / 2)) {
				roadCamera->setPosition((mPlayer->GetSpriteFront()->getPosition().x >roadCamera->getPosition().x) ? (roadTileMapSize.width - roadVisibleSize.width / 2) : roadVisibleSize.width / 2, (mPlayer->GetSpriteFront()->getPosition().y >roadCamera->getPosition().y) ? (roadTileMapSize.height - roadVisibleSize.height / 2) : roadVisibleSize.height / 2);
			}
			else if (abs(mPlayer->GetSpriteFront()->getPosition().x - roadTileMapSize.width / 2)>abs(roadTileMapSize.width / 2 - roadVisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - roadTileMapSize.height / 2)<abs(roadTileMapSize.height / 2 - roadVisibleSize.height / 2)) {
				roadCamera->setPosition((mPlayer->GetSpriteFront()->getPosition().x >roadCamera->getPosition().x) ? (roadTileMapSize.width - roadVisibleSize.width / 2) : roadVisibleSize.width / 2, mPlayer->GetSpriteFront()->getPosition().y);
			}
			else if (abs(mPlayer->GetSpriteFront()->getPosition().x - roadTileMapSize.width / 2)<abs(roadTileMapSize.width / 2 - roadVisibleSize.width / 2)
				&& abs(mPlayer->GetSpriteFront()->getPosition().y - roadTileMapSize.height / 2)>abs(roadTileMapSize.height / 2 - roadVisibleSize.height / 2)) {
				roadCamera->setPosition(mPlayer->GetSpriteFront()->getPosition().x, (mPlayer->GetSpriteFront()->getPosition().y >roadCamera->getPosition().y) ? (roadTileMapSize.height - roadVisibleSize.height / 2) : roadVisibleSize.height / 2);
			}
			else {
				roadCamera->setPosition(mPlayer->GetSpriteFront()->getPosition());
			}
		}
	}
}

int roadSum = 0;

void Road::UpdatePlayer(float dt) {
	roadSum++;
	if (roadSum >30) {
		if (mPlayer->isMoveDown) {
			mPlayer->StopWalkDown();
			mPlayer->WalkDown();
		}
		else if (mPlayer->isMoveLeft) {
			mPlayer->StopWalkLeft();
			mPlayer->WalkLeft();
		}
		else if (mPlayer->isMoveUp) {
			mPlayer->StopWalkUp();
			mPlayer->WalkUp();
		}
		else if (mPlayer->isMoveRight) {
			mPlayer->StopWalkRight();
			mPlayer->WalkRight();
		}
		else
		{
		}
		roadSum = 0;
	}
}


void Road::update(float dt) {
	UpdatePlayer(dt);
	UpdateCamera();
}

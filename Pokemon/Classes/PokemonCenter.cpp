
#include "PokemonCenter.h"
#include "ResourceManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "Pokemon\Charmander.h"
#include "Pokemon\Squirtle.h"
#include "Buttons.h"

USING_NS_CC;

Scene* PokemonCenter::createScene()
{
	auto scene = Scene::create();
	auto layer = PokemonCenter::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PokemonCenter::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto map = TMXTiledMap::create("res/Map/pc.tmx");
	addChild(map);
	Button *up = Buttons::getIntance()->GetButtonUp();
	up->removeFromParent();
	Button *right = Buttons::getIntance()->GetButtonRight();
	right->removeFromParent();
	Button *left = Buttons::getIntance()->GetButtonLeft();
	left->removeFromParent();
	Button *down = Buttons::getIntance()->GetButtonDown();
	down->removeFromParent();
	addChild(up, 100);
	addChild(right, 100);
	addChild(left, 100);
	addChild(down, 100);
    return true;
}

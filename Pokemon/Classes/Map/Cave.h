#pragma once
#include "cocos2d.h"
#include "Pokemon.h"
#include "Trainer.h"
#include <math.h>
using namespace cocos2d;
using namespace std;
class Cave : public cocos2d::Layer
{
private:
	Trainer * mPlayer;
	Sprite * mGateWay;
	TMXTiledMap* map;
	Sprite * mNpc;
	Sprite * entei;
	EventListenerTouchOneByOne* touchListener;
	Label* m_labelLog;
	Sprite* m_messageBox;
	int writing = 0;
public:
	static cocos2d::Scene* createScene();
	void TypeWriter(float deltaTime);
	void LogSetOpacity(GLubyte opacity);
	virtual bool init();
	bool onContactBegin(PhysicsContact & contact);
	void InitGrass();
	void InitObject();
	void UpdateCamera();
	void Log(string logg);
	bool onTouchBegan(Touch* touch, Event* e);
	bool onTouchEnd(Touch* t, Event* event);
	//void CreateButon();
	CREATE_FUNC(Cave);
	void UpdatePlayer(float dt);
	//void createPhysics();
	void update(float);
};
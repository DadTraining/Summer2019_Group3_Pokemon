#pragma once
#include "cocos2d.h"
#include "Pokemon.h"
using namespace cocos2d;

class BattleScene : public Layer
{
private:
	Sprite* m_background;
	Sprite* m_messageBox;
	Button* m_buttonFight;
	Button* m_buttonBag;
	Button* m_buttonPokemon;
	Button* m_buttonRun;
	Label* m_labelSkill1;
	Label* m_labelSkill2;
	Label* m_labelSkill3;
	Label* m_labelSkill4;
	Label* m_labelBattleLog;

	Label* m_labelOppLevel;
	Label* m_labelOppName;
	Sprite* m_hpPlayer;

	Label* m_labelPlayerLevel;
	Label* m_labelPlayerName;
	Sprite* m_hpOpponent;

	TMXTiledMap* m_tiledmap;

	Pokemon* m_player;
	Skill* playerSkill;
	Pokemon* m_opponent;
	Skill* oppSkill;
	int writing = 0;
	bool m_stateBattleLog = false;
	bool m_statePlayer = false;
	bool m_stateOpponent = false;
public:
	BattleScene();
	~BattleScene();
	static cocos2d::Scene* createScene();

	virtual bool init();

	void update(float deltaTime) override;

	void TypeWriter(float deltaTime);

	void DamageStepWithPlayerAttackFirst(float deltaTime);

	void DamageStepWithOpponentAttackFirst(float deltaTime);

	void ReduceHpPlayer(float deltaTime);

	void ReduceHpOpponent(float deltaTime);

	void BattleLogSetOpacity(GLubyte opacity);

	void InitTiledMap();

	void InitUI();

	void InitObject();

	void AddEventListener();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* e);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* e);

	bool onTouchBegan(Touch* touch, Event* e);

	void BattleLog(string logg);

	void SetButtonVisible(bool visible);

	void BattlePhase(int idSkill);

	void EndBattle();

	CREATE_FUNC(BattleScene);
};
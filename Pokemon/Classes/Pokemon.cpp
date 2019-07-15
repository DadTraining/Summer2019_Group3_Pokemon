#include "Pokemon.h"
#include <cstdlib>
#define length 3


float Pokemon::RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

Pokemon::Pokemon()
{
}


Pokemon::~Pokemon()
{
}

void Pokemon::Init()
{
}

void Pokemon::Init(int id_front, int id_back)
{
	this->m_spriteFront = ResourceManager::GetInstance()->GetSpriteById(id_front);
	this->m_spriteFront->setScale(2.5);
	this->m_spriteFront->setAnchorPoint(Vec2(0.5, 0));
	this->animationFront = ResourceManager::GetInstance()->GetAnimationById(id_front);
	this->animationFront->setDelayPerUnit(0.2);
	auto animateFront = Animate::create(this->animationFront);
	this->m_spriteFront->runAction(RepeatForever::create(animateFront));
	//
	//
	this->m_spriteBack = ResourceManager::GetInstance()->GetSpriteById(id_back);
	this->m_spriteBack->setScale(2.5);
	this->m_spriteBack->setAnchorPoint(Vec2(0.5, 0));
	this->animationBack = ResourceManager::GetInstance()->GetAnimationById(id_back);
	this->animationBack->setDelayPerUnit(0.2);
	auto animateBack = Animate::create(this->animationBack);
	this->m_spriteBack->runAction(RepeatForever::create(animateBack));
}

void Pokemon::Update(float deltaTime)
{
}

Pokemon * Pokemon::Evolve()
{
	return nullptr;
}

void Pokemon::LevelUp()
{
	this->m_level += 1;
	this->m_maxHealth += 10;
	this->m_currentHealth += 10;
	this->m_attack += 2;
	this->m_defense += 4;
	this->m_currentExp = 0;
	this->m_maxExp += 2;
}

string Pokemon::GetName()
{
	return this->m_name;
}

void Pokemon::SetName(string name)
{
	this->m_name = name;
}

int Pokemon::GetType()
{
	return this->m_type;
}

void Pokemon::SetType(int type)
{
	this->m_type = type;
}

int Pokemon::GetMaxHP()
{
	return this->m_maxHealth;
}

void Pokemon::SetMaxHP(int health)
{
	this->m_maxHealth = health;
}

int Pokemon::GetCurrentHP()
{
	return this->m_currentHealth;
}

void Pokemon::SetCurrentHP(int health)
{
	if (health == 0)
	{
		this->m_alive = false;
	}
	this->m_currentHealth = health;
}

int Pokemon::GetLevel()
{
	return this->m_level;
}

void Pokemon::SetLevel(int level)
{
	this->m_level = level;
}

int Pokemon::GetAtk()
{
	return this->m_attack;
}

void Pokemon::SetAtk(int atk)
{
	this->m_attack = atk;
}

int Pokemon::GetDef()
{
	return this->m_defense;
}

void Pokemon::SetDef(int def)
{
	this->m_defense = def;
}

int Pokemon::GetAtkSpeed()
{
	return this->m_attackSpeed;
}

void Pokemon::SetAtkSpeed(int atkSpeed)
{
	this->m_attackSpeed = atkSpeed;
}

int Pokemon::GetCurrentExp()
{
	return this->m_currentExp;
}

void Pokemon::SetCurrentExp(int exp)
{
	this->m_currentExp = exp;
}

int Pokemon::GetMaxExp()
{
	return this->m_maxExp;
}

void Pokemon::SetMaxExp(int exp)
{
	this->m_maxExp = exp;
}

bool Pokemon::IsAlive()
{
	return this->m_alive;
}

void Pokemon::SetState(bool state)
{
	this->m_state = state;
}

bool Pokemon::GetState()
{
	return this->m_state;
}

Skill * Pokemon::GetSkillById(int id)
{
	return this->m_skills.at(id);
}

int Pokemon::GetCountSkills()
{
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		if (this->m_skills.at(i) != nullptr)
		{
			count++;
		}
	}
	return count;
}

void Pokemon::Attack(Pokemon * target, Skill * skill)
{
	auto listener = CallFunc::create([this, target, skill]() {
		if (skill->GetState() == true)
		{
			float type = 1;
			if (skill->GetIdType() == target->GetType())
			{
				type = 0.5;
			}
			else if (skill->GetIdType() == MyObject::TYPE_FIRE)
			{
				if (target->GetType() == MyObject::TYPE_GRASS || target->GetType() == MyObject::TYPE_ELECTRIC)
				{
					type = 2;
				}
				else if (target->GetType() == MyObject::TYPE_WATER || target->GetType() == MyObject::TYPE_FLYING)
				{
					type = 0.5;
				}
			}
			else if (skill->GetIdType() == MyObject::TYPE_WATER)
			{
				if (target->GetType() == MyObject::TYPE_FIRE || target->GetType() == MyObject::TYPE_FLYING)
				{
					type = 2;
				}
				else if (target->GetType() == MyObject::TYPE_ELECTRIC || target->GetType() == MyObject::TYPE_GRASS)
				{
					type = 0.5;
				}
			}
			else if (skill->GetIdType() == MyObject::TYPE_GRASS)
			{
				if (target->GetType() == MyObject::TYPE_WATER || target->GetType() == MyObject::TYPE_FLYING)
				{
					type = 2;
				}
				else if (target->GetType() == MyObject::TYPE_FIRE || target->GetType() == MyObject::TYPE_ELECTRIC)
				{
					type = 0.5;
				}
			}
			else if (skill->GetIdType() == MyObject::TYPE_ELECTRIC)
			{
				if (target->GetType() == MyObject::TYPE_WATER || target->GetType() == MyObject::TYPE_GRASS)
				{
					type = 2;
				}
				else if (target->GetType() == MyObject::TYPE_FIRE || target->GetType() == MyObject::TYPE_FLYING)
				{
					type = 0.5;
				}
			}
			else if (skill->GetIdType() == MyObject::TYPE_FLYING)
			{
				if (target->GetType() == MyObject::TYPE_FIRE || target->GetType() == MyObject::TYPE_ELECTRIC)
				{
					type = 2;
				}
				else if (target->GetType() == MyObject::TYPE_WATER || target->GetType() == MyObject::TYPE_GRASS)
				{
					type = 0.5;
				}
			}
			else if (skill->GetIdType() == MyObject::TYPE_DRAGON)
			{
				if (target->GetType() == MyObject::TYPE_DRAGON)
				{
					type = 2;
				}
			}
			float crit = this->RandomFloat(1.1, 2);
			float randomm = this->RandomFloat(1, 1.5);
			int damage = ((2 * skill->GetPower() * this->m_attack / target->GetDef()) / 5 + 2) * type * crit * randomm;
			if (damage >= target->GetCurrentHP())
			{
				target->SetCurrentHP(0);
			}
			else
			{
				target->SetCurrentHP(target->GetCurrentHP() - damage);
			}
			skill->SetVisible(false);
			skill->SetState(false);
			this->m_state = true;
			skill->GetSpriteFront()->stopActionByTag(11);
		}
	});
	auto rp = RepeatForever::create(Spawn::create(listener, nullptr));
	rp->setTag(11);
	skill->GetSpriteFront()->runAction(rp);
	skill->RunAnimate();
}
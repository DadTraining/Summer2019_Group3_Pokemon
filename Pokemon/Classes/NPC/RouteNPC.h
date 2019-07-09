#pragma once
#include "MyObject.h"
#include <vector>
#include "NPC.h"

using namespace std;

class RouteNPC : public NPC
{
private:
	string name;
	vector<MyObject*> m_pokemons;
public:
	RouteNPC(Layer* layer);
	~RouteNPC();
	void Init() override;
	void Update(float deltaTime) override;
};
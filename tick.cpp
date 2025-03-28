#include "tick.h"

vector<Tickable*> Tickable::allTicks = vector<Tickable*>();

Tickable::Tickable()
{
	allTicks.push_back(this);
}

Tickable::~Tickable()
{
	allTicks.erase(remove(allTicks.begin(), allTicks.end(), this));
}

void Tickable::Tick()
{
}

vector<Tickable*>* Tickable::GetAllTicks()
{
	return &allTicks;
}

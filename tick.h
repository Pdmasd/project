#ifndef TICK_H
#define TICK_H

#include <vector>
#include <algorithm>

using namespace std;

class Tickable
{
public:
	Tickable();
	~Tickable();

	virtual void Tick();

	static vector<Tickable*>* GetAllTicks();
private:
	static vector<Tickable*> allTicks;
};

#endif

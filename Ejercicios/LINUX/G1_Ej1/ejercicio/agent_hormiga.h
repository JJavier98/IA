#ifndef AGENT__
#define AGENT__

#include <string>
#include <iostream>
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
	    FEROMONA_=false;
	}

	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
		actIDLE
	};

	void Perceive(const Environment &env);
	ActionType Think();

private:
	bool FEROMONA_;

	int girado_a_la_izda = 0,
		girado_a_la_dcha = 0;
};

string ActionStr(Agent::ActionType);

#endif

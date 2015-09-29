#pragma once

#include "trial.h"

class Session
{
public:
	Session();
	~Session();
private:
	Trial* trials_;
	int partial_score_;
	int absolute_score_;
};


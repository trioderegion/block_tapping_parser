#pragma once

#include "session.h"

class Subject
{
public:
	Subject();
	~Subject();
private:
	Session* sessions_;
};


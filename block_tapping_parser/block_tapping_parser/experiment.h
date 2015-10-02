#pragma once

#include "subject.h"
#include "common.h"
#include <vector>

class Experiment
{
public:
	Experiment();
	~Experiment();
private:
	std::vector<Subject> subjects_;
};

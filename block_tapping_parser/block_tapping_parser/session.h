#pragma once
#include <string>
#include <vector>
#include "trial.h"


class Session
{
public:
	Session(std::string, std::vector<std::string>);
	~Session();
  int Score(std::string, std::vector<std::string>);
private:
	std::vector<Trial> trials_;
	int partial_score_;
	int absolute_score_;
};

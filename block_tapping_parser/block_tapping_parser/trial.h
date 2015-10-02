#pragma once
#include <string>
class Trial
{
public:
	Trial(std::string, std::string);
	~Trial();
	int Score();
private:
	int trial_number_;
	int partial_score_;
	int absolute_score_;
	int span_size_;
	char * ans_;
	char * response_;
  std::string header_;
  std::string trialdata_;
};



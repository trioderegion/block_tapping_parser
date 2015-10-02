#pragma once
#include <string>
class Trial
{
public:
	Trial();
	~Trial();
	int Score(std::string , std::string);
private:
	int trial_number_;
	int partial_score_;
	int absolute_score_;
	int span_size_;
	char * ans_;
	char * response_;
};



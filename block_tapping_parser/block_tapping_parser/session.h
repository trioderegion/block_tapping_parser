#pragma once
#include <string>
#include <vector>
#include "trial.h"

struct Result
{
  int value;
  std::string name; //ex. BT_part4_1
  bool type; //false = partial, true = absolute
  Result(int v, std::string n, bool t) : value(v), name(n), type(t) {}
};

class Session
{
public:
	Session(std::string, std::vector<std::string>);
	~Session();
  int Score();
private:
	std::vector<Trial> trials_;
  std::vector<std::string> sessiondata_;
	int partial_score_;
	int absolute_score_;
  int session_number_;
  std::string header_;
  std::vector<Result> results_;
};

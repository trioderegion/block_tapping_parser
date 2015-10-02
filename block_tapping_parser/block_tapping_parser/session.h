#pragma once
#include <string>
#include <vector>
#include "trial.h"
#include "common.h"


class Session
{
public:
	Session(std::string, std::vector<std::string>);
	~Session();
  int Score();
  int GetPartialScore();
  int GetAbsoluteScore();
  int GetSessionNumber();
  bool operator < (const Session& sess) const
  {
    return (session_number_ < sess.session_number_);
  }
private:
	std::vector<Trial> trials_;
  std::vector<std::string> sessiondata_;
	int partial_score_;
	int absolute_score_;
  int session_number_;
  std::string header_;
  std::vector<Result> results_;
};

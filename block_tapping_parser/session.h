#pragma once
#include <string>
#include <vector>
#include "trial.h"
#include "common.h"


class Session
{
public:
	Session(std::string, std::vector<std::string>);
  Session();
	~Session();
  int Score();
  int GetSessionNumber();
  bool operator < (const Session& sess) const
  {
    return (session_number_ < sess.session_number_);
  }
  std::vector<Result> GetTrialResults();
private:
	std::vector<Trial> trials_;
  std::vector<std::string> sessiondata_;
  int session_number_;
  std::string header_;

  //part and abs scores for this SESSION (ex. BT_part_1)
	Result partial_score_;
	Result absolute_score_;

  //result container for all TRIAL results (organized into spans)
  //ex. BT_part4_1
  std::vector<Result> results_;
};

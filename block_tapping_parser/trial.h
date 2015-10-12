#pragma once
#include <string>
class Trial
{
public:
	Trial(std::string, std::string);
	~Trial();
	int Score();

  //overloaded < operator to allow sorting by
  //spansize_
  bool operator < (const Trial& trial) const
  {
    return (span_size_ < trial.span_size_);
  }
  int GetSpanSize();
  int GetPartialScore();
  int GetAbsoluteScore();
  int GetTrialNumber();
private:
	int trial_number_;
	int partial_score_;
	int absolute_score_;
	int span_size_;
  std::string header_;
  std::string trialdata_;
};



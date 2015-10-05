#pragma once

#include "session.h"
#include "common.h"
#include <vector> 
#include <string>

class Subject
{
public:
	Subject(std::string, std::vector<std::string>);
	~Subject();
  void Score();
  bool operator < (const Subject& sub) const
  {
    return (subject_number_ < sub.subject_number_);
  }
  unsigned int GetSessionsPresent();
  std::vector<Result> GetAllResults();
  int GetSubjectNumber();
private:
	std::vector<Session> sessions_;
  std::vector<std::string> subjectdata_;
  std::string header_;
  int subject_number_;
  unsigned int sessions_present_;

  //result container for all SESSION results
  std::vector<Result> results_;
};


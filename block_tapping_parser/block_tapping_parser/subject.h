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
  unsigned int Score();

private:
	std::vector<Session> sessions_;
  std::vector<std::string> subjectdata_;
  std::string header_;
  int subject_number_;

  //result container for all SESSION results
  std::vector<Result> results_;
};


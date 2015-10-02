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
  int Score();

private:
	std::vector<Session> sessions_;
  std::vector<std::string> subjectdata_;
  std::string header_;
  int subject_number_;
  std::vector<Result> results_;
};


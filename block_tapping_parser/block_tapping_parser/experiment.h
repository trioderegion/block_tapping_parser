#pragma once

#include "subject.h"
#include "common.h"
#include <vector>
#include <fstream>
#include <string>

class Experiment
{
public:
	Experiment(std::string , std::string);
	~Experiment();
  void Open();
  int Write();
  int Score();
private:
	std::vector<Subject> subjects_;
  std::ifstream inputfile_;
  std::ofstream outputfile_;
  std::string inputpath_;
  std::string outputpath_;
  std::string header_;
  std::vector<std::string> data_;

  std::vector<std::string> GenerateOutputHeader();
  std::vector<std::vector<std::string>> GenerateOutputData(std::vector<std::string>);
  void WriteOutputLine(std::vector<std::string>, char, char);
  void WriteOutputLine(std::vector<std::vector<std::string>>, char, char);
};

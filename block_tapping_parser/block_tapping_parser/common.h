#pragma once
#include <string>
#include <vector>

std::vector<std::string> &split(const std::string &, char , std::vector<std::string> &);
std::vector<std::string> split(const std::string, char);
int FindColumn(std::string, std::string);
std::string ReadCellAsString(std::string, std::string, std::string);
int ReadCellAsNum(std::string, std::string, std::string);

struct Result
{
  int value;
  std::string name; //ex. BT_part4_1
  bool type; //false = partial, true = absolute
  Result(int v, std::string n, bool t) : value(v), name(n), type(t) {}
};
/* Defines data file header names for parsing */


//const string experimentname ("ExperimentName");
//
extern const std::string experimentname; 
extern const std::string subjectnumber;
extern const std::string sessionnumber;
extern const std::string trialnumber;
extern const std::string keyprefix;
extern const std::string answerprefix;
extern const std::string spansize;


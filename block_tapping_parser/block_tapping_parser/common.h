#pragma once
#include <string>
#include <vector>

std::vector<std::string> &split(const std::string &, char , std::vector<std::string> &);
std::vector<std::string> split(const std::string, char);
int FindColumn(std::string, std::string);
std::string ReadCellAsString(std::string, std::string, std::string);
int ReadCellAsNum(std::string, std::string, std::string);

/* Defines data file header names for parsing */

//const string experimentname ("ExperimentName");
//const string subjectnumber ("Subject");
//const string sessionnumber ("Session");

extern const std::string experimentname; 
extern const std::string subjectnumber;
extern const std::string sessionnumber;
extern const std::string trialnumber;
extern const std::string keyprefix;
extern const std::string answerprefix;
extern const std::string spansize;


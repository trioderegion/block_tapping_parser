#include "stdafx.h"
#include "experiment.h"
#include "subject.h"
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace std;

Experiment::Experiment(string inputpath, string outputpath) : inputpath_(inputpath), outputpath_(outputpath)
{
}


Experiment::~Experiment()
{
}

//opens files and parses rows into individual strings
//for scoring
int Experiment::Open()
{
  string data;
  ifstream inputfile;
  inputfile.open(this->inputpath_);
  if (inputfile.good())
  {
    //read header (must be first line)
    getline(inputfile, this->header_);
    cout << "found header: " << this->header_;

    //read each row after header and add to data_ vector
    this->data_.clear();
    while (getline(inputfile, data))
    {
      this->data_.push_back(data);
    }
    inputfile.close();
    return 0;
  }
  else return -1;
}

int Experiment::Score()
{
  std::vector<std::vector<std::string>> subjectgroups;
  vector<int> subjectpos;
  int subjectnum;
  int loc;
  int counter;
  int size;

  //loop over all raw data rows and organize into groups of subjects
  //TODO: This "determine if present" operation may be better handled with a map or set
  for (vector<string>::iterator it = this->data_.begin(); it != this->data_.end(); ++it)
  {
    subjectnum = ReadCellAsNum(this->header_,*it,subjectnumber); //read this subject's number
    loc = -1; //flag location as needing a new bin (default) 
    counter=0; //start position to insert at 0 (looping variable)
    for (vector<int>::iterator jt = subjectpos.begin(); jt != subjectpos.end(); ++jt)
    {
      //if we find the subject number in the list of known subject numbers,
      //mark the location and stop searching
      if (*jt == subjectnum)
      {
        loc = counter;
        break;
      }
      counter++; //increment current position flag in vector
    }

    //if the location variable is still -1, we need a new bin for this subject
    if (loc == -1)
    {
      subjectpos.push_back(subjectnum); //add subject number to subject number list
      size = subjectpos.size();
      subjectgroups.resize(size); //increase size of groups to match unique subject numbers
      subjectgroups.at(size-1).push_back(*it); //add new data string to the new subject entry
    }
    else //we found the location of data for this subject, simply add more data
    {
      subjectgroups.at(loc).push_back(*it);
    }
  }

  //we now have a vector(subject) of vectors (subject data) that represents all
  //data in the input file.  Create and score subjects.
  for (vector<vector<string>>::iterator it = subjectgroups.begin(); it != subjectgroups.end(); ++it)
  {
    Subject* newsubject = new Subject(this->header_,*it); //create new Subject with all of its raw data
    newsubject->Score();

    this->subjects_.push_back(*newsubject);

  }
  
  //sort in ascending order
  sort(this->subjects_.begin(),this->subjects_.end());

  return 0;
}

int Experiment::Write()
{
  vector<string> output_header;
  vector<vector<string>> output_data;

  //open the output file as truncated (erase any existing file)
  this->outputfile_.open(this->outputpath_,ofstream::trunc);

  if (this->outputfile_.is_open())
  {

    //write header (column names)
    output_header = GenerateOutputHeader();
    WriteOutputLine(output_header,',','\n');

    //write all data lines
    output_data = GenerateOutputData(output_header);
    WriteOutputLine(output_data,',','\n');
  }
  else
  {
    return -1; //return -1 for file open error
  }
  
  outputfile_.close();
  return 0;
}

//will create a superset of header names
std::vector<std::string> Experiment::GenerateOutputHeader()
{
  vector<string> outputheader;
  vector<Result> results;
  outputheader.push_back("Subject"); //place first column header for subject number
  
  //ensure that all possible output variable names are present
  for (vector<Subject>::iterator it = this->subjects_.begin(); it != this->subjects_.end(); ++it)
  {
    results = it->GetAllResults();

    //loop over all of a subject's results and add any result names not already seen
    //TODO: Implement outputheader as a 'set' container.  This is a basic set operation on "output header" -- pushing an existing entry does nothing
    for (vector<Result>::iterator jt = results.begin(); jt != results.end(); ++jt)
    {
      //has this variable name been recorded already?
      if (std::find(outputheader.begin(), outputheader.end(), jt->name)==outputheader.end())
      {
        //can't find it, need to add it
        outputheader.push_back(jt->name);
      }
    }
  }
  
  return outputheader;
}

vector<vector<string>> Experiment::GenerateOutputData(vector<string> header)
{
  vector<vector<string>> outputdata;
  vector<string> singleline;
  vector<Result> results;
  ostringstream buffer;
 

  for (vector<Subject>::iterator it = this->subjects_.begin(); it != this->subjects_.end(); ++it)
  {
    //create NULL containers in which to insert variable values.
    //any empty session variables will remain NULL.
    singleline.clear();
    singleline.resize(header.size());

    //Read subject number and record as first entry
    buffer.str(std::string());
    buffer << it->GetSubjectNumber();
    singleline.at(0) = buffer.str();


    //loop over all variables in the header (after first, which is subject number)
    //find the corresponding variable name in the session, and record the value 
    results = it->GetAllResults();
    for (vector<string>::iterator jt = header.begin(); jt != header.end(); ++jt)
    {
      vector<Result>::iterator found = results.end();
      string columnname = *jt;

      //search result list for the result named the same as the current header column
      found = std::find_if(results.begin(), results.end(), [columnname](Result const& r) {
        return r.name == columnname;
      });
      //if the find function found a result named the current column name (jt)
      //store the result value in the corresponding position in the current line (singleline)
      if ( found != results.end())
      {
        buffer.str(std::string());
        buffer << found->value;
        singleline.at(jt-header.begin()) = buffer.str(); 
      }
    }
    //add this subject's data to the total output data
    outputdata.push_back(singleline);
  }
  return outputdata;
}

//Args: vector string of elements, character delimiter, character end of line
int Experiment::WriteOutputLine(vector<string> elements, char delim, char eol)
{
  //write "<element><delim>..n..<eol>" 
  for (vector<string>::iterator it = elements.begin(); it != elements.end(); ++it)
  {
    //write "column_name,"
    this->outputfile_ << *it << delim;
  }
  this->outputfile_ << eol;
  return 0;
}

int Experiment::WriteOutputLine(vector<vector<string>> elements, char delim, char eol)
{
   
  for (vector<vector<string>>::iterator it = elements.begin(); it != elements.end(); ++it)
  {
    WriteOutputLine(*it,delim,eol);
  }
  return 0;
}
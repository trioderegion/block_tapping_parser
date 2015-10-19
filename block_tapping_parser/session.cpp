#include "stdafx.h"
#include "session.h"
#include "trial.h"
#include "common.h"
#include <iterator>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace std;

const string sessionnumber ("Session");

Session::Session(string header, vector<string> sessiondata)
{
  header_ = header; 

  //pre-fill type flags for known session results
  partial_score_.type=false;
  absolute_score_.type=true;

  //copy input vector into empty private data member
  std::copy(sessiondata.begin(), sessiondata.end(), std::back_inserter(sessiondata_));
  session_number_=-1;
}

Session::Session()
{
  this->header_=std::string();
  this->sessiondata_.resize(1);
  this->results_.resize(0);
  session_number_=-1;
}


Session::~Session()
{
}

int Session::GetSessionNumber() { return this->session_number_; }
vector<Result> Session::GetTrialResults() {return this->results_;}

//Will loop over all provided trials in the vector,
//score them, the score the session
int Session::Score()
{
  unsigned int i = 0;
  int runningpart = 0;
  int runningabs = 0;
  ostringstream resultname;

  //directly populate simple fields
  this->session_number_ = ReadCellAsNum(this->header_,this->sessiondata_.at(0),sessionnumber);
  //loop over all provided trials in this session and score the trials
  for (vector<string>::iterator it = sessiondata_.begin(); it != sessiondata_.end(); ++it)
  {
    //construct a new Trial object, score it, and insert it into the
    //list of trials in this session
    Trial* newtrial = new Trial(header_,*it);
    
    newtrial->Score();
    //trial number = -1 if this is a practice run
    if (newtrial->GetTrialNumber() != -1)
    {
      trials_.push_back(*newtrial);
    }
    else
    {
      cout << " [practice]";
    }
  }
  
  //sort trials by the overloaded '<' operator, which will sort by spansize
  sort(trials_.begin(), trials_.end());

  //create a result container for every span size
  while (i<trials_.size())
  {
    //sum partial and absolute score from this trial
    runningpart += trials_.at(i).GetPartialScore();
    runningabs += trials_.at(i).GetAbsoluteScore();
    
    //check if this is the last trial for a given span (pre-sorted, always linear)
    //short circuit a possible out of range check
    if (
        ((i != this->trials_.size()-1) && 
          (this->trials_.at(i).GetSpanSize() != this->trials_.at(i + 1).GetSpanSize())) ||
        (i == this->trials_.size()-1)
        )
    {
      //construct span result name string of format <type><spansize>_<session#>
      resultname.str(std::string());
      resultname << "BT_part" << this->trials_.at(i).GetSpanSize() << "_" << this->session_number_;
      cout << "\nconstructing results for:" << resultname.str();
      this->results_.push_back(Result(runningpart,resultname.str(),false));

      resultname.str(std::string());
      resultname << "BT_abs" << this->trials_.at(i).GetSpanSize() << "_" << this->session_number_;
      cout << "\nconstructing results for:" << resultname.str();
      this->results_.push_back(Result(runningabs,resultname.str(),true));

      //reset counters
      runningpart = 0, runningabs = 0;
    }
    
    i++;
    
  }

  runningpart = 0, runningabs = 0; //reset for re-use

  //have now constructed score containers for all spans
  for (vector<Result>::iterator it = this->results_.begin(); it != this->results_.end(); ++it)
  {
    //TRUE = absolute score, FALSE = partial
    if (it->type) runningabs+=it->value;
    else runningpart+=it->value;
  }

  //populate session absolute and partial score containers
  resultname.str(std::string());
  resultname << "BT_abs_" << this->session_number_;
  absolute_score_.name = resultname.str();
  absolute_score_.value = runningabs;

  resultname.str(std::string());
  resultname << "BT_part_" << this->session_number_;
  partial_score_.name = resultname.str();
  partial_score_.value = runningpart;
  
  resultname.clear();
  return 0;
}
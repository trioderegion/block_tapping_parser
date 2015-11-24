#include "stdafx.h"
#include "subject.h"
#include "session.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>

using namespace std;

const string subjectnumber("Subject");

Subject::Subject(string header, vector<string> subjectdata)
{
  this->header_ = header;
  this->subject_number_ = -1;
  //copy data to member variable
  std::copy(subjectdata.begin(),subjectdata.end(),std::back_inserter(this->subjectdata_));
  sessions_present_=0;

}


Subject::~Subject()
{
  //TODO should deallocate vectors
}

int Subject::GetSubjectNumber()
{
  return this->subject_number_;
}

//will loop over all provided trials
void Subject::Score()
{
  ostringstream resultname;

  //need to split all subject's data into groups of sessions, create a session object, then score it
  vector<vector<string>> sessiongroups;
  int totalsessions = 0;

  //store subject number (all provided session data should correspond to the same subject)
  this->subject_number_ = ReadCellAsNum(this->header_,this->subjectdata_.at(0),subjectnumber);

  //split up session numbers into groups to create Session object from
  for (vector<string>::iterator it = this->subjectdata_.begin(); it != this->subjectdata_.end(); ++it)
  {
    int read_session_num; 

    //for each data row (subjectdata), identify session number,
    //and assign to the appropriate position in the sessiongroups vector
    read_session_num = ReadCellAsNum(this->header_,*it,sessionnumber);
    cout << "\nReading session number " << read_session_num;

    //what is the current largest number of sessions?
    if (totalsessions < read_session_num) totalsessions = read_session_num;
    try
    {
      //try to insert this individual row into the correct place in the group of sessions
      cout << "\nstoring into existing session";
      (sessiongroups.at(read_session_num-1)).push_back(*it);
      
    }
    //catch an out of range exception
    //means that our sessiongroup vector hasn't seen this session number yet
    //should occur on first occurance of a new session number
    //catch (const std::out_of_range& oor)
    catch(...)
    {
      cout << "\nhandling new session number: " << read_session_num;

      sessiongroups.resize(totalsessions); //allocate un-init'd space to place
      (sessiongroups.at(read_session_num-1)).push_back(*it); //place new row data
    }
  }

  cout << "\nAll sessions read for subject " << this->GetSubjectNumber();
  //create Session objects, score them, and add them to group of sessions
  //that correspond to a single subject
  for (vector<vector<string>>::iterator it = sessiongroups.begin(); it != sessiongroups.end(); ++it)
  {
    if (it->size()!=0)
    {

      //create and score session
      Session* newsession = new Session(this->header_, *it);
      newsession->Score();
      cout << "\nCreating and scoring session " << newsession->GetSessionNumber();

      //mark session number as present
      this->sessions_present_ |= (1 << newsession->GetSessionNumber());

    //add this session to list of sessions for this subject
      this->sessions_.push_back(*newsession);
    }
    else
    {
      cout << "\nIgnoring empty Session";
    }
  }
  
  cout << "\nsorting sessions";
  //sort and arrange by acending session number
  sort(this->sessions_.begin(),this->sessions_.end());

  //populate session scores
  for (vector<Session>::iterator it = this->sessions_.begin(); it != this->sessions_.end(); ++it)
  {
    int runningabs=0;
    int runningpart=0; 
    //read trial results
    vector<Result> trial_results = it->GetTrialResults();
    for (vector<Result>::iterator jt = trial_results.begin(); jt != trial_results.end(); ++jt)
    {
      if (jt->type) //if absolute score (TODO: clearer if I added a helper function and enums
      {
        runningabs += jt->value;
      }
      else //if partial score
      {
        runningpart += jt->value;
      }
    }
    resultname.str(std::string());
    resultname << "BT_abs_" << it->GetSessionNumber();
    cout << "\nconstructing results for:" << resultname.str();
    this->results_.push_back(Result(runningabs, resultname.str(), true));
    resultname.str(std::string());
    resultname << "BT_part_" << it->GetSessionNumber();
    cout << "\nconstructing results for:" << resultname.str();
    this->results_.push_back(Result(runningpart, resultname.str(), false));
  }
  
    return;
}

std::vector<Result> Subject::GetAllResults()
{
  //insert session results first (easy) 
  vector<Result> current_results = this->results_;

  //loop over all sessions and collect span results
  for (vector<Session>::iterator it = this->sessions_.begin(); it != this->sessions_.end(); ++it)
  {
    vector<Result> trial_results = it->GetTrialResults();
    current_results.insert(current_results.end(), trial_results.begin(), trial_results.end());
  }

  return current_results;
}
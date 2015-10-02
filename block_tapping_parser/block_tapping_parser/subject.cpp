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

}


Subject::~Subject()
{
}

//will loop over all provided trials
int Subject::Score()
{
  //need to split all subject's data into groups of sessions, create a session object, then score it
  int runningpart = 0;
  int runningabs = 0;
  vector<vector<string>> sessiongroups;
  int totalsessions = 0;
  ostringstream resultname;

  //store subject number (all provided session data should correspond to the same subject)
  this->subject_number_ = ReadCellAsNum(this->header_,this->subjectdata_.at(0),subjectnumber);

  //split up session numbers into groups to create Session object from
  for (vector<string>::iterator it = this->subjectdata_.begin(); it != this->subjectdata_.end(); ++it)
  {
    int read_session_num; 
    //for each data row (subjectdata), identify session number,
    //and assign to the appropriate position in the sessiongroups vector
    read_session_num = ReadCellAsNum(this->header_,*it,subjectnumber);

    //what is the current largest number of sessions?
    if (totalsessions < read_session_num) totalsessions = read_session_num;
    try
    {
      //try to insert this individual row into the correct place in the group of sessions
      (sessiongroups.at(read_session_num-1)).push_back(*it);
    }
    //catch an out of range exception
    //means that our sessiongroup vector hasn't seen this session number yet
    //should occur on first occurance of a new session number
    catch (const std::out_of_range& oor)
    {
      cout << "handling new session number from error:" << oor.what();

      sessiongroups.reserve(totalsessions); //allocate un-init'd space to place
      (sessiongroups.at(read_session_num-1)).push_back(*it); //place new row data
    }
  }

  //create Session objects, score them, and add them to group of sessions
  //that correspond to a single subject
  for (vector<vector<string>>::iterator it = sessiongroups.begin(); it != sessiongroups.end(); ++it)
  {
    Session newsession = Session::Session(this->header_,*it);
    newsession.Score();
    this->sessions_.push_back(newsession);
  }
  
  //sort and arrange by acending session number
  sort(this->sessions_.begin(),this->sessions_.end());

}
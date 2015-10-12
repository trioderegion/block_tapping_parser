#include "stdafx.h"
#include "trial.h"
#include "common.h"
#include <string>
#include <iostream>

using namespace std;

const string trialnumber ("TrialList");
const string keyprefix ("ans");
const string answerprefix ("Response");
const string spansize ("SpanSize");


Trial::Trial(string header, string trialdata)
{
	trial_number_ = 0;
	partial_score_ = 0;
	absolute_score_ = 0;
  span_size_ = 0;
  header_ = header;
  trialdata_ = trialdata;	
}



Trial::~Trial()
{
}

int Trial::GetSpanSize()
{
  return span_size_;
}

int Trial::GetPartialScore()
{
  return partial_score_;
}

int Trial::GetAbsoluteScore()
{
  return absolute_score_;
}

int Trial::GetTrialNumber()
{
  return this->trial_number_;
}

//Given two comma separated strings, will parse and score an individual trial
//comma-seperated columns must match between trialData and header (come from same file)
int Trial::Score()
{
  //initialize string to -1 to indicate an error when its
  //coverted to a number to store in the object
  string trial_num_string ("-1");
  string key_string ("-1");
  string answer_string ("-1");
  string key_lookup ("-1");
  string answer_lookup ("-1");
  int i = 0;
  bool done = false;
  int found = 0;

  //directly populate simple fields
  try
  {
    this->trial_number_ = ReadCellAsNum(this->header_, this->trialdata_, trialnumber);
  }
  catch (...)
  {
    this->trial_number_ = -1;
  }
  try
  {
    this->span_size_ = ReadCellAsNum(this->header_, this->trialdata_, spansize);
  }
  catch (...)
  {
    this->span_size_ = 0;
  }

  cout << "\nScoring trial " << this->trial_number_ << " (span: " << this->span_size_ << ")";
  while (!done)
  {
    answer_lookup = answerprefix + to_string(i+1); //generate header name to read, which is always +1
    answer_string = ReadCellAsString(this->header_, this->trialdata_,answer_lookup);
    if (answer_string == NOT_FOUND) done = true; //end if we cant find the Response column (should mean we are done)
    //If there is a response found, compare to expected key text
    else if (!answer_string.empty())
    {
      found++; //mark a found answer
      key_lookup = keyprefix + to_string(i); //generate header name that contains the key
      key_string = ReadCellAsString(this->header_, this->trialdata_, key_lookup); //Grab correct answer (key)
      if (key_string == answer_string) this->partial_score_++; //if correct, increment partial credit

    }
    //either couldn't find the name or it had empty data
    else
    {

      if (i>500 || found>100) //protect ourselves against bad code
      {
        this->absolute_score_ = -2;
        this->partial_score_ = -2;
        done = true; 
      }
      //if we havent found all of the items (valid answer blocks are contiguous
      //and should always be found in a group), then mark this trial with error flags
      else if (found < this->span_size_ && found>0)
      {
        //mark scores so that the file indicates an error
        this->absolute_score_ = -1;
        this->partial_score_ = -1;
        done=true;
      }
    }
    //havent found first item
    i++;
  }
  
  //calculate absolute score (= to partial if all answers were corret)
  if (this->partial_score_ == this->span_size_) this->absolute_score_ = this->partial_score_;
  else this->absolute_score_ = 0;

  return 0;
}

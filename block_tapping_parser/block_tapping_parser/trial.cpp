#include "trial.h"
#include "common.h"
#include <string>

using namespace std;

const string trialnumber ("TrialList");
const string keyprefix ("ans");
const string answerprefix ("Response");
const string spansize ("SpanSize");


Trial::Trial()
{
	trial_number_ = 0;
	partial_score_ = 0;
	absolute_score_ = 0;
	
}



Trial::~Trial()
{
}

//Given two comma separated strings, will parse and score an individual trial
//comma-seperated columns must match between trialData and header (come from same file)
int Trial::Score(string header, string trialdata)
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
  trial_number_ = ReadCellAsNum(header,trialdata,trialnumber);
  span_size_ = ReadCellAsNum(header,trialdata,trialnumber);

  while (!done)
  {
    answer_lookup = answerprefix + to_string(i+1); //generate header name to read
    answer_string = ReadCellAsString(header, trialdata,answer_lookup);

    //If there is a response found, compare to expected key text
    if (!answer_string.empty())
    {
      found++; //mark a found answer
      key_lookup = keyprefix + to_string(i);
      //TODO main scoring code
    }
    
    //either couldn't find the name or it had empty data
    else
    {

      //if we havent found all of the items (valid answer blocks are contiguous
      //and should always be found in a group), then mark this trial with error flags
      if (found < span_size_ && found>0)
      {
        //mark scores so that the file indicates an error
        absolute_score_ = -1;
        partial_score_ = -1;
        done=true;
      }

      //safety measure to protect aginst bad code
      else if (found > 500)
      {
        absolute_score_ = -2;
        partial_score_ = -2;
        done=true;
      }
      //havent found first item
      else 
      {
        i++; 
      }
    }
  }
  
  

  return 0;
}

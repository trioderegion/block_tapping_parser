#include "session.h"
#include "trial.h"

using namespace std;

const string sessionnumber ("Session");

Session::Session(string header, vector<string> sessiondata)
{
  absolute_score_ = 0;
  partial_score_ = 0;
}


Session::~Session()
{
}

//Will loop over all provided trials in the vector,
//score them, the score the session
int Session::Score()
{
  return 0;
}
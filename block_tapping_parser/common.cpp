#include "stdafx.h"
#include "common.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;
//following two splitting functions from  http://stackoverflow.com/questions/236129/split-a-string-in-c
vector<string> &split(const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


//given a string comprised of a comman seperated spreadsheet string, will return
//the index of the query string.  Returns -1 if not found.
int FindColumn(string cslist, string query)
{
  vector<string> columns;
  int location = -1;
  
  //create a usable list of column headers to search through
  split(cslist,',',columns);

  //iterate through columns names looking for the input query and return column number (start 0)
  for (vector<string>::iterator it = columns.begin();it!=columns.end();++it)
  {
    if (it->find(query) != std::string::npos)
    {
      location = it-columns.begin();
      break;
    }
  }
  return location;
}

//given a comma-seperated list of header and data strings and a given column name in the header
//to look for, will return the string from the corresponding data string column.
string ReadCellAsString(string csheader, string csdata, string dataname)
{
  int column = -1;
  vector<string> data_elements;

  column = FindColumn(csheader,dataname);
  split(csdata,',',data_elements);
  if (column == -1)
  {
    return NOT_FOUND;
  }
  else return data_elements[column];

}

//given a comma-seperated list of header and data strings and a given column name in the header
//to look for, will return the number from the corresponding data string column.
int ReadCellAsNum(string csheader, string csdata, string dataname)
{
  string readstring ("");

  readstring = ReadCellAsString(csheader,csdata,dataname);
  
  return stoi(readstring);
}
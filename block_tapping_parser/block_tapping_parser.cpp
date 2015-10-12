// block_tapping_parser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <vector>
#include <iostream>
#include "experiment.h"

using namespace std;

auto main(int argc, char **argv) -> int
{
  //convert to a string vector
  vector<string> args(argv, argv + argc);

  if (args.size() != 3)
  {
    cout << "2 arguments are required -- <inputPath> <outputPath>";
    return -1;
  }

  cout << "echo:" << args[1] << " -- " << args[2];

  Experiment exp = Experiment::Experiment(args.at(1), args.at(2));
  try
  {
    exp.Open();
    exp.Score();
    exp.Write();
  }
  catch (...)
  {
    cout << "[FATAL] Something went terribly, terribly wrong.";
  }
}
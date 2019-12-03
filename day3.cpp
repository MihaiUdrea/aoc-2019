// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

using Point = pair<int, int>;
struct PointData
{
  Point point;
  int dist = 0;
};
using DirSteps = pair<char, int>;
using Que = priority_queue<PointData>;
using Input = vector<vector<DirSteps>>;

bool operator <(const PointData& _Left, const PointData& _Right)
{
  
  if (_Left.dist == _Right.dist)
    return _Left.point.second < _Right.point.second;
  else 
    
    return _Left.dist > _Right.dist;
}

struct Solve {
Input vec;
Que que;
set<Point> history1, history2;
set<PointData>inters;
map<Point, int> stepsOne;
map<Point, int> stepsTwo;


int cols = 0;
int lines = 0;

Point start;

Solve(string inStr) {
  static const regex lineSepRx("\\n");
  static const regex colsSepRx(",");

  for (sregex_token_iterator iter(inStr.begin(), inStr.end(), lineSepRx, -1), end; iter != end; ++iter)
  {
    vec.emplace_back();

    string line = iter->str();
    for (sregex_token_iterator iter2(line.begin(), line.end(), colsSepRx, -1), end2; iter2 != end2; ++iter2)
    {
      char d = iter2->str()[0];
      int steps = atoi(iter2->str().substr(1).c_str());
      vec.back().push_back({ d,steps });
    }
    
  }  
};


string Do()
{
  // find start
  for (auto i : irange(0,2))
  {
    auto wire = vec[i];
    set<Point> * otherHist = &history2;
    set<Point> * thisHist = &history1;
    map<Point, int>* stepsMap = &stepsOne;
    if (i == 1)
    {
      otherHist = &history1;
      thisHist = &history2;
      stepsMap = &stepsTwo;
    }

    int dy = 0; int dx = 0;
    int st = 0;

    for (auto el : wire)
    {
      for (auto step : irange(0,el.second))
      {
        st++;

        if (el.first == 'R')
        {
          dx++;
        }
        else if (el.first == 'L')
        {
          dx--;
        }
        else if (el.first == 'D')
        {
          dy++;
        }
        else if (el.first == 'U')
        {
          dy--;
        }

        Point pt = { dx, dy };
        if (otherHist->find(pt) != otherHist->end())
          inters.insert({ pt, st });

        if (thisHist->find(pt) == thisHist->end())
        {
          thisHist->insert(pt);
          (*stepsMap)[pt] = st;
        }
        
      }

    }

  }

  int min = 100000000;
  for (auto el : inters)
  {
   if (stepsOne[el.point] + stepsTwo[el.point] < min)
      min = stepsOne[el.point] + stepsTwo[el.point];
  }

  return ToString(min);
}
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

//  REQUIRE(Solve("1,9,10,3,2,3,11,0,99,30,40,50").Do() == "3500"); // sample test
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
}


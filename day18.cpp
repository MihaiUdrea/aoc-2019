// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Priority que
//   .......
//   ....X..
//   #...X.@
//   ....X..
//   .......


//using Point = pair<int, int>;
struct PointData
{
  int keysMissing;
  int dist = 0;
  Point point;
  set<char> keys;  
  
  auto operator<=>(const PointData&) const = default;
};
using Que = priority_queue<PointData>;
using Input = vector<string>;

/** /
bool operator <(const PointData& _Left, const PointData& _Right)
{
  if (_Left.dist2 == _Right.dist2)
    return _Left.dist < _Right.dist;
  else
    return _Left.dist2 > _Left.dist2;
}
/**/
/** /
bool operator <(const PointData& _Left, const PointData& _Right)
{
  if (_Left.keys.size() == _Right.keys.size())
    if (_Left.dist == _Left.dist)
    {
      return _Left.point < _Right.point;
    }
    else
      return _Left.dist < _Right.dist;
  else    
    return _Left.keys.size() < _Left.keys.size();
}
/**/

array<Point, 4> delta = { Point{-1,0,0},{1,0,0},{0,-1,0},{0,1,0} };

struct Solve {
  Input vec;
  Que que;
  

  int cols = 0;
  int lines = 0;

  Point start;

  Solve(string inStr) {
    static const regex lineSepRx("\\n");

    for (sregex_token_iterator iter(inStr.begin(), inStr.end(), lineSepRx, -1), end; iter != end; ++iter)
    {
      string line = iter->str();
      vec.push_back(line);
    }

    cols = vec[0].size();
    lines = vec.size();

    // find start
    for (auto line : irange(0, lines))
    {
      int col = vec[line].find('@');

    }

    for (auto l : irange(0, vec.size()))
    {
      string& line = vec[l];
      for (auto c : irange(0, line.size()))
      {
        auto ch = line[c];
        if (ch == '@')
        {
          start = { c, l};          
          hall.insert(start);
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
          doors[{ c, l}] = ch;
          hall.insert({c,l});
        }
        else if (ch >= 'a' && ch <= 'z')
        {
          keys[{ c, l}] = ch;
          hall.insert({ c,l });
        }
        else if (ch >= '.')
        {
          hall.insert({ c,l});
        }
      }

    }

    for (auto h : hall)
    {
      for (auto d : delta)
      {
        auto newPoint = h + d;
        if (hall.find(newPoint) != hall.end())
        {
          next[h].push_back(newPoint);
        }
      }
    }

  };

  map<Point, char> doors;
  map<Point, char> keys;
  set<Point> hall;
  map<Point, vector<Point>> next;

  set<pair<Point, set<char>>> history;
  string Do()
  {
  
    que.push({ (int)keys.size(), 0, start, {} });
    //Input mark = vec;
    PointData best;
    while (!que.empty())
    {
      auto crEl = que.top();

      if (crEl.keys.size() == keys.size())
      {
        if (best.dist == 0 || best.dist > crEl.dist)
        {
          cout << "\nImproving: " << crEl.dist;
          best = crEl;
        }
      }

      que.pop();
      /**/
      history.insert({ crEl.point, crEl.keys });
      
      //for (auto d : delta)
      for(auto n : next[crEl.point])
      {
        PointData newDataPoint{ 0, crEl.dist + 1, n , crEl.keys};

        if (newDataPoint.dist >= 4502)
          continue;

        if (auto where = keys.find(newDataPoint.point); where != keys.end())
        {
          newDataPoint.keys.insert(where->second);
        }
        else if (auto whereD = doors.find(newDataPoint.point); whereD != doors.end())
        {
          auto neededKey = tolower(whereD->second);
          if (newDataPoint.keys.find(neededKey) == newDataPoint.keys.end())
            continue; // no key
        }
        else if (hall.find(newDataPoint.point) == hall.end())
        {
          continue; // wall
        }
        else if (history.find({ newDataPoint.point, newDataPoint.keys }) != history.end())
        {
          continue; // visited
        }

        newDataPoint.keysMissing = newDataPoint.keys.size();

        que.push(newDataPoint);
      }

      /**/
    }

    return to_string(best.dist);
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //  REQUIRE(Solve("1,9,10,3,2,3,11,0,99,30,40,50").Do() == "3500"); // sample test
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
  REQUIRE(Solve(ReadFileToString(L"sample/sample2.txt")).Do() == ReadFileToString(L"sample/result2.txt"));
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


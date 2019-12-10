// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Day 10: Monitoring Station
//.#..#
//.....
//#####
//....#
//...##

#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include <math.h> 

#define PI 3.14159265

using Point = pair<int, int>;
struct PointData
{
  Point point;
  int dist = 0;
};
using Que = priority_queue<PointData>;
using Input = vector<string>;

bool operator <(const PointData& _Left, const PointData& _Right)
{
  /*
  if (_Left.dist == _Right.dist)
    return _Left.point.second < _Right.point.second;
  else
  */
  return _Left.dist > _Right.dist;
}

struct Solve {
  Input vec;
  Que que;
  set<Point> history;
  set<Point> asteroids;

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

    for (auto i : irange(0, vec.size()))
    {
      string& line = vec[i];
      for (auto j : irange(0, line.size()))
      {
        if (line[j] == '#')
          asteroids.insert({ j,i });
      }
    }
  };


  string DoQ()
  {
    // find start
    for (auto line : irange(0, lines))
    {
      int col = vec[line].find('@');
      if (col != string::npos)
      {
        start = { col, line };
        break;
      }
    }

    que.push({ start, 0 });
    Input mark = vec;

    while (!que.empty())
    {
      auto crEl = que.top();
      que.pop();

      history.insert(crEl.point);
      mark[crEl.point.second][crEl.point.first] = '0' + crEl.dist;

      for (auto dx : irange(-1, 2))
        for (auto dy : irange(-1, 2))
        {
          Point pt = { crEl.point.first + dx, crEl.point.second + dy };
          if (pt.first >= cols || pt.first <= -1 || pt.second >= lines || pt.second <= -1)
            continue;

          if (vec[pt.second][pt.first] == '#')
            return ToString(crEl.dist + 1);
          else if (vec[pt.second][pt.first] == '.' && history.find(pt) == history.end())
            que.push({ pt, crEl.dist + 1 });
        }
    }

    return "asdf";
  }

  static double GetAngle(Point crAst, Point otherAst)
  {
    Point p2 = crAst;
    Point p1 = otherAst;

    auto x = p2.second - p1.second;
    auto y = p1.first - p2.first;
    //double newAngle = atan2(y, x) * 180 / PI; 
    //return newAngle < 0 ? newAngle + 360 : newAngle; // use it with less then comparator

    double newAngle = atan2(otherAst.first-crAst.first, -crAst.second+otherAst.second) * 180 / PI;   
    return newAngle;

    //return atan2(otherAst.first - crAst.first, otherAst.second - crAst.second);
  }

  int GetDist(Point crAst, Point otherAst)
  {
    return (otherAst.second - crAst.second) * (otherAst.second - crAst.second) +
      (otherAst.first - crAst.first) * (otherAst.first - crAst.first);
  }

  using Que = priority_queue<PointData>;
  using mymap = map<double, Que, greater<double>>;

  mymap maxAnglesList;
  set<Point> maxvisible;
  set<pair<Point, Point>> maxdropped;

  string Do()
  {
    int max = 0;
    Point maxPoint;
    for (auto crAst : asteroids)
    {
      mymap anglesList;
      set<double> angles;
      set<Point> visible;
      set<pair<Point, Point>> dropped;
      for (auto otherAst : asteroids)
      {
        if (crAst == otherAst)
          continue;

        auto newAngle = GetAngle(crAst, otherAst);

        if (angles.find(newAngle) != angles.end())
        {
          //auto whereList = anglesList.find({})
          anglesList[newAngle].push({ otherAst, GetDist(crAst, otherAst) });

          for (auto shadowingAst : visible)
          {
            if (GetAngle(crAst, shadowingAst) == newAngle)
            {
              dropped.insert({ otherAst, shadowingAst });
              break;
            }
          }
        }
        else
        {
          anglesList[newAngle].push({ otherAst, GetDist(crAst, otherAst) });

          visible.insert(otherAst);
        }

        angles.insert(newAngle);
      }

      if (max < visible.size())
      {
        max = visible.size();
        maxPoint = crAst;
        maxAnglesList = anglesList;
        maxvisible = visible;
        maxdropped = dropped;
      }
    }

    return ToString(max);
  }

  string Do2(int count)
  {
    Do();

    auto listIt = maxAnglesList.begin();
    int v = 0;
    for (int step = 0; step < count; step++)
    {
      cout << listIt->second.top().point.first << ", " << listIt->second.top().point.second << endl;
      v = listIt->second.top().point.first * 100 + listIt->second.top().point.second;

      listIt->second.pop();
      listIt++;

      if (listIt == maxAnglesList.end())
        listIt = maxAnglesList.begin();
      while (listIt->second.empty())
        listIt++;
    }

    return ToString(v);
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //  REQUIRE(Solve("1,9,10,3,2,3,11,0,99,30,40,50").Do() == "3500"); // sample test
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
  REQUIRE(Solve(ReadFileToString(L"sample/sample2.txt")).Do() == ReadFileToString(L"sample/result2.txt"));
  REQUIRE(Solve(ReadFileToString(L"sample/sample3.txt")).Do() == ReadFileToString(L"sample/result3.txt"));
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part 1 Tests", "[x.]") {
  
  cout << "{ 0, 0 }, { 0, -1 })  " << Solve::GetAngle({ 0, 0 }, { 0, -1 })   << endl;
  cout << "{ 0, 0 }, { 1, -1 })  " << Solve::GetAngle({ 0, 0 }, { 1, -1 })   << endl;
  cout << "{ 0, 0 }, { 1,  0 })  " << Solve::GetAngle({ 0, 0 }, { 1,  0 })   << endl;
  cout << "{ 0, 0 }, { 1,  1 })  " << Solve::GetAngle({ 0, 0 }, { 1,  1 })   << endl;
  cout << "{ 0, 0 }, { 0,  1 })  " << Solve::GetAngle({ 0, 0 }, { 0,  1 })   << endl;
  cout << "{ 0, 0 }, { -1,  1 }) " << Solve::GetAngle({ 0, 0 }, { -1,  1 })  << endl;
  cout << "{ 0, 0 }, { -1,  0 }) " << Solve::GetAngle({ 0, 0 }, { -1,  0 }) << endl;
  cout << "{ 0, 0 }, { -1,  -1 }) " << Solve::GetAngle({ 0, 0 }, { -1,  -1 }) << endl;

  /*
  REQUIRE(Solve::GetAngle({ 0, 0 }, { 0, -1 }) == Approx(0));
  REQUIRE(Solve::GetAngle({ 0, 0 }, { 1, -1 }) == Approx(45));
  REQUIRE(Solve::GetAngle({ 0, 0 }, { 1,  0 }) == Approx(90));
  REQUIRE(Solve::GetAngle({ 0, 0 }, { 1,  1 }) == Approx(135));
  REQUIRE(Solve::GetAngle({ 0, 0 }, { 0,  1 }) == Approx(180));
  REQUIRE(Solve::GetAngle({ 0, 0 }, { -1,  1 }) == Approx(225));
  REQUIRE(Solve::GetAngle({ 0, 0 }, { -1,  0 }) == Approx(270));
  */

  //REQUIRE(Solve(ReadFileToString(L"sample/sample4.txt")).Do2() == ReadFileToString(L"sample/result4.txt"));
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2(200));
}


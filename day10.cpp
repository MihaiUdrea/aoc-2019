// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Priority que
//   .......
//   ....X..
//   #...X.@
//   ....X..
//   .......

#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include <math.h> 

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

  double GetAngle(Point crAst, Point otherAst)
  {
    double newAngle = atan2(otherAst.second - crAst.second, otherAst.first - crAst.first);


    /*if (otherAst.second - crAst.second == 0)
    {
      if (otherAst.first - crAst.first < 0)
        newAngle = -100000000000;
      else
        newAngle = +100000000000;
    }
    else
      newAngle = (double)(otherAst.first - crAst.first) / (double)(otherAst.second - crAst.second);
      */
    return newAngle;
  }

  int GetDist(Point crAst, Point otherAst)
  {
    return (otherAst.second - crAst.second) * (otherAst.second - crAst.second) +
      (otherAst.first - crAst.first) * (otherAst.first - crAst.first);
  }

  using Que = priority_queue<PointData>;
  string Do()
  {
    map<double, Que> maxAnglesList;
    set<Point> maxvisible;
    set<pair<Point, Point>> maxdropped;

    int max = 0;
    Point maxPoint;
    for (auto crAst : asteroids)
    {
      map<double, Que> anglesList;
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

  string Do2()
  {
    map<double, Que> maxAnglesList;
    set<Point> maxvisible;
    set<pair<Point, Point>> maxdropped;

    int max = 0;
    Point maxPoint;
    for (auto crAst : asteroids)
    {
      map<double, Que> anglesList;
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
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //  REQUIRE(Solve("1,9,10,3,2,3,11,0,99,30,40,50").Do() == "3500"); // sample test
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
  REQUIRE(Solve(ReadFileToString(L"sample/sample2.txt")).Do() == ReadFileToString(L"sample/result2.txt"));
  REQUIRE(Solve(ReadFileToString(L"sample/sample3.txt")).Do() == ReadFileToString(L"sample/result3.txt"));
}

TEST_CASE("Part One", "[.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part 1 Tests", "[x.]") {
  
  REQUIRE(Solve(ReadFileToString(L"sample/sample4.txt")).Do2() == ReadFileToString(L"sample/result4.txt"));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}


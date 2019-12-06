// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TDay 6: Universal Orbit Map
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

//using Point = pair<int, int>;
struct Point {
  int x;
  int y;

  auto operator <=>(const Point& _Right) const = default;
};

Point operator +(const Point& _Left, const Point& _Right)
{
  return { _Left.x + _Right.x,  _Left.y + _Right.y };
}

static int ManhDist(Point a)
{
  return abs(a.x) + abs(a.y);
}



struct DirSteps {
  char dir;
  int steps;
};


struct Astru
{
  string name;
  set<string> directOrbiting;
  set<string> indirectOrgiting;
};

struct Data
{
  set<Point> points;
  map<Point, int> dists;
  vector<DirSteps> inputDirSteps;
};

struct PointData
{
  string point;
  int dist = 0;
};

using Input = vector<Data>;
bool operator <(const PointData& _Left, const PointData& _Right)
{
  return _Left.dist > _Right.dist;
};

struct Solve {
  Input vec;
  vector<Point> inters;
  set<Astru> aster;
  set<string> names;
  map<string, set<string>> directOrbiting;
  map<string, set<string>> directOrbited;
  map<string, set<string>> indirectOrgiting;
  map<string, set<string>> indirectOrgited;
  int sum = 0;
  Solve(string inStr) {
    static const regex lineSepRx("\\n");
        
    for (sregex_token_iterator iter(inStr.begin(), inStr.end(), lineSepRx, -1), end; iter != end; ++iter)
    {
      
      string line = iter->str();

      string name = line.substr(0, line.find(')'));

      string center = line.substr(line.find(')') + 1);

      directOrbiting[name].insert(center);
      directOrbited[center].insert(name);
      sum++;

      names.insert(name);
    }
    
  };

  void Parse(string & cr, vector<string> & crPath)
  {
    for (auto e : directOrbiting[cr])
    {
      for (auto prev : crPath)
      {
        indirectOrgiting[e].insert(prev);

        indirectOrgited[prev].insert(e);
      }

      //auto v = crPath;
      //crPath.push_back(cr);
      crPath.push_back(cr);
      Parse(e, crPath);
      crPath.pop_back();
    }
  }


  string Do()
  {
    vector<string> crPath;
    string start("COM");
    Parse(start, crPath);

    //int sum = 0;
    for (auto e : indirectOrgited)
      sum += e.second.size();

    return ToString(sum);
  }


  using Que = priority_queue<PointData>;
  set<string> history;



  string Do2()
  {
    vector<string> crPath;
    string start("COM");
    Parse(start, crPath);

    Que que;
    que.push({ "YOU", 0 });

    while (!que.empty())
    {
      auto crEl = que.top();
      que.pop();
       
      history.insert(crEl.point);
      if (crEl.point == "SAN")
        return ToString(crEl.dist - 2 );

      for (auto dx : directOrbiting[crEl.point])
       if (history.find(dx) == history.end())
        que.push({ dx, crEl.dist + 1 });

      for (auto dx : directOrbited[crEl.point])
        if (history.find(dx) == history.end())
          que.push({ dx, crEl.dist + 1 });

    }

    return ToString(sum);

  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve("").Do() == "6"); // sample part 1
  //REQUIRE(Solve("").Do2() == "30"); // sample part 2
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do2() == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

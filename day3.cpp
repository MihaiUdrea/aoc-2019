// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Day 3: Crossed Wires
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

struct PointData
{
  Point point;
  int dist = 0;
};

struct DirSteps {
  char dir;
  int steps;
};


struct Data
{
  set<Point> points;
  map<Point, int> dists;
  vector<DirSteps> inputDirSteps;
};

using Input = vector<Data>;

struct Solve {
  Input vec;  
  vector<Point> inters;


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
        vec.back().inputDirSteps.push_back({ d,steps });
      }
    }
  };



  static int ManhDist(Point a)
  {
    return abs(a.x) + abs(a.y);
  }

  string Do()
  {
    static map<char, Point> delta = { {'R', {0,1}}, {'L', {0,-1}}, {'U', {-1, 0}}, {'D', {1,0}} };

    for (auto &w : vec)
    {
      auto & [hist, distMap, wire] = w;
      
      int st = 0;

      Point pt = { 0,0 };
      for (auto el : wire)
      {
        for (auto step : irange(0, el.steps))
        {
          pt = pt + delta[el.dir];

          hist.insert(pt);
          distMap[pt] = ++st;
        }
      }

    }

    auto& firstSet = vec[0].points;
    auto& secondSet = vec[1].points;
    std::set_intersection(firstSet.begin(), firstSet.end(), secondSet.begin(), secondSet.end(), back_inserter(inters));

    auto minEl = min_element(inters.begin(), inters.end(), [](auto& l, auto& r) {
      return ManhDist(l) < ManhDist(r); });

    return ToString(ManhDist(*minEl));
  }

  int TotalSteps(Point pt) 
  {
    return vec[0].dists[pt] + vec[1].dists[pt];
  }

  string Do2()
  {
    Do();

    auto minEl = min_element(inters.begin(), inters.end(), [&](auto& l, auto& r) {
      return TotalSteps(l) < TotalSteps(r); });
    
    return ToString(TotalSteps(*minEl));

  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve("R8,U5,L5,D3\r\nU7,R6,D4,L4").Do() == "6"); // sample part 1
  REQUIRE(Solve("R8,U5,L5,D3\r\nU7,R6,D4,L4").Do2() == "30"); // sample part 2
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

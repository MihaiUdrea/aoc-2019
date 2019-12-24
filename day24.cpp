// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

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


array<Point, 4> delta = { Point{-1, 0, 0}, Point{1, 0, 0}, Point{0, -1, 0}, Point{0, 1, 0}, };

struct Solve : Program{

  map<Point, vector<Point>> m2DNext;
  set<Point> bugs;
  set<Point> space;
  vector<string> inputLines;
  Solve(const string& inStr) {
  
    inputLines = GetLines(inStr);

    for(auto l : irange(0, inputLines.size()))
      for (auto c : irange(0, inputLines[l].size()))
      {
        char ch = inputLines[l][c];
        if (ch == '#')
          bugs.insert({ c, l });
        else
          space.insert({ c,l });
      }
  
    m2DNext[{1, 2}] = { Point{0,0, 1}, Point{0,1, 1}, Point{0,2, 1}, Point{0,3, 1}, Point{0,4, 1} };
    m2DNext[{2, 1}] = { Point{0,0, 1}, Point{1,0, 1}, Point{2,0, 1}, Point{3,0, 1}, Point{4,0, 1} };
    m2DNext[{3, 2}] = { Point{4,0, 1}, Point{4,1, 1}, Point{4,2, 1}, Point{4,3, 1}, Point{4,4, 1} };
    m2DNext[{2, 3}] = { Point{0,4, 1}, Point{1,4, 1}, Point{2,4, 1}, Point{3,4, 1}, Point{4,4, 1} };

    m2DNext[{0, 0}] = { Point{1,2, -1} };
    m2DNext[{0, 1}] = { Point{1,2, -1} };
    m2DNext[{0, 2}] = { Point{1,2, -1} };
    m2DNext[{0, 3}] = { Point{1,2, -1} };
    m2DNext[{0, 4}] = { Point{1,2, -1} };

    m2DNext[{0,0 }] .push_back( Point{2,1, -1});
    m2DNext[{1,0 }] .push_back( Point{2,1, -1});
    m2DNext[{2,0 }] .push_back( Point{2,1, -1});
    m2DNext[{3,0 }] .push_back( Point{2,1, -1});
    m2DNext[{4,0 }] .push_back( Point{2,1, -1});
                    
    m2DNext[{4, 0}] .push_back( Point{3,2, -1});
    m2DNext[{4, 1}] .push_back( Point{3,2, -1});
    m2DNext[{4, 2}] .push_back( Point{3,2, -1});
    m2DNext[{4, 3}] .push_back( Point{3,2, -1});
    m2DNext[{4, 4}] .push_back( Point{3,2, -1});
                    
    m2DNext[{0, 4 }].push_back( Point{2,3, -1});
    m2DNext[{1, 4 }].push_back( Point{2,3, -1});
    m2DNext[{2, 4 }].push_back( Point{2,3, -1});
    m2DNext[{3, 4 }].push_back( Point{2,3, -1});
    m2DNext[{4, 4 }].push_back( Point{2,3, -1});
  };

  string Do(int steps)
  {
    space.erase({ 2,2 });
    set<size_t> hist;
    int lm = -10;
    int lM = 10;
    for(auto minute : irange(0,steps))
    {

      if (minute == steps - 1)
      for (auto level : irange(lm, lM + 1))
      {
        vector<Point> thiLevel;
        copy_if(bugs.begin(), bugs.end(), back_inserter(thiLevel), [=](auto e) { return e.z == level; });
        if (thiLevel.empty())
          continue;

        cout << "Level: " << level;
        auto s = to2Ds(thiLevel, [](auto& l, int) {return l; }, [](auto& l) {return "#"; }, to2DsFlags::full_header, '.');
        cout << endl << s;
      }

      set<Point> ding;
      set<Point> infesting;
      for (auto bug : bugs)
      {
        int neigh = 0;
        for (auto d : delta)
        {
          Point p = d + bug;
          if (bugs.find(p) != bugs.end())
            neigh++;
          else if (p.x >= 0 && p.x < 5 && p.y >= 0 && p.y < 5 && (p.x !=2 || p.y !=2))
            space.insert(p);
        }

        // other space 
        Point bug2d = bug;
        bug2d.z = 0;
        for (auto nt : m2DNext[bug2d])
        {
          Point neighBug = nt;
          neighBug.z = bug.z + nt.z;

          if (bugs.find(neighBug) != bugs.end())
            neigh++;
          else
            space.insert(neighBug);
        }

        if (neigh != 1)
          ding.insert(bug);
      }

      for (auto sp : space)
      {
        int neigh = 0;
        for (auto d : delta)
        {
          if (bugs.find(d + sp) != bugs.end())
            neigh++;
        }

        // other space 
        Point pt2d = sp;
        pt2d.z = 0;
        for (auto nt : m2DNext[pt2d])
        {
          Point neighBug = nt;
          neighBug.z = sp.z + nt.z;

          if (bugs.find(neighBug) != bugs.end())
            neigh++;
        }

        if (neigh == 1 || neigh == 2)
          infesting.insert(sp);
      }

      for (auto dingPt : ding)
      {
        bugs.erase(dingPt);
        space.insert(dingPt);
      }

      for (auto pt : infesting)
      {
        space.erase(pt);
        bugs.insert(pt);
      }

      size_t biodiv = 0;
      if (0)
      for(auto bug : bugs)
      { 
        size_t bugId = 1 << (bug.x + bug.y * inputLines[0].size());
        biodiv += bugId;
      }

      if (0)
      if (hist.insert(biodiv).second == false)
      {
        return to_string(biodiv);
      }
    }
    return ToString(bugs.size());
  }

  string Do2()
  {

    return ToString(1);
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").JustRun({}) == 99); // print yourself 

  //REQUIRE(Solve("").Do() == "6"); // sample part 1
  //REQUIRE(Solve("").Do2() == "30"); // sample part 2
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do(10) == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do(200));
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

/* Input ---------------

##.#.
#..#.
.....
....#
#.###

 ------------Input End*/

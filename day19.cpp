// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"
#include <conio.h>

struct DirSteps {
  char dir;
  int steps;
};



enum Dir
{
  left,
  right,
  up,
  down
};

auto chars = "LR";

//using Funtion = vector<Dir>;
using Funtion = vector<pair<Dir, int>>;

struct PointData
{
  Point pt;
  Dir dir;

  vector<Dir> path;
  set<Point> hist;
  int groups = 0;


  string Print()
  {
    stringstream s;
    auto [mainProg, functions] = ComputeGroupsCount();
    s << "Main: ";
    for (auto i : mainProg)
    {
      s << "Funtion " << 'A' + i << ',';
    }
    for (auto i : irange(0, functions.size()))
    {
      auto f = functions[i];
      s << "\n" << 'A' + i << ": ";
      for (auto inst : f)
      {
        assert(inst.first < Dir::up);
        s << chars[inst.first] << "," << inst.second << ",";
      }
    }
    return s.str();
  }

  int GetMaxMem()
  {
    auto [mainProg, functions] = ComputeGroupsCount();
    int maxmem = 0;

    // main size 
    int mainSize = mainProg.size() * 1 - 1;

    maxmem = mainSize;
    for (auto f : functions)
    {
      int fSize = f.size() * 2 - 1;
      maxmem = max(maxmem, fSize);
    }
    return maxmem;
  }

  pair<vector<int>, vector<Funtion>> ComputeGroupsCount()
  {
    // split in basic instr
    pair<vector<int>, vector<Funtion>> res{};

    vector<pair<Dir, int>> rawFunctions;
    int crStep = 1;
    for (auto idx : irange(1, path.size()))
    {
      if (path[idx] == path[idx - 1])
      {
        crStep++;
      }
      else
      {
        rawFunctions.emplace_back(path[idx - 1], crStep);
        crStep = 1;
      }
    }
    // try 

    return res;
  }
};

using Que = priority_queue<PointData>;
bool operator <(const PointData& _Left, const PointData& _Right)
{

  /*if (_Left.dist == _Right.dist)
    return _Left.point.second < _Right.point.second;
  else
  */
  return _Left.groups > _Right.groups;
}

struct Data
{
  set<Point> points;
  map<Point, int> dists;
  vector<DirSteps> inputDirSteps;
};

array<Point, 4> delta = { Point{-1,0,0},{1,0,0},{0,-1,0},{0,1,0} };
struct Solve : Program {

  Solve(const string& inStr) : Program({}, { 0 }, inStr, true), progInst(inStr) {};
  string progInst;
  Point start;
  set<Point> points;
  set<Point> intersections;
  map<Point, set<pair<Point, Dir>>> next;
  int_t JustRun(vector<int_t> a)
  {
    Run();

    int line = 0;
    int col = 0;
    for (auto el : output)
    {
      cout << (char)el;

      if ((char)el == '#')
        points.insert({ col, line, 0 });
      else if ((char)el == '^')
        start = { col, line, 0 };

      if (el == 10)
      {
        line++;
        col = 0;
      }
      else
        col++;

    }

    points.insert(start);

    for (auto p : points)
    {
      bool missing = false;
      for (auto di : irange(0, delta.size()))
      {
        auto d = delta[di];
        Point newi = p + d;
        if (points.find(newi) == points.end())
          missing = true;
        else
          next[p].insert({ newi, (Dir)di });
      }

      if (!missing)
        intersections.insert(p);
    }

    int sum = 0;
    for (auto i : intersections)
      sum += i.x * i.y;

    toClipboard(sum);
    return sum;
  }

  string Do()
  {
    return ToString(1);
  }

  Dir startDir = Dir::up;

  vector<vector<Dir>> solutions;

  void Parse(vector<Dir>& path, Point pt, Dir dir, set<Point>& history)
  {
    history.insert(pt);

    path.push_back(dir);

    for (auto e : next[pt])
    {
      Parse(path, e.first, e.second, history);
    }

    path.pop_back();
  }

  Que que;

  void Explore(Que::value_type& crEl)
  {
    for (auto e : next[crEl.pt])
    {
      PointData newState = { e.first, e.second, crEl.path, crEl.hist };
      newState.hist.insert(e.first);
      newState.path.push_back(e.second);

      newState.ComputeGroupsCount();

      que.push(newState);
    }
  }

  Point GetRightDown(Point p)
  {
    int y = p.y;
    for (;;y++)
    {
      Program pr({}, { p.x, y }, progInst, true);
      pr.Run();
      if (pr.output.back() == 0)
        break;      
    }

    int x = p.x;
    for (;; x++)
    {
      Program pr({}, { x, p.y }, progInst, true);
      pr.Run();

      if (pr.output.back() == 0)
        break;
    }
    return { x,y };
  }
  string Do2()
  {
    SetPorts({ /*in*/ }, {});
    
    int x = 0;
    int y = 0;
    set<Point> beam;
    Point pt{ 835, 940 };
    if (1)
    do {
      auto res = GetRightDown(pt);
      auto dif = res - pt;

      cout << pt.x << "," << pt.y << "->" << dif.x << "," << dif.y << endl;

      auto dx = 100 - dif.x;
      auto dy = 100 - dif.y;
      pt = pt + Point{ dx, dy };
      if (dx == dy && dx == 100)
        break;
    } while (true);

    for (auto y : irange(1000, 1200))
    {
      for (auto x : irange(950, 1100))
      {
        Program p({}, { x, y }, progInst, true);
        //regA.push_back(x);
        //regA.push_back(y);
        p.Run();

        if (p.output.back() == 1)
        {
          beam.insert({ x,y });
          cout << '#';
        }
        else
          cout << '.';
        
      }
      cout << endl;
    }

    /** /
    auto read = []() {
      char c;
      cin >> std::noskipws >> c;

      
      return c;
    };

    while (Run(read, 50) == outputReady)
    {
      copy(output.begin(), output.end(),
        ostream_iterator<char>(cout));
      cout << endl;

      output.clear();
    }
    /**/
    return ToString(-1);
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").JustRun({}) == 99); // print yourself 

  //REQUIRE(Solve("").Do() == "6"); // sample part 1
  //REQUIRE(Solve("").Do2() == "30"); // sample part 2
  REQUIRE(Solve(ReadFileToString(L"input.txt")).JustRun({}) == 1);
}

TEST_CASE("Part One", "[.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[x.]") {
  // cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

/* Input ---------------

3,1033,1008,1033,1,1032,1005,1032,31,1008,1033,2,1032,1005,1032,58,1008,1033,3,1032,1005,1032,81,1008,1033,4,1032,1005,1032,104,99,1002,1034,1,1039,102,1,1036,1041,1001,1035,-1,1040,1008,1038,0,1043,102,-1,1043,1032,1,1037,1032,1042,1106,0,124,1002,1034,1,1039,101,0,1036,1041,1001,1035,1,1040,1008,1038,0,1043,1,1037,1038,1042,1105,1,124,1001,1034,-1,1039,1008,1036,0,1041,102,1,1035,1040,1002,1038,1,1043,1002,1037,1,1042,1106,0,124,1001,1034,1,1039,1008,1036,0,1041,1002,1035,1,1040,101,0,1038,1043,1002,1037,1,1042,1006,1039,217,1006,1040,217,1008,1039,40,1032,1005,1032,217,1008,1040,40,1032,1005,1032,217,1008,1039,9,1032,1006,1032,165,1008,1040,3,1032,1006,1032,165,1102,2,1,1044,1105,1,224,2,1041,1043,1032,1006,1032,179,1101,1,0,1044,1106,0,224,1,1041,1043,1032,1006,1032,217,1,1042,1043,1032,1001,1032,-1,1032,1002,1032,39,1032,1,1032,1039,1032,101,-1,1032,1032,101,252,1032,211,1007,0,29,1044,1105,1,224,1101,0,0,1044,1105,1,224,1006,1044,247,102,1,1039,1034,1002,1040,1,1035,1001,1041,0,1036,1002,1043,1,1038,102,1,1042,1037,4,1044,1106,0,0,19,27,41,9,17,87,2,1,91,14,15,99,17,13,40,13,7,33,23,28,7,21,75,15,41,83,18,4,28,1,21,99,3,2,4,60,16,5,16,22,59,18,37,21,62,96,11,63,46,16,27,76,7,36,38,28,53,18,84,52,12,47,25,93,10,57,64,21,41,75,52,9,80,60,21,86,60,21,70,21,13,72,78,22,61,17,28,54,51,93,18,3,87,21,4,98,17,59,2,17,18,71,5,20,16,39,66,18,7,62,15,37,25,52,27,17,15,10,48,11,39,18,20,68,83,22,36,9,3,69,56,64,21,39,93,1,90,18,57,52,14,41,32,57,5,7,72,18,35,66,21,22,88,2,31,52,7,35,25,50,14,35,7,11,92,38,14,66,3,28,84,18,17,48,15,34,40,4,21,92,52,27,5,4,53,65,59,24,88,24,66,88,85,26,8,26,10,64,99,9,44,38,14,26,74,75,24,31,7,6,62,9,57,75,18,22,52,57,15,3,87,21,39,24,12,8,70,8,19,3,89,16,36,15,36,16,30,28,8,89,12,99,98,16,78,24,11,63,87,55,51,19,57,18,28,9,90,15,95,56,57,1,93,77,24,36,14,44,46,25,66,37,23,8,12,10,58,27,66,4,72,1,2,16,91,16,66,26,24,53,25,20,41,8,75,23,2,20,91,19,3,12,32,30,3,33,85,17,21,92,17,1,12,73,9,34,12,85,42,5,69,67,4,87,70,6,49,96,12,5,37,62,54,72,13,52,14,21,84,68,54,22,78,11,93,12,90,55,7,19,44,21,98,4,46,50,27,30,2,99,27,35,8,5,62,1,91,65,12,80,16,17,81,14,73,60,69,24,23,13,74,57,10,26,21,80,60,10,79,3,9,37,77,73,16,10,3,13,95,4,91,65,11,86,16,24,71,22,6,63,90,56,15,64,8,25,46,77,71,24,13,72,96,22,8,15,79,39,19,19,47,14,16,92,69,73,23,76,23,28,60,84,14,54,62,11,8,30,75,44,16,4,30,82,14,80,11,1,70,85,10,14,73,70,9,54,25,26,12,51,23,86,92,18,11,19,74,55,51,10,73,7,13,43,89,5,55,2,18,82,2,14,63,71,28,7,94,61,10,51,8,53,63,22,39,19,79,20,99,2,66,22,7,68,71,17,19,45,10,14,42,99,9,9,13,75,84,14,83,75,19,92,22,47,4,83,18,46,91,22,61,28,6,71,17,10,1,81,6,60,83,21,14,13,71,11,68,73,52,10,25,30,91,6,25,86,89,19,39,18,95,1,52,23,91,20,14,41,91,26,59,16,85,99,4,15,96,51,19,25,51,73,3,48,79,14,14,41,5,17,59,8,51,43,21,15,47,3,28,53,12,22,23,2,94,74,23,53,20,20,98,21,14,46,61,26,6,55,20,69,28,6,41,19,70,48,6,9,32,32,28,20,21,62,22,38,7,90,3,32,24,92,49,23,72,63,17,18,89,85,33,28,23,27,5,42,52,7,54,18,17,21,63,98,8,9,84,31,24,80,70,22,51,28,61,77,6,25,68,66,8,47,22,7,44,26,37,15,28,68,23,18,18,14,34,3,85,99,31,41,53,28,20,43,90,22,13,70,27,27,17,35,48,11,92,4,60,84,4,38,27,25,89,99,74,2,31,63,13,50,1,54,4,59,3,59,2,54,15,37,19,74,45,75,7,84,19,96,72,75,9,34,18,52,23,99,11,45,81,53,7,71,24,80,26,31,11,74,27,57,0,0,21,21,1,10,1,0,0,0,0,0,0

 ------------Input End*/

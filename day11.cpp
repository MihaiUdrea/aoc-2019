// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//--- Day 11: Space Police ----
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

enum Direction
{
  up,
  right,
  down,
  left,

  MAX
};

enum Color
{
  black,
  white
};

vector<Point> deltaMove = {
    {-1,  0}, // Direction::up,
    { 0,  1}, // Direction::right
    { 1,  0}, // Direction::down,  
    { 0, -1}  // Direction::left
};

vector<Direction> changeDir = { Direction::left, Direction::right };


struct Solve {
  vector<int_t> back;
  array<set<Point>, Color::white + 1> paintedLists;

  Solve(string inStr)
  {
    static const regex colsRxToken(",");
    forEachRxToken(inStr, colsRxToken, [&](string instr) {
      back.push_back(atoll(instr.c_str()));
      });
  };

  int_t JustRun(vector<int_t> a)
  {
    Program p(back, a);

    p.Run();

    return p.output.back();
  }

  int Do(const vector<int_t>& a)
  {
    Program p(back, a);

    Point crPoint;
    Direction direction = Direction::up;

    do
    {
      p.output.clear();

      p.Run(2);

      if (p.output.size() == 2)
      {
        size_t out = (size_t)p.output.front();

        auto& toAdd = paintedLists[out];
        auto& toRemove = paintedLists[1 - out];

        toAdd.insert(crPoint);
        toRemove.erase(crPoint);

        // orient robot        
        direction = Direction((direction + changeDir[(size_t)p.output.back()]) % Direction::MAX);

        // move robot
        crPoint = crPoint + deltaMove[direction];
      }

      p.regA = { contains(paintedLists[Color::white],crPoint) };
    } while (p.output.size() == 2);

    return paintedLists[Color::black].size() + paintedLists[Color::white].size();
  }

  void Do2(const vector<int_t>& a)
  {
    Do(a);

    // print white
    {
      auto minMaxX = minmax_element(paintedLists[Color::white].begin(), paintedLists[Color::white].end(), [](auto& l, auto& r) {
        return l.x < r.x;
        });
      auto minMaxY = minmax_element(paintedLists[Color::white].begin(), paintedLists[Color::white].end(), [](auto& l, auto& r) {
        return l.y < r.y;
        });

      for (auto y : irange(minMaxY.first->y, minMaxY.second->y + 1))
      {
        cout << endl;
        for (auto x : irange(minMaxY.first->x, minMaxY.second->x + 1))
          cout << (contains(paintedLists[Color::white], Point{ y,x }) ? "#" : " ");
      }
      cout << endl;
    }
  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").JustRun({}) == 99); // print yourself 
  REQUIRE(Solve("1102,34915192,34915192,7,4,7,99,0").JustRun({ }) == 1219070632396864); // some val test
  REQUIRE(Solve("104,1125899906842624,99").JustRun({  }) == 1125899906842624); // huge test
  //REQUIRE(Solve(ReadFileToString(L"input2.txt")).JustRun({ 1 }) == 2465411646); // sample test
  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 2 }) == 69781); // sample test
}

TEST_CASE("New1", "[x.]") {
  toClipboard((int)Solve(ReadFileToString(L"input.txt")).Do({ 0 }));
}

TEST_CASE("New2", "[x.]") {
  Solve(ReadFileToString(L"input.txt")).Do2({ 1 });
}

// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --- Day 12: The N-Body Problem ---
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

struct Data
{
  Point position;
  Point speed;

  auto operator<=>(const Data&) const = default;
};
auto DimensionsRange = irange(0, 3);
using Input = array<Data, 4>;

struct Solve {
  Input vec;

  Solve(string inStr) {
    int pos = 0;
    forEachRxToken(inStr, lineRxToken, [&](string line) {
      static const regex matchExp(R"~(<x=([^,]+), y=([^,]+), z=([^>]+)>)~");
      auto res = match_rx(line, matchExp);

      Point p{ stoi(res[2]), stoi(res[1]), stoi(res[3]) };
      vec[pos].position = p;
      pos++;
      });
  };

  array<Point, 4> gravity;

  void ApplyGravity()
  {
    fill(gravity.begin(), gravity.end(), Point());

    for (auto i : DimensionsRange)
    {
      if (reset.find(i) != reset.end())
        continue;

      Input& v = vec;

      for (auto moonIdx : irange(0, (int)v.size()))
      {
        auto smallerC = count_if(v.begin(), v.end(), [&](auto& l) {
          return l.position.GetAxys(i) < v[moonIdx].position.GetAxys(i);
          });
        auto greaterC = count_if(v.begin(), v.end(), [&](auto& l) {
          return l.position.GetAxys(i) > v[moonIdx].position.GetAxys(i);
          });

        gravity[moonIdx].IncAxyx(i, (int)(greaterC - smallerC));
      }

      // apply
      for (auto moonIdx : irange(0, (int)vec.size()))
      {
        vec[moonIdx].position.IncAxyx(i, gravity[moonIdx].GetAxys(i));
      }
    }
  }

  void ApplySpeed()
  {
    // move moon
    for (auto& moon : vec)
      moon.position = moon.position + moon.speed;

    // inc speed    
    for (auto it = vec.begin(); it != vec.end(); ++it)
      it->speed = it->speed + gravity[it - vec.begin()];
  }

  int GetEnergy()
  {
    return accumulate(vec.begin(), vec.end(), 0, [](auto s, auto moon) {
      return s + ManhDist(moon.speed) * ManhDist(moon.position); });
  }

  string Do(int steps = 1000)
  {
    for (auto step : irange(0, steps))
    {
      ApplyGravity();
      ApplySpeed();
    }

    return ToString(GetEnergy());
  }

  size_t GetDimHash(const Input& a, int dim)
  {
    return accumulate(a.begin(), a.end(), 0ui64, [=](auto s, auto e) {
      return (s << 20) + (e.position.GetAxys(dim) << 10) + e.speed.GetAxys(dim);
      });
  }

  array<set<size_t>, 4> history;
  map<size_t, size_t> reset;

  void AddToHistory(size_t step)
  {
    for (auto dim : DimensionsRange)
    {
      if (reset.find(dim) != reset.end())
        continue;

      if (!history[dim].insert(GetDimHash(vec, dim)).second)
      {
        reset[dim] = step;
      }
    }
  }

  string Do2()
  {
    for (size_t step = 0; reset.size() < 3; ++step)
    {
      AddToHistory(step);
      ApplyGravity();
      ApplySpeed();
    }

    return std::to_string(lcm(lcm(reset[0], reset[1]), reset[2]));
  }
};

TEST_CASE("Sample 0", "[x]") {
  REQUIRE(Solve(
    R"(<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>)").Do(10) == "179");
}

TEST_CASE("Part 2 Test", "[x.]") {
  REQUIRE(Solve(
    R"(<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>)").Do2() == "2772");

}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

/* Input ---------------

<x=-4, y=-9, z=-3>
<x=-13, y=-11, z=0>
<x=-17, y=-7, z=15>
<x=-16, y=4, z=2>

 ------------Input End*/

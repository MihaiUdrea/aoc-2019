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
  //set<Point> points;
  //map<Point, int> dists;
  Point position;
  Point speed;
};

using Input = vector<Data>;

struct Solve {
  Input vec;

  vector<int_t> back;
  vector<Point> inters;


  Solve(string inStr) {

    /** Int Computer * /
    forEachRxToken(inStr, regex(R"~(,)~"), [&](string line) {
      back.push_back(atoll(line.c_str()));
      });
    /**/

    /** /
    forEachLine(inStr, [&](string line)
    {
      vec.push_back(line);
    });
    */

    /** /
    vec = GetLines(inStr);
    /**/

    /* a1 b2 c3 */
    /* x2 yr zs */
    /* */
    forEachRxToken(inStr, lineRxToken, [&](string line) {
      vec.push_back({});

      static const regex matchExp(R"~([^=]+=([^,]+),[^=]+=([^,]+),[^=]+=([^>]+)>)~");      
      auto res = match_rx(line, matchExp);

      Point p{ atoi(res[2].str().c_str()), atoi(res[1].str().c_str()), atoi(res[3].str().c_str()) };
      vec.back().position = p;

      });
    /**/

    /* * /
    static const regex matchExp(R"~((\d+) (\d+))~");
    string s = "13 24";
    auto res = match_rx(s, matchExp);
    /* */


  };

  int_t JustRun(vector<int_t> a)
  {
    Program p(back, a);

    p.Run();

    return p.output.back();
  }

  vector<Point> gravity;

  void ApplyGravity()
  {
    gravity.clear();
    gravity.resize(vec.size());

    for (auto i : irange(0, 3))
    {
      Input & v = vec;

      /** /

      sort(v.begin(), v.end(), [&](auto& l, auto& r) {
        return l.position.GetAxys(i) < r.position.GetAxys(i);
        });
      /**/

      for (auto moonIdx : irange(0, v.size()))
      {
        int smallerC = count_if(v.begin(), v.end(), [&](auto& l) {
          return l.position.GetAxys(i) < v[moonIdx].position.GetAxys(i);
          });
        int greaterC = count_if(v.begin(), v.end(), [&](auto& l) {
          return l.position.GetAxys(i) > v[moonIdx].position.GetAxys(i);
          });

        gravity[moonIdx].IncAxyx(i, greaterC - smallerC);
      }

      // apply
      for (auto moonIdx : irange(0, vec.size()))
      {
        vec[moonIdx].position.IncAxyx(i, gravity[moonIdx].GetAxys(i));
      }

      // next axys
    }
  }

  void ApplySpeed()
  {
    for (auto i : irange(0, 3))
    {

      // apply
      for (auto& moon : vec)
      {
        moon.position.IncAxyx(i, moon.speed.GetAxys(i));
      }
    }


    // inc speed
    for (auto i : irange(0, 3))
    {

      for (auto moonIdx : irange(0, vec.size()))
      {
        vec[moonIdx].speed.IncAxyx(i, gravity[moonIdx].GetAxys(i));
      }
    }
  }

  int GetEnergy()
  {
    int res = 0;
    for (auto moon : vec)
    {
      res += ManhDist(moon.speed) * ManhDist(moon.position);
    }

    return res;
  }

  string Do()
  {

    for (auto step : irange(0, 100))
    {
      ApplyGravity();
      ApplySpeed();

      cout << "\nAfter " << step + 1 << " steps:\n";
      // print 
      for (auto moon : vec)
      {
        cout << "pos = <x = " << moon.position.x << ", y = " << moon.position.y << ", z =" << moon.position.z << ">, vel = <x = " << moon.speed.z << ", y = " << moon.speed.y << ", z = " << moon.speed.z << ">\n";
      }

    }

    return ToString(GetEnergy());
  }

  string Do2()
  {

    return ToString(1);
  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").JustRun({}) == 99); // print yourself 

  //REQUIRE(Solve("").Do() == "6"); // sample part 1
  //REQUIRE(Solve("").Do2() == "30"); // sample part 2
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

/* Input ---------------

<x=-4, y=-9, z=-3>
<x=-13, y=-11, z=0>
<x=-17, y=-7, z=15>
<x=-16, y=4, z=2>

 ------------Input End*/

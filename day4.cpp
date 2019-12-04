// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include <execution>

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
  int start, end;

  Solve(string inStr) {
    static const regex lineSepRx("\\n");
    static const regex colsSepRx("-");
      
    vector<int> v;
    for (sregex_token_iterator iter(inStr.begin(), inStr.end(), colsSepRx, -1), end; iter != end; ++iter)
    { 
      string line = iter->str();
      v.push_back(atoi(line.c_str()));
    }

    start = v[0];
    end = v[1];
  };

#include <mutex>

  string Do()
  {
    std::mutex mu;

    int count = 0;
    vector<int> v;
    for (auto i = start; i < end; i++)
      v.push_back(i);
    vector<int> passList;
    for_each(execution::par, v.begin(), v.end(), [&](auto i) {
      bool doubled = false;
      bool inc = false;
      int val = i;
      
      int matching = 0;
      do
      {
        int rD = i % 10;
        i = i / 10;
        int lD = i % 10;

        if (rD < lD)
        {
          inc = true;
          break;
        }
        else if (rD == lD)
        {
          //doubled = true;
          matching++;
        }
        else
        {
          if (matching == 1)
            doubled = true;

          matching = 0;
        }
      } while (i >= 10);

      if (!inc && (doubled || matching == 1))
      {
        mu.lock();
        count++;
        passList.push_back(val);
        mu.unlock();
      }
      });

   /** /
    {
      bool doubled = false;
      do
      {
        int rD = i % 10;
        i = i / 10;
        int lD = i % 10;

        if (rD < lD)
          break;
        else if (rD == lD)
          doubled = true;
      } while (i >= 10);

      if (i < 10 && doubled)
        count++;
    }
    */
  
    return ToString(count);
  }

  string Do2()
  {
    
    return ToString(1);

  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve("").Do() == "6"); // sample part 1
  //REQUIRE(Solve("").Do2() == "30"); // sample part 2
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

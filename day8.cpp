// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
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
  
  vector<int> inputList;
  int lines, cols;

  Solve(string inStr) {
    static const regex lineSepRx("\\n");
    static const regex colsSepRx(",");

    for (auto ch : inStr)
      inputList.push_back(ch-'0');

    

    /*
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
    */
  };

  struct Res
  {
    int zeros = 0;
    int ones = 0;
    int twos = 0;
  };

  vector<Res> resList;

  string Do(int l, int c)
  {
    lines = l;
    cols = c;

    int layerLength = lines * cols;
    int layerCount = inputList.size() / layerLength;
    
    

    for (auto l : irange(0, layerCount))
    {
      Res res;
      //cout << endl;
      for (auto i : irange(layerLength* l, layerLength* l + layerLength))
      {
        //cout << inputList[i];
        if (inputList[i] == 0)
        {
          res.zeros++;
        }
        else if (inputList[i] == 1)
        {
          res.ones++;
        }
        else if (inputList[i] == 2)
        {
          res.twos++;
        }
      }

      resList.push_back(res);
    }

    auto min = min_element(resList.begin(), resList.end(), [](auto l, auto r) {return l.zeros < r.zeros; });

//    cout << "Min idx: " << min - resList.begin() << endl;

    return ToString(min->ones * min->twos);
  }

  string Do2(int l, int c)
  {
    lines = l;
    cols = c;

    int layerLength = lines * cols;
    int layerCount = inputList.size() / layerLength;
    
    vector<int> visibleLayer(layerLength);

    for (auto l : irange(layerCount - 1, -1))
    {
      for (auto i : irange(layerLength* l, layerLength* l + layerLength))
      {
//        cout << inputList[i];
        if (inputList[i] == 0)
        {
          visibleLayer[i - layerLength * l] = inputList[i];
        }
        else if (inputList[i] == 1)
        {
          visibleLayer[i- layerLength * l] = inputList[i];
        }
      }

    }

    int pos = 0;
    for (auto i : irange(0, lines))
    {
      cout << endl;
      for (auto j : irange(0, cols))
      {
        cout << ((visibleLayer[pos] == 1) ? 'X' : ' ');
        pos++;

      }

    }

    
    return ToString(1);
  }

};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve("").Do() == "6"); // sample part 1
  //REQUIRE(Solve("").Do2() == "30"); // sample part 2
  REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do(2,2) == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do(6, 25));
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2(6,25));
}

// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

//#define FILE_OUT_MODE ofstream::trunc
#define FILE_OUT_MODE  ofstream::/**/trunc/** /app/**/

//FILE_PRINT_LEVELS  0 -no file printing; 1 - only Print1(true) 2 Any Printf; 3 - Any PrintF and Any PrintLineF
namespace PrintLevels {
  enum { none, onlyExtraPrintF, anyPrintf, all };
}

#ifdef _DEBUG
#define OUT_FILE LR"(outdebug.txt)"
#define TEST true
#define RUNONE false
#define RUNTWO false
#define FILE_PRINT_LEVEL PrintLevels::all
#else
#define OUT_FILE LR"(outrelease.txt)"
#define TEST false
#define RUNONE true
#define RUNTWO true
#define FILE_PRINT_LEVEL PrintLevels::onlyExtraPrintF
#endif // DEBUG


void PrintLineF(string line)
{
  if (FILE_PRINT_LEVEL < PrintLevels::all)
    return;

  ofstream cout;
  cout.open(OUT_FILE, ofstream::app);

  cout << line;

  cout.close();
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
  //set<Point> points;
  //map<Point, int> dists;
  Point position;
  Point speed;

  auto operator<=>(const Data&) const = default;

};

using Input = array<Data, 4>;

struct Solve {
  Input vec;

  //vector<int_t> back;
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
    int pos = 0;
    forEachRxToken(inStr, lineRxToken, [&](string line) {
      //vec.push_back({});

      static const regex matchExp(R"~([^=]+=([^,]+),[^=]+=([^,]+),[^=]+=([^>]+)>)~");
      auto res = match_rx(line, matchExp);

      Point p{ stoi(res[2]), stoi(res[1]), stoi(res[3]) };
      vec[pos].position = p;
      pos++;
      });
    /**/

    /* * /
    static const regex matchExp(R"~((\d+) (\d+))~");
    string s = "13 24";
    auto res = match_rx(s, matchExp);
    /* */


  };

//  array<Point, 4> gravity = { Point{0,0,0}, Point{0,0,0}, Point{0,0,0}, Point{0,0,0} };
  vector<Point> gravity;

  void ApplyGravity()
  {
    for (auto& el : gravity)
      el = {};
    
    for (auto i : irange(0, 3))
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

      // next axys
    }
  }

  void ApplySpeed()
  {
    for (auto& moon : vec)
    {
      moon.position = moon.position + moon.speed;
    }

    // inc speed
    
    for (auto moonIdx : irange(0, (int)vec.size()))
    {
      vec[moonIdx].speed = vec[moonIdx].speed + gravity[moonIdx];
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

  void Print(size_t step)
  {
    return;
    {
      cout << "\nAfter " << step << " steps:\n";
      // print 
      for (auto moon : vec)
      {
        cout << "pos=<x=" << std::setw(2) << moon.position.x << ", y=" << std::setw(2) << moon.position.y << ", z=" << std::setw(2) << moon.position.z << ">, vel=<x=" << std::setw(2) << moon.speed.x << ", y=" << std::setw(2) << moon.speed.y << ", z=" << std::setw(2) << moon.speed.z << ">\n";
      }
    }
  }

  void PrintPoint(Point position)
  {
    //cout << "pos=<x=" << std::setw(2) << position.x << ", y=" << std::setw(2) << position.y << ", z=" << std::setw(2) << position.z << ">\n";
  }

  string Do(int steps = 1000)
  {
    gravity.resize(vec.size());

    for (auto step : irange(0, steps))
    {
      Print(step);
      ApplyGravity();
      ApplySpeed();
    }

    return ToString(GetEnergy());
  }

#define BITS_COUNT 10

  size_t GetDimHash(const Input& a, int dim)
  {
    std::size_t ret = 0;

    for (auto& e : a)
    {
      ret = ret << BITS_COUNT;
      ret += e.position.GetAxys(dim);
      ret = ret << BITS_COUNT;
      ret += e.speed.GetAxys(dim);

    }
    return ret;
  }
  array<set<size_t>, 4> history;
  map<size_t, size_t> reset;

  void AddToHistory(Input& vec, size_t step)
  {
    for (auto dim : irange(0, 3))
    {
      if (reset.find(dim) != reset.end())
        continue;

      if (auto hash = GetDimHash(vec, dim); history[dim].find(hash) != history[dim].end())
      {
        cout << dim << " resets at: " << step << endl;
        reset[dim] = step;
      }
      else
        history[dim].insert(hash);
    }
  }

  void PrintF(const Input & a, bool aExtra = false, bool aEnd = false)
  {
    if (FILE_PRINT_LEVEL == PrintLevels::none || (FILE_PRINT_LEVEL == PrintLevels::onlyExtraPrintF && !aExtra))
      return;

    int charWidth = 1;
    if (aExtra)
      charWidth = 9;

    auto minX = min_element(a.begin(), a.end(), [](auto& l, auto& r) {
      return l.position.x < r.position.x;
      });
    auto minY = min_element(a.begin(), a.end(), [](auto& l, auto& r) {
      return l.position.y < r.position.y;
      });
    auto minZ = min_element(a.begin(), a.end(), [](auto& l, auto& r) {
      return l.position.z < r.position.z;
      });

    Point min{ minY->position.y, minX->position.x, minZ->position.z };

    auto maX = max_element(a.begin(), a.end(), [](auto& l, auto& r) {
      return l.position.x < r.position.x;
      });
    auto maxY = max_element(a.begin(), a.end(), [](auto& l, auto& r) {
      return l.position.y < r.position.y;
      });
    auto maxZ = max_element(a.begin(), a.end(), [](auto& l, auto& r) {
      return l.position.z < r.position.z;
      });

    Point max{ maxY->position.y, maX->position.x, maxZ->position.z };


    int minCol = min.y;

    int lastCol = max.y +1;
    //lastCol = 35;

    int minLine = min.x;

    int lastLine = max.x + 1;

    //ofstream out;
    //out.open(OUT_FILE, FILE_OUT_MODE);
    system("CLS");
    auto & out = cout;
    //out << "------------------------------" << path << "------------" << endl;

    // print top/cols count header 
    auto topHeader = [&](int minCol, int maxCol) {
      out << std::setfill(' ') << setw(4) << ' ';
      for (auto col : irange(minCol, maxCol))
      {
        out << setw(charWidth) << col / 100;
      }
      out << endl;
      out << std::setfill(' ') << setw(4) << ' ';
      for (auto col : irange(minCol, maxCol))
      {
        out << setw(charWidth) << (col % 100) / 10;
      }
      out << endl;
      out << std::setfill(' ') << setw(4) << ' ';
      for (auto col : irange(minCol, maxCol))
      {
        out << setw(charWidth) << col % 10;
      }
      out << endl;
    };

    //topHeader(minCol, lastCol);

    for (auto l : irange(minLine, lastLine))
    {
      //out << left << std::setfill(' ') << setw(4) << l;
      for (auto c : irange(minCol, lastCol))
      {
        Point crPt{ l, c, 0 };

        int found = -1;
        for (auto idx : irange(0, (int)a.size()))
        {
          if (a[idx].position.x == l && a[idx].position.y == c)
          {
            found = idx;
            break;
          }
        }

        //out << setw(charWidth) << type[l][c];
        /**/
        if (found > -1)
          out << (char)('A' + found);
        else
          out << ' ';        
      }
      out << endl;
    }
    Sleep(1000);
    //out.close();
  }

  string Do2()
  {
    gravity.resize(vec.size());

    AddToHistory(vec, 0);

    Input orig = vec;

    size_t step = 0;
    do
    {
      ApplyGravity();
      ApplySpeed();

      step++;
      AddToHistory(vec, step);
    } while (reset.size() < 3);

    cout << step;
    return std::to_string(lcm(lcm(reset[0],reset[1]),reset[2]));
  }
};

TEST_CASE("Sample 0", "[.]") {
  string s = 
R"(<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>)";
  REQUIRE(Solve(s).Do(10) == "179");
}

TEST_CASE("Part 2 Test", "[x.]") {
  string s =
    R"(<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>)";
  REQUIRE(Solve(s).Do2() == "2772");
  
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

/* Input ---------------

<x=-4, y=-9, z=-3>
<x=-13, y=-11, z=0>
<x=-17, y=-7, z=15>
<x=-16, y=4, z=2>

 ------------Input End*/

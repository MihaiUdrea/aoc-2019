// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Priority que
// --- Day 20: Donut Maze ---

struct SimplePointData
{
  Point point;
  int dist = 0;
  int depth = 0;
  vector<Point> prev;
};

bool operator <(const SimplePointData& _Left, const SimplePointData& _Right)
{
  return _Left.dist > _Right.dist;
}

using QueDist = priority_queue<SimplePointData>;

using Input = vector<string>;

array<Point, 4> delta = { Point{-1,0,0},{1,0,0},{0,-1,0},{0,1,0} };
array<Point, 2> deltaReadOrder = { Point{1,0,0},{0,1,0} };


set<Point> historySet;

struct Solve {

  map<Point, char> portallPt;
  map<pair<Point, Point>, string> portalDoors;
  map<Point, string> ptToDoorMap;
  map<string, pair<Point, Point>> teleportPoints;
  set<Point> hall;
  map<Point, vector<Point>> nextPt;

  Input vec;

  pair<Point, Point> limits;

  int cols = 0;
  int lines = 0;

  bool IsInner(Point p)
  {
    //return p.x > 6 && p.x < 40 && p.y > 6 && p.y < 32;
    const auto safeMargin = 5;
    return
      p.x > limits.first.x + safeMargin &&
      p.x < limits.second.x - safeMargin &&
      p.y > limits.first.y + safeMargin &&
      p.y < limits.second.y - safeMargin;
  }

  Solve(string inStr) {
    static const regex lineSepRx("\\n");

    for (sregex_token_iterator iter(inStr.begin(), inStr.end(), lineSepRx, -1), end; iter != end; ++iter)
    {
      string line = iter->str();
      vec.push_back(line);
    }

    cols = vec[0].size();
    lines = vec.size();

    // find start
    int startCount = 0;
    for (auto l : irange(0, vec.size()))
    {
      string& line = vec[l];
      for (auto c : irange(0, line.size()))
      {
        auto ch = line[c];

        if (ch >= 'A' && ch <= 'Z')
        {
          portallPt[{ c, l}] = ch;
        }
        else if (ch >= '.')
        {
          hall.insert({ c,l });
        }
      }
    }

    limits = MinMax(portallPt, [](auto& a) {return a.first; });

    for (auto h : portallPt)
    {
      for (auto d : deltaReadOrder)
      {
        auto newPoint = h.first + d;
        if (auto whre = portallPt.find(newPoint); whre != portallPt.end())
        {
          portalDoors[{ h.first, newPoint }] = string() + h.second + whre->second;

        }
      }
    }

    for (auto h : portalDoors)
    {
      for (auto d : delta)
      {
        auto newPoint = h.first.first + d;
        if (auto whre = hall.find(newPoint); whre != hall.end())
        {
          ptToDoorMap[newPoint] = h.second;
          if (teleportPoints[h.second].first == Point{ 0,0,0 })
            teleportPoints[h.second].first = newPoint;
          else
            teleportPoints[h.second].second = newPoint;
        }
      }
      for (auto d : delta)
      {
        auto newPoint = h.first.second + d;
        if (auto whre = hall.find(newPoint); whre != hall.end())
        {
          ptToDoorMap[newPoint] = h.second;

          if (teleportPoints[h.second].first == Point{ 0,0,0 })
            teleportPoints[h.second].first = newPoint;
          else
            teleportPoints[h.second].second = newPoint;
        }
      }
    }

    for (auto p : teleportPoints)
    {
      if (p.second.second != Point{ 0,0,0 })
      {
        bool firstDown = IsInner(p.second.first);
        nextPt[p.second.first].push_back(p.second.second + (firstDown ? Point{ 0,0,-1 } : Point{ 0,0,+1 }));
        nextPt[p.second.second].push_back(p.second.first - (firstDown ? Point{ 0,0,-1 } : Point{ 0,0,+1 }));
      }
    }


    for (auto p : hall)
    {
      for (auto d : delta)
      {
        auto newPoint = p + d;
        if (auto whre = hall.find(newPoint); whre != hall.end())
        {
          nextPt[p].push_back(newPoint);
        }
      }
    }

  };


  string Do(bool goDeep = false)
  {
    auto s = to2Ds(hall, [](auto& l) { return l; }, [](auto& l) { return "."; }, to2DsFlags::full_header, ' ', 1);
    cout << s;

    QueDist que;
    Point endPt = teleportPoints["ZZ"].first;

    que.push({ teleportPoints["AA"].first, 0, 0, {} });
    set<pair<Point, int>> hist{};
    int res = 0;
    while (!que.empty())
    {
      auto crEl = que.top();

      if (crEl.point == endPt && crEl.depth == 0)
      {
        res = crEl.dist;
        //double F = 10.1;
        //auto s = to2Ds(crEl.prev, [=](auto& l) { return Point{(int)( l.x * F / (abs(l.z) + 1)),(int)(l.y * F / (abs(l.z) + 1)) }; }, 
        //  [](auto& l) { return string() + (char)('A' - l.z); }, to2DsFlags::full_header, ' ', 1);

        if (1) // detail solution print
        {
          auto prevIt = crEl.prev.begin();
          for (auto depthChangeIt = find_if(crEl.prev.begin(), crEl.prev.end(), [](auto& l) {return l.z == -1; });
            prevIt != crEl.prev.end();
            depthChangeIt = find_if(prevIt, crEl.prev.end(), [&](auto& l) {return l.z != depthChangeIt->z; }))
          {
            cout << "Level " << depthChangeIt->z + 1 << ": " << endl;
            vector<Point> thisLevel(prevIt, depthChangeIt);

            thisLevel.push_back(limits.first);
            thisLevel.push_back(limits.second);
            //copy(prevIt, depthChangeIt, back_inserter(thisLevel));
            auto s = to2Ds(thisLevel, [=](auto& l) { return l; },
              [&](auto& l) { return string() + (char)('A' - prevIt->z); }, to2DsFlags::full_header, ' ', 1);
            cout << s;

            prevIt = depthChangeIt;
          }
        }
        break;
      }

      que.pop();

      if (hist.insert({ crEl.point, crEl.depth }).second == false)
        continue; // seen

      for (auto n : nextPt[crEl.point])
      {
        Point ptx = n;
        ptx.z = 0;

        SimplePointData newDataPoint{ ptx , crEl.dist + 1, crEl.depth + (goDeep ? n.z : 0), crEl.prev };

        if (newDataPoint.depth > 0)
          continue;

        /**/ // print each level change
        if (0)
          if (n.z != 0)
          {
            cout << "Will change level : " << newDataPoint.depth << " cr level: " << endl;

            vector<Point> thisLevel;
            copy_if(crEl.prev.begin(), crEl.prev.end(), back_inserter(thisLevel), [&](auto e) {return e.z == crEl.depth; });
            auto s = to2Ds(thisLevel, [](auto& l) { return l; }, [](auto& l) { return "*"; }, to2DsFlags::full_header, '.', 1);
            cout << s;
          }
        /**/

        newDataPoint.prev.push_back(crEl.point);
        newDataPoint.prev.back().z = newDataPoint.depth;
        que.push(newDataPoint);
      }
    }

    return to_string(res);
  }
};

TEST_CASE("P1 simple", "[.]")
{
  REQUIRE(Solve(R"(
         A           
         A           
  #######.#########  
  #######.........#  
  #######.#######.#  
  #######.#######.#  
  #######.#######.#  
  #####  B    ###.#  
BC...##  C    ###.#  
  ##.##       ###.#  
  ##...DE  F  ###.#  
  #####    G  ###.#  
  #########.#####.#  
DE..#######...###.#  
  #.#########.###.#  
FG..#########.....#  
  ###########.#####  
             Z       
             Z  )").Do() == "23");
}

TEST_CASE("P1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(R"(
                   A               
                   A               
  #################.#############  
  #.#...#...................#.#.#  
  #.#.#.###.###.###.#########.#.#  
  #.#.#.......#...#.....#.#.#...#  
  #.#########.###.#####.#.#.###.#  
  #.............#.#.....#.......#  
  ###.###########.###.#####.#.#.#  
  #.....#        A   C    #.#.#.#  
  #######        S   P    #####.#  
  #.#...#                 #......VT
  #.#.#.#                 #.#####  
  #...#.#               YN....#.#  
  #.###.#                 #####.#  
DI....#.#                 #.....#  
  #####.#                 #.###.#  
ZZ......#               QG....#..AS
  ###.###                 #######  
JO..#.#.#                 #.....#  
  #.#.#.#                 ###.#.#  
  #...#..DI             BU....#..LF
  #####.#                 #.#####  
YN......#               VT..#....QG
  #.###.#                 #.###.#  
  #.#...#                 #.....#  
  ###.###    J L     J    #.#.###  
  #.....#    O F     P    #.#...#  
  #.###.#####.#.#####.#####.###.#  
  #...#.#.#...#.....#.....#.#...#  
  #.#####.###.###.#.#.#########.#  
  #...#.#.....#...#.#.#.#.....#.#  
  #.###.#####.###.###.#.#.#######  
  #.#.........#...#.............#  
  #########.###.###.#############  
           B   J   C               
           U   P   P               
)").Do() == "58");
}

TEST_CASE("P2", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(R"(
             Z L X W       C                 
             Z P Q B       K                 
  ###########.#.#.#.#######.###############  
  #...#.......#.#.......#.#.......#.#.#...#  
  ###.#.#.#.#.#.#.#.###.#.#.#######.#.#.###  
  #.#...#.#.#...#.#.#...#...#...#.#.......#  
  #.###.#######.###.###.#.###.###.#.#######  
  #...#.......#.#...#...#.............#...#  
  #.#########.#######.#.#######.#######.###  
  #...#.#    F       R I       Z    #.#.#.#  
  #.###.#    D       E C       H    #.#.#.#  
  #.#...#                           #...#.#  
  #.###.#                           #.###.#  
  #.#....OA                       WB..#.#..ZH
  #.###.#                           #.#.#.#  
CJ......#                           #.....#  
  #######                           #######  
  #.#....CK                         #......IC
  #.###.#                           #.###.#  
  #.....#                           #...#.#  
  ###.###                           #.#.#.#  
XF....#.#                         RF..#.#.#  
  #####.#                           #######  
  #......CJ                       NM..#...#  
  ###.#.#                           #.###.#  
RE....#.#                           #......RF
  ###.###        X   X       L      #.#.#.#  
  #.....#        F   Q       P      #.#.#.#  
  ###.###########.###.#######.#########.###  
  #.....#...#.....#.......#...#.....#.#...#  
  #####.#.###.#######.#######.###.###.#.#.#  
  #.......#.......#.#.#.#.#...#...#...#.#.#  
  #####.###.#####.#.#.#.#.###.###.#.###.###  
  #.......#.....#.#...#...............#...#  
  #############.#.#.###.###################  
               A O F   N                     
               A A D   M 
)").Do(true) == "396");

}


TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do(true));
}

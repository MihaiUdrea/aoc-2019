// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Priority que
// --- Day 18: Many-Worlds Interpretation ---

struct MyStruct
{
  char first;
  int second;
};

bool operator <(const MyStruct& _Left, const MyStruct& _Right)
{
  if (_Left.second == _Right.second)
    return _Left.first < _Right.first;
  return _Left.second < _Right.second;
}

//using Point = pair<int, int>;
struct PointData
{
  //int keysMissing;
  int dist = 0;
  Point point;
  string keys;
  set<char> doors;
  //auto operator<=>(const PointData&) const = default;
};

struct KeyPointData
{
  char key;
  int dist = 0;
  set<char> prevKeys;
};
bool operator <(const KeyPointData& _Left, const KeyPointData& _Right)
{
  if (_Left.dist == _Right.dist)
    return _Left.key > _Right.key;
  return _Left.dist > _Right.dist;
}

struct KeyPointData2
{
  array<char, 4> key;
  int dist = 0;
  set<char> prevKeys;
};
bool operator <(const KeyPointData2& _Left, const KeyPointData2& _Right)
{
  if (_Left.dist == _Right.dist)
    return _Left.key > _Right.key;
  return _Left.dist > _Right.dist;
}

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

using QueKey2 = priority_queue<KeyPointData2>;
using QueKey = priority_queue<KeyPointData>;

using QueDist = priority_queue<SimplePointData>;

using Que = priority_queue<PointData>;
using Input = vector<string>;

/**/
bool operator <(const PointData& _Left, const PointData& _Right)
{
  if (_Left.dist == _Right.dist)
  {
    if (_Left.keys.size() == _Right.keys.size())
      if (_Left.doors.size() == _Left.doors.size())
        return _Left.keys < _Left.keys;
      else
        return _Left.doors.size() < _Left.doors.size();
    else
      return _Left.keys.size() < _Left.keys.size();
  }
  else
    return _Left.dist > _Right.dist;
}
/**/
/** /
bool operator <(const PointData& _Left, const PointData& _Right)
{
  if (_Left.keys.size() == _Right.keys.size())
    if (_Left.dist == _Left.dist)
    {
      return _Left.point < _Right.point;
    }
    else
      return _Left.dist < _Right.dist;
  else
    return _Left.keys.size() < _Left.keys.size();
}
/**/

array<Point, 4> delta = { Point{-1,0,0},{1,0,0},{0,-1,0},{0,1,0} };
array<Point, 2> deltaReadOrder = { Point{1,0,0},{0,1,0} };

array<Point, 4> start = { Point{-10,0,0},{-20,0,0},{-30,-1,0},{-40,1,0} };

array<char, 4> bots = { '@', '$', '%', '*' };




set<Point> historySet;

struct Solve {

  map<Point, char> doors;
  map<Point, char> keys;
  map<Point, char> portallPt;
  map<pair<Point, Point>, string> portalDoors;
  map<Point, string> ptToDoorMap;
  map<string, pair<Point, Point>> teleportPoints;
  set<Point> hall;
  map<Point, vector<Point>> nextPt;

  set<pair<Point, string>> history;
  map<char, vector<char> > lockings;

  Input vec;
  Que que;


  int cols = 0;
  int lines = 0;

  bool IsInner(Point p)
  {
    //return p.x > 6 && p.x < 40 && p.y > 6 && p.y < 32;
    return p.x > 20 && p.x < 100 && p.y > 20 && p.y < 110;
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

        if (ch == '@')
        {
          start[startCount++] = { c, l };
          hall.insert({ c, l });
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
          portallPt[{ c, l}] = ch;
          //hall.insert({ c,l });
        }
        else if (ch >= 'a' && ch <= 'z')
        {
          keys[{ c, l}] = ch;
          hall.insert({ c,l });
        }
        else if (ch >= '.')
        {
          hall.insert({ c,l });
        }
      }

    }

    for (auto h : portallPt)
    {
      for (auto d : deltaReadOrder)
      {
        auto newPoint = h.first + d;
        if (auto whre = portallPt.find(newPoint); whre != portallPt.end())
        {
          portalDoors[{ h.first, newPoint }] = string() + h.second + whre->second;

          
          //portalDoors[string() + h.second + whre->second] = { h.first, newPoint };
          //next[h].push_back(newPoint);
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
        nextPt[p.second.first].push_back(p.second.second + (firstDown ? Point{0,0,-1} : Point{ 0,0,+1 }));
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



void Dependencies()
  {
    for (auto sP : start)
    {
      //auto sP = start[0];
      vector<char> c;
      set<char> s;

      historySet.insert(sP);
      
      cout << "\nLockings:" << endl;

      for (auto x : lockings)
      {
        cout << x.first << endl;
        //for (auto y : x.second)
        {
          copy(x.second.begin(), x.second.end(),
            ostream_iterator<char>(cout));
          cout << endl;
        }
      }
      // used gates
      set<char> usedGates;
      for (auto key : lockings)
        for (auto y : key.second)
          usedGates.insert(y);
    }
  }

  map<pair<char, char>, int> distBetKeys;



  map<char, set<MyStruct>> distFromOneKey;

  void DistancesBetweenKeys()
  {
    for (auto pSIdx : irange(0, start.size()))
      keys[start[pSIdx]] = bots[pSIdx];

    for (auto l : keys)
    {
      QueDist que;

      que.push({ l.first, 0 });
      set<Point> hist{};
      while (!que.empty())
      {
        auto crEl = que.top();

        que.pop();
        /**/
        if (hist.insert(crEl.point).second == false)
          continue; // seen

        if (auto where = keys.find(crEl.point); where != keys.end())
        {
          {
            distBetKeys[{where->second, l.second}] = crEl.dist;
            auto& x = distFromOneKey[l.second];
            MyStruct b{ where->second, crEl.dist };
            x.insert(b);
            int p = 0;
            p++;
          }
        }

        //for (auto d : delta)

      }

      //cout << "D : " << distBetKeys.size();
    }
  }

  int BFS()
  {
    set<pair<char, set<char>>> history;
    QueKey que;
    que.push({ '@', 0 });

    KeyPointData best;

    while (!que.empty())
    {
      auto crEl = que.top();

      que.pop();

      if (history.insert({ crEl.key, crEl.prevKeys }).second == false)
        continue; // seen

      crEl.prevKeys.insert(crEl.key);

      if (crEl.prevKeys.size() == keys.size())
        return crEl.dist;

      if (crEl.prevKeys.size() > best.prevKeys.size())
        best = crEl;

      auto& list = distFromOneKey[crEl.key];
      for (auto n : list)
      {
        if (n.first == crEl.key)
          continue; // skip self

        if (crEl.prevKeys.find(n.first) != crEl.prevKeys.end())
          continue; // already been to this key

        KeyPointData newDataPoint{ n.first, crEl.dist + n.second, crEl.prevKeys };

        // verify all keys are present
        bool bad = false;
        for (auto key : lockings[n.first])
        {
          if (newDataPoint.prevKeys.find(tolower(key)) == newDataPoint.prevKeys.end())
          {
            bad = true;
            break;
          }
        }
        if (bad)
          continue;

        que.push(newDataPoint);
      }
    }
    return -1;
  }

  int BFS2()
  {
    set<pair<array<char, 4>, set<char>>> history;
    QueKey2 que;
    que.push({ bots, 0 });

    KeyPointData2 best;

    while (!que.empty())
    {
      auto crEl = que.top();

      que.pop();

      if (history.insert({ crEl.key, crEl.prevKeys }).second == false)
        continue; // seen

      for (auto x : crEl.key)
        crEl.prevKeys.insert(x);

      if (crEl.prevKeys.size() == keys.size())
        return crEl.dist;

      if (crEl.prevKeys.size() > best.prevKeys.size())
        best = crEl;

      for (auto xi : irange(0, crEl.key.size()))
      {
        auto x = crEl.key[xi];

        auto& list = distFromOneKey[x];
        for (auto n : list)
        {
          if (n.first == x)
            continue; // skip self

          if (crEl.prevKeys.find(n.first) != crEl.prevKeys.end())
            continue; // already been to this key

          KeyPointData2 newDataPoint{ crEl.key, crEl.dist + n.second, crEl.prevKeys };
          newDataPoint.key[xi] = n.first;

          // verify all keys are present
          bool bad = false;
          for (auto key : lockings[n.first])
          {
            if (newDataPoint.prevKeys.find(tolower(key)) == newDataPoint.prevKeys.end())
            {
              bad = true;
              break;
            }
          }
          if (bad)
            continue;

          que.push(newDataPoint);
        }
      }
    }
    return -1;
  }

  string Do()
  {
    //Dependencies();

    //DistancesBetweenKeys();

    auto s = to2Ds(hall, [](auto& l) { return l; }, [](auto& l) { return "."; }, to2DsFlags::full_header, ' ', 1);
    cout << s;


    //auto res = BFS();
    //auto res = BFS2();

    QueDist que;
    Point endPt = teleportPoints["ZZ"].first;

    que.push({ teleportPoints["AA"].first, 0, 0, {} });
    set<pair<Point,int>> hist{};
    int res = 0;
    while (!que.empty())
    {
      auto crEl = que.top();

      if (crEl.point == endPt && crEl.depth == 0)
      {
        res = crEl.dist;

        auto s = to2Ds(crEl.prev, [](auto& l) { return l; }, [](auto& l) { return "*"; }, to2DsFlags::full_header, '.', 1);
        cout << s;

        break;
      }

      
      que.pop();
      /**/
      if (hist.insert({ crEl.point, crEl.depth }).second == false)
        continue; // seen

      for (auto n : nextPt[crEl.point])
      {
        Point ptx = n;
        ptx.z = 0;

        SimplePointData newDataPoint{ ptx , crEl.dist + 1, crEl.depth + n.z, crEl.prev };

        if (newDataPoint.depth > 0)
          continue;

        /**/
        if (n.z != 0)
        {
          int p = 0;
          p++;

          //cout << "Will change level : " << newDataPoint.depth << " cr level: "<< endl;

          vector<Point> thisLevel;
          copy_if(crEl.prev.begin(), crEl.prev.end(), back_inserter(thisLevel), [&](auto e) {return e.z == crEl.depth; });
          auto s = to2Ds(thisLevel, [](auto& l) { return l; }, [](auto& l) { return "*"; }, to2DsFlags::full_header, '.', 1);
         // cout << s;
        }
        /**/

        newDataPoint.prev.push_back(crEl.point);
        newDataPoint.prev.back().z = newDataPoint.depth;
        que.push(newDataPoint);
      }
    }

    toClipboard(res);
    return to_string(res);
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve(ReadFileToString(L"sample/sample4.txt")).Do() == ReadFileToString(L"sample/result4.txt"));

  //REQUIRE(Solve(ReadFileToString(L"sample/sample3.txt")).Do() == ReadFileToString(L"sample/result3.txt"));

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

  //REQUIRE(Solve(ReadFileToString(L"sample/sample2.txt")).Do() == ReadFileToString(L"sample/result2.txt"));

}

TEST_CASE("Sample 2", "[x.]") {
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
)").Do() == "396");  

}

// !!!!!! change hardcoded before activate

TEST_CASE("Part One", "[x.]") { 
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
}

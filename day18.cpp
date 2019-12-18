// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Priority que
//   .......
//   ....X..
//   #...X.@
//   ....X..
//   .......

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

array<Point, 4> start = { Point{-10,0,0},{-20,0,0},{-30,-1,0},{-40,1,0} };

array<char, 4> bots = { '@', '$', '%', '*' };


struct Solve {
  Input vec;
  Que que;


  int cols = 0;
  int lines = 0;


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
          doors[{ c, l}] = ch;
          hall.insert({ c,l });
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

    for (auto h : hall)
    {
      for (auto d : delta)
      {
        auto newPoint = h + d;
        if (hall.find(newPoint) != hall.end())
        {
          next[h].push_back(newPoint);
        }
      }
    }

  };

  map<Point, char> doors;
  map<Point, char> keys;
  set<Point> hall;
  map<Point, vector<Point>> next;

  set<pair<Point, string>> history;
  map<char, vector<char> > lockings;

  set<Point> historySet;
  void Parse(vector<char>& prevDoors, set<char>& doorsSet, Point pt)
  {


    for (auto n : next[pt])
    {
      if (historySet.insert(n).second == false)
        continue; // been;

      if (auto whereD = doors.find(n); whereD != doors.end())
      {
        prevDoors.push_back(whereD->second);
        doorsSet.insert(whereD->second);
        Parse(prevDoors, doorsSet, n);
        prevDoors.pop_back();
        doorsSet.erase(whereD->second);
      }
      else if (auto where = keys.find(n); where != keys.end())
      {
        lockings[where->second] = prevDoors;
        Parse(prevDoors, doorsSet, n);
      }
      else
        Parse(prevDoors, doorsSet, n);
    }
  }

  void Dependencies()
  {
    for (auto sP : start)
    {
      //auto sP = start[0];
      vector<char> c;
      set<char> s;

      historySet.insert(sP);
      Parse(c, s, sP);

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
        for (auto n : next[crEl.point])
        {
          SimplePointData newDataPoint{ n , crEl.dist + 1 };

          que.push(newDataPoint);
        }
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
    Dependencies();

    DistancesBetweenKeys();

    //auto res = BFS();
    auto res = BFS2();
    toClipboard(res);
    return to_string(res);
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(ReadFileToString(L"sample/sample4.txt")).Do() == ReadFileToString(L"sample/result4.txt"));

  REQUIRE(Solve(ReadFileToString(L"sample/sample3.txt")).Do() == ReadFileToString(L"sample/result3.txt"));


  CHECK(Solve(R"(
########################
#f.D.E.e.C.b.A.@.a.B.c.#
######################.#
#d.....................#
########################
)").Do() == "86");

  REQUIRE(Solve(R"(
#########
#b.A.@.a#
#########)").Do() == "8");
  REQUIRE(Solve(ReadFileToString(L"sample/sample2.txt")).Do() == ReadFileToString(L"sample/result2.txt"));

}

TEST_CASE("Sample 2", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  CHECK(Solve(R"(
#######
#a.#Cd#
##@#$##
#######
##*#%##
#cB#.b#
#######
)").Do() == "8");

  REQUIRE(Solve(R"(
###############
#d.ABC.#.....a#
######@#$######
###############
######*#%######
#b.....#.....c#
###############
)").Do() == "24");

  REQUIRE(Solve(R"(
#############
#DcBa.#.GhKl#
#.###@#$#I###
#e#d#####j#k#
###C#*#%###J#
#fEbA.#.FgHi#
#############
)").Do() == "32");

  REQUIRE(Solve(R"(
#############
#g#f.D#..h#l#
#F###e#E###.#
#dCba@#$BcIJ#
#############
#nK.L*#%G...#
#M###N#H###.#
#o#m..#i#jk.#
#############
)").Do() == "72");

}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
}

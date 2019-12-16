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
  set<Point> points;
  map<Point, int> dists;
  vector<DirSteps> inputDirSteps;
};

using Input = vector<Data>;

struct Solve {
  Input vec;

  vector<int_t> back;
  vector<Point> inters;


  Solve(string inStr) {

    for (auto ch : inStr)
      back.push_back(ch - '0');

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
    /* * /
    forEachRxToken(inStr, lineRxToken, [&](string line) {
      static const regex colsSepRx(" ");
    
      vec.push_back({});
      forEachRxToken(line, colsSepRx, [&](string colStr) {
        vec.back().append(colStr);
        });
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

  string Do()
  {
    //vector<int> pattern = { 0,1,0,-1 };
    map<int, vector<int>> patterns;
    map<int, vector<int>> plusIdxListMap, minusIdxListMap;
    for (auto step : irange(0, back.size()))
    {
      vector<int> crPattern((back.size() + 1));
      vector<int> plusIdxList;
      vector<int> minusIdxList;
      for (int pPos = 0; pPos <= back.size();)
      {
        for (int firstZ = 0; firstZ < step + 1 && pPos <= back.size(); firstZ++)
        {
          crPattern[pPos++] = 0;
        }
        for (int firstZ = 0; firstZ < step + 1 && pPos <= back.size(); firstZ++)
        {
          plusIdxList.push_back(pPos - 1);

          crPattern[pPos++] = 1;
        }

        for (int firstZ = 0; firstZ < step + 1 && pPos <= back.size(); firstZ++)
        {
          crPattern[pPos++] = 0;
        }

        for (int firstZ = 0; firstZ < step + 1 && pPos <= back.size(); firstZ++)
        {
          minusIdxList.push_back(pPos - 1);

          crPattern[pPos++] = -1;
        }
      }

      patterns[step] = crPattern;
      plusIdxListMap[step] = plusIdxList;
      minusIdxListMap[step] = minusIdxList;
    }


    for (auto step : irange(0, 100))
    {
      vector <int> newInput(back.size());
      for (auto newIdx : irange(0, back.size()))
      {
        int patternPos = 1;
        int sum = 0;
        /** /
        auto& crPattern = patterns[newIdx];
        for (auto idx : irange(0, back.size()))
        {
          sum += back[idx] * crPattern[patternPos];
          patternPos = (patternPos + 1) % crPattern.size();
        }
        /**/

        /**/
        auto& plusIdxes = plusIdxListMap[newIdx];
        for (auto idx : plusIdxes)
        {
          sum += back[idx];          
        }
        auto& minusIdxes = minusIdxListMap[newIdx];
        for (auto idx : minusIdxes)
        {
          sum -= back[idx];
        }
        /**/

        newInput[newIdx] = abs(sum) % 10;
      }
      back = std::move(newInput);
    }

//    cout << "P1: \n";
    int res = 0;
    for (auto i : irange(0, 8))
      res = res * 10 + back[i];

    return ToString(res);
  }

  string Do2()
  {

    return ToString(1);
  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

  //REQUIRE(Solve("12345678").Do() == "01029498"); // print yourself 
  REQUIRE(Solve("80871224585914546619083218645595").Do() == "24176176"); // print yourself 

  //REQUIRE(Solve("").Do() == "6"); // sample part 1
  //REQUIRE(Solve("").Do2() == "30"); // sample part 2
  //REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do() == ReadFileToString(L"sample/result.txt"));
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

/* Input ---------------

59738476840592413842278931183278699191914982551989917217627400830430491752064195443028039738111788940383790187992349338669216882218362200304304999723624146472831445016914494176940890353790253254035638361091058562936884762179780957079673204210602643442603213181538626042470133454835824128662952579974587126896226949714610624975813583386749314141495655816215568136392852888525754247201021383516228214171660111826524421384758783400220017148022332694799323429711845103305784628923350853888186977670136593067604033507812932778183786479207072226236705355778245701287481396364826358903409595200711678577506495998876303181569252680220083046665757597971122614

 ------------Input End*/

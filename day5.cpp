// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Day 2: 1202 Program Alarm
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

using Input = vector<int>;

struct Solve {
  Input vec;
  Solve(string inStr) {
    static const regex lineSepRx(",");
    //static const regex lineSepRx("\\n");

  //   static const regex rx(R"((\d+), (\d+))");
  // 
  //   string s = "30, 59";
  // 
  //   smatch match;
  //   regex_match(s, match, rx);
  //   atoi(match[1].str().c_str());

    for (sregex_token_iterator iter(inStr.begin(), inStr.end(), lineSepRx, -1), end; iter != end; ++iter)
    {
      vec.push_back(atoi(iter->str().c_str()));
    }
    vec.resize(100000);
  };

  vector<int> outputs;
  int regA;
  int next(int pos, int  step)
  {
    return (pos + step) % vec.size();
  }

  bool Exec(int & pos)
  {
    int instr = vec[pos];
    int opCode = instr % 100;
    instr = instr / 100;

    int mode1 = instr % 10;
    instr = instr / 10;

    int mode2 = instr % 10;
    instr = instr / 10;

    int mode3 = 1;
    //instr % 10;
    instr = instr / 10;

    int first, second, third;
    int vFirst, vSecond, vThird;

    switch (opCode)
    {
    case 1:
    case 2:
    {
      first = vec[next(pos, 1)];
      second = vec[next(pos, 2)];
      third = vec[next(pos, 3)];
      pos = next(pos, 4);

      vFirst = mode1 ? first : vec[first];
      vSecond = mode2 ? second : vec[second];
      vThird = mode3 ? third : vec[third];

    }
    break;
    case 3:
    {
      first = vec[next(pos, 1)];
      vFirst = first;

      pos = next(pos, 2);
    }
    break;
    case 4:
    {
      first = vec[next(pos, 1)];
      vFirst = mode1 ? first : vec[first];

      pos = next(pos, 2);
    }
    break;

    case 5:
    case 6:
      first = vec[next(pos, 1)];
      second = vec[next(pos, 2)];
      pos = next(pos, 3);

      vFirst = mode1 ? first : vec[first];
      vSecond = mode2 ? second : vec[second];
      
      break;
    case 7:
    case 8:
      first = vec[next(pos, 1)];
      second = vec[next(pos, 2)];
      third = vec[next(pos, 3)];
      pos = next(pos, 4);

      vFirst = mode1 ? first : vec[first];
      vSecond = mode2 ? second : vec[second];
      vThird = mode3 ? third : vec[third];
      break;
    }

    
    int res = 0;
    if (opCode == 1)
    {
      res = vFirst + vSecond;
      vec[vThird] = res;
    }
    else if (opCode == 2)
    {
      res = vFirst * vSecond;
      vec[vThird] = res;
    }
    else if (opCode == 3)
    {
      vec[vFirst] = regA;
    }
    else if (opCode == 3)
    {
      vec[vFirst] = regA;
    }
    else if (opCode == 5) //j
    {
      if (vFirst)
        pos = vSecond;
    }
    else if (opCode == 6) // jn
    {
      if (vFirst == 0)
        pos = vSecond;
    }
    else if (opCode == 7) // lt
    {
      if (vFirst < vSecond)
        vec[vThird] = 1;
      else
        vec[vThird] = 0;

    }
    else if (opCode == 8) // eq
    {
      if (vFirst == vSecond)
        vec[vThird] = 1;
      else
        vec[vThird] = 0;
    }
    else if (opCode == 4)
    {
      outputs.push_back(vFirst);
    }
    else if (opCode == 99)
    {
      return false;
    }

    return true;
  }

  static int calc(int op, int l, int r)
  {
    return op == 1 ? l + r : l * r;
  }

  void perform(int pos)
  {
    auto v = calc(vec[pos], vec[vec[next(pos, 1)]], vec[vec[next(pos, 2)]]);
    vec[vec[next(pos, 3)]] = v;
  };

  string Do(int a)
  {
    regA = a;
    if (/*a > */0 )
    {
      vec[1] = a;
    }

    int pos = 0;

    while (vec[pos] != 99)
    {
      Exec(pos);
      //pos = next(pos, 4);
    }

    return ToString(outputs.back());
  }

  string Do2()
  {
    Input back = vec;
    for (auto i : irange(0, 100))
      for (auto j : irange(0, 100))
      {
        vec = back;
        if (Do(i) == "19690720")
          return ToString(i * 100 + j);
      }

    return "";
  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;


  REQUIRE(Solve("3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9").Do(5) == "1"); // sample test
  REQUIRE(Solve("3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9").Do(0) == "0"); // sample test
  //REQUIRE(Solve("1002,4,3,4").Do(100) == "100"); // sample test
  //REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do(-1) == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do(5));
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}


// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Day 7: Amplification Circuit 
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

using Input = vector<LONGLONG>;

struct Solve {
  Input back;
  Input* vec;
  Solve(string inStr) :vec(0) {
    static const regex lineSepRx(",");

    Input vec2;

    for (sregex_token_iterator iter(inStr.begin(), inStr.end(), lineSepRx, -1), end; iter != end; ++iter)
    {
      vec2.push_back(atoll(iter->str().c_str()));
    }

    back = vec2;
  };


  vector<LONGLONG> outputs;
  vector<LONGLONG> regA;

  LONGLONG base = 0;
  LONGLONG next(LONGLONG pos, LONGLONG  step)
  {
    return (pos + step) % vec->size();
  }

  void applymode (LONGLONG& p, LONGLONG mode, LONGLONG v)
  {
    if (mode == 1) //imd
      p = v;
    else if (mode == 0) // pos
    {
      if (v + base >= vec->size())
        vec->resize(v + base + 5);

      p = (*vec)[v];
    }
    else
    {
      if (v + base >= vec->size())
        vec->resize(v + base + 5);

      p = (*vec)[v + base];
    }
  }

  void applymode3 (LONGLONG& p, LONGLONG mode, LONGLONG v)
  {
    if (mode == 1) //imd
      p = v;
    else if (mode == 0) // pos same as imd for third
      p = v;      //p = (*vec)[v];
    else
    {
      if (v + base >= vec->size())
        vec->resize(v + base + 5);

      p = (*vec)[v + base];
    }
  }

  bool Exec(LONGLONG& pos)
  {
    LONGLONG instr = (*vec)[pos];
    LONGLONG opCode = instr % 100;
    instr = instr / 100;

    LONGLONG mode1 = instr % 10;
    instr = instr / 10;

    LONGLONG mode2 = instr % 10;
    instr = instr / 10;

    LONGLONG mode3 = instr % 10;
    instr = instr / 10;

    LONGLONG first, second, third;
    LONGLONG vFirst, vSecond, vThird;

    switch (opCode)
    {
    case 1:
    case 2:
    {
      first = (*vec)[next(pos, 1)];
      second = (*vec)[next(pos, 2)];
      third = (*vec)[next(pos, 3)];
      pos = next(pos, 4);

      applymode(vFirst, mode1, first);
      applymode(vSecond, mode2, second);
      applymode3(vThird, mode3, third);

      //vFirst = mode1 ? first : (*vec)[first];
      //vSecond = mode2 ? second : (*vec)[second];
      //vThird = mode3 ? third : (*vec)[third];

    }
    break;
    case 3:
    {
      first = (*vec)[next(pos, 1)];
      vFirst = first;

      pos = next(pos, 2);
    }
    break;
    case 4:
    {
      first = (*vec)[next(pos, 1)];
      //vFirst = mode1 ? first : (*vec)[first];
      applymode(vFirst, mode1, first);


      pos = next(pos, 2);
    }
    break;

    case 5:
    case 6:
      first = (*vec)[next(pos, 1)];
      second = (*vec)[next(pos, 2)];
      pos = next(pos, 3);

      //vFirst = mode1 ? first : (*vec)[first];
      //vSecond = mode2 ? second : (*vec)[second];

      applymode(vFirst, mode1, first);
      applymode(vSecond, mode2, second);


      break;
    case 7:
    case 8:
      first = (*vec)[next(pos, 1)];
      second = (*vec)[next(pos, 2)];
      third = (*vec)[next(pos, 3)];
      pos = next(pos, 4);

      /*
      vFirst = mode1 ? first : (*vec)[first];
      vSecond = mode2 ? second : (*vec)[second];
      vThird = mode3 ? third : (*vec)[third];
      */

      applymode(vFirst, mode1, first);
      applymode(vSecond, mode2, second);
      applymode3(vThird, mode3, third);

      break;

    case 9:
      first = (*vec)[next(pos, 1)];
      pos = next(pos, 2);

      base += first;
      
      break;
    }



    LONGLONG res = 0;
    if (opCode == 1)
    {
      res = vFirst + vSecond;
      (*vec)[vThird] = res;
    }
    else if (opCode == 2)
    {
      res = vFirst * vSecond;
      (*vec)[vThird] = res;
    }
    else if (opCode == 3)
    {
      if (regA.size() == 2)
      {
        //cout << "FirstRead" << endl;
      }
      else if (regA.size() == 1)
      {
        //cout << "SecondRead" << endl;
      }
      else if (regA.size() == 0)
      {
        cout << "ThRead" << endl;
      }
      (*vec)[vFirst] = regA.front();
      //remove(regA.erase(regA.begin(), regA.end());
      regA.erase(regA.begin());
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
        (*vec)[vThird] = 1;
      else
        (*vec)[vThird] = 0;

    }
    else if (opCode == 8) // eq
    {
      if (vFirst == vSecond)
        (*vec)[vThird] = 1;
      else
        (*vec)[vThird] = 0;
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

  static LONGLONG calc(LONGLONG op, LONGLONG l, LONGLONG r)
  {
    return op == 1 ? l + r : l * r;
  }

  void perform(LONGLONG pos)
  {
    auto v = calc((*vec)[pos], (*vec)[(*vec)[next(pos, 1)]], (*vec)[(*vec)[next(pos, 2)]]);
    (*vec)[(*vec)[next(pos, 3)]] = v;
  };


  LONGLONG Do(vector<LONGLONG> a)
  {
    LONGLONG prev = 0;
    Input amp = back;
    //for (auto i : a)
    {
      outputs.clear();
      regA = a;
      vec = &back;

      LONGLONG pos = 0;

      while ((*vec)[pos] != 99)
      {
        Exec(pos);
      }

      //prev = outputs.back();
    }
    return outputs.back();
  }

  LONGLONG DoOld(vector<LONGLONG> a)
  {
    LONGLONG prev = 0;
    Input amp = back;
    for (auto i : a)
    {
      outputs.clear();
      regA = { i, prev };
      vec = &amp;

      LONGLONG pos = 0;

      while ((*vec)[pos] != 99)
      {
        Exec(pos);
      }

      prev = outputs.back();
    }
    return prev;
  }

  string Try()
  {
    LONGLONG max = -10000;
    LONGLONG seq = 0;
    for (auto i1 : irange(0, 5))
      for (auto i2 : irange(0, 5))
        for (auto i3 : irange(0, 5))
          for (auto i4 : irange(0, 5))
            for (auto i5 : irange(0, 5))
            {
              vector<LONGLONG> x = { i1,i2,i3,i4,i5 };
              auto y = x;
              sort(y.begin(), y.end());
              bool ok = true;
              for (auto p : irange(0, y.size() - 1))
              {
                if (y[p] == y[p + 1])
                {
                  ok = false;
                  break;
                }
              }
              if (!ok)
                continue;

              auto c = Do(x);
              if (c > max)
              {
                seq = i1 * 10000 + i2 * 1000 + i3 * 100 + i4 * 10 + i5;

                cout << "Improving: " << c << " with seq: " << seq << endl;
                max = c;
              }
            }

    return ToString(max);
  }


  LONGLONG DoNew(vector<LONGLONG> a)
  {
    vector<Input> amps(a.size());
    for (auto& v : amps)
      v = back;

    map<LONGLONG, bool> stopped;
    vector<LONGLONG> ipList(a.size());

    LONGLONG step = 0;
    LONGLONG prev = 0;
    do {
      for (auto i : irange(0, a.size()))
      {
        if (step == 0)
          regA = { a[i], prev };
        else
          regA = { prev };

        vec = &amps[i];

        LONGLONG pos = ipList[i];

        while ((*vec)[pos] != 99)
        {
          LONGLONG prevInstPos = pos;
          Exec(pos);

          if ((*vec)[prevInstPos] == 4)
          {
            ipList[i] = pos;
            break;
          }
        }

        if ((*vec)[pos] == 99)
          stopped[i] = true;

        prev = outputs.back();
      }
      step++;
    } while (!stopped[4]);

    return prev;
  }

  string Try2()
  {
    LONGLONG max = -10000;
    LONGLONG seq = 0;
    for (auto i1 : irange(5, 10))
      for (auto i2 : irange(5, 10))
        for (auto i3 : irange(5, 10))
          for (auto i4 : irange(5, 10))
            for (auto i5 : irange(5, 10))
            {
              vector<LONGLONG> x = { i1,i2,i3,i4,i5 };
              auto y = x;
              sort(y.begin(), y.end());
              bool ok = true;
              for (auto p : irange(0, y.size() - 1))
              {
                if (y[p] == y[p + 1])
                {
                  ok = false;
                  break;
                }
              }
              if (!ok)
                continue;

              auto c = DoNew(x);
              if (c > max)
              {
                seq = i1 * 10000 + i2 * 1000 + i3 * 100 + i4 * 10 + i5;

                cout << "Improving: " << c << " with seq: " << seq << endl;
                max = c;
              }
            }

    return ToString(max);
  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;


  REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").Do({}) == 99); // sample test
  REQUIRE(Solve("1102,34915192,34915192,7,4,7,99,0").Do({ }) == 1219070632396864); // sample test
  REQUIRE(Solve("104,1125899906842624,99").Do({  }) == 1125899906842624); // sample test
  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 1,0,0,0,0 }) == 1421358); // sample test 121706

  //REQUIRE(Solve("3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9").Do(0) == "0"); // sample test
  //REQUIRE(Solve("1002,4,3,4").Do(100) == "100"); // sample test 
  //REQUIRE(Solve(ReadFileToString(L"sample/sample.txt")).Do(-1) == ReadFileToString(L"sample/result.txt"));
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  //REQUIRE(Solve("3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0").Try() == "44444");
  //REQUIRE(Solve("3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0").Try() == "55555");
  //REQUIRE(Solve("3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0").Try() == "66666");
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do({ 1 }));
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two test", "[.]") {
  cout << endl << "Part Two Test ------------- " << endl;

  REQUIRE(Solve("3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5").Try2() == "139629729");

}

TEST_CASE("Part Two", "[.]") {
  cout << endl << "Part Two ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Try());
  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 4,4,4,4,4 }) == 2);

  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}
TEST_CASE("Part Two2", "[.]") {
  cout << endl << "Part Two ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Try2());
  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 4,4,4,4,4 }) == 2);

  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

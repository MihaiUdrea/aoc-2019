// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//--- Day 11:
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"


enum class AccMode
{
  positional,
  immediate,
  relative
};

using int_t = LONGLONG;

using Input = vector<int_t>;
struct Program {

  vector<int_t> instructions;
  vector<LONGLONG> regA, output;
  size_t crPos;
  int_t base;
  stringstream logStream;
  stringstream logDataStream;

  struct InstrData
  {
    int opCode;
    std::function<bool()> exec;
    int paramCount;
    string name;
    string lead;

    InstrData(int o, std::function<bool()> e, int p, string s, string l) :opCode(o), exec(e), paramCount(p), name(s), lead(l) {}
  };

  map<int, InstrData> map;

  Program(const vector<int_t>& pr, const vector<int_t>& in) :instructions(pr), crPos(0), regA(in), base(0)
  {
    vector<InstrData> instrList{
      // Code Name                  ParamCount Sign      LogLead
      { 1, bind(&Program::Add,    this),    3, "+"       , ""             }, // LogLead empty only for 3 operators use v[3]
      { 2, bind(&Program::Mul,    this),    3, "*"       , ""             },
      { 3, bind(&Program::Read,   this),    1, ""        , " INP    -> "      },
      { 4, bind(&Program::Print,  this),    1, ""        , " OUT    <- "      },
      { 5, bind(&Program::Jump,   this),    2, " ?  "    , " IP     <- "      },
      { 6, bind(&Program::JumpNot,this),    2, " !? "    , " IP     <- "      },
      { 7, bind(&Program::Less,   this),    3, "<"       , ""                },
      { 8, bind(&Program::Equal,  this),    3, "=="      , ""                },
      { 9, bind(&Program::IncBase,this),    1, ""        , " BAS    += "   }
    };

    for (auto i : instrList)
      map.insert({ i.opCode,  i });
  }

  static size_t GetDigit(size_t v, size_t idx)
  {
    size_t d = v / (size_t)pow(10u, idx);
    return d % 10;
  }

  AccMode GetParamMode(size_t idx)
  {
    auto inst = instructions[crPos];
    return static_cast<AccMode>(GetDigit((size_t)inst, idx + 1));
  }

  void Expand(size_t pos)
  {
    if (pos >= instructions.size())
      instructions.resize(pos + 1);
  }

  int_t GetExpand(size_t pos)
  {
    Expand(pos);

    return instructions[pos];
  }
  int_t GetParam(size_t idx, bool mod3 = false)
  {
    //logStream << "{" << idx << ": ";

    auto mode = GetParamMode(idx);

    int_t v = instructions[crPos + idx];
    int_t res = v;
    switch (mode)
    {
    case AccMode::positional:
      logStream << "v[" << v << "]";
      res = (idx == 3 || mod3) ? v : GetExpand((size_t)v);
      break;
    case AccMode::immediate:
      logStream << v << " ";

      res = v;
      break;
    case AccMode::relative:
      logStream << "v[base+" << v << "]";

      res = (idx == 3 || mod3) ? v + base : GetExpand((size_t)(v + base));
      break;
    default:
      exit(1);
    }

    //logStream << res << "} ";

    return res;
  }

  void Store(size_t param3Idx, int_t val)
  {
    if (param3Idx != 3)
    {
//      cout << "HERE" << endl;
    }

    auto idx = (size_t)GetParam(param3Idx, true);

    Expand(idx);

    logDataStream << "     [" << idx << "]: " << instructions[idx] << " -> " << val;

    instructions[idx] = val;
  }

  bool Add()
  {
    Store(3, GetParam(1) + GetParam(2));

    return true; // advance
  }

  bool Mul()
  {
    Store(3, GetParam(1) * GetParam(2));
    return true; // advance
  }

  bool Read()
  {
    Store(1, regA.front());

    regA.erase(regA.begin());

    return true; // advance
  }

  bool Print()
  {
    output.push_back(GetParam(1));

    logDataStream << "     OUT +=" << output.back();

    return true; // advance
  }

  bool Jump()
  {
    if (GetParam(1))
    {
      crPos = (size_t)GetParam(2);

      logDataStream << "     IP <- " << crPos;

      return false; // don't advance
    }
    else
    {
      logDataStream << "     SKIP JUMP";
    }

    return true; // advance
  }

  bool JumpNot()
  {
    if (GetParam(1) == 0)
    {
      crPos = (size_t)GetParam(2);

      logDataStream << "     IP <- " << crPos;

      return false; // don't advance
    }
    else
    {
      logDataStream << "     SKIP JUMP";
    }

    return true; // advance
  }

  bool Less()
  {
    Store(3, GetParam(1) < GetParam(2));
    return true; // advance
  }

  bool Equal()
  {
    Store(3, GetParam(1) == GetParam(2));
    return true; // advance
  }

  bool IncBase()
  {
    logDataStream << "     BASE : " << std::setw(5) << base << " -> ";

    base += GetParam(1);

    logDataStream << base;

    return true;
  }

  void Log(const InstrData& inst)
  {
    stringstream codeLog;

    logStream.str("");
    codeLog.str("");

    codeLog << endl << std::setfill(' ') << std::setw(5) << crPos << " -> " << std::setw(5) << (size_t)instructions[crPos] << ": ";

    // v[56543] =  | IP  <- .....
    if (inst.lead.empty())
    {
      assert(inst.paramCount == 3);
      GetParam(3);
      logStream << " <- ";
    }
    else
    {
      logStream << inst.lead;
    }

    cout << codeLog.str() << std::setw(30) << logStream.str();

    logStream.str("");

    // v[first]
    GetParam(1);

    // +
    logStream << std::setw(4) << inst.name;

    // v[second]
    if (inst.paramCount >= 2)
      GetParam(2);

    cout << std::setw(18) << logStream.str();

    // data change during / after exec
  }

  void Run()
  {
    while (instructions[crPos] != 99)
    {

      auto inst = (size_t)instructions[crPos] % 100;
      const auto& instData = map.find(inst)->second;

      //Log(instData);

      logDataStream.str("");

      bool adv = instData.exec();
      if (adv)
        crPos += instData.paramCount + 1;

      //cout << logDataStream.str();

      if (output.size() == 2)
        break;
    }
  }
};

struct Solve {
  Input back;

  Solve(string inStr)
  {
    static const regex colsRxToken(",");
    forEachRxToken(inStr, colsRxToken, [&](string instr) {
      back.push_back(atoll(instr.c_str()));
      });
  };

  int_t Do(vector<int_t> a)
  {

    Program p(back, a);

    p.Run();

    return p.output.back();
  }

  using Point = pair<int, int>;
  int_t DoNew(vector<int_t> a)
  {

    Program p(back, a);

    //p.PrintProg();

    Point crPoint;
    char direction = '^';

    set<Point> paintedWhite;
    set<Point> paintedBlack;
    do
    {
      p.output.clear();

      p.Run();

      if (p.output.size() == 2)
      { 

        // mark new color
        if (p.output.front() == 0)
        {
          // black
          //cout << "BLACK";
          paintedBlack.insert(crPoint);
          paintedWhite.erase(crPoint);
        }
        else
        {
          //cout << "WHITE";

          // white
          assert(p.output.front() == 1);

          paintedWhite.insert(crPoint);
          paintedBlack.erase(crPoint);
        }

        // move robot
        if (p.output.back() == 0) // left
        {
          switch (direction)
          {
          case '^':
            direction = '<';
            break;
          case '<':
            direction = 'v';
            break;
          case 'v':
            direction = '>';
            break;
          case '>':
            direction = '^';
            break;
          }

        }
        else // right
        {
          switch (direction)
          {
          case '^':
            direction = '>';
            break;
          case '>':
            direction = 'v';
            break;
          case 'v':
            direction = '<';
            break;
          case '<':
            direction = '^';
            break;
          }
        }

        // move pos

        switch (direction)
        {
        case '^':
          crPoint.first--;
          break;
        case '>':
          crPoint.second++;
          break;
        case 'v':
          crPoint.first++;
          break;
        case '<':
          crPoint.second--;
          break;
        }

      }

      if (paintedWhite.find(crPoint) != paintedWhite.end())
        p.regA = { 1 };
      else
        p.regA = { 0 };

    } while (p.output.size() == 2);

    // print white
    {
      for (auto i : irange(0, 50))
      {
        cout << endl;
        for (auto j : irange(0, 50))
          if (paintedWhite.find({ i,j }) == paintedWhite.end())
            cout << " ";
          else
            cout << "#";
      }
    }


    return paintedBlack.size() + paintedWhite.size();
  }
};

TEST_CASE("Sample 1", "[.]") {

  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").Do({}) == 99); // print yourself 
  REQUIRE(Solve("1102,34915192,34915192,7,4,7,99,0").Do({ }) == 1219070632396864); // some val test
  REQUIRE(Solve("104,1125899906842624,99").Do({  }) == 1125899906842624); // huge test
  REQUIRE(Solve(ReadFileToString(L"input2.txt")).Do({ 1 }) == 2465411646); // sample test
  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 2 }) == 69781); // sample test
}

TEST_CASE("New1", "[.]") {

  cout << endl << "Tests   ------------- " << endl;

  
  toClipboard( Solve(ReadFileToString(L"input.txt")).DoNew({ 1 }));
  
}

TEST_CASE("New2", "[x.]") {

  cout << endl << "Tests   ------------- " << endl;


  toClipboard(Solve(ReadFileToString(L"input.txt")).DoNew({ 1 }));

}


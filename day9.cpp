// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Day 7: Amplification Circuit 
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

  struct InstrData
  {
    int opCode;
    std::function<bool()> exec;
    int paramCount;
    string name;

    InstrData(int o, std::function<bool()> e, int p, string s) :opCode(o), exec(e), paramCount(p), name(s) {}
  };

  map<int, InstrData> map;

  Program(const vector<int_t> & pr, const vector<int_t>& in) :instructions(pr), crPos(0), regA(in), base(0)
  {
    vector<InstrData> instrList{ 
      // Code Name Param Sign
      { 1, bind(&Program::Add,    this),    3, "+"        },
      { 2, bind(&Program::Mul,    this),    3, "*"        },
      { 3, bind(&Program::Read,   this),    1, "input"    },
      { 4, bind(&Program::Print,  this),    1, "print"    },
      { 5, bind(&Program::Jump,   this),    2, "jump"     },
      { 6, bind(&Program::JumpNot,this),    2, "jump not" },
      { 7, bind(&Program::Less,   this),    3, "<"        },
      { 8, bind(&Program::Equal,  this),    3, "=="       },
      { 9, bind(&Program::IncBase,this),    1, "Base+="   }
    };

    for (auto i : instrList)
      map.insert({ i.opCode,  i });
  }

  static size_t GetDigit(size_t v, size_t idx)
  {
    size_t d = v / (size_t) pow(10u, idx);
    return d % 10;
  }

  AccMode GetParamMode(size_t idx)
  {
    auto inst = instructions[crPos];
    return static_cast<AccMode>(GetDigit((size_t)inst, idx + 1));
  }

  int_t GetExpand(size_t pos)
  {
    if (pos >= instructions.size())
      instructions.resize(pos + 1);

    return instructions[pos];
  }
  int_t GetParam(size_t idx)
  {
    cout << "{" << idx << ": ";

    auto mode = GetParamMode(idx);

    int_t v = instructions[crPos + idx];
    int_t res = v;
    switch (mode)
    {
    case AccMode::positional:
      cout << "v[" << v << "]=";
      res = (idx == 3) ? v : GetExpand((size_t)v);
      break;
    case AccMode::immediate:
      res = v;
      break;
    case AccMode::relative:
      cout << "v[base+" << v << "]=";

      res = (idx == 3) ? v + base : GetExpand((size_t)(v + base));
      break;
    default:
      exit(1);
    }

    cout << res << "} ";

    return res;
  }

  void Store(size_t param3Idx, int_t val)
  {
    auto idx = (size_t)GetParam(3);
    if (idx >= instructions.size())
      instructions.resize(idx + 1);

    instructions[idx] = val;

    cout << "now: " << val;
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
    Store(2, regA.front());

    regA.erase(regA.begin());
    return true; // advance
  }

  bool Print()
  {
    output.push_back(GetParam(1));
    return true; // advance
  }

  bool Jump()
  {
    if (GetParam(1))
    {
      crPos = (size_t) GetParam(2);
      return false; // don't advance
    }

    return true; // advance
  }

  bool JumpNot()
  {
    if (GetParam(1) == 0)
    {
      crPos = (size_t) GetParam(2);
      return false; // don't advance
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
    base += GetParam(1);

    cout << "Base=" << base;

    return true;
  }

  void Log(const InstrData& inst)
  {
    cout << endl << crPos << " -> " << (size_t)instructions[crPos] << ": " << inst.name << " ";
  }

  void Run()
  {
    while (instructions[crPos] != 99)
    {
      auto inst = (size_t)instructions[crPos] % 100;
      const auto& instData = map.find(inst)->second;
      
      Log(instData);

      bool adv = instData.exec();
      if (adv)
        crPos += instData.paramCount + 1;
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

};

TEST_CASE("Sample 1", "[x.]") {

  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").Do({}) == 99); // print yourself 
  REQUIRE(Solve("1102,34915192,34915192,7,4,7,99,0").Do({ }) == 1219070632396864); // some val test
  REQUIRE(Solve("104,1125899906842624,99").Do({  }) == 1125899906842624); // huge test
  REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 1 }) == 2465411646); // sample test
  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 2 }) == 69781); // sample test
}


// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//--- Day 11: Space Police ----
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"


enum Direction
{
  up,
  right,
  down,
  left,

  MAX
};

enum Color
{
  black,
  white
};

struct Point
{
  int y = 0;
  int x = 0;

  auto operator<=>(const Point&) const = default;
  auto operator+(const Point& l) { return Point{ y + l.y, x + l.x }; };
};

vector<Point> deltaMove = {
    {-1,  0}, // Direction::up,
    { 0,  1}, // Direction::right
    { 1,  0}, // Direction::down,  
    { 0, -1}  // Direction::left
};

vector<Direction> changeDir = { Direction::left, Direction::right };

using int_t = LONGLONG;

using Input = vector<int_t>;
struct Program {

  enum class AccMode
  {
    positional,
    immediate,
    relative
  };

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

  int Log(const InstrData& inst)
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

    return 0;
  }

  void Run(int outputSizeBreak = 0)
  {
    bool log = false;
    while (instructions[crPos] != 99)
    {

      auto inst = (size_t)instructions[crPos] % 100;
      const auto& instData = map.find(inst)->second;

      log ? Log(instData):1;

      logDataStream.str("");

      bool adv = instData.exec();
      if (adv)
        crPos += instData.paramCount + 1;

      log ? (cout << logDataStream.str()) : cout;

      if (outputSizeBreak > 0 && output.size() == outputSizeBreak)
        break;
    }
  }
};

struct Solve {
  Input back;
  array<set<Point>, Color::white + 1> paintedLists;

  Solve(string inStr)
  {
    static const regex colsRxToken(",");
    forEachRxToken(inStr, colsRxToken, [&](string instr) {
      back.push_back(atoll(instr.c_str()));
      });
  };

  int_t JustRun(vector<int_t> a)
  {
    Program p(back, a);

    p.Run();

    return p.output.back();
  }

  int Do(vector<int_t> a)
  {
    Program p(back, a);

    Point crPoint;
    Direction direction = Direction::up;

    do
    {
      p.output.clear();

      p.Run(2);

      if (p.output.size() == 2)
      {
        size_t out = (size_t)p.output.front();

        auto & toAdd = paintedLists[out];
        auto & toRemove = paintedLists[1 - out];

        toAdd.insert(crPoint);
        toRemove.erase(crPoint);

        // orient robot        
        direction = Direction((direction + changeDir[(size_t)p.output.back()]) % Direction::MAX);

        // move robot
        crPoint = crPoint + deltaMove[direction];
      }

      if (contains(paintedLists[Color::white],crPoint))
        p.regA = { 1 };
      else
        p.regA = { 0 };

    } while (p.output.size() == 2);

    return paintedLists[Color::black].size() + paintedLists[Color::white].size();
  }

  void Do2(vector<int_t> a)
  {
    Do(a);

    // print white
    {
      auto minMaxX = minmax_element(paintedLists[Color::white].begin(), paintedLists[Color::white].end(), [](auto& l, auto& r) {
        return l.x < r.x;
        });
      auto minMaxY = minmax_element(paintedLists[Color::white].begin(), paintedLists[Color::white].end(), [](auto& l, auto& r) {
        return l.y < r.y;
        });

      for (auto y : irange(minMaxY.first->y, minMaxY.second->y + 1))
      {
        cout << endl;
        for (auto x : irange(minMaxY.first->x, minMaxY.second->x + 1))
          cout << (contains(paintedLists[Color::white], Point{ y,x }) ? "#" : " ");
      }
      cout << endl;
    }
  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99").JustRun({}) == 99); // print yourself 
  REQUIRE(Solve("1102,34915192,34915192,7,4,7,99,0").JustRun({ }) == 1219070632396864); // some val test
  REQUIRE(Solve("104,1125899906842624,99").JustRun({  }) == 1125899906842624); // huge test
  //REQUIRE(Solve(ReadFileToString(L"input2.txt")).JustRun({ 1 }) == 2465411646); // sample test
  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Do({ 2 }) == 69781); // sample test
}

TEST_CASE("New1", "[x.]") {
  toClipboard((int) Solve(ReadFileToString(L"input.txt")).Do({ 0 }));
}

TEST_CASE("New2", "[x.]") {
  Solve(ReadFileToString(L"input.txt")).Do2({ 1 });
}


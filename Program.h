using int_t = int;


struct Program {

  enum class AccMode
  {
    positional,
    immediate,
    relative
  };

  vector<int_t> instructions;
  vector<int_t> regA, output;
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
  bool readModeDrop = true;
  Program(const vector<int_t>& pr = {}, const vector<int_t>& in = {}, const string& rawInst = {}, bool readModeDrop = true) 
    :instructions(pr), crPos(0), regA(in), base(0), readModeDrop(readModeDrop)
  {
    if (!rawInst.empty())
    {
      static const regex colsRxToken(",");
      forEachRxToken(rawInst, colsRxToken, [&](string instr) {
        instructions.push_back((int_t)atoll(instr.c_str()));
        });
    }

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

    if (readModeDrop) 
      regA.erase(regA.begin());

    return true; // advance
  }

  void SetPorts(vector<int_t> in, vector<int_t> out = {})
  {
    output = out;
    regA = in;
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

      log ? Log(instData) : 1;

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

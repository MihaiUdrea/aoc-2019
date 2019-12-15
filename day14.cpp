// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

struct PointData
{
  Point point;
  __int64 dist = 0;
};

struct DirSteps {
  char dir;
  __int64 steps;
};


struct Product
{
  __int64 quantity;
  string product;
};

struct Reaction : Product
{
  map<string, __int64> receipe;
};

using Input = vector<Reaction>;

struct Solve {
  Input vec;

  vector<int_t> back;
  vector<Point> inters;


  Solve(string inStr) {

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
    /* */
    forEachRxToken(inStr, lineRxToken, [&](string line) {
    
      static const regex matchExp(R"~((.*)=> (\d+) (\w+))~");
      
      auto res = match_rx(line, matchExp);

      Reaction r;
      r.quantity = stoi(res[2].str());
      r.product = res[3].str();
    
      static const regex colsSepRx(", ");
      forEachRxToken(res[1].str(), colsSepRx, [&](string colStr) {
        static const regex matchExp2(R"~((\d+) (\w+)\s*)~");
        auto res = match_rx(colStr, matchExp2);

        Reaction r2;
        r2.quantity = stoi(res[1].str());
        r2.product = res[2].str();

        r.receipe[r2.product] = r2.quantity;
        });

      vec.push_back(r);
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

  auto Find(string s)
  {
    for (auto i : irange(0, vec.size()))
    {
      if (vec[i].product == s)
      {
        return i;
      }
    }
    return -1;
  }

/** /
  map<string, set<Product>> map;
  void GetRequirements(__int64 idx)
  {
    Reaction& crReac = vec[idx];

    for (auto it = crReac.receipe.begin(); it != crReac.receipe.end(); ++it)
    {
      __int64 oreReq = GetRequirements(Find(it->first));
      sum += oreReq;
    }
  }
  /**/

  using state = map<string, __int64>;
  
  using stateSmall = vector<__int64>;

  state remaining;
  state requiredToProduce;
  __int64 Parse0(__int64 idx, __int64 reqQuant)
  {
    if (idx == -1)
      return reqQuant;


    Reaction& crReac = vec[idx];
    // cout << endl;
    if (reqQuant > remaining[crReac.product])
    {

      reqQuant -= remaining[crReac.product];
      /** /
      cout << " Require: " << reqQuant << " of " << crReac.product;

      //if (remaining[crReac.product] > 0)
      cout << " reuse " << remaining[crReac.product];

      cout << " need to produce " << reqQuant << " of " << crReac.product;

      /**/

      __int64 sum = 0;

      requiredToProduce[crReac.product] += reqQuant;

      __int64 batches = (reqQuant - 1) / crReac.quantity + 1;
      for (auto bi : irange(0, batches)) {
        for (auto it = crReac.receipe.begin(); it != crReac.receipe.end(); ++it)
        {
          __int64 oreReq = Parse0(Find(it->first), it->second);
          sum += oreReq;
        }

        reqQuant -= crReac.quantity;
      }

      //__int64 quantProduced = batches * crReac.quantity;

      __int64 res = sum;

      remaining[crReac.product] = -reqQuant;

      //cout << " quant produced " << crReac.quantity << " of " << crReac.product << " total consumed ore: " << res << " remaining " << crReac.product << " " << remaining[crReac.product];

      return res;
    }
    else
    {
      //cout << " Fulfill " << reqQuant << " remaining " << crReac.product << " " << remaining[crReac.product];

      remaining[crReac.product] -= reqQuant;
      return 0;
    }
  }



  __int64 Parse(__int64 idx, __int64 reqQuant)
  {
    if (idx == -1)
      return reqQuant;


    Reaction& crReac = vec[idx];
   // cout << endl;
    if (reqQuant > remaining[crReac.product])
    {

      reqQuant -= remaining[crReac.product];
      /** /
      cout << " Require: " << reqQuant << " of " << crReac.product;

      //if (remaining[crReac.product] > 0)
      cout << " reuse " << remaining[crReac.product];

      cout << " need to produce " << reqQuant << " of " << crReac.product;

      /**/

      __int64 sum = 0;

      __int64 batches = (reqQuant - 1) / crReac.quantity + 1;
      for (auto bi : irange(0, batches)){
        for (auto it = crReac.receipe.begin(); it != crReac.receipe.end(); ++it)
        {
          __int64 oreReq = Parse(Find(it->first), it->second);
          sum += oreReq;
        }

        reqQuant -= crReac.quantity;
      } 

      
      //__int64 quantProduced = batches * crReac.quantity;

      __int64 res = sum;      

      remaining[crReac.product] = -reqQuant;

      //cout << " quant produced " << crReac.quantity << " of " << crReac.product << " total consumed ore: " << res << " remaining " << crReac.product << " " << remaining[crReac.product];

      return res;
    }
    else
    {
      //cout << " Fulfill " << reqQuant << " remaining " << crReac.product << " " << remaining[crReac.product];

      remaining[crReac.product] -= reqQuant;
      return 0;
    }
  }



  string Do()
  {
    //map<state, pair<__int64, __int64>> prev;
    map<stateSmall, pair<__int64, __int64>> prev2;
    __int64 max = 0;
    __int64 step = 0;
    __int64 cycleSum = 0;
    
    auto where = prev2.end();

    stateSmall s(vec.size());
    map<__int64, __int64> prev;
    for (auto i : vec)
    {
      remaining[i.product] = 0;
    }

    auto remainingToConsume = 1'000'000'000'000;

    auto fuelBatchSize = 1; 1'000'000'000'000 / 10;

    __int64 progStep = 0;
    for (; where == prev2.end(); where = prev2.find(s)) {
    //for (; ; ) {
      //state remainingBack = remaining;
      
      ClearUsed();
      auto x = Parse0(Find("FUEL"), 1);

      fuelBatchSize = fuelBatchSize / 2 + 1;

      cycleSum += x;
            
      prev[step] = x;

      prev2.insert({ s, {x, step} });

      /**/
      __int64 j = 0;
      for (auto& crEl : remaining)
      {
        s[j++] = crEl.second;
      }
      /**/

      int maxWaste = 0;
      {
        __int64 j = 0;
        bool noneleft = true;
        for (auto& crEl : remaining)
        {
          if (s[j] != 0)
          {

            crEl.second = crEl.second * fuelBatchSize;
            noneleft = false;

            if (crEl.second > maxWaste)
              maxWaste = crEl.second;
            if (fuelBatchSize == 1)
              break;
          }

          ++j;
        }

        //prev.insert({ remainingBack, {x, step} });
        step+= fuelBatchSize;
        progStep++;

        if (noneleft)
          break;

        //if (step % 100000 == 1)
        {
          cout << "step: " << step << endl;
          cout << "stones left: " << remainingToConsume - cycleSum;          
          PrintUsed();
        }

        //if (step % 100000 > 2 && step % 100000 < 12)
        {          
          if (step % 100000 == 3)
            cout << endl;

          PrintWasteLine(step);
        }

        auto wasteLimit = ((remainingToConsume - cycleSum) / 371695) / 100000;

        if (maxWaste < ((remainingToConsume - cycleSum) / 371695 ) / 100000)
          fuelBatchSize += fuelBatchSize;
        else
          fuelBatchSize = fuelBatchSize / 2 + 1;
      }
    
      if (remainingToConsume < cycleSum)
      {
        cout << "NO MORE STONES NO CYCLE" << step - 1 << endl;
        break;
      }
    }
    
    cout << "EXIT step: " << step << endl;

    //auto& prev = prev2;
    //auto oldwhere = prev.find(remaining);



    __int64 consumedBeforeCycle = 0, produced = 0;

    __int64 cycleSize = prev.size() - produced;

    cycleSum -= consumedBeforeCycle;

    // 
    
    auto batches = remainingToConsume / cycleSum;

    __int64 quantConsumed = batches * cycleSum;

    remainingToConsume -= quantConsumed;

    produced += cycleSize * batches;

    __int64 i = 0;
    while (remainingToConsume > 0)
    {
      auto el = prev[i];

      if (remainingToConsume > el)
      {
        produced++;
        remainingToConsume -= el;
        i++;
      }
      else
        break;
    } 
    

    cout << "DONE :" <<  produced << endl;
    return std::to_string(produced);

    return std::to_string(produced);
  }

  void Clear()
  {
    for (auto i : vec)
    {
      remaining[i.product] = 0;
      requiredToProduce[i.product] = 0;
    }
  }

  void ClearUsed()
  {
    for (auto i : vec)
    {
      requiredToProduce[i.product] = 0;
    }
  }

  auto Part1()
  {
    Clear();

    auto x = Parse0(Find("FUEL"), 1);

    /** /
    __int64 rem = 0;
    for (auto pr : remaining)
    {
      if (pr.second > 0)
      {
        Clear();
        auto remainedFromCr = Parse(Find(pr.first), pr.second);
        rem += remainedFromCr;
      }
    }

    cout << "\nRemains from 1 Fuel: " << rem;

    /**/
    cout << "Stones for 1 Fuel: " << x;

    return std::to_string(x);
  }

  string Do2()
  {
    map<state, pair<__int64, __int64>> prev;
    __int64 max = 0;
    __int64 step = 0;
    __int64 cycleSum = 0;

    auto where = prev.end();

    //Part1();    

    for (; where == prev.end(); where = prev.find(remaining)) {
      state remainingBack = remaining;
      auto x = Parse(Find("FUEL"), 1);

      cycleSum += x;

      prev.insert({ remainingBack, {x, step} });
      step++;

      if (step % 1000 == 1)
        cout << "step: " << step << endl;
    }

    auto remainingToConsume = 1'000'000'000'000;


    __int64 consumedBeforeCycle = 0, produced = 0;
    for (auto i : irange(0, where->second.second))
    {
      auto el = find_if(prev.begin(), prev.end(), [&](auto& el) {
        return el.second.second == i;
        });

      if (remainingToConsume > el->second.first)
      {
        produced++;
        remainingToConsume -= el->second.first;
        consumedBeforeCycle += el->second.first;
      }
    }
    __int64 cycleSize = prev.size() - produced;

    cycleSum -= consumedBeforeCycle;

    // 

    auto batches = remainingToConsume / cycleSum;

    __int64 quantConsumed = batches * cycleSum;

    remainingToConsume -= quantConsumed;

    produced += cycleSize * batches;

    __int64 i = where->second.second;
    while (remainingToConsume > 0)
    {
      auto el = find_if(prev.begin(), prev.end(), [&](auto& el) {
        return el.second.second == i;
        });

      if (remainingToConsume > el->second.first)
      {
        produced++;
        remainingToConsume -= el->second.first;
        i++;
      }
      else
        break;
    }


    cout << produced << endl;
    return std::to_string(produced);
  }

  void PrintWaste()
  {
    cout << "\n Waste  " << "NAME " << " :  QANTITY  " << endl;
    /**/
    __int64 rem = 0;
    for (auto pr : remaining)
    {
      if (pr.second > 0)
      {
        cout << "     " << pr.first << " : " << pr.second << endl;
      }
    }
  }

  void PrintWasteLine( __int64 step)
  {
    //cout << "\n WST step" << setw(8) << step;
    
    __int64 idx = 0;
    for (auto pr : remaining)
    {
      //if (pr.second > 0)
      {
        cout << pr.second;
      }
      idx++;
    }
    cout << endl;
  }

  void PrintUsed()
  {
    cout << "\n Used  " << "NAME " << " :  QANTITY  " << endl;
    for (auto pr : requiredToProduce)
    {
      if (pr.second > 0)
      {
        cout << "     " << pr.first << " : " << pr.second << endl;
      }
    }
  }

  auto Part1More(string s)
  {
    Clear();

    auto x = Parse0(Find(s), 1);

    cout << "Stones for 1 " << s << " = " << x;

    PrintWaste();

    PrintUsed();


    cout << "\n ==================DONE With " << s << endl;

    /**/

    return std::to_string(x);
  }

  auto Part1MoreAll()
  {
    for (auto& x : vec)
      Part1More(x.product);

    return std::to_string(1);
  }
};

TEST_CASE("Sample 0", "[.]") {

  /** /
  CHECK(Solve(
    R"(10 ORE => 10 A
1 ORE => 1 B
7 A, 1 B => 1 C
7 A, 1 C => 1 D
7 A, 1 D => 1 E
7 A, 1 E => 1 FUEL)").Do() == "31");

  CHECK(Solve(
    R"(9 ORE => 2 A
8 ORE => 3 B
7 ORE => 5 C
3 A, 4 B => 1 AB
5 B, 7 C => 1 BC
4 C, 1 A => 1 CA
2 AB, 3 BC, 4 CA => 1 FUEL)").Do() == "165");

  /**/
  //REQUIRE(Solve(ReadFileToString(L"sample\\sample.txt")).Part1MoreAll() == "13312"); // sample test
  REQUIRE(Solve(ReadFileToString(L"sample\\sample.txt")).Do() == ReadFileToString(L"sample\\result.txt")); // sample test
  cout << endl << "Sample 1 ok------------- " << endl;
}

TEST_CASE("Sample 1", "[.]") {
  REQUIRE(Solve(ReadFileToString(L"sample\\sample2.txt")).Do() == ReadFileToString(L"sample\\result2.txt")); // sample test
  cout << endl << "Sample 2 ok------------- " << endl;
  REQUIRE(Solve(ReadFileToString(L"sample\\sample3.txt")).Do() == ReadFileToString(L"sample\\result3.txt")); // sample test
  cout << endl << "Sample 3 ok------------- " << endl;

  //REQUIRE(Solve(ReadFileToString(L"input.txt")).Part1() == "371695"); // sample test
}

TEST_CASE("Part 1 Do", "[.]") {


  REQUIRE(Solve(
    R"(9 ORE => 2 A
8 ORE => 3 B
7 ORE => 5 C
3 A, 4 B => 1 AB
5 B, 7 C => 1 BC
4 C, 1 A => 1 CA
2 AB, 3 BC, 4 CA => 1 FUEL)").Do() == "165");

}

TEST_CASE("Part One", "[.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Part1());
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());
}

/* Input ---------------

3 DJDNR => 1 ZCMR
7 VWJH => 5 ZPGT
5 BHZP => 2 DJDNR
6 KCNKC, 19 MZWS => 4 PKVJF
21 GXSHP, 1 TWGP, 3 BGCW => 1 XHRWR
12 DZGWQ, 2 XRDL, 3 XNVT => 2 FTMC
7 VWJH, 33 BGCW, 1 TBVC => 9 DSDP
1 NMTGB, 4 KCNKC, 5 SBSJ, 4 MCZDZ, 7 DLCP, 2 GRBZF, 1 CLKP, 10 VQHJG => 6 DVCR
7 ZCMR => 9 VNTF
2 VNTF, 1 GKMN => 1 TZWBH
6 QMFV, 7 GRBZF => 7 RHDZ
8 PKVJF => 9 NJQH
110 ORE => 9 GZTS
4 DJDNR, 7 SFHV => 8 KQFH
1 ZTCZ, 5 LZFBP => 7 VWPMZ
2 GKMN, 6 TZWBH, 1 GXSHP => 1 MJHJH
2 DLCP, 4 NGJRN => 3 GRBZF
2 DJDNR, 1 GSRBL => 4 VWJH
7 RMQX => 3 SFHV
1 GZTS => 7 GSRBL
3 GZTS, 1 SFHV => 3 QLXCS
10 SFHV => 3 MKTHL
2 DJDNR, 2 BGCW, 4 FSTJ => 3 GKMN
2 KQFH, 7 GSRBL => 7 TWGP
22 RHDZ, 22 DZGWQ, 2 NGJRN, 14 XHRWR, 21 VWPMZ, 15 ZPXHM, 26 BHZP => 8 BPHZ
1 QLXCS => 6 ZBTS
12 DLCP, 9 DSDP => 9 ZPXHM
1 VNTF => 5 ZBTX
2 TZWBH, 2 JCDW => 1 CPLG
1 XHRWR, 7 FSTJ, 5 DZGWQ => 4 NGJRN
179 ORE => 3 RMQX
1 DSDP => 1 MZWS
140 ORE => 8 BHZP
1 LZFBP, 4 DZGWQ => 2 PMDK
1 GZTS => 1 GXSHP
10 CPLG, 8 MCZDZ => 5 ZTCZ
5 ZPGT, 4 THLBN, 24 GSRBL, 40 VNTF, 9 DVCR, 2 SHLP, 11 PMDK, 19 BPHZ, 45 NJQH => 1 FUEL
9 MKTHL => 7 KCNKC
5 NGJRN => 3 QMFV
1 ZTCZ, 6 VNTF => 2 VQHJG
5 FTMC, 5 ZBTX, 1 MJHJH => 1 CLKP
7 FSTJ => 6 DLCP
1 DSDP => 5 KTML
4 LZFBP, 8 MKTHL => 7 MCZDZ
1 SFHV => 1 DZGWQ
2 QLXCS => 4 ZMXRH
3 KQFH, 1 DJDNR => 7 TBVC
5 DSDP => 7 THLBN
9 BHZP, 1 VWJH => 6 BGCW
4 GXSHP => 6 JCDW
1 KQFH, 3 ZMXRH => 9 XNVT
6 TBVC => 4 GVMH
3 VWPMZ, 3 GRBZF, 27 MJHJH, 2 QMFV, 4 NMTGB, 13 KTML => 7 SHLP
1 GVMH => 2 FSTJ
2 VQHJG, 2 NJQH => 8 SBSJ
1 XNVT => 2 XRDL
2 KCNKC => 5 LZFBP
2 ZBTS, 8 DLCP => 4 NMTGB

 ------------Input End*/

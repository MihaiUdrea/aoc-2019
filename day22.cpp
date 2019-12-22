// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"
#include "DealincInlUnique.cpp"
#include "functional"

//#define CARDS 10
//#define CARDS 10007
//#define CARDS 211

#define CARDS       119315717514047
#define CICLE_TOTAL 101741582076661

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
  enum Type
  {
    cut,
    dealnew,
    dealincrement
  };

  Type type;
  int v;
};
size_t New(size_t x) { 
  return CARDS - 1 - x; 
}

size_t Cut1(size_t x) {  return (CARDS + -1353 + x) % CARDS; }
size_t Cut3(size_t x) { return (CARDS + -716 + x) % CARDS; }
size_t Cut5(size_t x) { return (CARDS + 1364 + x) % CARDS; }
size_t Cut7(size_t x) { return (CARDS + 1723 + x) % CARDS; }
size_t Cut10(size_t x) { return (CARDS + 11 + x) % CARDS; }
size_t Cut12(size_t x) { return (CARDS + -6297 + x) % CARDS; }
size_t Cut14(size_t x) { return (CARDS + -3560 + x) % CARDS; }
size_t Cut16(size_t x) { return (CARDS + 1177 + x) % CARDS; }
size_t Cut18(size_t x) { return (CARDS + 6033 + x) % CARDS; }
size_t Cut20(size_t x) { return (CARDS + -3564 + x) % CARDS; }
size_t Cut22(size_t x) { return (CARDS + 6447 + x) % CARDS; }
size_t Cut24(size_t x) { return (CARDS + -4030 + x) % CARDS; }
size_t Cut26(size_t x) { return (CARDS + -6511 + x) % CARDS; }
size_t Cut28(size_t x) { return (CARDS + -8748 + x) % CARDS; }
size_t Cut30(size_t x) { return (CARDS + 5816 + x) % CARDS; }
size_t Cut32(size_t x) { return (CARDS + 9892 + x) % CARDS; }
size_t Cut34(size_t x) { return (CARDS + -9815 + x) % CARDS; }
size_t Cut36(size_t x) { return (CARDS + 673 + x) % CARDS; }
size_t Cut38(size_t x) { return (CARDS + 4518 + x) % CARDS; }
size_t Cut40(size_t x) { return (CARDS + 9464 + x) % CARDS; }
size_t Cut42(size_t x) { return (CARDS + 902 + x) % CARDS; }
size_t Cut46(size_t x) { return (CARDS + -5167 + x) % CARDS; }
size_t Cut50(size_t x) { return (CARDS + -8945 + x) % CARDS; }
size_t Cut53(size_t x) { return (CARDS + 3195 + x) % CARDS; }
size_t Cut55(size_t x) { return (CARDS + -1494 + x) % CARDS; }
size_t Cut57(size_t x) { return (CARDS + -9658 + x) % CARDS; }
size_t Cut59(size_t x) { return (CARDS + -4689 + x) % CARDS; }
size_t Cut61(size_t x) { return (CARDS + -9697 + x) % CARDS; }
size_t Cut63(size_t x) { return (CARDS + -6857 + x) % CARDS; }
size_t Cut65(size_t x) { return (CARDS + -6790 + x) % CARDS; }
size_t Cut69(size_t x) { return (CARDS + -9354 + x) % CARDS; }
size_t Cut71(size_t x) { return (CARDS + 8815 + x) % CARDS; }
size_t Cut73(size_t x) { return (CARDS + 6618 + x) % CARDS; }
size_t Cut75(size_t x) { return (CARDS + -6746 +x) % CARDS; }
size_t Cut77(size_t x) { return (CARDS + 1336 +x) % CARDS; }
size_t Cut79(size_t x) { return (CARDS + 6655 +x) % CARDS; }
size_t Cut81(size_t x) { return (CARDS + 8941 +x) % CARDS; }
size_t Cut83(size_t x) { return (CARDS + -3046 +x) % CARDS; }
size_t Cut85(size_t x) { return (CARDS + -7818 +x) % CARDS; }
size_t Cut87(size_t x) { return (CARDS + 4140 +x) % CARDS; }
size_t Cut89(size_t x) { return (CARDS + 6459 +x) % CARDS; }
size_t Cut91(size_t x) { return (CARDS + -6791 +x) % CARDS; }
size_t Cut93(size_t x) { return (CARDS + 3821 +x) % CARDS; }
size_t Cut95(size_t x) { return (CARDS + 3157 + x) % CARDS; }
size_t Cut97(size_t x) { return (CARDS + 8524 +x) % CARDS; }
size_t Cut100(size_t x) { return (CARDS + 5944 +x) % CARDS; }





vector< std::function<size_t(size_t)> > opList{
Cut1  ,
DealInc63,
Cut3  ,
DealInc55,
Cut5  ,
DealInc61,
Cut7  ,
New,
DealInc51,
Cut10 ,
DealInc65,
Cut12 ,
DealInc69,
Cut14 ,
DealInc20,
Cut16 ,
DealInc29,
Cut18 ,
DealInc3,
Cut20 ,
New,
Cut22 ,
New,
Cut24 ,
DealInc3,
Cut26 ,
DealInc42,
Cut28 ,
DealInc38,
Cut30 ,
DealInc73,
Cut32 ,
DealInc16,
Cut34 ,
DealInc10,
Cut36 ,
DealInc12,
Cut38 ,
DealInc52,
Cut40 ,
DealInc68,
Cut42 ,
DealInc11,
New,
DealInc45,
Cut46,
DealInc68,
New,
DealInc24,
Cut50 ,
New,
DealInc36,
Cut53 ,
DealInc52,
Cut55 ,
DealInc11,
Cut57 ,
New,
Cut59 ,
DealInc34,
Cut61 ,
DealInc39,
Cut63 ,
DealInc19,
Cut65 ,
DealInc59,
New,
DealInc52,
Cut69 ,
DealInc71,
Cut71 ,
DealInc2,
Cut73 ,
DealInc47,
Cut75 ,
New,
Cut77 ,
DealInc53,
Cut79 ,
DealInc17,
Cut81 ,
DealInc25,
Cut83 ,
DealInc14,
Cut85 ,
DealInc25,
Cut87 ,
DealInc60,
Cut89 ,
DealInc27,
Cut91 ,
New,
Cut93 ,
DealInc13,
Cut95 ,
DealInc13,
Cut97 ,
New,
DealInc12,
Cut100
};

using myt = __int64;

size_t Back(size_t x, int startPos)
{
  for (int i = startPos - 1; i >= 0; i--)
    x = opList[i](x);

  return x;
}

struct Solve : Program{
  
  vector<Data> vec;

  Solve(const string& inStr) : Program({}, { 0 }, inStr, false) {

    forEachLine(inStr, [&](string line)
      {
        Data dat;
        auto res = match_rx(line, regex(R"~(cut (.+))~"));
        auto res2 = match_rx(line, regex(R"~(deal with increment (.+))~"));
        auto res3 = match_rx(line, regex(R"~(deal into new stack)~"));
        if (!res.empty())
        {
          dat.type = Data::cut;
          dat.v = stoi(res[1].str());
        }
        else if (!res2.empty())
        {
          dat.type = Data::dealincrement;
          dat.v = stoi(res2[1].str());
        }
        else
        {
          assert(!res3.empty());
          dat.type = Data::dealnew;          
        }

        vec.push_back(dat);
      });
  };


vector<size_t> deck, table;

string List(int target)
{
  deck.resize(CARDS);
  table.resize(CARDS);

  for (auto i : irange(0, deck.size()))
  {
    deck[i] = i;
  }



  for (auto m : vec)
  {
    switch (m.type)
    {
    case Data::cut:
    {
      
      break;
    }
    case Data::dealnew:
    {
     
      break;
    }
    default:
    {
      ofstream out;
      //string fileName("Dealinc" + to_string(m.v) + ".txt");
      //out.open(fileName.c_str(), ios_base::trunc);

      string fileName("DealincInl.cpp");
      out.open(fileName.c_str(), ios_base::app);

      int tablePos = 0;
      for (auto i : irange(0, table.size()))
      {
        table[tablePos] = deck[i];
        tablePos = (tablePos + m.v) % deck.size();
      }

      //std::swap(table, deck);
      string bd;
      bd += "size_t DealInc" + to_string(m.v) + "(size_t x){  auto d = x / " + to_string(m.v)+"; auto m = x % " + to_string(m.v) + ";\n";
      bd += "static constexpr array<size_t, " + to_string(m.v) + "> start = {";
      //auto s = to2Ds(table, [&](auto l, size_t idx) {return Point{ (int)idx % m.v, (int)idx / m.v }; }, [](auto l) {return to_string(l); }, to2DsFlags::full_header, ' ', 5);
      auto s = to2Ds(table, [&](auto l, size_t idx) {return Point{ (int)idx % m.v, (int)idx / m.v }; }, [](auto l) {return to_string(l)+','; }, to2DsFlags::no_header, ' ', 5, 
        { 0, 0 }, { m.v, 0 });

      bd += s;
      bd += "}; return start[m] + d; }\n";

      out << bd;
      break;
    }
    }

    if (0)
      for (auto card : deck)
        cout << std::setfill(' ') << setw(5) << card;

    //copy(deck.begin(), deck.end(), ostream_iterator<size_t>(cout));

  }

  for (auto i : irange(0, deck.size()))
  {
    if (deck[i] == target)
      return to_string(i);
  }


  return "-1";
}

  string Do(int target)
  {
    deck.resize(CARDS);
    table.resize(CARDS);

    int cols = 3589;
    for (auto i : irange(0, deck.size()))
    {
      deck[i] = i;
    }

    int step = 0;
    for (auto m : vec)
    {
      switch (m.type)
      {
      case Data::cut:
      {
        cout << "\Cut : " << m.v << " : ";

        int cutPos = m.v > 0 ? m.v : deck.size() + m.v;
        for (auto i : irange(0, deck.size() - cutPos))
          table[i] = deck[i + cutPos];
        for (auto i : irange(deck.size() - cutPos, deck.size()))
          table[i] = deck[i - deck.size() + cutPos];

        std::swap(table, deck);

        auto s = to2Ds(deck, [&](auto l, size_t idx) {return Point{ (int)idx % 50, (int)idx / 50 }; }, [](auto l) {return to_string(l); }, to2DsFlags::full_header, ' ', 5);
        cout << endl <<s;

        break;
      }
      case Data::dealnew:
      {
        cout << "\nDeal new line : --------------------------------";
        for (auto i : irange(0, deck.size() / 2))
          std::swap(deck[i], deck[deck.size() - i - 1]);

        auto s = to2Ds(deck, [&](auto l, size_t idx) {return Point{ (int)idx % 50, (int)idx / 50 }; }, [](auto l) {return to_string(l); }, to2DsFlags::full_header, ' ', 5);
        cout << endl << s;

        break;
      }
      default:
      {
        //cout << "\nDeal with increment: " << m.v << " : \n";
        int tablePos = 0;
        for (auto i : irange(0, table.size()))
        {
          table[tablePos] = deck[i];
          tablePos = (tablePos + m.v) % deck.size();
        }
        std::swap(table, deck);

        auto s = to2Ds(deck, [&](auto l, size_t idx) {return Point{ (int)idx % m.v, (int)idx / m.v }; }, [](auto l) {return to_string(l); }, to2DsFlags::full_header, ' ', 5);
        cout << endl << s;

        auto p = Back(10, step + 1);
        break;
      }
      }
    
      if (0)
      for (auto card : deck)
        cout << std::setfill(' ') << setw(5) << card;
      
      //copy(deck.begin(), deck.end(), ostream_iterator<size_t>(cout));
    
      step++;


      //check we know ehre 2019 is
      size_t res = 0;
      for (auto i : irange(0, deck.size()))
      {
        if (deck[i] == target)
        {
          res = i;
          break;
        }
      }

      if (auto c = Back(res, step) != target)
      {
        cout << "Bad "  << c;
      }
    }

    size_t res = 0;
    for (auto i : irange(0, deck.size()))
    {
      if (deck[i] == target)
      {
        res = i;
        break;
      }
    }
  
    auto p2 = Back(res, opList.size());
    
    return to_string(res);
  }

  vector<size_t> prev;
  set<size_t> vals;
  string Do2()
  {
    size_t start = 2020;
    for (size_t i = /*123402*/ CICLE_TOTAL; i > 0 ; i--)
    {
      start = Back(start, opList.size());

      cout << "\n step : " << i << " val: " << start << " " ;
      if(1)
      if (vals.insert(start).second == false)
      {
        auto xi = prev[253];
        auto res = Back(xi, 61);
        cout << "RES: " << res;
        cout << " been here ";
        auto where = find(prev.begin(), prev.end(), start);
        auto idx = where - prev.begin();
        cout << " Found in history at idx: " << idx;

        size_t whereTojump = i - (i / 224 - 10) * 224;

        if (0)
          i = whereTojump;

        cout << " Same will be in " << whereTojump;

      }
      if (1)
      prev.push_back(start);

      if (i % 1000000 == 1 )
      {
        cout << i << " : " << start << " " << ((double)i) / 101741582076661. << endl;
      }


    }

    cout << " : " << start << " " << endl;

    return ToString(start);
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(R"(deal with increment 63
deal into new stack
deal into new stack
)").Do(9) == "3");
  
  REQUIRE(Solve(R"(deal with increment 7
deal into new stack
deal into new stack
)").Do(9) == "3"); 

  REQUIRE(Solve(R"(cut 6
deal with increment 7
deal into new stack
)").Do(9) == "8"); 
  
  REQUIRE(Solve(R"(deal with increment 7
deal with increment 9
cut -2
)").Do(9) == "9");

  REQUIRE(Solve(R"(deal into new stack
cut -2
deal with increment 7
cut 8
cut -4
deal with increment 7
cut 3
deal with increment 9
deal with increment 3
cut -1
)").Do(9) == "0");

}

TEST_CASE("Part One", "[.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do(2019));
  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do(12, 2));
}

TEST_CASE("Part Two", "[x.]") {
  cout << endl << "Part Two ------------- " << endl;

  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

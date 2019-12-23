// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"
#include "DealincInlUniqueBig.cpp"
#include "functional"

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/integer/mod_inverse.hpp>

namespace mp = boost::multiprecision;

//#define CARDS 10
//#define CARDS 10007
//#define CARDS 211

#define CARDS       119315717514047
#define CICLE_TOTAL 101741582076661


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

#define CARDS2 0

size_t New(size_t x) {
  return CARDS - 1 - x;
}

size_t Cut1(size_t x) {
  return  (CARDS + -1353 + x) > CARDS ? ((CARDS + -1353 + x) - CARDS) : (CARDS + -1353 + x);
}
size_t Cut3(size_t x)   { return  (CARDS + -716 + x)  > CARDS ? ((CARDS + -716 + x)  - CARDS) : (CARDS  + -716 + x); }
size_t Cut5(size_t x)   { return  (CARDS2 + 1364 + x) > CARDS ? ((CARDS2 + 1364 + x) - CARDS) : (CARDS2 + 1364 + x); }
size_t Cut7(size_t x)   { return  (CARDS2 + 1723 + x) > CARDS ? ((CARDS2 + 1723 + x) - CARDS) : (CARDS2 + 1723 + x); }
size_t Cut10(size_t x)  { return  (CARDS2 + 11 + x)   > CARDS ? ((CARDS2 + 11 + x)   - CARDS) : (CARDS2 + 11 + x); }
size_t Cut12(size_t x)  { return  (CARDS + -6297 + x) > CARDS ? ((CARDS + -6297 + x) - CARDS) : (CARDS  + -6297 + x); }
size_t Cut14(size_t x)  { return  (CARDS + -3560 + x) > CARDS ? ((CARDS + -3560 + x) - CARDS) : (CARDS  + -3560 + x); }
size_t Cut16(size_t x)  { return  (CARDS2 + 1177 + x) > CARDS ? ((CARDS2 + 1177 + x) - CARDS) : (CARDS2 + 1177 + x); }
size_t Cut18(size_t x)  { return  (CARDS2 + 6033 + x) > CARDS ? ((CARDS2 + 6033 + x) - CARDS) : (CARDS2 + 6033 + x); }
size_t Cut20(size_t x)  { return  (CARDS + -3564 + x) > CARDS ? ((CARDS + -3564 + x) - CARDS) : (CARDS  + -3564 + x); }
size_t Cut22(size_t x)  { return  (CARDS2 + 6447 + x) > CARDS ? ((CARDS2 + 6447 + x) - CARDS) : (CARDS2 + 6447 + x); }
size_t Cut24(size_t x)  { return  (CARDS + -4030 + x) > CARDS ? ((CARDS + -4030 + x) - CARDS) : (CARDS  + -4030 + x); }
size_t Cut26(size_t x)  { return  (CARDS + -6511 + x) > CARDS ? ((CARDS + -6511 + x) - CARDS) : (CARDS  + -6511 + x); }
size_t Cut28(size_t x)  { return  (CARDS + -8748 + x) > CARDS ? ((CARDS + -8748 + x) - CARDS) : (CARDS  + -8748 + x); }
size_t Cut30(size_t x)  { return  (CARDS2 + 5816 + x) > CARDS ? ((CARDS2 + 5816 + x) - CARDS) : (CARDS2 + 5816 + x); }
size_t Cut32(size_t x)  { return  (CARDS2 + 9892 + x) > CARDS ? ((CARDS2 + 9892 + x) - CARDS) : (CARDS2 + 9892 + x); }
size_t Cut34(size_t x)  { return  (CARDS + -9815 + x) > CARDS ? ((CARDS + -9815 + x) - CARDS) : (CARDS  + -9815 + x); }
size_t Cut36(size_t x)  { return  (CARDS2 + 673 + x)  > CARDS ? ((CARDS2 + 673 + x)  - CARDS) : (CARDS2 + 673 + x); }
size_t Cut38(size_t x)  { return  (CARDS2 + 4518 + x) > CARDS ? ((CARDS2 + 4518 + x) - CARDS) : (CARDS2 + 4518 + x); }
size_t Cut40(size_t x)  { return  (CARDS2 + 9464 + x) > CARDS ? ((CARDS2 + 9464 + x) - CARDS) : (CARDS2 + 9464 + x); }
size_t Cut42(size_t x)  { return  (CARDS2 + 902 + x)  > CARDS ? ((CARDS2 + 902 + x)  - CARDS) : (CARDS2 + 902 + x); }
size_t Cut46(size_t x)  { return  (CARDS + -5167 + x) > CARDS ? ((CARDS + -5167 + x) - CARDS) : (CARDS  + -5167 + x); }
size_t Cut50(size_t x)  { return  (CARDS + -8945 + x) > CARDS ? ((CARDS + -8945 + x) - CARDS) : (CARDS  + -8945 + x); }
size_t Cut53(size_t x)  { return  (CARDS2 + 3195 + x) > CARDS ? ((CARDS2 + 3195 + x) - CARDS) : (CARDS2 + 3195 + x); }
size_t Cut55(size_t x)  { return  (CARDS + -1494 + x) > CARDS ? ((CARDS + -1494 + x) - CARDS) : (CARDS  + -1494 + x); }
size_t Cut57(size_t x)  { return  (CARDS + -9658 + x) > CARDS ? ((CARDS + -9658 + x) - CARDS) : (CARDS  + -9658 + x); }
size_t Cut59(size_t x)  { return  (CARDS + -4689 + x) > CARDS ? ((CARDS + -4689 + x) - CARDS) : (CARDS  + -4689 + x); }
size_t Cut61(size_t x)  { return  (CARDS + -9697 + x) > CARDS ? ((CARDS + -9697 + x) - CARDS) : (CARDS  + -9697 + x); }
size_t Cut63(size_t x)  { return  (CARDS + -6857 + x) > CARDS ? ((CARDS + -6857 + x) - CARDS) : (CARDS  + -6857 + x); }
size_t Cut65(size_t x)  { return  (CARDS + -6790 + x) > CARDS ? ((CARDS + -6790 + x) - CARDS) : (CARDS  + -6790 + x); }
size_t Cut69(size_t x)  { return  (CARDS + -9354 + x) > CARDS ? ((CARDS + -9354 + x) - CARDS) : (CARDS  + -9354 + x); }
size_t Cut71(size_t x)  { return  (CARDS2 + 8815 + x) > CARDS ? ((CARDS2 + 8815 + x) - CARDS) : (CARDS2 + 8815 + x); }
size_t Cut73(size_t x)  { return  (CARDS2 + 6618 + x) > CARDS ? ((CARDS2 + 6618 + x) - CARDS) : (CARDS2 + 6618 + x); }
size_t Cut75(size_t x)  { return  (CARDS + -6746 + x) > CARDS ? ((CARDS + -6746 + x) - CARDS) : (CARDS  + -6746 + x); }
size_t Cut77(size_t x)  { return  (CARDS2 + 1336 + x) > CARDS ? ((CARDS2 + 1336 + x) - CARDS) : (CARDS2 + 1336 + x); }
size_t Cut79(size_t x)  { return  (CARDS2 + 6655 + x) > CARDS ? ((CARDS2 + 6655 + x) - CARDS) : (CARDS2 + 6655 + x); }
size_t Cut81(size_t x)  { return  (CARDS2 + 8941 + x) > CARDS ? ((CARDS2 + 8941 + x) - CARDS) : (CARDS2 + 8941 + x); }
size_t Cut83(size_t x)  { return  (CARDS + -3046 + x) > CARDS ? ((CARDS + -3046 + x) - CARDS) : (CARDS  + -3046 + x); }
size_t Cut85(size_t x)  { return  (CARDS + -7818 + x) > CARDS ? ((CARDS + -7818 + x) - CARDS) : (CARDS  + -7818 + x); }
size_t Cut87(size_t x)  { return  (CARDS2 + 4140 + x) > CARDS ? ((CARDS2 + 4140 + x) - CARDS) : (CARDS2 + 4140 + x); }
size_t Cut89(size_t x)  { return  (CARDS2 + 6459 + x) > CARDS ? ((CARDS2 + 6459 + x) - CARDS) : (CARDS2 + 6459 + x); }
size_t Cut91(size_t x)  { return  (CARDS + -6791 + x) > CARDS ? ((CARDS + -6791 + x) - CARDS) : (CARDS  + -6791 + x); }
size_t Cut93(size_t x)  { return  (CARDS2 + 3821 + x) > CARDS ? ((CARDS2 + 3821 + x) - CARDS) : (CARDS2 + 3821 + x); }
size_t Cut95(size_t x)  { return  (CARDS2 + 3157 + x) > CARDS ? ((CARDS2 + 3157 + x) - CARDS) : (CARDS2 + 3157 + x); }
size_t Cut97(size_t x)  { return  (CARDS2 + 8524 + x) > CARDS ? ((CARDS2 + 8524 + x) - CARDS) : (CARDS2 + 8524 + x); }
size_t Cut100(size_t x) { return  (CARDS2 + 5944 + x) > CARDS ? ((CARDS2 + 5944 + x) - CARDS) : (CARDS2 + 5944 + x); }





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

size_t BackInl(size_t x)
{

  x = Cut100(x);
  x = DealInc12(x);
  x = New(x);
  x = Cut97(x);
  x = DealInc13(x);
  x = Cut95(x);
  x = DealInc13(x);
  x = Cut93(x);
  x = New(x);
  x = Cut91(x);
  x = DealInc27(x);
  x = Cut89(x);
  x = DealInc60(x);
  x = Cut87(x);
  x = DealInc25(x);
  x = Cut85(x);
  x = DealInc14(x);
  x = Cut83(x);
  x = DealInc25(x);
  x = Cut81(x);
  x = DealInc17(x);
  x = Cut79(x);
  x = DealInc53(x);
  x = Cut77(x);
  x = New(x);
  x = Cut75(x);
  x = DealInc47(x);
  x = Cut73(x);
  x = DealInc2(x);
  x = Cut71(x);
  x = DealInc71(x);
  x = Cut69(x);
  x = DealInc52(x);
  x = New(x);
  x = DealInc59(x);
  x = Cut65(x);
  x = DealInc19(x);
  x = Cut63(x);
  x = DealInc39(x);
  x = Cut61(x);
  x = DealInc34(x);
  x = Cut59(x);
  x = New(x);
  x = Cut57(x);
  x = DealInc11(x);
  x = Cut55(x);
  x = DealInc52(x);
  x = Cut53(x);
  x = DealInc36(x);
  x = New(x);
  x = Cut50(x);
  x = DealInc24(x);
  x = New(x);
  x = DealInc68(x);
  x = Cut46(x);
  x = DealInc45(x);
  x = New(x);
  x = DealInc11(x);
  x = Cut42(x);
  x = DealInc68(x);
  x = Cut40(x);
  x = DealInc52(x);
  x = Cut38(x);
  x = DealInc12(x);
  x = Cut36(x);
  x = DealInc10(x);
  x = Cut34(x);
  x = DealInc16(x);
  x = Cut32(x);
  x = DealInc73(x);
  x = Cut30(x);
  x = DealInc38(x);
  x = Cut28(x);
  x = DealInc42(x);
  x = Cut26(x);
  x = DealInc3(x);
  x = Cut24(x);
  x = New(x);
  x = Cut22(x);
  x = New(x);
  x = Cut20(x);
  x = DealInc3(x);
  x = Cut18(x);
  x = DealInc29(x);
  x = Cut16(x);
  x = DealInc20(x);
  x = Cut14(x);
  x = DealInc69(x);
  x = Cut12(x);
  x = DealInc65(x);
  x = Cut10(x);
  x = DealInc51(x);
  x = New(x);
  x = Cut7(x);
  x = DealInc61(x);
  x = Cut5(x);
  x = DealInc55(x);
  x = Cut3(x);
  x = DealInc63(x);
  x = Cut1(x);

  return x;
}

struct Solve : Program {

  vector<Data> vec;
  map<size_t, mp::int128_t> moduloInvMap;
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

          moduloInvMap[dat.v] = boost::integer::mod_inverse<mp::int128_t>(dat.v, CARDS);
        }
        else
        {
          assert(!res3.empty());
          dat.type = Data::dealnew;
        }

        vec.push_back(dat);
      });
  };


  size_t IncDeal(size_t p, size_t inc)
  {
    auto mi = moduloInvMap[inc];
    auto mis = mi.convert_to<string>();
    mp::int128_t res = ((mp::int128_t)p * mi) % CARDS;
    auto resS = res.convert_to<string>();

    return static_cast<size_t>(res);
  }

  static size_t NewDeal(size_t p) {
    return CARDS - 1 - p;
  }

  static size_t Cut(size_t x, size_t howMuch)
  {
    return  (CARDS + howMuch + x) > CARDS ? ((CARDS + howMuch + x) - CARDS) : (CARDS + howMuch + x);
  }

  size_t BackInv(size_t x, int startPos)
  {
    for (int i = startPos - 1; i >= 0; i--)
    {
      switch (vec[i].type)
      {
      case Data::dealincrement:
        x = IncDeal(x, vec[i].v);
        break;
      case Data::dealnew:
        x = NewDeal(x);
        break;
      case Data::cut:
        x = Cut(x, vec[i].v);
      default:
        break;
      }
    }

    return x;
  }

  vector<size_t> deck, table;

  string List(int target)
  {
    deck.resize(10007);
    table.resize(10007);

    for (auto i : irange(0, deck.size()))
    {
      deck[i] = i;
    }


    set<int> delDone;
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
        if (!delDone.insert(m.v).second)
          break;

        string fileName("DealincInl.cpp");
        out.open(fileName.c_str(), ios_base::app);

        int tablePos = 0;
        for (auto i : irange(0, table.size()))
        {
          table[tablePos] = deck[i];
          tablePos = (tablePos + m.v) % deck.size();
        }

        //std::swap(table, deck);
        if (0)
        {
          string bd;
          bd += "size_t DealInc" + to_string(m.v) + "(size_t x){  auto d = x / " + to_string(m.v) + "; auto m = x % " + to_string(m.v) + ";\n";
          bd += "static constexpr array<size_t, " + to_string(m.v) + "> start = {";
          //auto s = to2Ds(table, [&](auto l, size_t idx) {return Point{ (int)idx % m.v, (int)idx / m.v }; }, [](auto l) {return to_string(l); }, to2DsFlags::full_header, ' ', 5);
          auto s = to2Ds(table, [&](auto l, size_t idx) {return Point{ (int)idx % m.v, (int)idx / m.v }; }, [](auto l) {return to_string(l) + ','; }, to2DsFlags::no_header, ' ', 5,
            { 0, 0 }, { m.v, 0 });

          bd += s;
          bd += "}; return start[m] + d; }\n";

          out << bd;
        }

        // ------------------
        size_t lines = CARDS / m.v;
        size_t mod = CARDS % m.v;
        size_t cols = m.v;
        int firstSmallCols = mod;

        size_t colInc = m.v - mod;
        vector<size_t> cache;
        cache.resize(cols);
        int prevCol = 0;
        int newCol = 0;
        for (auto step : irange(0, cols))
        {
          newCol = (prevCol + colInc) % cols;
          size_t incWith = lines + 1;

          if (prevCol >= firstSmallCols)
            incWith--;
          cache[newCol] = cache[prevCol] + incWith;

          prevCol = newCol;
        }
        cache[0] = 0;

        {
          string bd;
          bd += "size_t DealInc" + to_string(m.v) + "(size_t x){  auto d = x / " + to_string(m.v) + "; auto m = x % " + to_string(m.v) + ";\n";
          bd += "static constexpr array<size_t, " + to_string(m.v) + "> start = {";
          //auto s = to2Ds(table, [&](auto l, size_t idx) {return Point{ (int)idx % m.v, (int)idx / m.v }; }, [](auto l) {return to_string(l); }, to2DsFlags::full_header, ' ', 5);
          stringstream s;
          for (auto v : cache)
            s << v << ", ";

          bd += s.str();
          bd += "}; return start[m] + d; }\n";

          out << bd;
        }
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
        cout << endl << s;

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
        cout << "Bad " << c;
      }

      if (auto c = BackInv(res, step) != target)
      {
        cout << "Bad " << c;
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

  auto GetAtStep(size_t step, mp::int128_t a, mp::int128_t b)
  {
  mp::int128_t resultA = (mp::powm(a, step, CARDS) * 2020) % CARDS;
  cout << "ResA:" << resultA.convert_to<string>() << endl;
  mp::int128_t resultB = ((b * (mp::powm(a, step, CARDS) - 1))%CARDS) * boost::integer::mod_inverse<mp::int128_t>(a - 1, CARDS);
  cout << "ResB:" << resultB.convert_to<string>() << endl;

  auto res = (resultA + resultB) % CARDS;
  cout << "SOL: " << res.convert_to<string>();
  return res.convert_to<string>();
  }

  vector<size_t> prev;
  map<size_t, size_t> vals;
  vector<pair<string,string>> prevIDX;
  string Do2()
  {
    //vector<size_t> histSet;
    //histSet.resize(CARDS / 1024);

    size_t b = BackInl(0);
    size_t a = BackInl(1) - b;
    {
      size_t b1 = BackInv(0,100);
      size_t a1 = BackInv(1,100) - b;
    }
    size_t start = 2020;
    prev.push_back(2020);
    for (size_t i = /*123402*/ CICLE_TOTAL; i > 0; i--)
    {
      //start = Back(start, opList.size());
      start = BackInl(start);

      //cout << "\n step : " << i << " val: " << start << " ";
      if (1)
        //if (vals.insert({ start, i}).second == false)
        if (start == 2020)
        {
          /** /
          auto xi = prev[253];
          auto res = Back(xi, 61);
          cout << "RES: " << res;
          /**/

          cout << " been here ";
          cout << " Found in history at step: " << vals[start];
          auto interval = CICLE_TOTAL - i + 1;

          cout << " Interval: " << interval;

          size_t whereTojump = i - (i / interval) * interval;

          if (1)
            i = whereTojump;

          cout << " Same will be in " << whereTojump;

        }
      if (1)
        prev.push_back(start);

      if (prev.size() >= 3)
      {
        mp::int128_t f1 = prev[prev.size() - 2];
        mp::int128_t f2 = prev[prev.size() - 1];
        auto x2 = f1;
        auto x1 = prev[prev.size() - 3];

        mp::int128_t a = (f2 - f1) * boost::integer::mod_inverse<mp::int128_t>(CARDS + x2 - x1, CARDS) % CARDS;
        mp::int128_t b = (f1 - a * x1)%CARDS;
        if (b < 0)
          b += CARDS;
        if (a < 0)
          a += CARDS;
        cout << "A:" << a.convert_to<string>() << endl;
        cout << "B:" << b.convert_to<string>() << endl;
      
        auto res = GetAtStep(CICLE_TOTAL, a, b);

        prevIDX.emplace_back( a.convert_to<string>(), b.convert_to<string>());

        size_t nr = prev[prev.size() - 1] - prev[prev.size() - 2];
        size_t num = prev[prev.size() - 2] - prev[prev.size() - 3];

        cout << "A: " << a << endl;
      }

      if (1)
        if (i % 1000000 == 1)
        {
          cout << i << " : " << start << " " << ((double)i) / 101741582076661. << endl;
        }

      if (0)
      {
        cout << start << endl;
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
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
  //auto x = Cut40(119315717514046);
  //cout << x;

  //x = DealInc3(119315717514046);
  //cout << x;

  cout << endl << "Part Two ------------- " << endl;

  //toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
}

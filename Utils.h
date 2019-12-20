#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"


string ReadFileToString(const wchar_t* aFileName)
{
  ifstream inFile;
  inFile.open(aFileName);
  if (!inFile) {
    wcerr << L"Unable to open file: " << aFileName;
    exit(1);   // call system to stop
  }

  string str;

  inFile.seekg(0, std::ios::end);
  str.reserve((const unsigned int)inFile.tellg());
  inFile.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(inFile)),
    std::istreambuf_iterator<char>());

  return str;
}

string ToString(int aNr)
{
  char buff[100];
  _itoa_s(aNr, buff, 10);
  return buff;
}


void toClipboard(const std::string& s) {
  OpenClipboard(0);
  EmptyClipboard();
  HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
  if (!hg) {
    CloseClipboard();
    return;
  }
  memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
  GlobalUnlock(hg);
  SetClipboardData(CF_TEXT, hg);
  CloseClipboard();
  GlobalFree(hg);

  cout << endl << s.c_str() << " is in clipboad." << endl;
}

void toClipboard(int nr) {
  toClipboard(ToString(nr));
}

class irange
{
  int aFirst;
  int aLast;
  int aStep;

  class Iter
  {
    int idx;
    int step;
  public:
    Iter(int x, int s) :idx(x), step(s) {}
    Iter(const Iter& mit) : idx(mit.idx), step(mit.step) {}
    Iter& operator++() { idx += step; return *this; }
    Iter operator++(int) { Iter tmp(*this); operator++(); return tmp; }
    bool operator==(const Iter& rhs) const { return step > 0 ? idx >= rhs.idx : idx <= rhs.idx; }
    bool operator!=(const Iter& rhs) const { return step > 0 ? idx < rhs.idx : idx>rhs.idx; }
    int& operator*() { return idx; }
  };

public:

  irange(int aFirst, int aLast, int pStep = 1) : aFirst(aFirst), aLast(aLast), aStep(pStep)
  {
    assert(aStep > 0);

    if (aFirst > aLast)
      aStep = -aStep;
  }

  Iter begin()
  {
    return Iter(aFirst, aStep);
  }

  Iter end()
  {
    return Iter(aLast, aStep);
  }
};

template <typename Ret, typename OpT>
Ret Compare(OpT aL, OpT aR, Ret LtResult, Ret EqResult, Ret GtResult)
{
  if (aL < aR)
    return LtResult;
  else if (aL == aR)
    return EqResult;
  else
    return GtResult;
}

const regex lineRxToken("\\n");

void forEachRxToken(const string& inStr, const regex& sepRx, std::function<void(string)> fct)
{
  for (sregex_token_iterator iter(inStr.begin(), inStr.end(), sepRx, -1), end; iter != end; ++iter)
  {
    fct(iter->str());
  }
}

void forEachLine(const string& inStr, std::function<void(string)> fct)
{
  forEachRxToken(inStr, lineRxToken, fct);
}

vector<string> Tokenize(const string& inStr, const regex& sepRx)
{
  vector<string> list;
  forEachLine(inStr, [&](string line) {list.push_back(line); });
  return list;
}

vector<string> GetLines(string& inStr)
{
  return Tokenize(inStr, lineRxToken);
}


// use as:
//
// auto res = match_rx(line, sepRx);
template <class _StTraits, class _StAlloc, class _Elem, class _RxTraits>
auto match_rx(const basic_string<_Elem, _StTraits, _StAlloc>& _Str,
  const basic_regex<_Elem, _RxTraits>& _Re, regex_constants::match_flag_type _Flgs = regex_constants::match_default)
{
  match_results<typename basic_string<_Elem, _StTraits, _StAlloc>::const_iterator, allocator<sub_match< basic_string<_Elem, _StTraits, _StAlloc>::const_iterator>>> _Matches;
  // try to match regular expression to target text
  _Regex_match1(_Str.begin(), _Str.end(), _STD addressof(_Matches), _Re, _Flgs, true);
  return _Matches;
}

template <class _Col, class _Ty>
inline bool contains(const _Col& _Collection, const _Ty& _Val)
{
  return find(_Collection.begin(), _Collection.end(), _Val) != _Collection.end();
}

template <class _Col, class _Pr>
inline bool contains_pred(const _Col& _Collection, _Pr _Pred)
{
  return find_if(_Collection.begin(), _Collection.end(), _Pred) != _Collection.end();
}

struct Point
{
  int x = 0;
  int y = 0;
  int z = 0;

  auto operator<=>(const Point&) const = default;
  auto operator+(const Point& l) const { return Point{ x + l.x, y + l.y, z + l.z }; };
  auto operator-(const Point& l) const { return Point{ x - l.x, y - l.y, z - l.z }; };

  int GetAxys(int ax) const
  {
    switch (ax)
    {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      break;
    }
    return -1;
  }

  void IncAxyx(int ax, int v)
  {
    switch (ax)
    {
    case 0:
      x += v;
      break;
    case 1:
      y += v;
      break;
    case 2:
      z += v;
      break;
    default:
      break;
    }
  }
};

static int ManhDist(Point a)
{
  return abs(a.x) + abs(a.y) + abs(a.z);
}


void toConsole(Point p, const char* c, int length, int sleep = -1) {

  DWORD dw;
  COORD here;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  here.X = p.x;
  here.Y = p.y;
  WriteConsoleOutputCharacterA(hStdOut, c, length, here, &dw);

  if (sleep > 0)
    Sleep(sleep);
};

void toConsole(Point p, string c, int sleep = -1) {

  DWORD dw;
  COORD here;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  here.X = p.x;
  here.Y = p.y;
  WriteConsoleOutputCharacterA(hStdOut, c.c_str(), c.size(), here, &dw);

  if (sleep > 0)
    Sleep(sleep);
};

struct to2DsFlags {
  enum DisplayMode
  {
    full_header,
    no_header,
    cols_count,
    line_count
  };
};

template <class _Col>
pair<Point,Point> MinMax(const _Col& _Collection, std::function<Point(const typename _Col::value_type&)> toPtFct)
{
  auto & flatList = _Collection;
  
  auto limX = minmax_element(flatList.begin(), flatList.end(), [&](auto& l, auto& r) {
    return toPtFct(l).x < toPtFct(r).x;
    });
  auto limY = minmax_element(flatList.begin(), flatList.end(), [&](auto& l, auto& r) {
    return toPtFct(l).y < toPtFct(r).y;
    });
  auto limZ = minmax_element(flatList.begin(), flatList.end(), [&](auto& l, auto& r) {
    return toPtFct(l).z < toPtFct(r).z;
    });

  Point min{ limX.first->first.x, limY.first->first.y, limZ.first->first.z };
  Point max{ limX.second->first.x, limY.second->first.y, limZ.second->first.z };
  return { min, max };
}


// auto ss = to2Ds(vals, [](auto& l) { return l.x; }, [](auto& l) { return string() + l.ch; }, to2DsFlags::full_header, '.', 1);
template <class _Col>
string to2Ds(const _Col& _Collection, std::function<Point(const typename _Col::value_type&)> toPtFct, std::function<string(const typename _Col::value_type&)> toStringFct,
  to2DsFlags::DisplayMode mode = to2DsFlags::no_header, char fill = ' ', int charWidth = 1)
{
  map<Point, string> flatList;
  for_each(_Collection.begin(), _Collection.end(), [&](auto& el) {
    Point pt = toPtFct(el);
    pt.z = 0;

    string str = toStringFct(el);
    flatList[pt] = str;
    });

  auto limX = minmax_element(flatList.begin(), flatList.end(), [](auto& l, auto& r) {
    return l.first.x < r.first.x;
    });
  auto limY = minmax_element(flatList.begin(), flatList.end(), [](auto& l, auto& r) {
    return l.first.y < r.first.y;
    });

  Point min{ limX.first->first.x, limY.first->first.y };
  Point max{ limX.second->first.x, limY.second->first.y };


  int minCol = min.x;

  int lastCol = max.x + 1;
  //lastCol = 35;

  int minLine = min.y;
  int lastLine = max.y + 1;

  stringstream out;

  //ofstream out;
  //out.open(OUT_FILE, FILE_OUT_MODE);
  //system("CLS");
  //auto& out = cout;
  //out << "------------------------------" << path << "------------" << endl;

  // print top/cols count header 
  auto topHeader = [&](int minCol, int maxCol) {
    out << std::setfill(' ') << setw(4) << ' ';
    for (auto col : irange(minCol, maxCol))
    {
      if (col >= 0)
        out << setw(charWidth) << col / 100;
      else
        out << setw(charWidth) << '-';
    }
    out << endl;
    out << std::setfill(' ') << setw(4) << ' ';
    for (auto col : irange(minCol, maxCol))
    {
      out << setw(charWidth) << (abs(col) % 100) / 10;
    }
    out << endl;
    out << std::setfill(' ') << setw(4) << ' ';
    for (auto col : irange(minCol, maxCol))
    {
      out << setw(charWidth) << abs(col) % 10;
    }
    out << endl;
  };

  if (mode == to2DsFlags::full_header || mode == to2DsFlags::cols_count)
    topHeader(minCol, lastCol);

  for (auto l : irange(minLine, lastLine))
  {
    if (mode == to2DsFlags::full_header || mode == to2DsFlags::line_count)
      out << right << std::setfill(' ') << setw(4) << l;

    for (auto c : irange(minCol, lastCol))
    {
      Point crPt{ c, l, 0 };

      if (auto where = flatList.find(crPt); where != flatList.end())
        out << setw(charWidth) << where->second;
      else
        out << setw(charWidth) << fill;
    }
    out << endl;
  }
  //Sleep(1000);
  //system("pause");
  //out.close();

  return out.str();
}

#endif // UTILS_H

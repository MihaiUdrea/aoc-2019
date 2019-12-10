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

void forEachRxToken(string& inStr, const regex& sepRx, std::function<void(string)> fct)
{
  for (sregex_token_iterator iter(inStr.begin(), inStr.end(), sepRx, -1), end; iter != end; ++iter)
  {
    fct(iter->str());
  }
}

void forEachLine(string& inStr, std::function<void(string)> fct)
{
  forEachRxToken(inStr, lineRxToken, fct);
}

vector<string> Tokenize(string& inStr, const regex& sepRx)
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


#endif // UTILS_H
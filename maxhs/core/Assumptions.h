/***********[Assumptions.h]
Copyright (c) 2012-2013 Jessica Davies, Fahiem Bacchus

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

***********/
#ifndef ASSUMPTIONS_h
#define ASSUMPTIONS_h

#include <vector>
#include <algorithm>
#include "maxhs/ifaces/SatSolver.h"
#include "maxhs/core/Wcnf.h"
#include "maxhs/core/Bvars.h"
#include "maxhs/utils/io.h"
#include "maxhs/core/MaxSolverTypes.h"

using Minisat::Lit;
using Minisat::var;
using Minisat::lbool;

class Assumps {
  //MaxSolver helper class
public:
//  Assumps(MaxHS_Iface::SatSolver* s, Bvars& b, vector<uint8_t>& mx) : satsolver{s}, bvars (b), inMx (mx) {
  Assumps(MaxHS_Iface::SatSolver* s, Bvars& b) : satsolver{s}, bvars (b) {
    map.resize(bvars.n(), -1); }
  ~Assumps() {}
  void init(const vector<Lit>& ivals, CoreType ctype); 
  void initAllSofts();
  void update(const vector<Lit>& conflict, bool remove);
  void remove(const vector<Lit>& conflict);
  void flip(const vector<Lit>& conflict);
  const vector<Lit>& vec() const { return assumps; }
  template <class Compare>
  void sort(Compare comp) {
    std::sort(assumps.begin(), assumps.end(), comp);
    setMap();
  }

private:
  vector<Lit> assumps;
  vector<int> map;
  MaxHS_Iface::SatSolver* satsolver;
  Bvars& bvars;
//  const uint8_t inMxdvar = 2;
//  const uint8_t inMxbvar = 1;
//  vector<uint8_t>& inMx;
//
  void clearIndex(Lit l) {
    map[bvars.toIndex(var(l))] = -1;
  }
  int getIndex(Lit l) {
    return map[bvars.toIndex(var(l))];
  }
      
  void clearMap() {
    for(size_t i = 0; i < map.size(); i++)
      map[i] = -1;
  }
  void setMap() {
    clearMap();
    for(size_t i = 0; i < assumps.size(); i++) 
      map[bvars.toIndex(var(assumps[i]))] = i;
  }
  void checkUpdate(Lit l) {
    if(getIndex(l) < 0) 
      cout << "c ERROR tried to update literal not in assumptions\n";
    if(assumps[getIndex(l)] != ~l) 
      cout << "c WARNING conflict agrees with assumption---no real update being done\n";
  }
//  bool dVar(Lit b) {
//    return (static_cast<size_t>(bvars.toIndex(b)) < inMx.size() && inMx[bvars.toIndex(b)] == inMxdvar);
//  }
//  bool bVarInMx(Lit b) {
//    return (static_cast<size_t>(bvars.toIndex(b)) < inMx.size() && inMx[bvars.toIndex(b)] == inMxbvar);
//  }
};

#endif

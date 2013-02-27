#ifndef __GRADAI_AUCTION_H__
#define __GRADAI_AUCTION_H__

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <ctime>

#include "CSV.h"

using namespace std;

// A bid is an XOR (vector) over pairs of (valuation, {seg1, seg2, .., segk})
typedef vector<pair<double, set<int> > > bid;
typedef bid* bid_ptr;

class Auction {

 public:

  int m_n;  // number of bidders
  int m_m;  // number of wireless segments
  map<int, bid_ptr> m_bids; // map of bidders -> full XOR bids

  double m_solve_time_s;  // how long did it take to solve this MIP?
  double m_objective;

  Auction(const string& filename);
  ~Auction();

  virtual void solve_wdp() = 0;

 protected:
  bool stoi(const string& input, int* output);
  bool stod(const string& input, double* output);
  string itst(int i);

 private:
  bool load_from_file(const string& filename);

};

#endif


#ifndef __GRADAI_AUCTIONCPLEXMIP_H__
#define __GRADAI_AUCTIONCPLEXMIP_H__

#include <ilcplex/ilocplex.h>
#include "Auction.h"


class AuctionCPLEXMIP : public Auction {

 public:
  
  AuctionCPLEXMIP(const string& filename);
  ~AuctionCPLEXMIP();
  
  virtual void solve_wdp();

 private:

  string mip_filename;
  bool write_mip_to_file();
  bool solve_with_cplex();

  string make_dec_var(int bidder, int conj_id);
};


#endif

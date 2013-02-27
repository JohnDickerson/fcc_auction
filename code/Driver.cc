#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>
#include <ctime>

#include "Auction.h"
#include "AuctionCPLEXMIP.h"

using namespace std;

int main(int argc, char* argv[]) {

  // All of the students' test inputs
  const char* const inputs[] = {"auction_m100_n100_p0.fcc", "auction_m100_n100_p2.fcc", "auction_m100_n200_p0.fcc", "auction_m100_n200_p2.fcc", "auction_m100_n20_p0.fcc", "auction_m100_n20_p2.fcc", "auction_m100_n50_p0.fcc", "auction_m100_n50_p2.fcc", "auction_m10_n10_p0.fcc", "auction_m10_n10_p2.fcc", "auction_m10_n20_p0.fcc", "auction_m10_n20_p2.fcc", "auction_m10_n2_p0.fcc", "auction_m10_n2_p2.fcc", "auction_m10_n5_p0.fcc", "auction_m10_n5_p2.fcc", "auction_m50_n100_p0.fcc", "auction_m50_n100_p2.fcc", "auction_m50_n10_p0.fcc", "auction_m50_n10_p2.fcc", "auction_m50_n25_p0.fcc", "auction_m50_n25_p2.fcc", "auction_m50_n50_p0.fcc", "auction_m50_n50_p2.fcc"};
  //const char* const inputs[] = {"auction_m10_n2_p0.fcc"};
  const size_t num_inputs = sizeof(inputs) / sizeof(inputs[0]);

  // For each test input, solve the winner determination problem
  for(size_t i=0; i<num_inputs; ++i) {
  
    // Load the auction model from the input file
    string input = string(inputs[i]);
    Auction* auction = NULL;
    try {
      auction = new AuctionCPLEXMIP(input);
    } catch(int e) {
      cerr << "ERROR: could not create auction for input " << input << endl;
      continue;
    }
    
    //cout << "Loaded " << auction->m_m << " segments and " << auction->m_n << " bidders" << endl;

    // Solve the WDP
    try {
      auction->solve_wdp();
      cout << input << " " << auction->m_objective << endl;
    } catch(int e) {
      cerr << "ERROR: something bad happened during the WDP solve." << endl;
    }

    delete auction;
  }


  return 0;
}

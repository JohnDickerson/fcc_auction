#include "Auction.h"

Auction::Auction(const string& filename) {
  
  // Read the file description of the auction into our model
  if(!load_from_file(filename)) {
    throw -1;
  }
}

Auction::~Auction() {
  // Delete anything this auction threw on the heap
  for(map<int, bid_ptr>::iterator b_it = m_bids.begin(); b_it != m_bids.end(); ++b_it) {
    bid_ptr b = b_it->second;
    delete b;
  }
}

// Converts a string to an integer, returning false if a problem occurs
bool Auction::stoi(const string& input, int* output) {
  stringstream s(input);
  s >> (*output);
  return !!s;
}

// Converts a string to a double, returning false if a problem occurs
bool Auction::stod(const string& input, double* output) {
  stringstream s(input);
  s >> (*output);
  return !!s;
}

// Converts int to a string, returns the string
string Auction::itst(int i) {
  ostringstream ss;
  ss << i;
  return ss.str();
}

bool Auction::load_from_file(const string& filename) {

  // Loads an auction from a space-delimited text file

  ifstream f(filename.c_str());
  if(!f) {
    cerr << "Trouble creating input stream for file " << filename << endl;
    return false;
  }

  int line_num = 0;  // 1-indexed line number for readability
  for(CSVIterator f_it(f); f_it != CSVIterator(); ++f_it) {

    line_num++;
    if(line_num == 1) {
      // The first row is distinct <num-segments> <num-bidders>, load it
      int num_segments, num_bidders;
      if(f_it->size() != 2 || !stoi((*f_it)[0], &num_segments) || !stoi((*f_it)[1], &num_bidders)) {
	cerr << "Malformed header line in file " << filename << endl;
	return false;
      }

      m_m = num_segments;
      m_n = num_bidders;
    
      // Inititalize the bidder mapping; we assume bidder IDs are 1-indexed
      for(int bidder_id=1; bidder_id<=num_bidders; ++bidder_id) {
	m_bids[bidder_id] = new bid();
      }

    } else {
      
      // All other rows are <bidder-id> <valuation> <seg1> <seg2> ... <segK>, load them  
      if(f_it->size() < 3) {
	cerr << "Not enough data on line " << line_num << " of file " << filename << endl;
	return false;
      }

      // Load and store the bidder who owns this conjunction, and her valuation
      int bidder_id;
      double valuation;
      if(!stoi((*f_it)[0], &bidder_id) || !stod((*f_it)[1], &valuation)) {
	cerr << "Trouble reading bidder and valuation on line " << line_num << " of file " << filename << endl;
	return false;
      }

      // Load the actual conjunction into a set, one literal at a time
      set<int> conj;
      for(size_t l_idx=2; l_idx<f_it->size(); ++l_idx) {
	int lit_id;
	if(!stoi((*f_it)[l_idx], &lit_id)) {
	  cerr << "Could not read literal " << (*f_it)[l_idx] << " from line " << line_num << " of file " << filename << endl;
	  return false;
	}
	conj.insert(lit_id);
      }	

      // Store the (value, conjunction) pair of this bidder's XOR bid
      m_bids[bidder_id]->push_back( make_pair(valuation, conj) );
    }

  }

  return true;
}

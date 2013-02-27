#include "AuctionCPLEXMIP.h"

AuctionCPLEXMIP::AuctionCPLEXMIP(const string& filename) : Auction(filename) {

  // We write the MIP out to a file for students, use this filename
  mip_filename = filename + string("_cplexmip.lp");
};
 
void AuctionCPLEXMIP::solve_wdp() {
  
  // Store the full MIP in LP format, so we can share it with students
  if(!write_mip_to_file()) {
    cerr << "ERROR: could not successfully write full MIP to file " << mip_filename << endl;
    throw -1;
  }

  // Call CPLEX on the written MIP
  if(!solve_with_cplex()) {
    cerr << "ERROR: CPLEX had trouble solving the MIP stored in " << mip_filename << endl;
    throw -1;
  }

  return;
}

string AuctionCPLEXMIP::make_dec_var(int bidder, int conj_id) {
  return "x_" + itst(bidder) + "_" + itst(conj_id);
}


bool AuctionCPLEXMIP::write_mip_to_file() {

  const char * m = mip_filename.c_str();
  ofstream mipfile;
  mipfile.open(m, ios::out);
  if (!mipfile.good()) {
    cerr << "ERROR: could not open the MIP file " << mip_filename << " for writing.";
    return false;
  }

  // Write the objective (revenue maximization)
  // \argmax \sum_{i \in B}  \sum_{S \subseteq L}   v_i(S) x_i(S)
  mipfile << "Maximize\n obj: ";
  for(map<int, bid_ptr>::const_iterator b_it=m_bids.begin(); b_it!=m_bids.end(); ++b_it) {
    
    int bidder_id = b_it->first;
    bid_ptr bid_p  = b_it->second;
    
    int conj_id=0;
    for(bid::const_iterator c_it=bid_p->begin(); c_it!=bid_p->end(); ++c_it) {
      
      string dec_var = make_dec_var(bidder_id, conj_id);
      double val = c_it->first;
      mipfile << " + " << val << " " << dec_var;
      conj_id++;
    }
  }
  mipfile << "\nSubject To\n";


  // Write the two sets of constraints
  // 1.  Each bidder can have at most one of his XOR conjunctions win
  //     \forall i \in B,   \sum_{S \subseteq L} x_i(S) \leq 1
  for(map<int, bid_ptr>::const_iterator b_it=m_bids.begin(); b_it!=m_bids.end(); ++b_it) {
    
    int bidder_id = b_it->first;
    bid_ptr bid_p  = b_it->second;
    
    mipfile << " b_" << itst(bidder_id) << ": ";
    int conj_id=0;
    for(bid::const_iterator c_it=bid_p->begin(); c_it!=bid_p->end(); ++c_it) {
      mipfile << " + " << make_dec_var(bidder_id, conj_id);
      conj_id++;
    }
    mipfile << " <= 1\n";
  }


  // 2.  Each segment can be allocated to at most one bidder
  //     \forall j \in L,   \sum_{i \in B} \sum_{S \in L, j \in S} x_i(S) \leq 1
  for(int seg_id=1; seg_id<=m_m; seg_id++) {

    mipfile << " l_" << itst(seg_id) << ": ";

    for(map<int, bid_ptr>::const_iterator b_it=m_bids.begin(); b_it!=m_bids.end(); ++b_it) {
      int bidder_id = b_it->first;
      bid_ptr bid_p  = b_it->second;

      int conj_id=0;
      for(bid::const_iterator c_it=bid_p->begin(); c_it!=bid_p->end(); ++c_it) {
	if(c_it->second.count(seg_id) != 0) {
	  // If this conjunction contains the current wireless segment, write it
	  mipfile << " + " << make_dec_var(bidder_id, conj_id);
	}
	conj_id++;
      }
    }

    mipfile << " <= 1\n";
  }
  
  // Enforce that the decision variables are binary
  mipfile << "Binary\n";
  for(map<int, bid_ptr>::const_iterator b_it=m_bids.begin(); b_it!=m_bids.end(); ++b_it) {
    
    int bidder_id = b_it->first;
    bid_ptr bid_p  = b_it->second;
    
    int conj_id=0;
    for(bid::const_iterator c_it=bid_p->begin(); c_it!=bid_p->end(); ++c_it) {
      mipfile << make_dec_var(bidder_id, conj_id) << "\n";
      conj_id++;
    }
  }

  mipfile << "End\n";

  return true;
}


bool AuctionCPLEXMIP::solve_with_cplex() {

  IloEnv env;
  try {
    
    IloModel model(env);
    IloObjective obj;
    IloNumVarArray var(env);
    IloRangeArray rng(env);
    
    // Load the model from file
    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());
    cplex.importModel(model, mip_filename.c_str(), obj, var, rng);
    cplex.extract(model);
    
    time_t start_time_ms, end_time_ms;
    time(&start_time_ms);
    
    // Find a solution to the full MIP
    if(!cplex.solve()) {
      env.error() << "CPLEX's .solve() failed." << endl;
      return false;
    }
    
    // Calculate time (in seconds) for optimization only
    time(&end_time_ms);
    m_solve_time_s = difftime(end_time_ms,start_time_ms);
    
    // Check that the soluton we found is optimal
    bool successful = (cplex.getStatus() == IloAlgorithm::Optimal);
    if (successful) {
      //env.out() << "Objective: " << cplex.getObjValue() << endl;
      m_objective = cplex.getObjValue();
    } else {
      env.error() << "CPLEX's .solve() returned a non-optimal solution!" << endl;
    }
    
    // Get the segment allocation vector
    IloNumArray vals(env);
    cplex.getValues(vals, var);
    
    // Set active ISP's new pricing vector
    /*ISP* active_isp = gen->getISPs()->at(active_isp_id);
    for(int b_idx=0; b_idx<num_bundles; b_idx++) {
      active_isp->setPrice(b_idx, vals[b_idx]);
      }*/	      
    
  } catch(IloException& e) {
    cerr << "CPLEX exception: " << e << endl;
    return false;
  } catch(...) {
    cerr << "Unknown exception!" << endl;
    return false;
  }
  
  env.end();
  return true;
}

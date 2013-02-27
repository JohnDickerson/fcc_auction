#!/usr/bin/python -O

import sys
import random
import csv
import hashlib


# Generator for HW2:  generate(m,n) generates an m-segment auction with n bidders, each with k XOR-bids. 

# 
# John P. Dickerson -- 18 February 2013 -- FCC Auction Generator
# Homework #2: Informed Search and Planning
# Graduate AI 2013 -- Professors Tuomas Sandholm and Manuela Veloso
#
# FCC Auction (segments, bidders in XOR language) generator for Problem #2
#
# Sample usage:
#   $> python generator.py 10 2 3 sample.fcc [some-seed]
#   This generates an auction with 10 wireless segments and 2 bidders, with 
#   random valuations truncated to 3 decimal places and stores the resulting 
#   auction in the file "sample.fcc".  If the optional seed argument is 
#   provided, we seed with the sha hash of the parameter

def trunc(f, n):
    '''Truncates/pads a float f to n decimal places without rounding'''
    if n > 0:
        slen = len('%.*f' % (n, f))
        return str(f)[:slen]
    else:
        return str(int(f))

def generate(m, n, prec, outfile):
    '''Generates an m-segment, n-XOR-bidder auction with random valuations'''

    print 'Generating %d-segment auction with %d bidder%s, precision=%d.' % (m, n, 's' if n != 1 else '', prec)

    # Generate a single compound XOR bid per bidders
    bids = {}
    for bidder in range(n):

        # How many conjunctions should we have?  ~Uint[1,10]
        bid_len = random.randint(1,10)

        # Generate a chunk of segments to care about per XOR bit
        bid = []
        for conj in range(bid_len):
            
            # Which conjunctions should this bid care about?
            conj_len = random.randint(1,m)
            conj = random.sample(range(m), conj_len)

            # How much do I value this conjunction?
            conj_val = 1.0 + random.random() * 10.0
            
            # Append the (value, segments) conjunction to the full XOR
            bid.append( (conj_val, conj) )

        # Append the bidder's full XOR bid to the dict of all bids
        bids[bidder] = bid
        

    print "Writing to %s" % (outfile)
    with open(outfile, "wb") as f:
        writer = csv.writer(f, delimiter=' ')

        # Write the auction details
        writer.writerow([m, n])

        # Write each conjunction in each XOR bid (1-indexed)
        for bidder, bid in bids.iteritems():
            for (value, conj) in bid:
                writer.writerow([bidder+1, trunc(value, prec)] + map(lambda x: x+1, conj))

        f.close()


if __name__ == '__main__':

    if(len(sys.argv) < 5 or len(sys.argv) > 6):
        print "Usage: python generator.py <num-segments> <num-bidders> <output-file> [<seed>]"
        sys.exit(-1)

    # Four parameters: num vertices, prob of edges, num tokens, output file
    m = int(float(sys.argv[1]))
    n = int(float(sys.argv[2]))
    prec = int(float(sys.argv[3]))
    outfile = sys.argv[4]

    if len(sys.argv) == 6:
        # If the user passes in a seed, seed our random roller with this; 
        # otherwise, seed the way Python normally does (I guess using systime)
        seed_str = sys.argv[5]
        seed = int(hashlib.md5(seed_str).hexdigest(), 16)
        random.seed(seed)
    

    # Make sure user-supplied parameters are shipshape 
    assert m > 0, 'Number of segments must be positive'
    assert n > 0, 'Number of bidders must be positive'
    assert prec >= 0, 'Decimal precision must not be negative'

    # Generate the FCC auction
    generate(m,n,prec,outfile)

    print "Done"
    

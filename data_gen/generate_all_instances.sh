#!/usr/bin/env bash

# Number of wireless segments in auction
for num_segments in 10 50 100
do
    echo "Number of segments: ${num_segments}"

    # Number of bidders = number of segments/bidder div
    for bidder_div in 5 2 1 0.5
    do
	num_bidders=`echo ${num_segments}/${bidder_div} | bc`
	seed=jpdicker_${num_segments}_${num_bidders}

	# Round bids to dollars or to cents, same seed otherwise
	for precision in 0 2
	do
	    outfile=auction_m${num_segments}_n${num_bidders}_p${precision}.fcc
	    ./generator.py ${num_segments} ${num_bidders} ${precision} ${outfile} ${seed}
	done
    done
done

# Tarball the instances into a personal archive
echo "Tarballing instances ..."
tarball=hw2_instances.tgz
tar cvfz ${tarball} *.fcc generator.py generate_all_instances.sh

# Clean up the scraps
echo "Cleaning up ..."
rm -f *.fcc

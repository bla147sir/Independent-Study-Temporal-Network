# UB_Independent_Study_Temporal_Network

The code that takes a temporal graph (node-id-1, node-id-2, timestamp), sliding window length, and slide step and gives us a set of charts that summarizes important statistics from the NUCLEI files of ALL the windows.

-compile : g++ -std=c++11 slideGraph.cpp -o slide.out

-run : ./slide.out <dataset> <algorithm> <sliding window length> <sliding step length>
e.g.   ./slide.out stackoverflow_a2q.txt 12 180 90 
<dataset>: input file format: node1 node2 unixtimestamp

<algorithm>: run what kind of algo of nucleus decomposition, can be 12, 13, 14, 23, or 34. First digit is the r value and second digit is the s value for (r,s)-nucleus decomposition. 12 is k-core, 23 is k-truss with triangle connectivity

<sliding window length>: count in day

<sliding step length>: count in day
-For instance, if we have a 180-day window with a step of 90-day, and our temporal graph spans to 4 years, then there will be 15 graphs; ((totalSpan - windowSize) / stepSize) + 1. 

-Nucleus decomposition:        

"Finding the Hierarchy of Dense Subgraphs using Nucleus Decompositions"
	A. E. Sariyuce, C. Seshadhri, A. Pinar, U. V. Catalyurek
	International World Wide Web Conference (WWW), 2015.

"Fast Hierarhcy Construction for Dense Subgraphs"
	A. E. Sariyuce, A. Pinar
	Very Large Data Bases Conference (VLDB), 2017	

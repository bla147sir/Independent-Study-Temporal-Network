#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <queue>
#include <sstream>  

using namespace std;

int main() {

	fstream ori_file;
	ofstream newfile;
	ori_file.open("stackoverflow_a2q.txt");
	newfile.open("undirected_a2q.txt");

	if (!ori_file.is_open() || !newfile.is_open()) {
		cout << "can't open the file" << endl;
		return -1;
	} 

	string linebuf;
	map<int, set<int> > edges;      // store all the edges in the graph, from 2014/01

	while (getline(ori_file, linebuf))   
	{	
		int node[2];
		stringstream ss(linebuf);
    	
    	cout << linebuf << endl;
        ss>>node[0]>>node[1];

		if (node[0] == node[1]) continue;     
		set<int> temp;
		
		// v1-v2  *******
		// put the edge in the whole graph map   
		auto iter = edges.find(node[0]);
		if (iter != edges.end()) { 
			temp = edges[node[0]];
			if (temp.find(node[1]) != temp.end())
				continue;	
		}
		temp.insert(node[1]);
		//edges.insert(pair<int, set<int> >(node[0], temp));
		edges[node[0]] = temp;

		//   v2-v1    *****
		temp.clear();
		iter = edges.find(node[1]);
		if (iter != edges.end()) {
			temp = edges[node[1]];
		}
		temp.insert(node[0]);
		//edges.insert(pair<int, set<int> >(node[1], temp));
		edges[node[1]] = temp;
	}
	cout << "Here";
	//sort and write the edges to the file
	map<int, int> rename_new2old, rename_old2new;           //rename the node id, makes it start from 0, and increase 1 <original name, new name>
	int index = 0;
	priority_queue<int, vector<int>, greater<int> > pq;
	for (auto it = edges.begin(); it != edges.end(); it++) {
		rename_new2old.insert(pair<int, int>(index, it->first));			
		rename_old2new.insert(pair<int, int>(it->first, index));
		pq.push(index);
		index++;
	}
	while (!pq.empty()) {
		int cur = pq.top();
		int old = rename_new2old[cur];
		pq.pop();
		set<int> s = edges[old];
		priority_queue<int, vector<int>, greater<int> > tpq;
		for (auto j : s) {
			int t = rename_old2new[j];
			tpq.push(t);
		}
		while(!tpq.empty()) {
			if (tpq.top() > cur) {
				newfile << to_string(cur) + " " + to_string(tpq.top()) << endl;
			}
			tpq.pop();
		}
	}

	newfile.close();
	ori_file.close();
	
	return 0;
}
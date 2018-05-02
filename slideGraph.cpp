#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <sstream> 
#include <unordered_map>
#include <unordered_set>

using namespace std;

int exe (string dataset, int win_len, int step_len, string algo);
void runNucleus (string inputfile, string algo);
void summary (string dataset, int win_len, int step_len, string algo);
string* calculate (fstream& nucleusFile);
void drawGraph(string inputfile);    // use python 3

int main (int argc, char *argv[]) {

	string dataset(argv[1]);
	string algo(argv[2]);
	string a1(argv[3]);
	string a2(argv[4]);
	int win_len = stoi(a1);
	int step_len = stoi(a2);
	
	if (exe(dataset, win_len, step_len, algo) != 0) return 0;

	summary(dataset, win_len, step_len, algo);

	
	return 0;
}

int exe (string dataset, int win_len, int step_len, string algo) {
	int unit_day_len = 86400;
	int stampstart = 1217548800;
	int stampend = stampstart + unit_day_len * (win_len - 1);
	fstream ori_file;
	ofstream newfile;
	
	ori_file.open("./stackoverflow_dataset_output/" + dataset);
	if (!ori_file.is_open()) {
		cout << "can't open the file" << endl;
		return -1;
	} 
	
	string linebuf;

	//int i = 0;
	while (true) {	
		if (ori_file.eof()) break;
		
		unordered_map<int, unordered_set<int> > edg_mon;    // store the edges in the current period
		string name = to_string(stampstart) + "_" + to_string(stampend) + ".txt";
		newfile.open("./stackoverflow_dataset_output/slide/" + dataset.substr(14, 3) + "/" + name);
		if (!newfile.is_open()) {
			cout << "can't open " + name << endl;
			return -1;
		}

		bool mark_next = false;
		streampos next_start;
		while (getline(ori_file, linebuf) && !ori_file.eof()){
			// split the string
			stringstream ss(linebuf);
			int ts;
			int node[2];
	
	        ss >> node[0] >> node[1] >> ts;
	        
			if (ts < stampstart || node[0] == node[1]) continue;         // last period
			else if (ts >= stampstart && ts < stampend){   
				if (!mark_next && ts >= stampstart + step_len * unit_day_len) {
					next_start = ori_file.tellg();
					mark_next = true;
				}
				unordered_set<int> temp;
				
				// v1-v2  *******
				// put the edge in the current interval map 
				auto iter = edg_mon.find(node[0]);
				if (iter != edg_mon.end()) {
					temp = edg_mon[node[0]];
					if (temp.find(node[1]) != temp.end())
						continue;
				}
				temp.insert(node[1]);
				edg_mon[node[0]] = temp;
				
				//   v2-v1    *****
				temp.clear();
				iter = edg_mon.find(node[1]);
				if (iter != edg_mon.end()) {
					temp = edg_mon[node[1]];
				}
				temp.insert(node[0]);
				edg_mon[node[1]] = temp;
			} else {                                       // next interval
				ori_file.seekg(next_start);    // go back to the next start
				break;
			}

		}
		//sort and write the edges to the file
		priority_queue<int, vector<int>, greater<int> > pq;
		for (auto it = edg_mon.begin(); it != edg_mon.end(); it++) {
			pq.push(it->first);
		}
		while (!pq.empty()) {
			int cur = pq.top();
			pq.pop();
			unordered_set<int> temp = edg_mon[cur];
			priority_queue<int, vector<int>, greater<int> > tpq;
			for (auto j : temp) {
				tpq.push(j);
			}
			while(!tpq.empty()) {
				if (tpq.top() > cur) {
					newfile << to_string(cur) + " " + to_string(tpq.top()) << endl;
				}
				tpq.pop();
			}
		}
		newfile.close();
		string runfile = "./stackoverflow_dataset_output/slide/" + dataset.substr(14, 3) + "/" + name;
		runNucleus(runfile, algo);
		stampstart += step_len * unit_day_len;
		stampend = stampstart + (win_len - 1) * unit_day_len;
		//i++;
	}

	ori_file.close();
	return 0;
}

void runNucleus (string inputfile, string algo) {
	string command = "./nucleus " + inputfile + " " + algo + " YES";
	cout << command << endl;
	system(command.c_str());
}

void summary(string dataset, int win_len, int step_len, string algo) {
	int unit_day_len = 86400;
	int stampstart = 1217548800;
	int stampend = stampstart + unit_day_len * (win_len - 1);
	string type = dataset.substr(14, 3);
	string subgraphSum = type + "_" + algo + "_subgraph#_summary.txt";
	string leavesSum = type + "_" + algo + "_leaves#_summary.txt";
	string avgChildrenSum = type + "_" + algo + "_avg#children_summary.txt";
	string maxKSum = type + "_" + algo + "_maxK_summary.txt";
	string moreOneSum = type + "_" + algo + "_more_than_one_child_summary.txt";
	ofstream fileSubgraph, fileLeaves, fileAvgChild, fileMaxK, fileMoreOne;
	
	fileSubgraph.open(subgraphSum);
	fileLeaves.open(leavesSum);
	fileAvgChild.open(avgChildrenSum);
	fileMaxK.open(maxKSum);
	fileMoreOne.open(moreOneSum);

	while (true) {
		string intervals = to_string(stampstart) + "_" + to_string(stampend);
		string fileName = intervals + ".txt_" + algo + "_NUCLEI";
		fstream nucleusFile;

		nucleusFile.open(fileName);
		if (!nucleusFile.is_open()) break;

		string *result = calculate(nucleusFile);

		fileLeaves << type + "_" + intervals + ":" + result[0] << endl;		
		fileSubgraph << type + "_" + intervals + ":" + result[1] << endl;
		fileAvgChild << type + "_" + intervals + ":" + result[2] << endl;
		fileMaxK << type + "_" + intervals + ":" + result[3] << endl;
		fileMoreOne << type + "_" + intervals + ":" +result[4] << endl;
		nucleusFile.close();
		stampstart += step_len * unit_day_len;
		stampend = stampstart + (win_len - 1) * unit_day_len;
	}
	fileLeaves.close();
	fileSubgraph.close();
	fileMaxK.close();
	fileAvgChild.close();
	fileMoreOne.close();

	drawGraph(subgraphSum);
	drawGraph(leavesSum);
	drawGraph(avgChildrenSum);
	drawGraph(maxKSum);
	drawGraph(moreOneSum);
}

string* calculate (fstream& nucleusFile) {
	string* result = new string[5];
	int num_leaves = 0, num_subgraph = 0, max_K = INT_MIN, num_children = 0, num_more_one = 0;
	unordered_map<int, int> id_child_num;     // key = node id , value = num of children
	string linebuf;
	int id, k, token, leaf, parentId;
	
	while (true) {
		if (nucleusFile.eof()) break;
		while (getline(nucleusFile, linebuf) && !nucleusFile.eof()) {
			stringstream ss(linebuf);
		//subgraph id   k     |V|      |E|   density  leaf or not / parent node id     
			ss >> id >> k >> token >> token >> token >> leaf >> parentId; 
			num_subgraph++;
			max_K = max(max_K, k);
			if (leaf == 1) num_leaves++;

			auto iter = id_child_num.find(parentId);
			if (iter != id_child_num.end()) { 
				int temp = id_child_num[parentId];
				id_child_num[parentId] = ++temp;			
			} else {
				id_child_num[parentId] = 1;
			}
		}
	}

	for (auto iter = id_child_num.begin(); iter != id_child_num.end(); iter++) {
		int count = id_child_num[iter->first];
		num_children += count;
		if (count > 1) num_more_one++;
	}

	result[0] = "number of leaves: " + to_string(num_leaves);
	result[1] = "number of subgraph: " + to_string(num_subgraph);
	result[2] = "average number of children: " + to_string((float)num_children / (float)(num_subgraph));
	result[3] = "max K: " + to_string(max_K);
	result[4] = "num of subgraph more than one children: " + to_string(num_more_one);

	return result;
}

void drawGraph(string inputfile) {
	string command = "python3 chart.py " + inputfile;
	cout << command << endl;
	system(command.c_str());
}
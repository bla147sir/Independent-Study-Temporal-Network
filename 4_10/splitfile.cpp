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
	ori_file.open("stackoverflow_c2q.txt");

	if (!ori_file.is_open()) {
		cout << "can't open the file" << endl;
		return -1;
	} 

	time_t rawtime;
	struct tm * timeinfo;
	int year = 2008, day = 1, hour = 0, min = 0, sec = 0;
	int stampstart, stampend;
	
	time(&rawtime);   // get current time info
	timeinfo = localtime(&rawtime);    // convert to struct
	// modify the time do the given date
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = 0;   // Jan
	timeinfo->tm_mday = day;
	timeinfo->tm_hour = hour;
	timeinfo->tm_min = min;
	timeinfo->tm_sec = sec;
	stampstart = timegm(timeinfo);

	//timeinfo->tm_year = year + 1 - 1900;
	timeinfo->tm_mon = 6;  //July
	stampend = timegm(timeinfo);

	string linebuf;
	int i = 0;
	map<int, set<int> > edges;      // store all the edges in the graph, from 2014/01

	//while (!ori_file.eof())   
	while (true) 
	{	
		if (ori_file.eof()) break;
		
		map<int, set<int> > edg_mon;    // store the edges in the current month
		if (i % 12 == 0 && i != 0) year++;
		string name = "c2q_" + to_string(year) + "_" + to_string((i + 1) % 12 == 0 ? 12 : (i + 1) % 12) + ".txt";
		newfile.open(name);
		if (!newfile.is_open()) {
			cout << "can't open " + name << endl;
			return -1;
		}
		while (getline(ori_file, linebuf) && !ori_file.eof()){
			// split the string
			streampos oldpos = ori_file.tellg();   //record the current position
			stringstream ss(linebuf);
			int ts;
			int node[2];
	
	        ss >> node[0] >> node[1] >> ts;
	        
			if (ts < stampstart || node[0] == node[1]) continue;                 // last month
			else if (ts >= stampstart && ts < stampend){   // current month
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
				edges[node[0]] = temp;

				// put the edge in the current month map 
				temp.clear();
				iter = edg_mon.find(node[0]);
				if (iter != edg_mon.end()) {
					temp = edg_mon[node[0]];
					if (temp.find(node[1]) != temp.end())
						continue;
				}
				temp.insert(node[1]);
				edg_mon[node[0]] = temp;
				
				//   v2-v1    *****
				temp.clear();
				iter = edges.find(node[1]);
				if (iter != edges.end()) {
					temp = edges[node[1]];
				}
				temp.insert(node[0]);
				edges[node[1]] = temp;

				temp.clear();
				iter = edg_mon.find(node[1]);
				if (iter != edg_mon.end()) {
					temp = edg_mon[node[1]];
				}
				temp.insert(node[0]);
				edg_mon[node[1]] = temp;
			} else {                                       // next month
				ori_file.seekg(oldpos);    // go back to the previous line
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
			set<int> temp = edg_mon[cur];
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

		// modify the time info 
		stampstart = stampend;
		// monthly 
/*		i += 1;
		if (i % 12 == 11) 
			timeinfo->tm_year = year + 1 - 1900;
			
		timeinfo->tm_mon = (i+1) % 12 ;
		*/

		// half yearly
		i += 6;
		if (i % 12 == 6) {
			timeinfo->tm_year = year + 1 - 1900;
			timeinfo->tm_mon = 0;
		} else {
			timeinfo->tm_mon = 6;
		}

		// yearly
		/*
		year++;
		timeinfo->tm_year = year + 1 - 1900; */

		stampend = timegm(timeinfo);
	}

	ori_file.close();
	
	return 0;
}
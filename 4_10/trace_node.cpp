#include <iostream>
#include <fstream>
#include <map>
#include <sstream> 
#include <vector>  

using namespace std;

vector<int> createNodeList(int maxK, string pathfile, ofstream& resultfile);
void traceDown(int nodeID, string nufilestr, ofstream& resultfile, int year, int mon);

int main () {

	string type = "c2a";
	string algo = "12";
	string file = type + "_" + algo + "_nodeTrack_maxK.txt";
	string path = "./stackoverflow_dataset_output/" + type + algo + "_6mon/";
	ofstream resultfile;
	fstream sumFile;
	string token, linebuf, first6mon;
	int maxK, year, mon;

	resultfile.open(file);
	sumFile.open("./summary/" + type + "_" + algo + "_maxK_summary.txt");
	
	getline(sumFile, linebuf);
	stringstream ss(linebuf);
	ss >> token >> token >> token;
	maxK = stoi(token);

	sumFile.close();

	first6mon = type + "_2008_7.txt_" + algo + "_NUCLEI";
	vector<int> nodes = createNodeList(maxK, path + first6mon, resultfile);

	for (int i = 0; i < nodes.size(); i++) {
		year = 2009;
		mon = 1;
		int nodeID = nodes[i];
		resultfile << to_string(nodeID) << endl; 
		while (year < 2016) {
			string fileName = type + "_" + to_string(year) + "_" + to_string(mon) + ".txt_" + algo + "_NUCLEI";
			fstream nufile;
			nufile.open(path + fileName);
			traceDown(nodeID, path + fileName, resultfile, year, mon);

			mon = (mon + 6) % 12;
			if (mon == 1) year++;
		
		}
	}
	resultfile.close();

	return 0;
}

void traceDown(int nodeID, string nufilestr, ofstream& resultfile, int year, int mon) {
	fstream ffile; 
	string linebuf;

	ffile.open(nufilestr);

	while (true) {
		if (ffile.eof()) break;
		while (getline(ffile, linebuf)) {
			stringstream ss(linebuf);
			string token;
			int i = 0;
			bool contains = false;
			while (!ss.eof()) {
				ss >> token;
				if (i >= 7 && stoi(token) == nodeID) {
					contains = true;
					break;
				}
				i++;
			}
			if (contains) 
				resultfile << to_string(year) + "_" + to_string(mon) + ":" + linebuf << endl;

		}
	}
	ffile.close();
	return;
}

vector<int> createNodeList(int maxK, string pathfile, ofstream& resultfile) {
	vector<int> nodes;
	fstream ffile;
	string linebuf, nodestr;
	int k = maxK;

	ffile.open(pathfile);

	while (true) {
		if (ffile.eof() || k != maxK) break;
		while (getline(ffile, linebuf)) {
			stringstream ss(linebuf);
			vector<string> temp;
			string token;
			while (!ss.eof()) {
				ss >> token;
				temp.push_back(token);
			}
			k = stoi(temp[1]);
 			if (k == maxK) {
 				resultfile << linebuf << endl;
 				int i = 7;
 				while (i < temp.size() - 1) {
 					nodes.push_back(stoi(temp[i++]));
 				}
 			} else {
 				break;
 			}
		}
	}
	ffile.close();
	return nodes;
}
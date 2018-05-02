#include <iostream>
#include <fstream>
#include <map>
#include <sstream>  
#include <unordered_map>

using namespace std;

string* calculate(string type, string algo, int year, int mon, int date, string pathfile);

int main(int argc, char *argv[]) {

	string type (argv[1]);
	string algo (argv[2]);
	string dataset (argv[3]);
	//string newName = type + "_" + algo +"_summary.txt";
	//ofstream newfile;
	//newfile.open(newName);
	string subgraphSum = type + "_" + algo + "_subgraph#_summary.txt";
	string leavesSum = type + "_" + algo + "_leaves#_summary.txt";
	string avgChildrenSum = type + "_" + algo + "_avg#children_summary.txt";
	string maxKSum = type + "_" + algo + "_maxK_summary.txt";
	ofstream fileSubgraph, fileLeaves, fileAvgChild, fileMaxK;
	
	fileSubgraph.open(subgraphSum);
	fileLeaves.open(leavesSum);
	fileAvgChild.open(avgChildrenSum);
	fileMaxK.open(maxKSum);

	if (dataset.compare("stack") == 0) {
		int year = 2008, mon = 7;
		string path = "./stackoverflow_dataset_output/" + type + algo + "_6mon/";

		while (year < 2016) {
			string fileName = type + "_" + to_string(year) + "_" + to_string(mon) + ".txt_" + algo + "_NUCLEI";
			string pathfile = path + fileName; 
			string *result = calculate(type, algo, year, mon, -1, pathfile);
			/*
			newfile << type + "_" + to_string(year) + "_" + to_string(mon) + ":" << endl;
			newfile << result[0] << endl;   // subgraph
			newfile << result[1] << endl;   // leaves 
			newfile << result[2] << endl;   // avg children
			newfile << result[3] << endl;   // children > 1
			newfile << result[4] << endl;   // density
			newfile << result[5] << endl;   // density and size
			newfile << result[6] << endl;   // max K
			newfile << result[7] << endl;   // vertices or edges
			newfile << "--------------------------" << endl;
*/
			fileSubgraph << type + "_" + to_string(year) + "_" + to_string(mon) + ":" + result[0] << endl;
			fileLeaves << type + "_" + to_string(year) + "_" + to_string(mon) + ":" + result[1] << endl;
			fileAvgChild << type + "_" + to_string(year) + "_" + to_string(mon) + ":" + result[2] << endl;
			fileMaxK << type + "_" + to_string(year) + "_" + to_string(mon) + ":" + result[6] << endl;
			mon = (mon + 6) % 12;
			if (mon == 1) year++;
			
		}

	} else if (dataset.compare("as") == 0){
		int year = 1997, mon = 11, date = 8, index = 0;
		int endDate[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
		string strM, strD, fileName;
		string path = "./as_output/" + algo + "/";

		while (true) {
			strM = mon < 10 ? "0" + to_string(mon) : to_string(mon);
			strD = date < 10 ? "0" + to_string(date) : to_string(date);
			fileName = "as" + to_string(year) + strM + strD + ".txt" + "_" + algo + "_NUCLEI";
			string pathfile = path + fileName;
			string *result = calculate(type, algo, year, mon, date, pathfile);
			/*
			newfile << type + "_" + to_string(year) + "_" + strM + strD + ":" << endl;
			newfile << result[0] << endl;   // subgraph
			newfile << result[1] << endl;   // leaves 
			newfile << result[2] << endl;   // avg children
			newfile << result[3] << endl;   // children > 1
			newfile << result[4] << endl;   // density
			newfile << result[5] << endl;   // density and size
			newfile << result[6] << endl;   // max K
			newfile << result[7] << endl;   // vertices or edges
			newfile << "--------------------------" << endl;
			*/
			if (result[0].length() > 0) {
				fileSubgraph << type + "_" + to_string(year) + "_" + strM + strD + ":" + result[0] << endl;
				fileLeaves << type + "_" + to_string(year) + "_" + strM + strD + ":" + result[1] << endl;
				fileAvgChild << type + "_" + to_string(year) + "_" + strM + strD + ":" + result[2] << endl;
				fileMaxK << type + "_" + to_string(year) + "_" + strM + strD + ":" + result[6] << endl;
			}
			
			if (date >= endDate[mon - 1]){
				date = 1; 
				if (mon == 12) year++;
				mon = mon == 11 ? 12 : (mon + 1) % 12;
			}
			else 
				date++;
			if (year == 2000 && mon == 1 && date == 3) break;
			index ++;
		}
	}
	

	//newfile.close();
	fileSubgraph.close();
	fileLeaves.close();
	fileAvgChild.close();
	fileMaxK.close();
	return 0;
}

string* calculate (string type, string algo, int year, int mon, int date, string pathfile) {
	string* result = new string[8];
	fstream ori_file;
	ori_file.open(pathfile);

	if (!ori_file.is_open()) {
	//	cout << "can't open the file: " + pathfile << endl;
		return result;
	} 

	int num_leaves = 0, num_subgraph = 0, max_K = INT_MIN, v_withMaxK = -1, e_withMaxK = -1, size_limit = 5;
	int num_den_overlimit = 0, num_V_overlimit = 0, num_pass_both = 0, more1_children = 0, num_children = 0;
	float den_limit = 0.8;
	
	unordered_map<int, int> id_child_num;     // key = node id , value = num of children
	string linebuf;
	int id, k, numV, numE, leaf, parentId;
	float density;
	
	while (true) {
		if (ori_file.eof()) break;
		while (getline(ori_file, linebuf) && !ori_file.eof()) {
			stringstream ss(linebuf);
			ss >> id >> k >> numV >> numE >> density >> leaf >> parentId; 
			num_subgraph++;
			if (numV == -1 || parentId == -1) continue;

			if (max_K < k) {
				max_K = k;
				v_withMaxK = numV;
				e_withMaxK = numE;
			}

			if (numV > size_limit) num_V_overlimit++;
				
			if (density > den_limit) num_den_overlimit++;
			
			if (numV > size_limit && density > den_limit) num_pass_both++;

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
	ori_file.close();

	for (auto iter = id_child_num.begin(); iter != id_child_num.end(); iter++) {
		int count = id_child_num[iter->first];
		num_children += count;
		if (count > 1) more1_children++;
	}

	result[0] = "number of subgraph: " + to_string(num_subgraph);
	result[1] = "number of leaves: " + to_string(num_leaves);
	result[2] = "average number of children: " + to_string((float)num_children / (float)(num_subgraph - num_leaves));
	result[3] = "factor of number of children > 1: " + to_string((float)more1_children / (float)num_subgraph);
	result[4] = "factor of density > " + to_string(den_limit) + ": " + to_string((float)num_den_overlimit / (float)num_subgraph);
	result[5] = "factor of (density > " + to_string(den_limit) + " and size > " + to_string(size_limit) + "):" + to_string((float)num_pass_both / (float)num_subgraph);
	result[6] = "max K: " + to_string(max_K);

	if (algo.compare("12") == 0)    // k-core
		result[7] = "Vertices with max K: " + to_string(v_withMaxK);
	else if (algo.compare("23") == 0)  // k-truss
		result[7] = "Edges with max K: " + to_string(e_withMaxK);
	else
		result[7] = "";

	return result;

}
#include <stdlib.h>
#include <iostream>

using namespace std;

int main () {

	string type[3] = {"a2q", "c2q", "c2a"};
	string algo[3] = {"12", "23", "34"};
	int i = 0, j = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			string command = "./ana.out " + type[i] + " " + algo[j] + " stack";
			system(command.c_str());

		}
	}

	return 0;
}

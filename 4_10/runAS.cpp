#include <stdlib.h>
#include <iostream>

using namespace std;

int main (int argc, char *argv[]) {

	int year = 1997, mon = 11, date = 8, index = 0;
	int endDate[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
	string strM, strD, inputFile, addr = "./as/";
	string hie = "YES" , algo = "12";

	while (true) {
		strM = mon < 10 ? "0" + to_string(mon) : to_string(mon);
		strD = date < 10 ? "0" + to_string(date) : to_string(date);
		inputFile = "as" + to_string(year) + strM + strD + ".txt";
		string command = "./nucleus " + addr + inputFile + " " + algo + " " + hie;
		system(command.c_str());
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

	return 0;
}

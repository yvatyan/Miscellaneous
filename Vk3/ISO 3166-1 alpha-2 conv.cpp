#include <fstream>
#include <iostream>
#include <cstdio>
using namespace std;

unsigned char next2pow(unsigned char x) {

	if(x&x-1) {
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		return x+1;
	}
	return x;
}

void show_help() {
	cout << 
		"Output and Input files construction:\n"			<<
		"==== <file name>.txt ====\n"						<<
		"ISO 3166-1 alpha-2\n"								<<
		"< <code> <country name> >\n"						<<
		"< <code> <country name> >\n"						<<
		"etc.\n\n"											<<
		
		"==== ISO31661A2.dat ====\n"						<<
		"ISO 3166-1 alpha-2 ; [0, 23]\n" 					<<
		"<oFfset between countries> ; [24]\n"				<<
		"<code> <country name> ; [25 + F*i, 25 + F*(i+1)]\n"<<
		"    where i is from 0\n";
	return;
}

int main(int _argv, char **_argc) {
	
	cerr << "\nISO 3166-1 alpha-2 to binary data converter\nver. 01.01.02.16\n" << endl << endl;
	if(_argv != 2) {
		if(_argv > 2) cerr << "[ERROR:] Converter should get only text file." << endl;
		else          cerr << "[ERROR:] Converter should get text file." << endl;
		return 1;
	}

	if(_argc[1][0] == '-' && _argc[1][1] == 'h') {
		show_help();
		return 0;
	}
	
	ifstream fin;
	fin.open(_argc[1]);
	if(!fin.is_open()) {
		cerr << "[ERROR:] Can not open the file." << endl;
		return 1;
	}
	string str, str2;
	getline(fin, str);
	if(str != "ISO 3166-1 alpha-2") {
		cerr << "[ERROR:] Invalid file header." << endl;
		return 1;
	}
	cerr << "  [OK:] File accepted." << endl;
	
	unsigned char max = 0;
	while(fin >> str2) {
		getline(fin, str);
		if(str2.length() != 2) {
			cerr << "[ERROR:] Code length should be 2 for " << str << '.' << endl;
			return 1;			
		}
		max = str.length() > max ? str.length() : max;
	}
	cerr << "[INFO:] Maximal length of country name is " << (unsigned int) max << '.' << endl;
	
	fin.clear();
	fin.seekg(0);
	getline(fin, str);

	max = next2pow(max+3);
	FILE * binFile = fopen("ISO31661A2.dat", "wb");
	unsigned char buffer[18] = {'I', 'S', 'O', ' ', '3', '1', '6', '6', '-', '1', ' ','a', 'l', 'p', 'h', 'a', '-', '2'};
	
	fwrite(buffer, 18, 1, binFile);
	fseek(binFile, 24, SEEK_SET);
	fwrite(&max, 1, 1, binFile);
	int count = 0;
	while(fin >> str2) {
		getline(fin, str);
		fseek(binFile, 25 + count*max, SEEK_SET);
		fwrite(str2.c_str(), 2, 1, binFile);
		fwrite(str.c_str(), str.length(), 1, binFile);
		++count;
	}
	
	cerr << "  [OK:] Successfully converted." << endl;
	return 0;
}

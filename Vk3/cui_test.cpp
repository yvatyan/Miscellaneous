#include "Source/Headers/cui.h"

#include <iostream>
#include <string>
#include <windows.h>

int main() {

	cui::progressBar bar(10, 10, 25, 2);
	cui::dword cur_value = 0, max_value = 50;
	// �� ��� �� ��. 4 �������� ��� ������� ������������� ����������, �������� �� ����� ���������� )))
	// �� ����� ������. �� ��� 0 �������, ��� 1-��. ��. � ����� �����. �����
	
	
	char t;
	
	COORD coord;
	coord.X = 0;
	coord.Y = 0;/*
	COORD coord2;
	coord2.X = 10;
	coord2.Y = 10;
	COORD coord3;
	coord3.X = 31;
	coord3.Y = 10;*/
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	while(8) {
		SetConsoleCursorPosition(hConsole, coord);
		std::cout << " \b";
		std::cin >> t;
		/*
		SetConsoleCursorPosition(hConsole, coord3);
		std::cout << '|';
		SetConsoleCursorPosition(hConsole, coord2);*/
		switch(t) {
			case	'q'	:	goto E;        // �����
			case	'd'	:	cur_value += 1; bar.Update_n_Show(cur_value, max_value); break; // ���������� �����
			case	'h'	:	bar.Hide(); break; // ������ ����
			case	'm' :	cur_value -= 1; bar.Update_n_Show(cur_value, max_value);break; // ����������� �����
		};
	}
	E:
		// TextBox ��� ����������� �����, ��� ������ ���� ����������, ���������� �� ��������� ���, �� �� � ������� ��������
		// ����� ��� ���������� �����������
		// ������ 10? :D
		// ��� �����
		// ���-�� ���������
		// count ��� ���������� �������� ��� ���� ��������
		// ��� �� �������, ������ ��������������� ���������� ��� ������ ������� - ���� �������, �� ������� � ����� �������
		// cin � ������ ���
		// memory �� �� ��� � timer, ������ ��� �������� ���-�� ������
		// � ��������� cui - progressBar
	
	// ����,
	//system("pause");
	return 0;
}

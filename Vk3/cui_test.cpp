#include "Source/Headers/cui.h"

#include <iostream>
#include <string>
#include <windows.h>

int main() {

	cui::progressBar bar(10, 10, 25, 2);
	cui::dword cur_value = 0, max_value = 50;
	// но это не всё. 4 параметр это степень расширенность информации, название не очень получилось )))
	// ща лучше покажу. то был 0 уровень, щас 1-ый. всё. Я спать пойду. Давай
	
	
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
			case	'q'	:	goto E;        // выход
			case	'd'	:	cur_value += 1; bar.Update_n_Show(cur_value, max_value); break; // сбрасывает текст
			case	'h'	:	bar.Hide(); break; // прячет бокс
			case	'm' :	cur_value -= 1; bar.Update_n_Show(cur_value, max_value);break; // увеличивает текст
		};
	}
	E:
		// TextBox его родительски класс, там только есть многоточие, сокращения до инициалов нет, тк не с именами работает
		// свитч для управления изминениями
		// почему 10? :D
		// ьтл коорд
		// кол-вл аргументо
		// count это переменная счётчика что надо ввывести
		// тот же счётчик, только приспособленный специально для вывода времени - даёшь секундв, он выводит в таком формате
		// cin в очеред ага
		// memory то же что и timer, только для перевода кол-ва памяти
		// и жемчужина cui - progressBar
	
	// аааа,
	//system("pause");
	return 0;
}

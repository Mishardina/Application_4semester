#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <signal.h>
#include <Windows.h>

struct str_info {    //структура, содержащая данные о дисках
	std::string singer;      //исполнитель
	std::string album;       //альбом
	std::string year;          //год выпуска альбома
	std::string style;          //музыкальный стиль
	std::string country;        //страна происхождения
} info;


void readFile(std::ifstream& inp, std::vector<str_info>& vect); //считывание файла в вектор структур

bool checkFileName(std::string str);  //проверка имени файла на недопустимые символы
bool checkMistake(char ch);  //проверка нажатия некорректной клавиши 

//функции сортировки по выбранному полю. Алгоритм - быстрая сортировка (Quicksort)
void QuickSortIncrSinger(int min, int max, std::vector<str_info>& str_vec);  //по возрастанию
void QuickSortDecrSinger(int min, int max, std::vector<str_info>& str_vec);  //по убыванию

//по альбомам
void QuickSortIncrAlbum(int min, int max, std::vector<str_info>& vect);
void QuickSortDecrAlbum(int min, int max, std::vector<str_info>& vect);

//по годам
void QuickSortIncrYear(int min, int max, std::vector<str_info>& vect);
void QuickSortDecrYear(int min, int max, std::vector<str_info>& vect);

//по стилям
void QuickSortIncrStyle(int min, int max, std::vector<str_info>& vect);
void QuickSortDecrStyle(int min, int max, std::vector<str_info>& vect);

//по странам
void QuickSortIncrCountry(int min, int max, std::vector<str_info>& vect);
void QuickSortDecrCountry(int min, int max, std::vector<str_info>& vect);

void writeTableHat(char pole, std::ofstream& out);  //вывод шапки таблицы

void writeFile(char pole, char napr, std::vector<str_info> str_vec, std::string inpFileName);  //запись в файл в соответствии с полем, по которому производилась сортировка и с направлением сортировки

void signal_handler(int signum); //обработчик сигнала

int main() {

	setlocale(LC_ALL, "Russian");


	std::vector<str_info> vect_info; //вектор структур
	 
	std::string inpName; //имя файла с исходными данными

	std::ifstream informFile; //файл с исходными данными

	bool isInp = true; //флаг для проверки завершения работы программы

	while (isInp == true) { //пока пользователь не нажмет N, программа не завершит работу
		bool isCorrect = false; //проверка на существование файла с исходными данными

		signal(SIGINT, signal_handler);  //обработка сигнала

		std::cout << "Программа предназначена для сортировки информации из входного файла и вывода отсортированной информации в виде таблицы в текстовый файл\n\n";
		std::cout << "В любой момент вы можете выйти из программы, нажав при вводе названия файла ctrl+c или Esc при выборе поля для сортировке или направления сортировки\n\n";
		std::cout << "Сначала введите название входного файла, желательно имеющего расширение .csv (допустимо вводить и другие), затем выберите поле для сортировки и направление сортировки, а после введите название выходного файла\n\n";
		std::cout << "Результат будет выведен в виде таблицы в файле указанного вами расширения\n\n";

		while (isCorrect == false) {

			std::cout << "Введите имя файла: ";
			std::cin >> inpName;

			while (checkFileName(inpName) == true) { //проверка на недопустимые символы
				std::cout << "Введено некорректное имя файла! Введите заново\n";
				std::cin >> inpName;
			}

			informFile.open(inpName); //попытка открыть файл

			if (!informFile.is_open()) {
				std::cout << "Указанный файл не существует или указано невереное расширение\n";
			}
			else isCorrect = true;

		}

		readFile(informFile, vect_info); //парсинг файла

		informFile.close(); //после парсинга файл больше не нужен, закрываем файл

		bool isSameFile = true; //работа с тем же файлом или нет

		while (isSameFile == true) {

			std::cout << "\nВыберите, по какому полю сортировать:\n" << "1 - по исполнителям\n" << "2 - по альбомам\n" << "3 - по годам\n" << "4 - по стилям\n" << "5 - по странам\n\n";

			char pole = _getch(); //выбор поля, по которому произвести сортировку

			while (!(pole >= '1' && pole <= '5' || pole == 27)) { //проверка введенной цифры на корректность
				pole = _getch();
			}

			char napr; //направление сортировки, 1 - по возрастанию, 2 - по убыванию

			switch (pole)
			{

			case '1':
				std::cout << "Выберите, в каком направлении сортировать:\n" << "1 - по возрастанию\n" << "2 - по убыванию\n\n";
				napr = _getch(); //выбор направления сортировки

				while (checkMistake(napr) == false) { //проверка на корректность
					napr = _getch();
				}

				if (napr == '1') { //сортировка в соответствии с направлением
					QuickSortIncrSinger(0, vect_info.size() - 1, vect_info);
				}

				else if (napr == '2') {
					QuickSortDecrSinger(0, vect_info.size() - 1, vect_info);
				}
				else if (napr == 27) raise(SIGINT);

				std::cout << "!!!СУЩЕСТВУЮЩИЕ ФАЙЛЫ ПРИ ПОВТОРНОЙ СОРТИРОВКЕ БУДУТ ПЕРЕЗАПИСАНЫ!!!\n\n";
				writeFile(pole, napr, vect_info, inpName);
				break;

			case '2':
				std::cout << "Выберите, в каком направлении сортировать:\n" << "1 - по возрастанию\n" << "2 - по убыванию\n\n";
				napr = _getch();

				while (checkMistake(napr) == false) {
					napr = _getch();
				}

				if (napr == '1') {
					QuickSortIncrAlbum(0, vect_info.size() - 1, vect_info);
				}

				else if (napr == '2') {
					QuickSortDecrAlbum(0, vect_info.size() - 1, vect_info);
				}
				else if (napr == 27) raise(SIGINT);

				std::cout << "!!!СУЩЕСТВУЮЩИЕ ФАЙЛЫ ПРИ ПОВТОРНОЙ СОРТИРОВКЕ БУДУТ ПЕРЕЗАПИСАНЫ!!!\n\n";
				writeFile(pole, napr, vect_info, inpName);
				break;

			case '3':
				std::cout << "Выберите, в каком направлении сортировать:\n" << "1 - по возрастанию\n" << "2 - по убыванию\n\n";
				napr = _getch();

				while (checkMistake(napr) == false) {
					napr = _getch();
				}

				if (napr == '1') {
					QuickSortIncrYear(0, vect_info.size() - 1, vect_info);
				}

				else if (napr == '2') {
					QuickSortDecrYear(0, vect_info.size() - 1, vect_info);
				}
				else if (napr == 27) raise(SIGINT);

				std::cout << "!!!СУЩЕСТВУЮЩИЕ ФАЙЛЫ ПРИ ПОВТОРНОЙ СОРТИРОВКЕ БУДУТ ПЕРЕЗАПИСАНЫ!!!\n\n";
				writeFile(pole, napr, vect_info, inpName);
				break;

			case '4':
				std::cout << "Выберите, в каком направлении сортировать:\n" << "1 - по возрастанию\n" << "2 - по убыванию\n\n";
				napr = _getch();
				while (checkMistake(napr) == false) {
					napr = _getch();
				}

				if (napr == '1') {
					QuickSortIncrStyle(0, vect_info.size() - 1, vect_info);
				}

				else if (napr == '2') {
					QuickSortDecrStyle(0, vect_info.size() - 1, vect_info);
				}
				else if (napr == 27) raise(SIGINT);

				std::cout << "!!!СУЩЕСТВУЮЩИЕ ФАЙЛЫ ПРИ ПОВТОРНОЙ СОРТИРОВКЕ БУДУТ ПЕРЕЗАПИСАНЫ!!!\n\n";
				writeFile(pole, napr, vect_info, inpName);
				break;

			case '5':
				std::cout << "Выберите, в каком направлении сортировать:\n" << "1 - по возрастанию\n" << "2 - по убыванию\n";
				napr = _getch();
				while (checkMistake(napr) == false) {
					napr = _getch();
				}

				if (napr == '1') {
					QuickSortIncrCountry(0, vect_info.size() - 1, vect_info);
				}

				else if (napr == '2') {
					QuickSortDecrCountry(0, vect_info.size() - 1, vect_info);
				}
				else if (napr == 27) raise(SIGINT);

				std::cout << "!!!СУЩЕСТВУЮЩИЕ ФАЙЛЫ ПРИ ПОВТОРНОЙ СОРТИРОВКЕ БУДУТ ПЕРЕЗАПИСАНЫ!!!\n\n";
				writeFile(pole, napr, vect_info, inpName);
				break;
			case 27:
				raise(SIGINT);
				break;
			}


			std::cout << "Введите Y, если хотите продолжить работу с тем же файлом, или N, если хотите завершить работу с файлом / выйти из программы\n\n";
			char ch;
			ch = _getch();

			while (!(ch == 'Y' || ch == 'y' || ch == 'N' || ch == 'n')) {
				ch = _getch();
			}

			if (ch == 'N' || ch == 'n') { //переход к другому файлу
				isSameFile = false; 
				vect_info.clear();  //очистка вектора перед прочтением другого файла
			}

		}

		std::cout << "Введите Y, если хотите ввести имя нового файла, или N, если хотите завершить работу\n\n";
		char ch;
		ch = _getch();

		while (!(ch == 'Y' || ch == 'y' || ch == 'N' || ch == 'n')) {
			ch = _getch();
		}

		if (ch == 'Y' || ch == 'y') { //завершение или продолжение работы
			system("cls");
			continue;
		}
		else if (ch == 'N' || ch == 'n') { 
			isInp = false; 
			system("cls");
		}
	}
}	  

//реализация функций, объявленных в начале

void readFile(std::ifstream& inp, std::vector<str_info>& vect) {

	while (!inp.eof()) {                     //пока не конец файла, считать строку до следующего разделителя
		getline(inp, info.singer, ';');
		getline(inp, info.album, ';');
		getline(inp, info.year, ';');
		getline(inp, info.style, ';');
		getline(inp, info.country, '\n');
		vect.push_back(info);
	}
	vect.erase(vect.end() - 1); //в конце вектора находится один лишний элемент
}

bool checkMistake(char ch) { 
	return (ch == '1' || ch == '2' || ch == 27); 
}

bool checkFileName(std::string str) {
	return !(str.find('+') == std::string::npos) || 
		!(str.find('/') == std::string::npos) || 
		!(str.find(':') == std::string::npos) || 
		!(str.find('*') == std::string::npos) || 
		!(str.find(char(92)) == std::string::npos) || 
		!(str.find('<') == std::string::npos) || 
		!(str.find('>') == std::string::npos) || 
		!(str.find('?') == std::string::npos) || 
		!(str.find('|') == std::string::npos);
}

void QuickSortIncrSinger(int min, int max, std::vector<str_info>& vect) {

	int i = min; //начальный элемент
	int j = max; //конечный элемент

	std::string x = vect[(min + max) / 2].singer; //медиана массива

	do {
		while (vect[i].singer < x) { i++; } //прибавлять i, пока элемент меньше медианы
		while (x < vect[j].singer) { j--; } //убавлять j, пока элемент больше медианы

		if (i <= j) {  
			std::swap(vect[i], vect[j]); //поменять местами элементы
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortIncrSinger(min, j, vect); //вызов рекурсии
	}

	if (i < max) {
		QuickSortIncrSinger(i, max, vect); //вызов рекурсии
	}
}

void QuickSortDecrSinger(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].singer;

	do {
		while (vect[i].singer > x) { i++; }
		while (x > vect[j].singer) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortDecrSinger(min, j, vect);
	}

	if (i < max) {
		QuickSortDecrSinger(i, max, vect);
	}
}

void QuickSortIncrAlbum(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].album;

	do {
		while (vect[i].album < x) { i++; }
		while (x < vect[j].album) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min <= j) {
		QuickSortIncrAlbum(min, j, vect);
	}

	if (i < max) {
		QuickSortIncrAlbum(i, max, vect);
	}
}

void QuickSortDecrAlbum(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].album;

	do {
		while (vect[i].album > x) { i++; }
		while (x > vect[j].album) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortDecrAlbum(min, j, vect);
	}

	if (i < max) {
		QuickSortDecrAlbum(i, max, vect);
	}
}

void QuickSortIncrYear(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].year;

	do {
		while (vect[i].year < x) { i++; }
		while (x < vect[j].year) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortIncrYear(min, j, vect);
	}

	if (i < max) {
		QuickSortIncrYear(i, max, vect);
	}
}

void QuickSortDecrYear(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].year;

	do {
		while (vect[i].year > x) { i++; }
		while (x > vect[j].year) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortDecrYear(min, j, vect);
	}

	if (i < max) {
		QuickSortDecrYear(i, max, vect);
	}
}

void QuickSortIncrStyle(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].style;

	do {
		while (vect[i].style < x) { i++; }
		while (x < vect[j].style) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortIncrStyle(min, j, vect);
	}

	if (i < max) {
		QuickSortIncrStyle(i, max, vect);
	}
}

void QuickSortDecrStyle(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].style;

	do {
		while (vect[i].style > x) { i++; }
		while (x > vect[j].style) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortDecrStyle(min, j, vect);
	}

	if (i < max) {
		QuickSortDecrStyle(i, max, vect);
	}
}

void QuickSortIncrCountry(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].country;

	do {
		while (vect[i].country < x) { i++; }
		while (x < vect[j].country) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortIncrCountry(min, j, vect);
	}

	if (i < max) {
		QuickSortIncrCountry(i, max, vect);
	}
}

void QuickSortDecrCountry(int min, int max, std::vector<str_info>& vect) {

	int i = min;
	int j = max;

	std::string x = vect[(min + max) / 2].country;

	do {
		while (vect[i].country > x) { i++; }
		while (x > vect[j].country) { j--; }

		if (i <= j) {
			std::swap(vect[i], vect[j]);
			i++;
			j--;
		}

	} while (i < j);

	if (min < j) {
		QuickSortDecrCountry(min, j, vect);
	}

	if (i < max) {
		QuickSortDecrCountry(i, max, vect);
	}
}

void writeTableHat(char pole, std::ofstream& out) {
	switch (pole)
	{
	case '1':
		out << "Исполнитель" << std::setw(29) << "Альбом" << std::setw(15) << "Год выпуска" << std::setw(31) << "Стиль" << std::setw(35) << "Страна" << '\n';
		break;

	case '2':
		out << "Альбом" << std::setw(34) << "Исполнитель" << std::setw(40) << "Год выпуска" << std::setw(30) << "Стиль" << std::setw(33) << "Страна" << '\n';
		break;

	case '3':
		out << "Год выпуска" << std::setw(29) << "Альбом" << std::setw(35) << "Исполнитель" << std::setw(35) << "Стиль" << std::setw(35) << "Страна" << '\n';
		break;

	case '4':
		out << "Стиль" << std::setw(35) << "Альбом" << std::setw(35) << "Исполнитель" << std::setw(35) << "Год выпуска" << std::setw(35) << "Страна" << '\n';
		break;

	case '5':
		out << "Страна" << std::setw(34) << "Альбом" << std::setw(35) << "Исполнитель" << std::setw(35) << "Год выпуска" << std::setw(35) << "Стиль" << '\n';
		break;
	}
}

void writeFile(char pole, char napr, std::vector<str_info> str_vec, std::string inpFileName) {
	std::string outFilePath; //название выходного файла
	std::ofstream output;

	std::cout << "Введите название файла для записи и его формат\n\n";

	std::cin >> outFilePath;

	while (checkFileName(outFilePath) == true) { //проверка на корректность
		std::cout << "Введено некорректное имя файла! Введите заново\n\n";
		std::cin >> outFilePath;
	}

	output.open(outFilePath, std::ios::trunc); //открытие на перезапись

	output << "Отсортирована информация из файла " << inpFileName << "\n\n";

	switch (pole) 
	{
	case '1':
		output << "Сортировка проводилась по исполнителям\n\n";
		switch (napr)
		{
		case '1':
			output << "Сортировка проводилась по возрастанию\n\n";
			break;

		case '2':
			output << "Сортировка проводилась по убыванию\n\n";
			break;
		}

		writeTableHat('1', output);

		for (int i = 0; i < str_vec.size(); i++) {
			output << str_vec[i].singer << std::setw(40 - str_vec[i].singer.length()) << str_vec[i].album << std::setw(15) << str_vec[i].year << std::setw(35 - str_vec[i].year.length()) << str_vec[i].style << std::setw(35) << str_vec[i].country << '\n';
		}
		break;
	case '2':
		output << "Сортировка проводилась по альбомам\n\n";

		switch (napr)
		{
		case '1':
			output << "Сортировка проводилась по возрастанию\n\n";
			break;

		case '2':
			output << "Сортировка проводилась по убыванию\n\n";
			break;
		}

		writeTableHat('2', output);

		for (int i = 0; i < str_vec.size(); i++) {
			output << str_vec[i].album << std::setw(40 - str_vec[i].album.length()) << str_vec[i].singer << std::setw(35) << str_vec[i].year << std::setw(35) << str_vec[i].style << std::setw(35) << str_vec[i].country << '\n';
		}
		break;
	case '3':
		output << "Сортировка проводилась по годам\n\n";

		switch (napr)
		{
		case '1':
			output << "Сортировка проводилась по возрастанию\n\n";
			break;

		case '2':
			output << "Сортировка проводилась по убыванию\n\n";
			break;
		}

		writeTableHat('3', output);

		for (int i = 0; i < str_vec.size(); i++) {
			output << str_vec[i].year << std::setw(40 - str_vec[i].year.length()) << str_vec[i].album << std::setw(35) << str_vec[i].singer << std::setw(35) << str_vec[i].style << std::setw(35) << str_vec[i].country << '\n';
		}
		break;
	case '4':
		output << "Сортировка проводилась по стилям\n\n";

		switch (napr)
		{
		case '1':
			output << "Сортировка проводилась по возрастанию\n\n";
			break;

		case '2':
			output << "Сортировка проводилась по убыванию\n\n";
			break;
		}

		writeTableHat('4', output);

		for (int i = 0; i < str_vec.size(); i++) {
			output << str_vec[i].style << std::setw(40 - str_vec[i].style.length()) << str_vec[i].album << std::setw(35) << str_vec[i].singer << std::setw(35) << str_vec[i].year << std::setw(35) << str_vec[i].country << '\n';
		}
		break;
	case '5':
		output << "Сортировка проводилась по странам\n\n";

		switch (napr)
		{
		case '1':
			output << "Сортировка проводилась по возрастанию\n\n";
			break;

		case '2':
			output << "Сортировка проводилась по убыванию\n\n";
			break;
		}

		writeTableHat('5', output);

		for (int i = 0; i < str_vec.size(); i++) {
			output << str_vec[i].country << std::setw(40 - str_vec[i].country.length()) << str_vec[i].album << std::setw(35) << str_vec[i].singer << std::setw(35) << str_vec[i].year << std::setw(35) << str_vec[i].style << '\n';
		}
		break;
	}

	output.close();
	std::cout << "Информация успешно отсортирована и записана в файл " << outFilePath << "!\n\n";

}

void signal_handler(int signum)  //обработчик сигнала
{

	if (signum == SIGINT){
		system("cls");
		exit(0);
	}
		

}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

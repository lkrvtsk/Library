#include "Header.h"
#include <sstream>
#include <chrono>


vector<Book> vectBook(0);
vector<User> vectUser(0);

void GoToXY(short x, short y);
void ConsoleCursorVisible(bool show, short size);
void Green();
void Cyan();
void Purpule();
void UserMenu();
void unblockUser();
void sortByYear(vector <Book>& books);
void sortByAuthorSurname(vector<Book>& books);
void searchByTitle();
void OverwriteUser();
void OverwrBooks();
void filterBooksByYear();
void filterBooksByAuthor();
void displayBooks();
bool deleteUser();
bool deleteBook();
void banUser();
bool authenticateUser();
void addBook();
bool isValidDate(int day, int month);
void adminMenu();

string Password(string password) {
	Cyan();
	password = "";
	char ch;
	bool ctrlPressed = false;
	GoToXY(58, 17);

	cout << "Введите пароль: ";

	while (true) {
		ch = _getch();
		if (ch == 13) { // 13 - код клавиши Enter
			break;
		}
		if (ch == 8 || ch == 127) { // 8 - код клавиши Backspace, 127 - код клавиши Delete
			if (password.length() > 0) {
				password.erase(password.length() - 1);
				cout << "\b \b"; // удаляем последнюю звездочку
			}
		}
		else if (ch >= 32 && ch <= 126 && ch != 0) { // игнорируем клавиши F1-F12
			if (ctrlPressed) { // проверяем, была ли нажата клавиша Ctrl
				if (ch == 'c' || ch == 'C' || ch == 'v' || ch == 'V') { // проверяем, была ли нажата клавиша C или V
					continue; // игнорируем клавиши сочетания
				}
			}
			password += ch;
			cout << '*';
		}
		else if (ch == -32) { // игнорируем коды направлений стрелок
			ch = _getch(); // считываем второй байт кода клавиши
			if (ch == 72 || ch == 75 || ch == 77 || ch == 80) { // проверяем, является ли второй байт кодом стрелки
				continue; // игнорируем стрелки
			}
		}
		else if (ch == 3 || ch == 22 || ch == 24) { // игнорируем клавиши сочетания Ctrl+C, Ctrl+V, Ctrl+X
			ctrlPressed = true;
		}
		else {
			ctrlPressed = false;
		}
	}
	return password;
}

void countDown(int seconds) {

	Cyan();
	int x = 65;
	/*GoToXY(54, 13);
	cout << "------------------------";*/
	GoToXY(65, 14);
	cout << "------------------------";
	//GoToXY(64, 14);
	//cout << "|";
	GoToXY(65, 14);
	cout << "  ";
	//GoToXY(87, 14);
	//cout << "|";

	for (int i = seconds; i > 0; --i) {

		GoToXY(x = x + 2, 14);
		cout << "  " << endl;
		GoToXY(69, 13);
		cout << "Осталось секунд: " << i << "       " << endl;

		this_thread::sleep_for(chrono::seconds(1));

	}

}

void writeToFile(const Book& book) {
	ofstream outFile("booksAdmin.txt", ios::app);
	if (!outFile) {
		cout << "Ошибка открытия файла." << endl;
		return;
	}

	if (book.isTaken) {
		outFile << book.registrNumber << "|" << book.author << "|" << book.title << "|"
			<< book.yearOfPubl << "|" << book.publisher << "|" << book.numOfPages << "|"
			<< book.lastNumber << "|" << book.isTaken << "|" << book.info.dateInD << "|"
			<< book.info.dateInM << "|" << book.info.dateOutD << "|" << book.info.dateOutM << endl;
	}

	outFile.close();
}

void ReadUserFromFile() {

	User user;
	fstream file("users.txt", fstream::in);
	if (file.is_open()) {
		while (file >> user.login >> user.password >> user.isAdmin >> user.isBanned >> user.isDept >> user.number) {
			vectUser.push_back(user);
		}
	}
	else {
		cout << "Ошибка открытия файла\n";
	}
}

void printDebtors() {
	ifstream inFile("booksAdmin.txt");
	if (!inFile.is_open()) {
		cout << "Ошибка открытия файла!" << endl;
		return;
	}
	int currentDay, currentMonth;
	do {
		cout << "Введите дату: ";
		cin >> currentDay;
		cout << "Введите месяц : ";
		cin >> currentMonth;

		if (!isValidDate(currentDay, currentMonth)) {
			cout << "Некорректная дата. Пожалуйста, введите снова." << endl;
		}
	} while (!isValidDate(currentDay, currentMonth));

	cout << "Список должников:" << endl;

	// Вывод заголовков таблицы
	cout << setw(20) << left << "Регистрационный номер"
		<< "|" << setw(20) << left << "Автор"
		<< "|" << setw(30) << left << "Название"
		<< "|" << setw(15) << left << "Год"
		<< "|" << setw(20) << left << "Издатель"
		<< "|" << setw(15) << left << "Клиент"
		<< "|" << setw(15) << left << "Кол-во страниц"
		<< "|" << setw(10) << left << "Выдана"
		<< "|" << setw(15) << left << "Дата возврата"
		<< "|" << setw(10) << left << "Дней задолженности" << "|" << endl;

	// Вывод разделительной строки
	cout << setfill('-') << setw(183) << "-" << setfill(' ') << endl;

	Book book;
	string line;
	while (getline(inFile, line)) {
		istringstream iss(line);

		getline(iss, book.registrNumber, '|');
		getline(iss, book.author, '|');
		getline(iss, book.title, '|');
		iss >> book.yearOfPubl;
		iss.ignore();
		getline(iss, book.publisher, '|');
		iss >> book.numOfPages;
		iss.ignore();
		getline(iss, book.lastNumber, '|');
		iss >> book.isTaken;
		iss.ignore();
		iss >> book.info.dateInD;
		iss.ignore();
		iss >> book.info.dateInM;
		iss.ignore();
		iss >> book.info.dateOutD;
		iss.ignore();
		iss >> book.info.dateOutM;



		int daysOverdue = 0;
		if (book.isTaken && (book.info.dateOutM < currentMonth || (book.info.dateOutM == currentMonth && book.info.dateOutD < currentDay))) {
			int daysInMonth = 31; // По умолчанию считаем, что в каждом месяце 31 день
			switch (currentMonth) {
			case 2:
				daysInMonth = 28; // Февраль - 28 дней
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				daysInMonth = 30; // Апрель, июнь, сентябрь, ноябрь - 30 дней
				break;
			}

			int daysInPreviousMonth = daysInMonth; // По умолчанию считаем, что в предыдущем месяце столько же дней, сколько в текущем месяце

			if (currentMonth != book.info.dateOutM) {
				int previousMonth = currentMonth - 1;
				if (previousMonth == 0) {
					previousMonth = 12;
				}

				switch (previousMonth) {
				case 2:
					daysInPreviousMonth = 28; // Февраль - 28 дней
					break;
				case 4:
				case 6:
				case 9:
				case 11:
					daysInPreviousMonth = 30; // Апрель, июнь, сентябрь, ноябрь - 30 дней
					break;
				}

				daysOverdue = currentDay + (daysInPreviousMonth - book.info.dateOutD) + (currentMonth - book.info.dateOutM - 1) * daysInMonth;
			}
			else {
				daysOverdue = currentDay - book.info.dateOutD;
			}
		}

		// Вывод информации о книге в таблице
		cout << setw(20) << left << book.registrNumber << "|"
			<< setw(20) << left << book.author << "|"
			<< setw(30) << left << book.title << "|"
			<< setw(15) << right << book.yearOfPubl << "|"
			<< setw(20) << left << book.publisher << "|"
			<< setw(15) << left << book.lastNumber << "|"
			<< setw(15) << right << book.numOfPages << "|"
			<< setw(10) << left << (book.isTaken ? "Да" : "Нет") << "|"
			<< setw(2) << right << book.info.dateOutD << setw(1) << "."
			<< setw(2) << right << book.info.dateOutM << setw(10) << "|"
			<< setw(10) << left << daysOverdue << "дней" << "|" << endl;
		cout << setfill('-') << setw(183) << "-" << setfill(' ') << endl;
	}

	inFile.close();
	_getch();
	system("cls");
}

bool isValidDate(int day, int month) {
	if (month < 1 || month > 12)
		return false;

	int maxDays;
	if (month == 2) {
		maxDays = 28;
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11) {
		maxDays = 30;
	}
	else {
		maxDays = 31;
	}

	if (day < 1 || day > maxDays)
		return false;

	return true;
}

void showActiveBook() {
	User user;
	cout << "Введите номер читательского билета: ";
	getline(cin, user.number);

	// Откройте файл для чтения
	std::ifstream file("booksAdmin.txt");
	if (!file.is_open()) {
		cout << "Ошибка открытия файла." << endl;
		return;
	}

	bool found = false;

	// Читаем файл построчно и ищем соответствующие записи
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);

		Book book;
		getline(iss, book.registrNumber, '|');
		getline(iss, book.author, '|');
		getline(iss, book.title, '|');
		iss >> book.yearOfPubl;
		iss.ignore();
		getline(iss >> std::ws, book.publisher, '|');


		iss >> book.numOfPages;
		iss.ignore();
		getline(iss, book.lastNumber, '|');

		iss >> book.isTaken;
		iss.ignore();

		iss >> book.info.dateInD;
		iss.ignore();

		iss >> book.info.dateInM;
		iss.ignore();

		iss >> book.info.dateOutD;
		iss.ignore();

		iss >> book.info.dateOutM;
		iss.ignore();

		// Проверяем, соответствует ли читательский билет
		if (book.lastNumber == user.number) {
			cout << "\nРегистрационный номер: " << book.registrNumber << endl;
			cout << "Автор: " << book.author << endl;
			cout << "Название: " << book.title << endl;
			cout << "Год публикации: " << book.yearOfPubl << endl;
			cout << "Издательство: " << book.publisher << endl;
			cout << "Читательский билет: " << book.lastNumber << endl;
			cout << "Количество страниц: " << book.numOfPages << endl;
			cout << "Статус: " << (book.isTaken ? "Взята на руки" : "Доступна") << endl;
			cout << "Дата выдачи: " << book.info.dateInD << "." << book.info.dateInM << endl;
			cout << "Дата сдачи: " << book.info.dateOutD << "." << book.info.dateOutM << endl;

			cout << endl;
			found = true;
		}
	}

	// Если не найдено ни одной книги
	if (!found) {
		cout << "Книги, связанные с данным читательским билетом, не найдены." << endl;
	}

	// Закрываем файл
	file.close();
	_getch();
	system("cls");
}

void lendBook() {
	int x, y;
	User user;
	Book books;
	Cyan();
	const int MAX_ATTEMPTS = 3;
	int attempts = 0;
	bool authenticated = false;
	bool isNumberExist = 0, isRegisterExist = 0;

	cout << "Введите номер читательского билета: ";
	do {
		getline(cin, user.number);

		if (user.number.size() != 6) {
			cout << "Читательский билет должен содержать 6 цифр" << endl;
		}
		else {
			isNumberExist = false;
			for (int i = 0; i < vectUser.size(); i++) {
				if (user.number == vectUser[i].number) {
					x = i;
					isNumberExist = true;
					break;
				}
			}

			if (!isNumberExist) {
				cout << "Номера не существует" << endl;
			}
		}

		if (!isNumberExist || user.number.size() != 6) {
			_getch();
			system("cls");
			cout << "Введите номер читательского билета: ";
		}
	} while (!isNumberExist || user.number.size() != 6);

	cout << "Введите регистрационный номер книги: " << endl;
	do {
		cin >> books.registrNumber;

		isRegisterExist = false;
		for (int i = 0; i < vectBook.size(); i++) {
			if (books.registrNumber == vectBook[i].registrNumber) {
				y = i;
				isRegisterExist = true;
				break;
			}
		}

		if (!isRegisterExist) {
			cout << "Номера не существует" << endl;
		}

		if (!isRegisterExist) {
			_getch();
			system("cls");
			cout << "Введите регистрационный номер книги: " << endl;
		}
	} while (!isRegisterExist);
	if (!isRegisterExist) {
		cout << "Номера не существует";
		_getch();
		system("cls");
	}
	else {

		do {
			cout << "Введите дату выдачи (день месяца): ";
			cin >> vectBook[y].info.dateInD;
			cout << "Введите месяц выдачи: ";
			cin >> vectBook[y].info.dateInM;

			if (!isValidDate(vectBook[y].info.dateInD, vectBook[y].info.dateInM)) {
				cout << "Некорректная дата. Пожалуйста, введите снова." << endl;
			}
		} while (!isValidDate(vectBook[y].info.dateInD, vectBook[y].info.dateInM));

		do {
			cout << "Введите дату сдачи (день месяца): ";
			cin >> vectBook[y].info.dateOutD;
			cout << "Введите месяц сдачи: ";
			cin >> vectBook[y].info.dateOutM;

			if (!isValidDate(vectBook[y].info.dateOutD, vectBook[y].info.dateOutM)) {
				cout << "Некорректная дата. Пожалуйста, введите снова." << endl;
			}
		} while (!isValidDate(vectBook[y].info.dateOutD, vectBook[y].info.dateOutM));
		vectUser[x].isDept = 1;
		vectBook[y].isTaken = 1;
		vectBook[y].lastNumber = vectUser[x].number;
		writeToFile(vectBook[y]);

	}
	_getch();
	system("cls");
}

void Task() {
	int year;
	cout << "Введите год: ";
	sortByAuthorSurname(vectBook);
	while (!(cin >> year) || year < 1900)
	{

		cin.clear();
		while (cin.get() != '\n'); {

			cout << "Неверный ввод. Повторите." << endl;

		}

	}

	std::cout << std::setw(5) << "ID" << '|'
		<< std::setw(20) << "Автор" << '|'
		<< std::setw(35) << "Название" << '|'
		<< std::setw(12) << "Год издания" << '|'
		<< std::setw(15) << "Издательство" << '|'
		<< std::setw(20) << "Читательский билет" << '|'
		<< std::setw(10) << "Страницы" << '|'
		<< std::setw(15) << "Статус книги" << '|' << std::endl;
	std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;

	for (int i = 0; i < vectBook.size(); i++) {
		if (vectBook[i].yearOfPubl >= year) {
			std::cout << std::setw(5) << vectBook[i].registrNumber << '|'
				<< std::setw(20) << vectBook[i].author << '|'
				<< std::setw(35) << vectBook[i].title << '|'
				<< std::setw(12) << vectBook[i].yearOfPubl << '|'
				<< std::setw(15) << vectBook[i].publisher << '|'
				<< std::setw(20) << vectBook[i].lastNumber << '|'
				<< std::setw(10) << vectBook[i].numOfPages << '|'
				<< std::setw(15) << (vectBook[i].isTaken ? "У читателя" : "В библиотеке") << '|' << std::endl;
			std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;
		}
	}
	_getch();
}

void Registration() {
	Cyan();
	User user;
	bool isExist = false;
	int att = 0;
	while (att != 3) {

		GoToXY(58, 17);
		cout << "Введите имя пользователя: ";
		getline(cin, user.login);
		system("cls");
		if (user.login.find(' ') != string::npos) {
			GoToXY(58, 17);
			cout << "Логин не должен содержать пробелы." << endl;
			_getch();
			system("cls");
			att++;
		}
		else {
			for (int i = 0; i < vectUser.size(); i++) {
				if (user.login == vectUser[i].login) {
					isExist = true;
					break;
				}
			}
			if (isExist) {
				GoToXY(58, 17);
				cout << "Пользователь с таким именем уже существует";
				att++;
				isExist = 0;

				_getch();
				system("cls");
				if (att == 3) {
					GoToXY(58, 17);
					cout << "Превышено максимальное количество попыток, доступ заблокирован на 10 секунд\n";
					countDown(10);
					att = 0;
					system("cls");

					continue;
				}
				continue;
			}
			break;
		}


	}


	int att2 = 0;
	while (att2 != 3) {
		GoToXY(58, 17);
		cout << "Введите пароль: ";
		getline(cin, user.password);
		system("cls");
		if (user.password.size() < 8) {
			GoToXY(58, 17);
			cout << "Пароль должен содержать не менее 8 символов." << endl;
			att2++;
			_getch();
			system("cls");
		}
		else {
			break;
		}
		if (att2 == 3) {
			GoToXY(58, 17);
			cout << "Превышено максимальное количество попыток, доступ заблокирован 10 секунд\n";
			countDown(10);
			system("cls");
			att2 = 0;
			continue;
		}
	}
	setlocale(LC_ALL, "Russian");


	bool isNumExist = 0;

	string number;
	while (true) {
		GoToXY(58, 17);
		cout << "Введите номер читательского билета: ";
		getline(cin, number);
		system("cls");
		bool hasDigits = false;
		for (char c : number) {
			if (!isdigit(c)) {
				hasDigits = true;
				break;
			}
		}
		if (hasDigits) {
			GoToXY(58, 17);
			cout << "Номер не должно содержать символы." << endl;
			_getch();
			system("cls");
			continue;
		}
		else {
			if (number.size() != 6) {
				GoToXY(58, 17);

				cout << "Читательский билет должен cодержать 6 цифр" << endl;
				_getch();
				system("cls");
				continue;

			}
			for (int i = 0; i < vectUser.size() && isNumExist == 0; i++) {
				if (number == vectUser[i].number) {
					GoToXY(58, 17);

					cout << "Номер читательского билета уже существует " << endl;
					isNumExist = 1;
					_getch();
					system("cls");
					break;
				}


			}
			if (!isNumExist) {
				break;
			}
			else {
				isNumExist = 0;
			}
		}

	}


	user.number = number;


	system("cls");

	user.isAdmin = false;
	user.isBanned = false;
	user.isDept = false;
	system("cls");
	vectUser.push_back(user);
	GoToXY(58, 17);
	cout << "Регистрация прошла успешно." << endl;
	_getch();
	system("cls");

}

void searchByAuthor()
{
	Cyan();
	string searchAuthor;
	bool found = false;
	GoToXY(58, 17);

	cout << "Введите автора книги, которую желаете найти: ";
	getline(cin, searchAuthor);
	system("cls");

	cout << std::setw(5) << "ID" << '|'
		<< std::setw(20) << "Автор" << '|'
		<< std::setw(35) << "Название" << '|'
		<< std::setw(12) << "Год издания" << '|'
		<< std::setw(15) << "Издательство" << '|'
		<< std::setw(10) << "Страницы" << '|' << std::endl;
	cout << std::setfill('-') << std::setw(105) << "-" << std::setfill(' ') << endl;

	for (const auto& book : vectBook)
	{
		if (book.author == searchAuthor)
		{
			found = true;
			cout << std::setw(5) << book.registrNumber << '|'
				<< std::setw(20) << book.author << '|'
				<< std::setw(35) << book.title << '|'
				<< std::setw(12) << book.yearOfPubl << '|'
				<< std::setw(15) << book.publisher << '|'
				<< std::setw(10) << book.numOfPages << '|' << std::endl;
			cout << std::setfill('-') << std::setw(105) << "-" << std::setfill(' ') << endl;
		}
	}

	if (!found)
	{
		GoToXY(58, 17);
		cout << "Книга с таким автором не найдена." << endl;
	}
}

bool authenticateUser() {
	Cyan();
	const int MAX_ATTEMPTS = 3;
	int attempts = 0;
	bool authenticated = false;

	// Read user data from file

	string username;
	GoToXY(58, 17);
	cout << "Введите логин: ";
	getline(cin, username);
	system("cls");
	auto user = find_if(vectUser.begin(), vectUser.end(), [username](const User& u) { return u.login == username; });
	if (user == vectUser.end()) {
		GoToXY(58, 17);
		cout << "Логин неверный. Аутентификация невозможна." << endl;
		GoToXY(58, 18);
		cout << "Повторите ввод или обратитесь к администратору для регистрации." << endl;
		_getch();
		system("cls");
		return authenticated;
	}

	if (user->isBanned) {
		GoToXY(58, 17);
		cout << "Ваш аккаунт заблокирован." << endl;
		_getch();
		system("cls");
		return authenticated;
	}

	if (user->isAdmin) {
		while (attempts < MAX_ATTEMPTS) {
			string password;
			GoToXY(58, 17);
			cout << "Введите пароль: ";
			password = Password(password);
			system("cls");
			if (user->password == password) {
				if (user->isBanned == 0) {
					GoToXY(58, 17);
					cout << "Вы вошли как администратор." << endl;
					authenticated = true;
					_getch();
					system("cls");
					adminMenu();

					break;
				}
				else {
					GoToXY(58, 17);
					cout << "Ваш аккаунт заблокирован." << endl;
					_getch();
					system("cls");
					return authenticated;
				}
			}
			else {
				attempts++;
				GoToXY(58, 17);

				cout << "Пароль неверный. Пожалуйста, повторите попытку." << endl;
				_getch();
				system("cls");
			}
			if (attempts == MAX_ATTEMPTS) {
				GoToXY(58, 17);
				cout << "Превышено максимальное количество попыток,доступ заблокирован на 10 секунд\n";
				countDown(10);
				attempts = 0;
				system("cls");
				continue;
			}
			
		}

	}

	while (attempts < MAX_ATTEMPTS) {
		string password;
		password = Password(password);

		if (user->password == password) {
			if (user->isBanned == 0) {
				GoToXY(58, 17);
				cout << "Аутентификация успешна. Вы вошли как пользователь" << endl;
				_getch();
				system("cls");
				UserMenu();

				authenticated = true;
				break;
			}
			else {
				GoToXY(58, 17);
				cout << "Ваш аккаунт заблокирован." << endl;
				return authenticated;
			}
		}
		else {
			attempts++;
			GoToXY(58, 17);
			cout << "Пароль неверный. Пожалуйста, повторите попытку." << endl;
			_getch();
			system("cls");


		}
		if (attempts == MAX_ATTEMPTS) {
			GoToXY(58, 17);
			cout << "Превышено максимальное количество попыток, доступ заблокирован на 10 секунд\n";
			countDown(10);
			attempts = 0;
			system("cls");
			continue;
		}
	}


	return authenticated;
}

void mainMenu() {
	Cyan();
	GoToXY(58, 17);
	cout << "Здравствуйте!";
	GoToXY(58, 18);
	cout << "Приветсвуем Вас в нашей библиотеке.";
	GoToXY(58, 19);
	cout << "Чтобы продолжить работу, Вам необходимо войти в аккаунт";
	GoToXY(58, 20);
	cout << "или зарегистрировать новый аккаунт.";
	_getch();
	system("cls");
	int y = 0;
	string menu[] = { "Авторизация","Просмотреть каталог книг","Завершить программу" };



	int active_menu = 0; char ch;
	while (true) {
		int x = 65, y = 17;
		GoToXY(x, y);
		for (int i = 0; i < size(menu); i++) {
			if (i == active_menu)Green();
			else Purpule();
			GoToXY(x, y++);
			cout << menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32)ch = _getch();
		switch (ch)
		{
		case UP:
			if (active_menu > 0)
				--active_menu;
			break;
		case DOWN:
			if (active_menu < size(menu) - 1)
				++active_menu;
			break;

		case ENTER:

			switch (active_menu)
			{

			case 0:
				system("cls");

				authenticateUser();

				break;
			case 1:
				system("cls");

				displayBooks();
				_getch();
				system("cls");
				break;
			case 2:
				system("cls");
				OverwriteUser();
				OverwrBooks();
				exit(0);
			}

		default:
			break;
		}
	}

}

void sortMenu() {
	int y = 0;
	string menu[] = { "Сортировать книги по фамилии автора","Сортировать книги по году","Назад" };



	int active_menu = 0; char ch;
	while (true) {
		int x = 65, y = 17;
		GoToXY(x, y);
		for (int i = 0; i < size(menu); i++) {
			if (i == active_menu)Green();
			else Purpule();
			GoToXY(x, y++);
			cout << menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32)ch = _getch();
		switch (ch)
		{
		case UP:
			if (active_menu > 0)
				--active_menu;
			break;
		case DOWN:
			if (active_menu < size(menu) - 1)
				++active_menu;
			break;
		case ESC:
			exit(0);
		case ENTER:

			switch (active_menu)
			{
			case 0:
				system("cls");

				sortByAuthorSurname(vectBook);
				displayBooks();
				_getch();
				system("cls");
				break;
			case 1:
				system("cls");
				sortByYear(vectBook);
				displayBooks();
				_getch();
				system("cls");
				break;
			case 2:
				system("cls");
				UserMenu();
			}
		default:
			break;
		}
	}
}

void sortMenu2() {
	int y = 0;
	string menu[] = { "Сортировать книги по фамилии автора","Сортировать книги по году","Назад" };



	int active_menu = 0; char ch;
	while (true) {
		int x = 65, y = 17;
		GoToXY(x, y);
		for (int i = 0; i < size(menu); i++) {
			if (i == active_menu)Green();
			else Purpule();
			GoToXY(x, y++);
			cout << menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32)ch = _getch();
		switch (ch)
		{
		case UP:
			if (active_menu > 0)
				--active_menu;
			break;
		case DOWN:
			if (active_menu < size(menu) - 1)
				++active_menu;
			break;
		case ESC:
			exit(0);
		case ENTER:

			switch (active_menu)
			{
			case 0:
				system("cls");

				sortByAuthorSurname(vectBook);
				displayBooks();
				_getch();
				system("cls");
				break;
			case 1:
				system("cls");
				sortByYear(vectBook);
				displayBooks();
				_getch();
				system("cls");
				break;
			case 2:
				system("cls");
				adminMenu();
			}
		default:
			break;
		}
	}
}

void filtrMenu() {
	int y = 0;
	string menu[] = { "Найти книгу по названию","Найти книгу по фамилии автора","Фильтровать книги по автору","Фильтровать книги году","Назад" };



	int active_menu = 0; char ch;
	while (true) {
		int x = 58, y = 17;
		GoToXY(x, y);
		for (int i = 0; i < size(menu); i++) {
			if (i == active_menu)Green();
			else Purpule();
			GoToXY(x, y++);
			cout << menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32)ch = _getch();
		switch (ch)
		{
		case UP:
			if (active_menu > 0)
				--active_menu;
			break;
		case DOWN:
			if (active_menu < size(menu) - 1)
				++active_menu;
			break;
		case ESC:
			exit(0);
		case ENTER:

			switch (active_menu)
			{
			case 0:
				system("cls");

				searchByTitle();
				_getch();
				system("cls");
				break;

			case 1:
				system("cls");

				searchByAuthor();
				_getch();
				system("cls");
				break;

			case 2:
				system("cls");

				filterBooksByAuthor();
				_getch();
				system("cls");
				break;
			case 3:
				system("cls");

				filterBooksByYear();
				_getch();
				system("cls");
				break;

			case 4:
				system("cls");
				UserMenu();
			}





		default:
			break;
		}
	}
}

void filtrMenu2() {
	int y = 0;
	string menu[] = { "Найти книгу по названию","Фильтровать книги по автору","Фильтровать книги году","Назад" };



	int active_menu = 0; char ch;
	while (true) {
		int x = 58, y = 17;
		GoToXY(x, y);
		for (int i = 0; i < size(menu); i++) {
			if (i == active_menu)Green();
			else Purpule();
			GoToXY(x, y++);
			cout << menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32)ch = _getch();
		switch (ch)
		{
		case UP:
			if (active_menu > 0)
				--active_menu;
			break;
		case DOWN:
			if (active_menu < size(menu) - 1)
				++active_menu;
			break;
		case ESC:
			exit(0);
		case ENTER:

			switch (active_menu)
			{
			case 0:
				system("cls");

				searchByTitle();
				_getch();
				system("cls");
				break;
			case 1:
				system("cls");

				filterBooksByAuthor();
				_getch();
				system("cls");
				break;
			case 2:
				system("cls");

				filterBooksByYear();
				_getch();
				system("cls");
				break;

			case 3:
				system("cls");
				adminMenu();
			}





		default:
			break;
		}
	}
}

void filterBooksByYear()
{

	int min_year, max_year;
	GoToXY(58, 17);

	cout << "Введите минимальный год издания: ";
	cin >> min_year;
	GoToXY(58, 18);

	cout << "Введите максимальный год издания: ";
	cin >> max_year;
	system("cls");
	std::cout << std::setw(5) << "ID" << '|'
		<< std::setw(20) << "Автор" << '|'
		<< std::setw(35) << "Название" << '|'
		<< std::setw(12) << "Год издания" << '|'
		<< std::setw(15) << "Издательство" << '|'
		<< std::setw(20) << "Читательский билет" << '|'
		<< std::setw(10) << "Страницы" << '|'
		<< std::setw(15) << "Статус книги" << '|' << std::endl;
	std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;

	for (int i = 0; i < vectBook.size(); i++) {
		if (vectBook[i].yearOfPubl >= min_year && vectBook[i].yearOfPubl <= max_year) {
			std::cout << std::setw(5) << vectBook[i].registrNumber << '|'
				<< std::setw(20) << vectBook[i].author << '|'
				<< std::setw(35) << vectBook[i].title << '|'
				<< std::setw(12) << vectBook[i].yearOfPubl << '|'
				<< std::setw(15) << vectBook[i].publisher << '|'
				<< std::setw(20) << vectBook[i].lastNumber << '|'
				<< std::setw(10) << vectBook[i].numOfPages << '|'
				<< std::setw(15) << (vectBook[i].isTaken ? "У читателя" : "В библиотеке") << '|' << std::endl;
			std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;
		}
	}







}

void filterBooksByAuthor() {
	char startChar, endChar;
	GoToXY(58, 17);
	std::cout << "Введите начальную букву: ";
	std::cin >> startChar;
	GoToXY(58, 18);
	std::cout << "Введите конечную букву: ";
	std::cin >> endChar;

	system("cls");

	vector<Book> result(0);

	for (const Book& book : vectBook) {
		if (!book.author.empty() && book.author[0] >= startChar && book.author[0] <= endChar) {
			result.push_back(book);
		}
	}
	sortByAuthorSurname(result);
	std::cout << std::setw(5) << "ID" << '|'
		<< std::setw(20) << "Автор" << '|'
		<< std::setw(35) << "Название" << '|'
		<< std::setw(12) << "Год издания" << '|'
		<< std::setw(15) << "Издательство" << '|'
		<< std::setw(20) << "Читательский билет" << '|'
		<< std::setw(10) << "Страницы" << '|'
		<< std::setw(15) << "Статус книги" << '|' << std::endl;
	std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;
	for (const auto& book : result) {
		std::cout << std::setw(5) << book.registrNumber << '|'
			<< std::setw(20) << book.author << '|'
			<< std::setw(35) << book.title << '|'
			<< std::setw(12) << book.yearOfPubl << '|'
			<< std::setw(15) << book.publisher << '|'
			<< std::setw(20) << book.lastNumber << '|'
			<< std::setw(10) << book.numOfPages << '|'
			<< std::setw(15) << (book.isTaken ? "У читателя" : "В библиотеке") << '|' << std::endl;
		std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;

	}

}

void adminMenu() {
	int y = 0;
	string menu[] = { "Зарегистрировать пользователя","Заблокировать пользователя","Разблокировать пользователя","Удалить пользователя","Добавить книгу в каталог", "Удалить книгу из каталога","Вывести список должников","Выдать книгу","Сортировать книги","Фильтровать книги","Выйти из аккаунта" };

	int active_menu = 0; char ch;
	while (true) {
		int x = 65, y = 17;
		GoToXY(x, y);
		for (int i = 0; i < size(menu); i++) {
			if (i == active_menu)Green();
			else Purpule();
			GoToXY(x, y++);
			cout << menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32)ch = _getch();
		switch (ch)
		{
		case UP:
			if (active_menu > 0)
				--active_menu;
			break;
		case DOWN:
			if (active_menu < size(menu) - 1)
				++active_menu;
			break;
		case ESC:
			exit(0);
		case ENTER:

			switch (active_menu)
			{
			case 0:
				system("cls");
				Registration();
				break;
			case 1:
				system("cls");

				banUser();

				break;
			case 2:
				system("cls");

				unblockUser();

				break;
			case 3:
				system("cls");

				deleteUser();

				break;
			case 4:
				system("cls");

				addBook();

				break;
			case 5:
				system("cls");

				deleteBook();

				break;
			case 6:
				system("cls");

				printDebtors();
				break;
			case 7:
				system("cls");
				lendBook();
				break;
			case 8:
				system("cls");
				sortMenu2();
				break;
			case 9:
				system("cls");
				filtrMenu2();
				break;
			case 10:
				system("cls");
				mainMenu();
			}

		default:
			break;
		}
	}
}

void UserMenu() {
	int y = 0;
	string menu[] = { "Просмотреть каталог книг","Отсортировать книгии по какому-либо параметру","Отфильтровать книги по какому-либо параметру","Выполнение задачи", "Просмотреть свои книги","Выйти из аккаунта" };



	int active_menu = 0; char ch;
	while (true) {
		int x = 58, y = 17;
		GoToXY(x, y);
		for (int i = 0; i < size(menu); i++) {
			if (i == active_menu)Green();
			else Purpule();
			GoToXY(x, y++);
			cout << menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32)ch = _getch();
		switch (ch)
		{
		case UP:
			if (active_menu > 0)
				--active_menu;
			break;
		case DOWN:
			if (active_menu < size(menu) - 1)
				++active_menu;
			break;
		case ESC:
			exit(0);
		case ENTER:

			switch (active_menu)
			{
			case 0:
				system("cls");

				displayBooks();
				_getch();
				system("cls");
				break;
			case 1:
				system("cls");

				sortMenu();
				_getch();
				system("cls");
				break;
			case 2:
				system("cls");

				filtrMenu();
				_getch();
				system("cls");
				break;
			case 3:
				system("cls");
                Task();

				_getch();
				
				system("cls");
				break;
			case 4:
				system("cls");
                showActiveBook();
				_getch();
				
				system("cls");
				break;

			case 5:
				system("cls");
				mainMenu();
			}





		default:
			break;
		}
	}

}

void OverwriteUser() {
	fstream inf;

	inf.open("users.txt", fstream::out);
	for (int i = 0; i < vectUser.size(); i++) {
		if (i != 0)
			inf << endl;
		inf << vectUser[i].login << " " << vectUser[i].password
			<< " " << vectUser[i].isAdmin << " "
			<< vectUser[i].isBanned << " " << vectUser[i].isDept << " " << vectUser[i].number;
	}
	inf.close();
}

void banUser() {
	string username;
	GoToXY(58, 17);
	cout << "Введите пользователя которого хотите заблокировать: ";
	cin >> username;


	ifstream file("users.txt");

	bool userFound = false;

	// Поиск пользователя и установка флага isBanned в true
	for (auto& user : vectUser) {
		if (user.login == username) {
			userFound = true;
			user.isBanned = true;

			// Запись измененных данных в файл
			ofstream file("users.txt");
			for (const auto& u : vectUser) {
				file << u.login << " " << u.password << " " << u.isAdmin << " " << u.isBanned << " " << u.isDept << endl;
			}
			file.close();

			break;
		}
	}

	// Вывод сообщения об ошибке, если пользователь не найден
	if (!userFound) {
		GoToXY(58, 17);
		cout << "Пользователь не найден." << endl;

		_getch();
		system("cls");
	}
	else {
		GoToXY(58, 17);
		cout << "Пользователь " << username << " заблокирован." << endl;
		_getch();
		system("cls");

	}
}

void unblockUser() {
	{
		string username;
		GoToXY(58, 17);
		cout << "Введите пользователя которого хотите разблокировать: ";
		getline(cin, username);

		_getch();
		system("cls");
		ifstream file("users.txt");


		bool userFound = false;

		// Поиск пользователя и установка флага isBanned в true
		for (auto& user : vectUser) {
			if (user.login == username) {
				userFound = true;
				user.isBanned = false;

				// Запись измененных данных в файл
				ofstream file("users.txt");
				for (const auto& u : vectUser) {
					file << u.login << " " << u.password << " " << u.isAdmin << " " << u.isBanned << " " << u.isDept << "" << u.number << endl;
				}
				file.close();

				break;
			}
		}

		// Вывод сообщения об ошибке, если пользователь не найден
		if (!userFound) {
			GoToXY(58, 17);
			cout << "Пользователь не найден." << endl;
			_getch();
			system("cls");
		}
		else {
			GoToXY(58, 17);
			cout << "Пользователь " << username << " разблокирован." << endl;
			_getch();
			system("cls");

		}
	}
}

bool deleteUser() {
	string username;
	GoToXY(58, 17);
	cout << "Введите пользователя которого хотите удалить:";

	getline(cin, username);
	// Ищем пользователя с заданным логином
	auto it = find_if(vectUser.begin(), vectUser.end(), [username](const User& user) { return user.login == username; });
	if (it == vectUser.end()) {
		GoToXY(58, 17);
		cout << "Пользователь не найден" << endl;
		_getch();
		system("cls");
		return false;
	}

	// Удаляем пользователя из вектора
	vectUser.erase(it);
	GoToXY(58, 17);
	cout << "Пользователь успешно удален" << endl;
	_getch();
	system("cls");
	return true;
}

void ReadBooksFromFile() {
	Book books;
	ifstream inFile("books.txt");
	if (!inFile.is_open()) {
		cout << "Ошибка открытия файла!" << endl;

	}

	string line;




	int i = 0;
	while (getline(inFile, line)) {
		getline(inFile, books.registrNumber, '|');

		//vectBook[i].registrNumber = books.registrNumber;
		getline(inFile, books.author, '|');
		//vectBook[i].author = books.author;
		getline(inFile, books.title, '|');
		//vectBook[i].title = books.title;
		inFile >> books.yearOfPubl;
		inFile.ignore();
		//vectBook[i].yearOfPubl = books.yearOfPubl;
		getline(inFile, books.publisher, '|');
		//vectBook[i].publisher = books.publisher;



		inFile >> books.numOfPages;
		inFile.ignore();
		getline(inFile, books.lastNumber, '|');
		//vectBook[i].lastNumber = books.lastNumber;
		inFile >> books.isTaken;
		//vectBook[i].isTaken = books.isTaken;
		vectBook.push_back(books);
	}

	inFile.close();
}

void OverwrBooks() {
	fstream inf;

	inf.open("books.txt", fstream::out);
	for (int i = 0; i < vectBook.size(); i++) {
		if (i != 0)
			inf << endl;
		inf << vectBook[i].registrNumber << "|" << vectBook[i].author
			<< "|" << vectBook[i].title << "|"
			<< vectBook[i].yearOfPubl << "|" << vectBook[i].publisher << "|" <<
			vectBook[i].numOfPages << "|" << vectBook[i].lastNumber << "|" << vectBook[i].isTaken;
	}
	inf.close();
}

void addBook() {
	Book books;
	bool isNumExist = 0;

	setlocale(LC_ALL, "Rus");

	string number;
	while (true) {
		GoToXY(58, 17);
		cout << "Введите регистрационный номер книги: ";
		GoToXY(58, 18);
		getline(cin, number);
		system("cls");
		bool hasDigits = false;
		for (char c : number) {
			if (!isdigit(c)) {
				hasDigits = true;
				break;
			}
		}
		if (hasDigits) {
			GoToXY(58, 17);
			cout << "Номер не должен содержать символы." << endl;
			_getch();
			system("cls");
			continue;
		}
		else if (number.size() != 4) {
			GoToXY(58, 17);
			cout << "Номер  должен содержать 4 цифры." << endl;
			_getch();
			system("cls");
			continue;
		}
		for (int i = 0; i < vectBook.size(); i++) {
			if (number == vectBook[i].registrNumber) {
				isNumExist = true;
				break;
			}
		}
		if (isNumExist) {
			GoToXY(58, 17);
			cout << "Номер  существует" << endl;
			isNumExist = false;
			_getch();
			system("cls");
			continue;
		}
		else {
			books.registrNumber = number;
			break;
		}
	}

	string FIO;
	while (true) {
		GoToXY(58, 17);
		cout << "Введите ФИО автора книги: ";
		GoToXY(58, 18);
		getline(cin, FIO);
		system("cls");
		bool hasDigits = false;
		for (char c : FIO) {
			if (isdigit(c)) {
				hasDigits = true;
				break;
			}
		}
		if (hasDigits) {
			GoToXY(58, 17);
			cout << "ФИО не должно содержать цифры." << endl;
			_getch();
			system("cls");
			continue;
		}
		else if (FIO.size() == 0) {
			GoToXY(58, 17);
			cout << "Название не должно быть пустым" << endl;
			_getch();
			system("cls");
			continue;
		}
		else {
			books.author = FIO;
			break;
		}
	}


	string name;
	while (true) {
		GoToXY(58, 17);
		cout << "Введите Название: ";
		GoToXY(58, 18);
		getline(cin, name);
		system("cls");
		bool hasDigits = false;
		for (char c : name) {
			if (isdigit(c)) {
				hasDigits = true;
				break;
			}
		}
		if (hasDigits) {
			GoToXY(58, 17);
			cout << "Название не должно содержать цифры." << endl;
			_getch();
			system("cls");
			continue;
		}
		else if (name.size() == 0) {
			GoToXY(58, 17);
			cout << "Название не должно быть пустым" << endl;
			_getch();
			system("cls");
			continue;
		}
		else {
			books.title = name;
			break;
		}
	}

	GoToXY(58, 17);
	cout << "Введите год публикации:" << endl;
	GoToXY(58, 18);
	while (!(cin >> books.yearOfPubl) || (books.yearOfPubl < 1900 || books.yearOfPubl>2023))
	{
		cin.clear();
		while (cin.get() != '\n'); {
			GoToXY(58, 19);
			cout << "Неверный ввод. Повторите." << endl;
			_getch();
			system("cls");
		}
		GoToXY(58, 17);
		cout << "Введите год публикации:" << endl;
		GoToXY(58, 18);
	}


	system("cls");
	getchar();
	string izd;
	while (true) {
		GoToXY(58, 17);
		cout << "Введите издательство книги:";
		GoToXY(58, 18);
		getline(cin, izd);
		system("cls");
		bool hasDigits = false;
		for (char c : izd) {
			if (isdigit(c)) {
				hasDigits = true;
				break;
			}
		}
		if (hasDigits) {
			GoToXY(58, 17);
			cout << "Издательство не должно содержать цифры." << endl;
			_getch();
			system("cls");
			continue;
		}
		else if (izd.size() == 0) {
			GoToXY(58, 17);
			cout << "Издательство не должно быть пустым" << endl;
			_getch();
			system("cls");
			continue;
		}
		else {
			books.publisher = izd;
			break;
		}
	}


	GoToXY(58, 17);
	cout << "Введите количество страниц:" << endl;
	GoToXY(58, 18);
	while (!(cin >> books.numOfPages) || books.numOfPages < 2)
	{
		cin.clear();
		while (cin.get() != '\n'); {
			GoToXY(58, 19);
			cout << "Неверный ввод. Повторите." << endl;
			_getch();
			system("cls");

		}
		GoToXY(58, 17);
		cout << "Введите количество страниц:" << endl;
		GoToXY(58, 18);

	}
	books.lastNumber = '0';
	books.isTaken = 0;
	fstream infile("books.txt", ios::app);
	if (!infile.is_open()) {
		GoToXY(58, 17);
		cout << "Ошибка открытия файла";
		return;
	}
	else {
		infile << endl << books.registrNumber << "|" << books.author << "|" << books.title << "|"
			<< books.yearOfPubl << "|" << books.publisher << "|" << books.numOfPages << "|" << books.lastNumber << "|" << books.isTaken << endl;
	}

	infile.close();
	_getch();
	system("cls");
	vectBook.push_back(books);
}

bool deleteBook() {
	std::string registerNumber;
	GoToXY(58, 17);
	std::cout << "Введите номер книги, которую хотите удалить: ";
	GoToXY(58, 18);
	std::cin >> registerNumber;

	auto it = std::find_if(vectBook.begin(), vectBook.end(), [&](const Book& book) {
		return book.registrNumber == registerNumber;
		});

	if (it != vectBook.end()) {
		vectBook.erase(it);

		GoToXY(58, 19);
		std::cout << "Книга успешно удалена" << std::endl;
		_getch();
		system("cls");
		return true;
	}

	GoToXY(58, 19);
	std::cout << "Книга не найдена" << std::endl;
	_getch();
	system("cls");
	return false;
}

void searchByTitle()
{
	Cyan();
	string searchTitle;
	bool found = false;
	GoToXY(58, 17);

	cout << "Введите название книги, которую желаете найти: ";
	getline(cin, searchTitle);
	system("cls");

	cout << std::setw(5) << "ID" << '|'
		<< std::setw(20) << "Автор" << '|'
		<< std::setw(35) << "Название" << '|'
		<< std::setw(12) << "Год издания" << '|'
		<< std::setw(15) << "Издательство" << '|'
		<< std::setw(10) << "Страницы" << '|' << std::endl;
	cout << std::setfill('-') << std::setw(129) << "-" << std::setfill(' ') << endl;

	for (const auto& book : vectBook)
	{
		if (book.title == searchTitle)
		{
			found = true;
			cout << std::setw(5) << book.registrNumber << '|'
				<< std::setw(20) << book.author << '|'
				<< std::setw(35) << book.title << '|'
				<< std::setw(12) << book.yearOfPubl << '|'
				<< std::setw(15) << book.publisher << '|'
				<< std::setw(10) << book.numOfPages << '|' << std::endl;
			cout << std::setfill('-') << std::setw(129) << "-" << std::setfill(' ') << endl;
		}
	}

	if (!found)
	{
		GoToXY(58, 17);
		cout << "Книга с таким названием не найдена." << endl;
	}
}

void displayBooks() {

	std::cout << std::setw(5) << "ID" << '|'
		<< std::setw(20) << "Автор" << '|'
		<< std::setw(35) << "Название" << '|'
		<< std::setw(12) << "Год издания" << '|'
		<< std::setw(15) << "Издательство" << '|'
		<< std::setw(20) << "Читательский билет" << '|'
		<< std::setw(10) << "Страницы" << '|'
		<< std::setw(15) << "Статус книги" << '|' << std::endl;
	std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;
	for (const auto& book : vectBook) {
		std::cout << std::setw(5) << book.registrNumber << '|'
			<< std::setw(20) << book.author << '|'
			<< std::setw(35) << book.title << '|'
			<< std::setw(12) << book.yearOfPubl << '|'
			<< std::setw(15) << book.publisher << '|'
			<< std::setw(20) << book.lastNumber << '|'
			<< std::setw(10) << book.numOfPages << '|'
			<< std::setw(15) << (book.isTaken ? "У читателя" : "В библиотеке") << '|' << std::endl;
		std::cout << std::setfill('-') << std::setw(139) << "-" << std::setfill(' ') << endl;

	}
}

void sortByAuthorSurname(vector<Book>& books) {
	std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
		return a.author < b.author;
		});
}

void sortByYear(vector <Book>& books) {

	std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
		return a.yearOfPubl < b.yearOfPubl;
		});

}



//int main() {
//	SetConsoleCP(1251);
//	SetConsoleOutputCP(1251);
//	ReadBooksFromFile();
//	ReadUserFromFile();
//	Registration();
//	 // banUser();
//	//Registration();
//	//authenticateUser();
//   //addBook();
//   //deleteBook();
//	//sortByAuthorSurname(vectBook);
//	//displayBooks();
//
//
//   //unblockUser();
//	//deleteUser();
//	//displayDept();
//	//OverwriteUser();
//	//OverwrBooks();
//}
void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ReadBooksFromFile();
	ReadUserFromFile();
	//filterBooksByAuthor();
	mainMenu();
	//lendBook();
	//printDebtors();
	//filterBooksByYear();
	//showActiveBook();
	//	Task();
}


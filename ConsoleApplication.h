#pragma once
#include "Table.h"
#include <string>
#include <iostream>

class ConsoleApplication {

private:
	Table table;

	bool hasChanged = false;
	bool isSaved = false;


public:
	void startUp();
};

vector<string> split(const string& input, const char& symbol);

void ConsoleApplication::startUp() {

	size_t rows, cols;
	std::cout << "Enter table size:" << std::endl;
	std::cout << "rows: ";
	std::cin >> rows;
	std::cout << "cols: ";
	std::cin >> cols;
	std::cin.ignore();

	this->table.setRows(rows);
	this->table.setCols(cols);

	while (true) {

		std::cin.clear();
		std::cout << "Type function (type 'help' for a list of functions):" << std::endl;
		string input;
		std::getline(std::cin, input);

		vector<string> arguments;
		arguments = split(input, ' ');

		string command = arguments[0];

		if (command == "help") {
				std::cout << "1. SET <address name> <expression> change in the value of the address cell" << std::endl;
				std::cout << "2. PRINT VAL <address> prints the value of the cell on the current address " << std::endl;
				std::cout << "3. PRINT EXPR <address> prints the expression of the cell on the current address" << std::endl;
				std::cout << "4. PRINT VAL ALL prints the value of all cells in table " << std::endl;
				std::cout << "5. PRINT EXPR ALL prints the expressions of all cells in table " << std::endl;
				std::cout << "6. SAVE <file name>" << std::endl;
				std::cout << "7. LOAD <file name>" << std::endl;
				std::cout << "8. ++ <address> if the expression of the current address represents an integer, increase by one" << std::endl;
				std::cout << "9. -- <address> if the expression of the current address represents an integer, reduce by one" << std::endl;
				std::cout << "SUM <address1> <address2> prints sum of the cells in this range" << std::endl;
				std::cout << "COUNT <address1> <address2> prints cout not empty cells in this range" << std::endl;
				std::cout << "EXIT - save table in file and program stop working" << std::endl;
		}
		else if (command == "SET") {
			try {
				if (arguments.size() == 3) {
					string address = arguments[1];
					string expression = arguments[2];

					this->table.set(address, expression);
					std::cout << "The " << expression << " at address " << address << " was set successfully!" << std::endl;
					this->hasChanged = true;

				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}
		else if (command == "PRINT" && arguments[1] == "VAL" && arguments[2] == "ALL") {

			try {
				if (arguments.size() == 3) {
					this->table.printValueAll();
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}

		else if (command == "PRINT" && arguments[1] == "EXPR" && arguments[2] == "ALL") {
			try {
				if (arguments.size() == 3) {
					this->table.printExpressionAll();
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}



		else if (command == "PRINT" && arguments[1] == "VAL") {
			try {
				if (arguments.size() == 3) {
					string address = arguments[2];
					std::cout<<this->table.printValue(address);
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}

		else if (command == "PRINT" && arguments[1] == "EXPR") {
			try {
				if (arguments.size() == 3) {
					string address = arguments[2];
					std::cout<<this->table.printExpression(address);
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}

		

		else if (command == "SAVE") {

			try {
				if (arguments.size() == 2) {
					string fileName = arguments[1];

					this->table.save(fileName);

					this->hasChanged = false;
					this->isSaved = true;
					std::cout << "Table was saved successfully!" << std::endl;
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}

		}

		else if (command == "LOAD") {

			try {
				if (arguments.size() == 2) {
					string fileName = arguments[1];

					this->table.load(fileName);
					this->hasChanged = false;
					this->isSaved = false;
					std::cout << "Table was loaded successfully!" << std::endl;
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}

		else if (command == "++") {

			try {
				if (arguments.size() == 2) {
					string address = arguments[1];

					this->table.increaseByOne(address);
					this->hasChanged = false;
					this->isSaved = false;


				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}

		else if (command == "--") {

			try {
				if (arguments.size() == 2) {
					string address = arguments[1];

					this->table.reduceByOne(address);
					this->hasChanged = false;
					this->isSaved = false;
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}

		else if (command == "SUM") {

			try {
				if (arguments.size() == 3) {
					string address1 = arguments[1];
					string address2 = arguments[2];

					std::cout << this->table.sum(address1, address2) << std::endl;
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}
		else if (command == "COUNT") {

			try {
				if (arguments.size() == 3) {
					string address1 = arguments[1];
					string address2 = arguments[2];

					std::cout << this->table.count(address1, address2) << std::endl;
				}
				else {
					throw std::invalid_argument("Invalid number of arguments!");
				}
			}
			catch (std::invalid_argument ex) {
				std::cerr << ex.what() << std::endl;
				continue;
			}
		}
		else if (command == "exit") {
		if (hasChanged == true) {
			std::cout << "Enter file name to save it:" << std::endl;
			std::string fileName;
			std::cin >> fileName;

			this->table.save(fileName);

			this->hasChanged = false;
			this->isSaved = true;

			std::cout << "Table is saved!" << std::endl;
			}
		break;

		}
		else {
		std::cout << "Invalid function name!" << std::endl;
		}
	}
}

vector<string> split(const string& input, const char& symbol) {

	vector<string> arguments;

	int i = 0;
	int countSpaces = 0;

	for (; i < input.size(); ++i) {
		if (input[i] == ' ') {
			while (input[i] == ' ')
			{
				++i;
			}
			countSpaces++;
		}
	}

	int j = 0;
	for (int p = 0; p < countSpaces + 1; p++) {
		string currString;

		while (input[j] != symbol && j < input.size()) {

			currString.push_back(input[j]);
			j++;
		}
		arguments.push_back(currString);

		while (input[j] == ' ')
		{
			j++;

		}
	}
	return arguments;
}
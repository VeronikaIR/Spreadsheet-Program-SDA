#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include "Calculator.h"

#include "HashTable.h"

using std::string;
using std::pair;
using std::stack;
using std::unordered_set;

pair<int, int> absoluteAddressToCoordinates(const string& adress);
string coordinatesToAbsoluteAddress(const int& i, const int& j);
string typeOfCell(const string& data);
bool isInteger(const double& num);
bool isInteger(const string& num);

struct Cell {
	string typeOfCell;
	string expression;
	string value;			
	unordered_set<string> references;  
	unordered_set<string> refrencedBy; 
};

string to_string_with_precision(const double& a_value);

class Table {
private:

	HashTable<string, Cell> table;
	size_t rows, cols;
	size_t maxCellSize = 0;


	

public:
	Table();
	Table(const size_t& _rows, const size_t& _cols);

	void setRows(const size_t& _rows);
	void setCols(const size_t& _cols);
	
	bool checkIsOperator(const char& symbol)const;
	double calculateValue(const string& address,const string& expr)const;
	bool isValidAddress(const string& address)const;
	string relativeToAbsolute(const string& absAddress, string addressRelative)const;
	bool isValidExpression(const string& data)const;
	string returnAbsAddress(const string& expr, int& i)const;
	string returnRelativeAddress(const string& expr, int& i)const;

	void set(const string& address, const string& expression);

	int count(const string& address1, const string& address2);
	double sum(const string& adress1, const string& adress2);

	string printValue(const string& address) const;
	string printExpression(const string& address) const;

	void printValueAll() const;
	void printExpressionAll() const;

	void save(const string& filePath) const;
	void load(const string& filePath);

	void increaseByOne(const string& address);
	void reduceByOne(const string& address);



	bool areMutuallyConnected(const string& currAddress, unordered_set<string> references);

};

Table::Table() {
	this->rows = 0;
	this->cols = 0;
}

Table::Table(const size_t& _rows, const size_t& _cols) {
	this->rows = _rows;
	this->cols = _cols;
}

void Table::setRows(const size_t& _rows) {
	this->rows = _rows;
}

void Table::setCols(const size_t& _cols) {
	this->cols = _cols;
}

bool Table::checkIsOperator(const char& symbol)const {
	switch (symbol) {
	case'&':
	case'+':
	case'|':
	case'=':
	case'!':
	case'-':
	case'*':
	case'/':
	case'<':
	case'>':
	case'(':
	case':':
	case')':
	case'[':
	case']':
	case'?':
		return true;
		break;
	default:
		return false;
	}
}

bool isText(const string& data) {

	if (data[0] == '\"' && data[data.size() - 1] == '\"') {
		return true;
	}
	return false;

}

double Table::calculateValue(const string& currAddress,const string& expr)const {

	string convertedExpr;
	if (isText(expr) || expr.empty()) {
		return 0;
	}
	for (int i = 0; i < expr.size(); i++) {
		if (expr[i] == 'R') {
			string address;
			int counter = 0;
			if (expr[i + 1] == '[') {
				string relAddress;
				while (counter !=2)
				{
					relAddress.push_back(expr[i]);
					if (expr[i] == ']') { counter++; }
					i++;
				}
				address+= relativeToAbsolute(currAddress, relAddress);
			}
			else {
				
				while (i < expr.size() && !checkIsOperator(expr[i])) {
					address.push_back(expr[i]);
					i++;
				}
			}
			i--;
			if (isValidAddress(address)) {
				Cell cellOnCurrAdress;
				table.get(address, cellOnCurrAdress);

				if (cellOnCurrAdress.expression.empty()) {
					convertedExpr.push_back('0');
				}
				else {
					convertedExpr += cellOnCurrAdress.value;
				}

				continue;
			}
			else {
				throw std::invalid_argument("Invalid address!");
			}
		}
		convertedExpr.push_back(expr[i]);
	}
	Calculator c(convertedExpr);
	return c.calculate();
}

bool Table::isValidAddress(const string& address)const {
	pair<int, int> coordinates = absoluteAddressToCoordinates(address);
	if ((coordinates.first >= 0 && coordinates.first <= this->rows) && (coordinates.second >= 0 && coordinates.second <= this->cols)) {
		return true;
	}
	return false;
}

int Table::count(const string& address1, const string& address2){

	if (isValidAddress(address1) && isValidAddress(address2)) {

		pair<int, int> adress1Coord = absoluteAddressToCoordinates(address1);
		pair<int, int> adress2Coord = absoluteAddressToCoordinates(address2);

		int minX = 0, maxX = 0, minY = 0, maxY = 0;

		(adress1Coord.first <= adress2Coord.first) ? (minX = adress1Coord.first, maxX = adress2Coord.first) : (minX = adress2Coord.first, maxX = adress1Coord.first);
		(adress1Coord.second <= adress2Coord.second) ? (minY = adress1Coord.second, maxY = adress2Coord.second) : (minY = adress2Coord.second, maxY = adress1Coord.second);

		int currX = minX;
		int currY = minY;

		int count = 0;
		for (int i = minX; (i < maxX + 1) && (i < this->rows); i++) {
			for (int j = minY; (j < maxY + 1) && (j < this->cols); j++) {
				string currAddress = coordinatesToAbsoluteAddress(i, j);
				Cell cellOnCurrAdress;
				if (table.get(currAddress, cellOnCurrAdress)) {
					count += 1;
				}
			}
		}
		return count;
	}
	else {
		throw std::invalid_argument("Invalid address!");
		return 0;
	}
}

double Table::sum(const string& address1, const string& address2) {

	if (isValidAddress(address1) && isValidAddress(address2)) {

		pair<int, int> address1Coord = absoluteAddressToCoordinates(address1);
		pair<int, int> address2Coord = absoluteAddressToCoordinates(address2);

		int minX = 0, maxX = 0, minY = 0, maxY = 0;
		(address1Coord.first <= address2Coord.first) ? (minX = address1Coord.first, maxX = address2Coord.first) : (minX = address2Coord.first, maxX = address1Coord.first);
		(address1Coord.second <= address2Coord.second) ? (minY = address1Coord.second, maxY = address2Coord.second) : (minY = address2Coord.second, maxY = address1Coord.second);
		int currX = minX;
		int currY = minY;

		double sum = 0;

		for (int i = minX; i != maxX + 1; i++) {
			for (int j = minY; j != maxY + 1; j++) {
				string currAddress = coordinatesToAbsoluteAddress(i, j);
				Cell cellOnCurrAddress;
				if (table.get(currAddress, cellOnCurrAddress)) {
					if (cellOnCurrAddress.typeOfCell == "expression") {
						sum += this->calculateValue(currAddress, cellOnCurrAddress.value);
					}
				}
			}
		}
		return sum;
	}
	else {
		throw std::invalid_argument("Invalid address!");
		return 0;
	}
}

bool Table::areMutuallyConnected(const string& currAddress, unordered_set<string> references) {

	for (string adress : references) {
		if (adress == currAddress) { return true; }
		else {
			Cell nextCellToExplore;
			this->table.get(adress, nextCellToExplore);
			if (nextCellToExplore.references.empty()) {continue;}
			else {
				return areMutuallyConnected(currAddress, nextCellToExplore.references);
			}
		}
	}
	return false;
}


void Table::set(const string& address, const string& inputExpr){
	
	if (isValidExpression(inputExpr)) {
		if (isValidAddress(address)) {
			Cell newCell;
			newCell.expression = inputExpr;
			
			if (isText(inputExpr)) {
				newCell.typeOfCell = "text";
				newCell.value = inputExpr;
			}
			else {
				newCell.typeOfCell = "expression";
				string newValue = to_string_with_precision(calculateValue(address, inputExpr));
				newCell.value = newValue;
			}
			
			for (int i = 0; i < inputExpr.size(); i++) {
				string refAddress;
				if (inputExpr[i] == 'R') {
					if (inputExpr[i + 1] == '[') {
						refAddress = this->returnRelativeAddress(inputExpr, i);
					}
					else {
						refAddress = returnAbsAddress(inputExpr, i);
					}
					newCell.references.insert(refAddress);
				}
			}

			Cell existingCell;
			if (table.get(address, existingCell)) {

				for (auto itPrev = existingCell.references.begin(); itPrev != existingCell.references.end(); itPrev++) {

					if (newCell.references.find(*itPrev) == newCell.references.end()) {
						Cell refCell;
						this->table.get(*itPrev, refCell);
						refCell.refrencedBy.erase(address);
						this->table.put(*itPrev, refCell);
					}
				}

				newCell.refrencedBy = existingCell.refrencedBy;
				if (areMutuallyConnected(address, newCell.references) == false) {
					this->table.put(address, newCell);
				}
				else {
					throw std::invalid_argument("Cells are mutully connected");
				}

				for (auto itNew = newCell.refrencedBy.begin(); itNew != newCell.refrencedBy.end(); itNew++) {
					Cell refCell;
					this->table.get(*itNew, refCell);
					this->set(*itNew, refCell.expression);
				}
			}
			else {
				if (newCell.expression.size() > this->maxCellSize) { this->maxCellSize = newCell.expression.size(); }
				if (newCell.value.size() > this->maxCellSize) { this->maxCellSize = newCell.value.size(); }
				this->table.put(address, newCell);
			}

			for (auto itNew = newCell.references.begin(); itNew != newCell.references.end(); itNew++) {
				Cell refCell;
				this->table.get(*itNew, refCell);
				refCell.refrencedBy.insert(address);
				/*if (refCell.expression.size() > this->maxCellSize) { this->maxCellSize = refCell.expression.size(); }
				if (refCell.value.size() > this->maxCellSize) { this->maxCellSize = refCell.value.size(); }*/
				this->table.put(*itNew, refCell);
			}
		}
		else {
			throw std::invalid_argument("Invalid address!");
		}
	}
	else {
		throw std::invalid_argument("Invalid input!");
	}
}

string Table::printValue(const string& address) const {

	if (isValidAddress(address)) {
		Cell cell;
		if (this->table.get(address, cell)) {
			return cell.value;
		}
		else {
			throw std::invalid_argument("Empty cell!");
		}
	}
	else {
		throw std::invalid_argument("Invalid address!");
	}
}

string Table::printExpression(const string& address) const {

	if (isValidAddress(address)) {
		Cell cell;
		if (this->table.get(address, cell)) {
			return cell.expression;
		}
		else {
			throw std::invalid_argument("Empty cell!");
		}
	}
	else {
		throw std::invalid_argument("Invalid address!");
	}
}

void Table::printValueAll() const {
	int line = (maxCellSize + 4) * cols;
	for (int i = 0; i < line; i++) { std::cout << "="; }
	std::cout << std::endl;

	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			string refAddress = coordinatesToAbsoluteAddress(i, j);
			Cell cell;
			if(table.get(refAddress, cell)) {
				if (isText(cell.expression)) {
					
					std::cout << "| " << cell.expression;
					int spaces = maxCellSize - cell.expression.size();
					for (int i = 0; i < spaces; i++) { std::cout << " "; }
					std::cout<<	" |";
				}
				else {
					std::cout << "| " << cell.value;
					int spaces = maxCellSize - cell.value.size();
					for (int i = 0; i < spaces; i++) { std::cout << " "; }
					std::cout << " |";
				}
			}
			
			else {
				std::cout << "| ";
				for (int i = 0; i < maxCellSize; i++) { std::cout << " "; }
				std::cout << " |";
			}
		}
		std::cout << std::endl;
		for (int i = 0; i < line; i++) { std::cout << "="; }
		std::cout << std::endl;
	}
}

void Table::printExpressionAll() const {
	int line = (maxCellSize + 4) * cols;
	for (int i = 0; i < line; i++) { std::cout << "="; }
	std::cout << std::endl;
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			string refAdress = coordinatesToAbsoluteAddress(i, j);
			Cell cell;
			if (table.get(refAdress, cell)) {
				std::cout << "| " << cell.expression;
				int spaces = maxCellSize - cell.expression.size();
				for (int i = 0; i < spaces; i++) { std::cout << " "; }
				std::cout << " |";
			}
			else {
				std::cout << "| ";
				for (int i = 0; i < maxCellSize; i++) { std::cout << " "; }
				std::cout << " |";
			}
		}
		std::cout << std::endl;
		for (int i = 0; i < line; i++) { std::cout << "="; }
		std::cout << std::endl;
	}
}

pair<int, int> absoluteAddressToCoordinates(const string& address) {

	pair<int, int> cordinates;
	int i = 1;
	bool isFoundX = false;

	string currNumber;
	while (i < address.size()) {

		if (address[i] == 'C') {
			cordinates.first = stoi(currNumber);
			currNumber.clear();
		}
		else {
			currNumber.push_back(address[i]);
		}
		i++;
	}
	cordinates.second = stoi(currNumber);
	return cordinates;
}

string coordinatesToAbsoluteAddress(const int& i, const int& j) {

	string address = "R";
	address += std::to_string(i);
	address += "C";
	address += std::to_string(j);

	return address;
}

bool isNumber(const string& data) {
	for (char ch : data) {
		if (!((ch >= '0' && ch <= '9') || (ch == '.'))) {
			return false;
		}
	}
	return true;
}

string typeOfCell(const string& data) {

	if (isNumber(data)) { return "number"; }
	else if (isText(data)) { return "text"; }
	else { return "expression"; }
}

bool isInteger(const double& num){

	int x = num;
	if ((num - x) > 0) { return false; }
	return true;
}

bool isInteger(const string& num) {

	if (isText(num)) {
		return false;
	}
	double doubleNum = stod(num);
	return isInteger(doubleNum);
}

bool Table::isValidExpression(const string& data)const {

	if (!isNumber(data) && !isText(data)) {
		for (char s : data) {
			if ((s >= '0' && s <= '9') || (checkIsOperator(s)==true) || s =='R' || s == 'C' || s == ' ' || s == 'i' || s == 'f') {
				continue;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

string Table::relativeToAbsolute(const string& absAddress,string addressRelative)const{ 

	pair<int, int> absCoord = absoluteAddressToCoordinates(absAddress);
	pair<int, int> relativeCoord;
	int i = 1;
	int countBrackets = 0;
	string currNumber;
	while (i < addressRelative.size()) {

		if (addressRelative[i] == '[') {
			i++;
			while (addressRelative[i] != ']') {
				currNumber.push_back(addressRelative[i]);
				i++;
				
			}
			countBrackets++;
			if(countBrackets==1){ 
				relativeCoord.first = stoi(currNumber);
				currNumber.clear();
			}
			else{ 
				relativeCoord.second = stoi(currNumber);
			}
		}
		i++;
	}
	
	absCoord.first += relativeCoord.first;
	absCoord.second += relativeCoord.second;

	string addressToReturn = coordinatesToAbsoluteAddress(absCoord.first, absCoord.second);
	
	return addressToReturn;
}

void Table::increaseByOne(const string& address) {
	if (isValidAddress(address)) {
		Cell cell;
		table.get(address, cell);
		if (isInteger(cell.value)) {
			string newVal = to_string(stoi(cell.value) + 1);
			std::cout << "New value of cell is " << newVal << std::endl;
			this->set(address, newVal);
		}
		else {
			throw std::invalid_argument("Value is not integer!");
		}
	}
}

void Table::reduceByOne(const string& address) {
	if (isValidAddress(address)) {
		Cell cell;
		table.get(address, cell);
		if (isInteger(cell.value)) {
			string newVal = to_string(stoi(cell.value) - 1);
			std::cout << "New value of cell is " << newVal << std::endl;
			this->set(address, newVal);
		}
		else {
			throw std::invalid_argument("Value is not integer!");
		}
	}
}

string Table::returnAbsAddress(const string& expr, int& i)const {

	string toReturn;
	while (!checkIsOperator(expr[i]) && expr[i] != ' ' && i < expr.size()) {
		toReturn.push_back(expr[i]);
		i++;
	}
	return toReturn;
}

string Table::returnRelativeAddress(const string& expr, int& i)const {

	string relAddress;
	int counter = 0;
	while (counter != 2)
	{
		relAddress.push_back(expr[i]);
		if (expr[i] == ']') { counter++; }
		i++;
	}
	return relAddress;
}

void Table::save(const string& filePath)const {

	bool isCSVFile = filePath.compare(filePath.size() - 4, 4, ".csv") == 0;
	if (!isCSVFile) {
		throw std::invalid_argument("Invalid file extension");
	}
	ofstream myfile(filePath);
	if (myfile.is_open()) {

		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < this->cols; j++) {
				string refAdress = coordinatesToAbsoluteAddress(i, j);
				Cell cell;
				if (table.get(refAdress, cell)) {
					myfile << cell.expression;
				}

				if (j != this->cols - 1) {
				    myfile << ";";
				}
			}
			myfile << std::endl;
		}
		myfile.close();
	}

	else {
		string errMsg = "Unable to open " + filePath;
		throw std::invalid_argument(errMsg);
	}
}

void Table::load(const string& filePath) {

	bool isCSVFile = filePath.compare(filePath.size() - 4, 4, ".csv") == 0;
	if (!isCSVFile) {
		throw std::invalid_argument("Invalid file extension");
	}
	

	ifstream myfile(filePath);
	if (myfile.is_open()) {
		string line;
		int _rows = 0;
		int _cols = 1;
		while (getline(myfile, line)) {
			if (_rows == 0) {
				for (int i = 0; i < line.size(); i++) {
					if (line[i] == ';') { _cols++; }
				}
			}
			_rows++;
		}
		
		line.clear();

		myfile.clear();
		myfile.seekg(0);

		this->rows = _rows;
		this->cols = _cols;

		
		int currRow = 0, currCol = 0;

		while (getline(myfile, line)) {
			stringstream lineStram(line);
			string expression;
			
			while (getline(lineStram, expression, ';')) {
				if (expression != "") {
					string address = coordinatesToAbsoluteAddress(currRow, currCol);
					this->set(address, expression);
				}
				currCol++;
			}
			currCol = 0;
			currRow++;
		}
	}

	else {
		string errMsg = "Unable to open " + filePath;
		throw std::invalid_argument(errMsg);
	}
}

string to_string_with_precision(const double& value)
{
	std::ostringstream out;
	out.precision(2);
	out << std::fixed << value;
	return out.str();
}
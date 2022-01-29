#include <iostream>
#include "ConsoleApplication.h"

//#include "Table.h"


int main() {

	//Table table(5, 5);



	/*table.set("R0C0", "\"hello\"");
	table.set("R0C1", "if((R0C2*2)>2)?R1C0:3");
	table.set("R0C2", "3 + 5");
	table.set("R0C4", "\"obicham veronica\"");
	table.set("R1C0", "1");
	table.set("R1C1", "R[0]C[-1]");
	table.set("R1C2", "\"hello2\"");
	table.set("R2C0", "\"hello3\"");
	table.set("R2C1", "876 + 33");
	table.set("R2C2", "\"hello12\"");*/

	//std::cout << "count is: " << table.count("R0C0", "R2C2") << std::endl;
	//table.save("file1.csv");

	//table.set("R1C0", "1");
	//table.set("R1C2", "\"veronika\"");

	//table.printValue("R1C2");

	/*Table tableFromFile;
	tableFromFile.load("file1.csv");

	tableFromFile.printExpressionAll();*/

	ConsoleApplication app;
	app.startUp();

	

	return 0;
}
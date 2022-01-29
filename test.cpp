#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <stdexcept>

#include "../el_tables_project_test/Table.h"


TEST_CASE("Test for correct address")
{
	Table table(20, 30);

	SECTION("Valid address") {
		REQUIRE(table.isValidAddress("R10C20") == true);
	}
	SECTION("Invalid adress") {
		REQUIRE(table.isValidAddress("R20C40") == false);
	}
}

TEST_CASE("Create and set cell") 
{
	Table table(20, 30);

	table.set("R0C0", "1");
	table.set("R0C1", "2 + R0C0 + R0C5");
	table.set("R0C2", "3 + R0C1");
	table.set("R0C3", "4 + R0C2");
	table.set("R0C4", "5");

	SECTION("Test for setting cell with invalid data:")
	{
		REQUIRE_THROWS(table.set("R0C6", "hello"));  //missing \" 
		REQUIRE(table.count("R0C0", "R10C10") == 6);
	}

	SECTION("Test chain of refernced cells get updated:")
	{
		REQUIRE(table.sum("R0C0", "R1C4") == 22.0);   ///??
	}

	SECTION("Test for mutual connected cells:")
	{
		REQUIRE_THROWS(table.set("R0C5", "5 + R0C1"));
	}

	/*SECTION("Test for ++ function:")
	{
		table.reduceByOne("R0C0");
		REQUIRE(table.printValue("R0C0") == "2");

	}

	SECTION("Test for -- function:")
	{
		table.reduceByOne("R0C4");
		REQUIRE(table.printValue("R0C4") == "4");
	}*/

	SECTION("Test for mutual connected cells:")
	{
		REQUIRE_THROWS(table.set("R0C5", "5 + R0C1"));
	}

	SECTION("Test for table count") {

		REQUIRE(table.count("R0C0", "R0C4") == 5);
	}

}

TEST_CASE("Test chain of refernced cells get updated:")
{
	Table table(20, 30);

	table.set("R0C0", "1 + R0C4");
	table.set("R0C1", "2 + R0C0");
	table.set("R0C2", "3 + R0C1");
	table.set("R0C3", "4 + R0C2");
	table.set("R0C4", "5");

	REQUIRE(table.sum("R0C0", "R1C4") == 45.0);
}

TEST_CASE("Test for mutual connected cells:")
{
	Table table(20, 30);


	table.set("R0C0", "1 + R0C4");
	table.set("R0C1", "2 + R0C0");
	table.set("R0C2", "3 + R0C1");
	table.set("R0C3", "4 + R0C2");

	REQUIRE_THROWS(table.set("R0C4", "5 + R0C0"));

}


TEST_CASE("Test for ++ and -- operators")
{
	Table table(20, 30);

	table.set("R0C0", "\"hello\"");
	table.set("R0C1", "1");
	table.set("R0C2", "3 + 5");
	table.set("R1C0", "");  // this should not insert anything into the table cell
	table.set("R1C1", "\"hello1\"");
	table.set("R1C2", "\"hello2\"");
	table.set("R2C0", "\"hello3\""); 
	table.set("R2C1", "876 + 33");
	table.set("R2C2", "\"hello12\"");

	SECTION("Test for ++ :")
	{
		table.increaseByOne("R0C1");
		REQUIRE(table.printValue("R0C1") == "2.00");
		REQUIRE_THROWS(table.increaseByOne("R1C1"));
	}

	SECTION("Test for --") {
		table.reduceByOne("R0C2");
		REQUIRE(table.printValue("R0C2") == "7.00");
		REQUIRE_THROWS(table.reduceByOne("R2C2"));
	}
}

TEST_CASE("Test for table sum")
{
	Table table(20, 30);

	table.set("R0C0", "\"hello\"");
	table.set("R0C1", "1");
	table.set("R0C2", "3 + 5");
	table.set("R1C0", "");
	table.set("R1C1", "\"hello1\"");
	table.set("R1C2", "\"hello2\"");
	table.set("R2C0", "\"hello3\"");
	table.set("R2C1", "876 + 33");
	table.set("R2C2", "\"hello12\"");

	REQUIRE(table.sum("R0C0", "R2C2") == 918.0);
}

TEST_CASE("Test for table sum (with adresses in expression)")
{
	Table table(20, 30);

	table.set("R0C0", "\"hello\"");
	
	table.set("R0C1", "3 + 5");
	table.set("R0C2", "1"); 
	table.set("R1C0", "if((R0C1*2)>2)?R0C2:3");
	table.set("R1C1", "R[0]C[-1]");
	table.set("R1C2", "\"hello2\"");
	table.set("R2C0", "\"hello3\"");
	table.set("R2C1", "876 + 33");
	table.set("R2C2", "\"hello12\"");

	REQUIRE(table.sum("R0C0", "R2C2") == 920.0);
}





//TEST_CASE("Test for mutual connected cells:")   ///chek in main
//{
//	Table table(20, 30);
//
//	table.set("R0C0", "1 + R0C1");
//	table.set("R0C1", "2 + R0C2");
//	
//	REQUIRE_THROWS(table.set("R0C2", "3 + R0C0"));
//}


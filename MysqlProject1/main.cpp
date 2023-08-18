/****************************
	 Header Files
*****************************/

#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <thread>
#include <iomanip>
#include <windows.h>



#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;





sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt, *stmt1;
sql::PreparedStatement* pstmt, *pstmt1, *pstmt2, *pstmt3;
sql::ResultSet* res, *res1, *res2;

const string server = /*"SERVER NAME"*/;
const string username = /*"USERNAME"*/;
const string password = /*"PASSWORD"*/;




/************************************
	 Defination of Global Functions
*************************************/

void gotoxy(int x, int y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/**********************************
	 Declaration of Functions
***********************************/

void clearance_problem(int, int);
void show_data_header();
void admin_id();
void note(int);
void intro();
void menu();
void setup();
void delete_lug_info();
void ticket_db_input();

/********************************
	 Class Starts Here
*********************************/

class ticket {
public:
	int Ticket_no=0;
	char lastName[20], firstName[20], depart_point[50], arrival_point[50];
	int age;

	int Ticket_gen();
	void ticket_reg();
	
	int retTicket_no() {
		return Ticket_no;
	}
	char* retfname()
	{
		return firstName;
	}

	char* retlname()
	{
		return lastName;
	}
	char* retdp()
	{
		return depart_point;
	}

	char* retap()
	{
		return arrival_point;
	}
};



/********************************
	 Class Luggage
*********************************/

class Luggage {
	long wght;
	int v, arr[5], rno, ch, no_of_bag, defwght;
	char fname[20], lname[20];

public:

	void submission_of_luggage();
	void show_record();
	void receiving_of_luggage();
	void ncp();
	


	int retrno()
	{
		return rno;
	}

	int ret_no_of_bag()
	{
		return no_of_bag;
	}

	char* retfname()
	{
		return fname;
	}

	char* retlname()
	{
		return lname;
	}

	int* retchoice_store() {
		return arr;
	}

};

/*************************************
	 Class Administrator
**************************************/

class administrator
{
	int no_of_ad;
	char fname[50], lname[50];
public:

	void admin_name();
	char* retad_name() { return fname; return lname; }
};




/********************************************
	Ticket Registration(Member Function)
*********************************************/
int ticket::Ticket_gen() {
	int i = 1, f, t;
	Ticket_no;
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
		catch (sql::SQLException e)
		{
			std::cout << "Could not connect to server. Error message: " << e.what() << endl;
			std::system("pause");
			exit(1);
		}

	con->setSchema("sia_db");
	stmt = con->createStatement();
	srand(time(0));
	do {
		while (i <= 4)
		{
			t = rand() % 9 + 1;
			Ticket_no = (Ticket_no * 10) + t;
			i++;
		}

		res = stmt->executeQuery("SELECT COUNT(*) AS count FROM passenger_ticket WHERE ticket_no = " + to_string(Ticket_no));
		bool existsInDatabase = res->next() && res->getInt("count") > 0;

		if (existsInDatabase) {
			f = 1;
		}
		else {
			f = 0;
		}

	} while (f == 1);

}

void ticket::ticket_reg() {
	std::system("cls");
	


	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}

	
		con->setSchema("sia_db");
		stmt = con->createStatement();
		stmt->execute("CREATE TABLE IF NOT EXISTS passenger_ticket (ticket_no INTEGER PRIMARY KEY, first_name VARCHAR(50), last_name VARCHAR(50), depart_point VARCHAR(50), arrival_point VARCHAR(50))");		
		delete stmt;

		pstmt3 = con->prepareStatement("INSERT INTO passenger_ticket (ticket_no, first_name, last_name, depart_point, arrival_point) VALUES (?,?,?,?,?)");
		
		Ticket_gen();
		
	
		pstmt3->setInt(1, Ticket_no);
		pstmt3->setString(2, firstName);
		pstmt3->setString(3, lastName);
		pstmt3->setString(4, depart_point);
		pstmt3->setString(5, arrival_point);

		pstmt3->execute();
		std::cout << "\n\tGenerating your reciept";
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << ".";
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << ".";
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "." << endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Please don't forget to collect your Ticket No.: " << Ticket_no << endl;
		

	
	std::system("pause");
	intro();
	
}




/********************************************
	 Admin Name(Member Function)
*********************************************/

void administrator::admin_name()
{

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}
	con->setSchema("sia_db");
	stmt = con->createStatement();
	stmt->execute("DROP TABLE IF EXISTS admin_record");
	
	stmt->execute("CREATE TABLE admin_record (fname VARCHAR(50), lname VARCHAR(50))");
	delete stmt;
	
	std::cout << "\nProvide Number Of Allowed Member To Access Confedential Info:";
	std::cin >> no_of_ad;
	
	pstmt = con->prepareStatement("INSERT INTO admin_record(fname, lname) VALUES(?,?)");

	for (int i=0;i < no_of_ad;i++)
	{
		std::cout << "Provide Name Of Allowed Member To Access Confedential Info:"<<endl;
		std::cout << "ADMIN first name:";
		std::cin >> fname;
		std::cout << "      last  name:";
		std::cin >> lname;
		pstmt->setString(1, fname);
		pstmt->setString(2, lname);
		pstmt->execute();
		
	}
	

	
	
	delete pstmt;
	delete con;
	

	
	std::cout << "Admin Created Successfully." << endl;

	
	
	
}



/*********************************************
	 No Clearance Problem
**********************************************/

void Luggage::ncp()
{
	int r, i = 1, f;
	rno = 0;
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}

	
	con->setSchema("sia_db");
	stmt = con->createStatement();

	
	srand(time(0));

	do {
		while (i <= 4)
		{
			r = rand()% 9 + 1;
			rno = (rno * 10) + r;
			i++;
		}

		res = stmt->executeQuery("SELECT COUNT(*) AS count FROM luggage_record WHERE rno = " + to_string(rno));
		bool existsInDatabase = res->next() && res->getInt("count") > 0;

		if (existsInDatabase) {
			f = 1;
		}
		else {
			f = 0;
		}

	} while (f == 1);

	
	std::cout << "\n\tGenerating your reciept";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << ".";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << ".";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "." << endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Please don't forget to collect your receipt: " << rno << endl;
	delete res;
	delete stmt;
	delete con;

	std::system("pause");
}

/*****************************************
	 Showing Record(Member Function)
******************************************/

void Luggage::show_record()
{
	int f = 0, Fval_db, Foval_db;
	string Sval_db, Tval_db;

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}

	

	con->setSchema("sia_db");
	stmt = con->createStatement();
	string query = "SELECT * FROM luggage_record ";
	res1 = stmt->executeQuery(query);
	

	while (res1->next()) {
		Fval_db = res1->getInt("rno");
		Sval_db = res1->getString("fname");
		Tval_db = res1->getString("lname");
		Foval_db = res1->getInt("no_of_bag");

		show_data_header();
		
	}




	std::cout << Fval_db << std::setw(50);
	std::cout << Sval_db << " " << Tval_db << std::setw(38);

	std::cout << Foval_db << std::setw(51);

	string query2 = "SELECT * FROM type";
	sql::ResultSet* res2 = stmt->executeQuery(query2);

	while (res2->next())
	{
		if (res2->getInt("bag_ch_no") == 1)
		{
			std::cout << "Backpack" << endl << std::setw(148.5);

		}
		else if (res2->getInt("bag_ch_no") == 2)
		{
			std::cout << "Handbag" << endl << std::setw(148.5);

		}
		else if (res2->getInt("bag_ch_no") == 3)
		{
			std::cout << "Trolley Bag" << endl << std::setw(148.5);

		}
	}
	std::cout << endl;
	delete res1;
	delete res2;
	delete stmt;

	
}

/***************************************************
	 Submission Of Luggage(Member function)
***************************************************/

void Luggage::submission_of_luggage()
{
	
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}
	
	
	con->setSchema("sia_db");
	


	stmt = con->createStatement();
	
	const char* createTable1Query = "CREATE TABLE IF NOT EXISTS luggage_record (rno INTEGER PRIMARY KEY, fname VARCHAR(50), lname VARCHAR(50), no_of_bag INTEGER)";
	stmt->execute(createTable1Query);
	const char* createTable2Query = "CREATE TABLE IF NOT EXISTS type (rno INTEGER, bag_ch_no INTEGER)";
	stmt->execute(createTable2Query);
	
	delete stmt;
	
	
	pstmt1 = con->prepareStatement("INSERT INTO luggage_record(rno, fname, lname, no_of_bag) VALUES(?,?,?,?)");
	
	pstmt2 = con->prepareStatement("INSERT INTO type(rno, bag_ch_no) VALUES(?,?)");
	

	int w, r, n = 0, i;

	std::cout << "\n\n\tTo Submit Your Luggage" << endl;
	std::cout << "\tPlease Provide Name of The Owner of The Luggage="<<endl;
	std::cout << "\tfirst name:";
	std::cin >> fname;
	std::cout << "\tlast  name:";
	std::cin >> lname;
	std::cout << "\n\t!!ACCORDING TO OUR GUIDELINES YOU CAN ADD UPTO 3 BAGS ONLY!!\n\n\tPlease Enter Number of Bags (min:3):";
	std::cin >> no_of_bag;

	if (no_of_bag <= 3) {

		for (i = 0; i < no_of_bag; i++)
		{
			std::cout << "\t1.BACKPACK\t2.HANDBAGS\t3.TROLLEY BAGS\n" << endl;
			std::cout << "\t\tPlease Verify Your Choice(1-3)=";
			std::cin >> ch;

			arr[i] = ch;


			switch (ch)
			{
			case 1:
				defwght = 10;
				std::system("cls");
				n = 1;
				note(n);
				std::cout << "\n\nYou Selected";
				std::cout << "\n_____________\nBackpack\n";
				std::cout << "\nEnter Weight of Backpack =";
				std::cin >> wght;


				if (wght > defwght)
				{
					w = 1;
				}
				else if (no_of_bag > i)
				{
					r = 2;
				}

				if (w == 1 || r == 2)
				{
					switch (w)
					{
					case 1:
						std::system("cls");
						std::cout << "\n\tYour Luggage Weight " << wght << "     Exceding Idol Limit";
						std::cout << "\n\tWe Have Some Clearance Problem";
						clearance_problem(wght, defwght);

					case 2:
						break;
					}
				}


				break;


			case 2:
				std::system("cls");
				defwght = 15;
				n = 2;
				note(n);
				std::cout << "\n\nYou Selected";
				std::cout << "\n_____________\nHandbag\n";
				std::cout << "Enter weight of Handbags =";
				std::cin >> wght;

				if (wght > defwght)
				{
					w = 1;
				}
				else if (no_of_bag > i)
				{
					r = 2;
				}

				if (w == 1 || r == 2)
				{
					switch (w)
					{
					case 1:
						std::system("cls");
						std::cout << "\n\tYour Luggage Weight " << wght << " Exceding Idol Limit";
						std::cout << "\n\tWe Have Some Clearance Problem";
						clearance_problem(wght, defwght);

					case 2:
						break;

					}
				}

				break;

			case 3:
				std::system("cls");
				defwght = 30;
				n = 3;
				note(n);
				std::cout << "\n\nYou Selected";
				std::cout << "\n_____________\nTrolley Bag\n";
				std::cout << "Enter weight of Trolley Bag=";
				std::cin >> wght;

				if (wght > defwght)
				{
					w = 1;
				}
				else if (no_of_bag > i)
				{
					r = 2;
				}

				if (w == 1 || r == 2)
				{
					switch (w)
					{
					case 1:
						std::system("cls");
						std::cout << "\n\tYour Luggage Weight " << wght << " Exceding Idol Limit";
						std::cout << "\n\tWe Have Some Clearance Problem";
						clearance_problem(wght, defwght);

					case 2:
						break;

					}
				}

				break;

			default:std::cout << "\nPlease enter valid choice";
			}

		}
		ncp();

		int rpTicket_no = retrno();
		

		pstmt1->setInt(1, rpTicket_no);
		pstmt1->setString(2, fname);
		pstmt1->setString(3, lname);
		pstmt1->setInt(4, no_of_bag);
		pstmt1->execute();


		for (int i = 0; i < no_of_bag; i++)
		{
			pstmt2->setInt(1, rpTicket_no);
			pstmt2->setInt(2, arr[i]);

			pstmt2->execute();
		}
	}
	else {
		std::cout << "\n\n\n\t\t\t\t!!!GUDELINES ARE VIOLATED SENDING YOU BACK TO MAIN PAGE!!!\n\n\t\t\t\t\t";
		
		std::system("pause");
	}
	
	
}



/***************************************************
	 Data Header(function)
****************************************************/

void show_data_header()
{
	std::cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	std::cout << "Reciept number"<< std::setw(45) <<"Owner name"<< std::setw(45) <<"Number of bags"<< std::setw(43.5)<<"Bag type"<< endl;
	std::cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;

	
}

/**************************************************
	 Receiving Of Luggage(Member Function)
***************************************************/

void Luggage::receiving_of_luggage()
{

	int receipt_no, f = 0, Fval_db, Foval_db;
	string Sval_db , Tval_db;
	std::system("cls");

	Luggage lug;
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}

	
	
	

	std::cout << "\n\nTo Receiving of Luggage ";
	std::cout << "\nPlease Enter Your Receipt Number to Receive your Luggage  ->";
	std::cin >> receipt_no;
	

	con->setSchema("sia_db");
	stmt = con->createStatement();
	string query = "SELECT * FROM luggage_record WHERE rno = " + to_string(receipt_no);
	res1 = stmt->executeQuery(query);
	std::cout << "data stored";

	while (res1->next()) {
		Fval_db = res1->getInt("rno");
		Sval_db = res1->getString("fname");
		Tval_db = res1->getString("lname");
		Foval_db = res1->getInt("no_of_bag");

		show_data_header();
		f = 1;
	}



	if (f == 0)
	{
		std::cout << "error:wrong receipt number";
	}


	std::cout << Fval_db << std::setw(50);
	std::cout << Sval_db << " " << Tval_db << std::setw(38);

	std::cout << Foval_db << std::setw(51);

	string query2 = "SELECT * FROM type WHERE rno = " + to_string(receipt_no);
	sql::ResultSet* res2 = stmt->executeQuery(query2);

	while (res2->next())
	{
		if (res2->getInt("bag_ch_no") == 1)
		{
			std::cout << "Backpack" << endl << std::setw(148.5);

		}
		else if (res2->getInt("bag_ch_no") == 2)
		{
			std::cout << "Handbag" << endl << std::setw(148.5);

		}
		else if (res2->getInt("bag_ch_no") == 3)
		{
			std::cout << "Trolley Bag" << endl << std::setw(148.5);

		}
	}
	std::cout << endl;
	delete res1;
	delete res2;
	delete stmt;

	
	std::system("pause");
	intro();
}

/******************************************
	 Note function
*******************************************/

void note(int n)
{
	if (n == 1)
	{
		std::cout << "\nNOTE:\na.Backpack should have 10kg of idol weight" << endl;
	}
	else if (n == 2)
	{
		std::cout << "b.Handbag should have 15kg of idol weight" << endl;
	}
	else if (n == 3)
	{
		std::cout << "c.Trolley bags should have 30kg of idol weight" << endl;
	}
}

/**********************************************
	 Clearance Problem Function
***********************************************/

void clearance_problem(int wght, int defwght)
{
	int c;
	char ch1;

	std::cout << "\nDo you want to";
	std::cout << "\n1.Removing Extra Material\n\t\t\tOR\n2.Payement of Extra Material";

	ch1 = _getche();


	switch (ch1)
	{
	case '1':
		std::cout << "\nThank you for taking out of extra material" << endl;

		break;

	case '2':
		c = (wght - defwght) * 50;
		std::cout << "\n\n\n\nPlease pay " << c << " for extra luggage of " << wght - defwght << "kg" << endl;
	}
	std::system("pause");
}

/***************************************
	 Setup
****************************************/

void setup()
{
	administrator ad;
	ad.admin_name();
}


/******************************************
	Writing Ticket Record
*******************************************/


void ticket_db_input(){
	std::system("cls");
	char fname[20], lname[20], d_point[50], a_point[50];
	ticket user;

	 std::string currentTime = __TIME__;
	 std::string hourString = currentTime.substr(0, 2);  

	 int hour = std::stoi(hourString);  

	 if (hour >= 0 && hour < 12) {
		 std::cout << "Have A Great Morning All...." << std::endl;
	 }
	 else if (hour >= 12 && hour < 18) {
		 std::cout << "Have A Shiny Day All...." << std::endl;
	 }
	 else {
		 std::cout << "Have A Wonderful Night A...." << std::endl;
	 }
	
	 std::cout << "Let Us Know Your Good Name........" << endl;
	
	 std::cout << "Your First Name :";
	 std::cin >> user.firstName;

	


	 std::cout << "Your Last Name :";
	 std::cin >> user.lastName;

	
	 std::cout << endl << "Now Tell Me Where You Wanna Ride With Us (Please Use Camel Technique to Input Places)...... " << endl;
	 std::cout << "Tell Us FROM Where You Wanna Depart  :";
	 std::cin >> user.depart_point;

	 std::cout << "Tell Us TO Where You Wanna Arrive :";
	 std::cin >> user.arrival_point;
	 std::cout << "we are here";

	 user.ticket_reg();
	 std::cout << "end";
	 
	 std::system("pause");
}



/*******************************************
	 Writing Record
********************************************/

void write_lug_record()
{

	Luggage lug;
	lug.submission_of_luggage();

	std::cout << "\n\nThe Luggage Record Has Been Created ";
	
	intro();
}

/*******************************************
	 Modify Record
********************************************/

void modify_lug_info()
{
	std::system("cls");

	Luggage lug;
	int no, found = 0, i = 0, Fval_db, Foval_db;
	string Sval_db, Tval_db;
		
 
	 try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}

	
	
	
	std::cout << "\n\n\tTo Modify ";
	std::cout << "\n\n\tPlease Enter The Receipt No. of The luggage :";
	std::cin >> no;
	
	


	con->setSchema("sia_db");
	stmt = con->createStatement();
	string query = "SELECT * FROM luggage_record WHERE rno = " + to_string(no);
	res1 = stmt->executeQuery(query);
	

	while (res1->next()) {
		Fval_db = res1->getInt("rno");
		Sval_db = res1->getString("fname");
		Tval_db = res1->getString("lname");
		Foval_db = res1->getInt("no_of_bag");

		show_data_header();
		found = 1;
	}



	if (found == 0)
	{
		std::cout << "error:wrong receipt number";
	}


	std::cout << Fval_db << std::setw(50);
	std::cout << Sval_db << " " << Tval_db << std::setw(38);

	std::cout << Foval_db << std::setw(51);

	string query2 = "SELECT * FROM type WHERE rno = " + to_string(no);
	sql::ResultSet* res2 = stmt->executeQuery(query2);

	while (res2->next())
	{
		if (res2->getInt("bag_ch_no") == 1)
		{
			std::cout << "Backpack" << endl << std::setw(148.5);

		}
		else if (res2->getInt("bag_ch_no") == 2)
		{
			std::cout << "Handbag" << endl << std::setw(148.5);

		}
		else if (res2->getInt("bag_ch_no") == 3)
		{
			std::cout << "Trolley Bag" << endl << std::setw(148.5);

		}
	}



	lug.submission_of_luggage();
	delete_lug_info();

	std::system("pause");
	intro();
}

/***************************************
	 Delete Record
****************************************/

void delete_lug_info()
{
	std::system("cls");

	Luggage lug;
	int no;
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}

	con->setSchema("sia_db");

	stmt = con->createStatement();
	
	std::cout << "\n\n\n\tDelete Record";
	std::cout << "\n\nPlease Enter The Receipt no of The Luggage Info You Want To Remove : ";
	std::cin >> no;

	string sql = "DELETE FROM luggage_record WHERE rno =" + to_string(no);
	int rows_affected = stmt->executeUpdate(sql);
	std::cout << rows_affected << "record deleted." << endl;
	string sql2 = "DELETE FROM type WHERE rno =" + to_string(no);
	int rows_affected2 = stmt->executeUpdate(sql2);

	std::cout << rows_affected2 << "record deleted." << endl;
	std::system("pause");
	intro();
}

/*****************************************
	 Admin Identification
******************************************/

void admin_id()
{
	std::system("cls");

	administrator ad;
	Luggage lug;
	int i = 0, loop=0;
	char userFirstName[50], userLastName[50];
	int f = 0, Fval_db, Foval_db;
	string Sval_db, Tval_db;
	
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << endl;
		std::system("pause");
		exit(1);
	}
	

	std::cout << "To\nCheck You Are Eligible To Enter In The Confedential Area\n";
	std::cout << "Input Your first Name:";
	std::cin >> userFirstName;
	std::cout << "           Last Name:";
	std::cin >> userLastName;


	con->setSchema("sia_db");
	pstmt = con->prepareStatement("SELECT * FROM admin_record WHERE fname = ? AND lname = ?");
	pstmt->setString(1, userFirstName);
	pstmt->setString(2, userLastName);
	res = pstmt->executeQuery();

	
	if (res->next())
	{
		f = 1;
	}
	

	
	delete res;
	delete pstmt;
	

	if (f == 1)
	{
		
		size_t size = sizeof(*res);
		std::cout << "You Are Eligible To Enter";
		

		stmt = con->createStatement();
		string query = "SELECT * FROM luggage_record ";
		res1 = stmt->executeQuery(query);

		string query2 = "SELECT * FROM type";
		sql::ResultSet* res2 = stmt->executeQuery(query2);
		std::cout << "data stored";
			

		while (res1->next()) {
			show_data_header();
			std::cout << res1->getInt("rno") << std::setw(50);
			std::cout << res1->getString("fname") << " " << res1->getString("lname") << std::setw(38);
			std::cout << res1->getInt("no_of_bag") << std::setw(51);
			Fval_db = res1->getInt("rno");
			Foval_db = res1->getInt("no_of_bag");


			while (Foval_db > 0) {

				while (res2->next())
				{
					if (res2->getInt("rno") == Fval_db)
					{
						
					
							if (res2->getInt("bag_ch_no") == 1)
							{
								std::cout << "Backpack" << endl << std::setw(148.5);

							}
							else if (res2->getInt("bag_ch_no") == 2)
							{
								std::cout << "Handbag" << endl << std::setw(148.5);

							}
							else if (res2->getInt("bag_ch_no") == 3)
							{
								std::cout << "Trolley Bag" << endl << std::setw(148.5);

							}
							
							break;
						
					}

					
				}
				Foval_db--;
			}

			
		}
			std::cout << endl;
			std::cout << endl;
		
	}
	else {
		std::cout << "Sorry You Are Not Eligible:";
	}

	
	std::system("pause");
	intro();
}

/********************************************
	Security
*********************************************/

void security()
{
	char password[20];
	int ch, f = 1, start;
	ticket t;

	gotoxy(30, 13);

	do
	{
		std::system("cls");

		gotoxy(65, 1);
		std::cout << "_______________";
		gotoxy(67, 2);
		std::cout << " MAIN MENU";
		gotoxy(65, 3);
		std::cout << "_______________";


		std::cout << "\n\n\n\t\t\t   01. TICKET BOOKING SECTION";
		std::cout << "\t\t02. LUGGAGE SECTION";
		std::cout << "\t\t03. ADMINISTRATOR SECTION";
		gotoxy(4, 38);
		std::cout << "--------------------------------------------------------------------------------------------";
		std::cout << "---------------------------------------------------------";
		gotoxy(4, 39);
		std::cout << "exit=0";
		gotoxy(67, 39);
		std::cout << "DATE: " << __DATE__ <<"\t\t\t\t\t\t\t   TIME: " << __TIME__;
		
		
		gotoxy(57, 37);
		std::cout << "Please Select Your Option (1-4) : ";
		ch = _getche();


		switch (ch)
		{
		case '1': ticket_db_input();
			break;
		case '2': intro();
		
		case '3': start:
			std::system("cls");
			std::cout << "\n\tPlease Input password: ";
			std::cin >> password;

			if (_strcmpi(password, "CHSGC") == 0)
			{
				setup();
				std::cout << "We Stored Our admin Please press enter";
				f = 0;
				std::system("pause");
			}
			else {
				std::cout << "Incorrect password";
				f = 1;
			}

			if (f == 1)
			{
				goto start;
			}

			break; 

		case'0': exit(0);
			break;

		default:std::cout << "\a";
		}
	} while (ch != '0');
}

/*****************************************
	Intro function
******************************************/

void intro()
{
	std::system("cls");
	std::cout << "____________________________________________________________________________________________________________________________________________________________" << endl;
	gotoxy(65,2);
	std::cout << "!!WELCOME TO!!"<<endl;
	std::cout << "____________________________________________________________________________________________________________________________________________________________" << endl;
	gotoxy(56, 6);
	std::cout << "SNEH INTERNATONAL AIRPORT (SIA)";
	std::cout << "\n\t\t     _______________________________";
	std::cout << "______________________________________________________";
	std::cout << "__________________________" << endl;
	gotoxy(4, 38);
	std::cout << "--------------------------------------------------------------------------------------------";
	std::cout << "---------------------------------------------------------";
	gotoxy(4, 39);
	std::cout << "exit=0";
	gotoxy(67, 39);
	std::cout << "DATE: " << __DATE__ << "\t\t\t\t\t\t\t   TIME: " << __TIME__;


	
	menu();
}
/*******************************************
	Menu Function
********************************************/

void menu()
{
	Luggage lug;
	char ch2;

	gotoxy(64, 12);
	std::cout << "________________";
	gotoxy(67, 14);
	std::cout << "MAIN MENU";
	gotoxy(64, 15);
	std::cout << "________________";
	gotoxy(21, 19);
	std::cout << "1. SUBMISSION OF LUGGAGE";
	gotoxy(106, 20);
	std::cout << "2. RECEIVING OF LUGGAGE";
	gotoxy(21, 21);
	std::cout << "3. MODIFY INFO";
	gotoxy(106, 22);
	std::cout << "4. DELETE INFO";
	gotoxy(21, 23);
	std::cout << "5. CONFIDENTIAL DATA";
	gotoxy(57, 37);
	std::cout << " Enter Your Choice : ";
	ch2 = _getche();

	switch (ch2)
	{
	case '0': exit(0);
	case '1': std::system("cls");
			  write_lug_record();
		      break;
	case '2': lug.receiving_of_luggage();
		      break;
	case '3': modify_lug_info(); break;
	case '4': delete_lug_info(); break;
	case '5': admin_id(); break;
		      break;
	default: std::cout << "\a"; intro();
	}
	gotoxy(1, 27);
	std::cout << "exit=0";
}

/*******************************************
	Project First Page
********************************************/

void first_page()
{
	std::system("cls");
	
	gotoxy(44, 2);
	std::cout << "----------------------------------------------------------------";
	gotoxy(58, 3);
	std::cout << "SNEH INTERNATONAL AIRLINES (SIA)";
	gotoxy(44, 4);
	std::cout << "----------------------------------------------------------------";

	gotoxy(68, 7);
	std::cout << "Presents";
	
	
	gotoxy(44, 9);
	std::cout << "----------------------------------------------------------------";
	gotoxy(51, 10);
	std::cout << "Enhanced Airport Luggage Management System";
	gotoxy(44, 11);
	std::cout << "----------------------------------------------------------------";
	
	gotoxy(2, 16);
	std::cout << "Coded By:";
	
	
	gotoxy(2, 17);
	std::cout << "Vishwas Yadav";
	
	gotoxy(57, 30);
	std::system("pause");
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userdata) {
	std::string* response = static_cast<std::string*>(userdata);
	size_t totalSize = size * nmemb;
	response->append(static_cast<char*>(contents), totalSize);
	return totalSize;
}

/***************************************
	Main Functioning
****************************************/

int main()
{

	first_page();
	security();


	std::system("pause");
	return 0;
}












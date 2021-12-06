//*****************************************************************************************
//Program by: Adam LaFleur
//Date: February 22, 2019
//Class: CS163 - Data Structures
//Program: #3
//File: main.cpp
//Purpose: The purpose of this program is to keep track of information pertaining to a topic
//and have it be searchable without having the perform a sequential search, the searching is
//done through search keys that act as shortcuts to the locations of relavent information.
//main.cpp has the purpose of controlling the program and inputting data to be sent to the ADT. 
//*****************************************************************************************
#include <iostream>
#include <cstring>
#include "table.h"

using namespace std;



void add(entry &insert);
void print(const entry pentry);
void status(int num);
void remove(entry &remove);



// The purpose of main is the initialize and run the program, main has a menu that gives the user an
// option of what they want to do, the menu repeats until the user specifices when to stop. The
// main program also creates a struct entry, fills it, and then sends it to the adt to be added or sends an array
// of entries to the ADT and receives matching entries back. The user is given the options of adding a entry, searching
// for a key which will return back an array of matching entires, remove by name, display all matches for searck key,
// display all, and then end the program.
int main()
{
	entry insert; //entry objects used to grab data from ADT
	entry ** search;
	char input[64];
	table info(541);
	int choice = 0, num = 0, count = 0; //choice used to control which menu option selected, num is used to record ADT return values
	bool control = true; //Controls whether the program loops or not

	cout << endl;
	cout << "----------------------------------------------" << endl;
        cout << "*********** Program #3: Hash Table ***********" << endl;
        cout << "----------------------------------------------" << endl;
	while(control)
	{
		num = 0;
		cout << "Please choose an option: " << endl;
		cout << "01. Add a new channel or website." << endl;
		cout << "02. Search for a key." << endl;
		cout << "03. Remove by name." << endl;
		cout << "04. Display all matches for a search key." << endl;
		cout << "05. Display all (not in order!)." << endl;
		cout << "06. Quit client program." << endl;
		cout << "----------------------------------------------" << endl;
		cout << "Please enter a choice: ";
		cin >> choice;
		cin.ignore();
		cout << endl;
		switch(choice)
		{
			case 1:
				add(insert);
				num = info.addTable(insert);
				status(num);
				remove(insert);
				break;
			case 2:
				search = new entry*[300]; //used to retrieve matching ADTs back
				for(int i = 0; i < 100; ++i)
				{
					search[i] = nullptr;
				}
				cout << "Please enter the search key you want to search for: ";
				cin.getline(input, sizeof(input));
				num = info.searchTable(input, search, count);
				if(num)
				{
					cout << "Entries with found keys: " << endl;
					cout << "----------------------------------------------" << endl;
					for(int j = 0; j < count; ++j)
					{
						print(*search[j]);
						remove(*search[j]);
						delete search[j];
						search[j] = nullptr;
					}
					search = nullptr;
					count = 0;
				}
				delete [] search;
				status(num);
				break;
			case 3:
				cout << "Please enter the name you want to remove: ";
				cin.getline(input, sizeof(input));
				num = info.removeTable(input);
				status(num);	
				break;
			case 4:
				cout << "Please enter the search key you want to search for: ";
				cin.getline(input, sizeof(input));
				num = info.displaySearch(input);
				status(num);
				break;
			case 5:
				num = info.displayTable();
				status(num);
				break;
			case 6:
				control = false;
				status(1);
				break;
			default:
				break;
		}
	}
	return 0;
}



//Function used to fill a struct to be sent into the ADT or anywhere else in the program, 
//takes in an etry type using pass by reference
void add(entry &insert)
{
	char name[64], address[64], description[64], information[128], search_key[64];
	int control = 1;

	cout << "Please enter a name: ";
	cin.getline(name, sizeof(name));
	insert.name = new char[strlen(name) + 1];
	strcpy(insert.name, name);
	
	insert.search_keys = new char *[10];
	insert.number_of_keys = 0;
	for(int i = 0; i < 10; ++i)
	{
		insert.search_keys[i] = nullptr;
	}
	do
	{
		cout << "Please enter a value for the search key: ";
		cin.getline(search_key, sizeof(search_key));	
		insert.search_keys[insert.number_of_keys] = new char[strlen(search_key) + 1];
		strcpy(insert.search_keys[insert.number_of_keys], search_key);
		++insert.number_of_keys;
		cout << "Do you want to enter another key (1 for yes, 0 for no): ";
		cin >> control;
		cin.ignore();
	}
	while(control && insert.number_of_keys < 10);
	
	cout << "Please enter an address: ";
	cin.getline(address, sizeof(address));
	insert.address = new char[strlen(address) + 1];
	strcpy(insert.address, address);

	cout << "Please enter a description: ";
	cin.getline(description, sizeof(description));
	insert.description = new char[strlen(description) + 1];
	strcpy(insert.description, description);

	cout << "Please etner some information: ";
	cin.getline(information, sizeof(information));
	insert.information = new char[strlen(information) + 1];
	strcpy(insert.information, information);

	cout << "Please enter the rating: ";
	cin >> insert.rating;
}



// The purpose of print is to print off the entry data that is returned back from the 
// ADT, takes a entry data type
void print(const entry to_print)
{
	cout << "Name: " << to_print.name << endl;
	cout << "Search Keys: ";
	for(int i = 0; i < to_print.number_of_keys; ++i)
	{
		cout << to_print.search_keys[i];
		if(i != to_print.number_of_keys - 1)
		{
			cout << ", ";
		}
	}
	cout << endl;
	cout << "Number of Search Keys: " << to_print.number_of_keys << endl;
	cout << "Address: " << to_print.address << endl;
	cout << "Description: " << to_print.description << endl;
	cout << "Information: " << to_print.information << endl;
	cout << "Rating: " << to_print.rating << endl;
	cout << endl;
}



//The purpose of remove is to delete a data entry after it is sent in or returned back, 
//it takes a pass by reference entry argument
void remove(entry &to_delete)
{
	delete [] to_delete.name;
	for(int i = 0; i < to_delete.number_of_keys; ++i)
	{
		delete [] to_delete.search_keys[i];
		to_delete.search_keys[i] = nullptr;
	}
	delete [] to_delete.search_keys;
	to_delete.search_keys = nullptr;
	delete [] to_delete.address;
	delete [] to_delete.description;
	delete [] to_delete.information;
}



// The prupose of status is to display whether the action succeded or not. Status takes in
// an argument int num which is supplied from main, if the number supplied is 0 then
// Failure will be printed out, if 1 is supplied then Success will be printed out.
void status(int num)
{
	cout << endl;
	cout << "Status: ";
	if(num == 0)
	{
		cout << "Failure! / Empty List!" << endl;
	}
	if(num == 1)
	{
		cout << "Success!" << endl;
	}
	cout << endl;
}

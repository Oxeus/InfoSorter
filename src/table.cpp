//*****************************************************************************************
//Program by: Adam LaFleur
//Date: February 22, 2019
//Class: CS163 - Data Structures
//Program: #3
//File: table.cpp
//Purpose: The purpose of this program is to keep track of information pertaining to a topic
//and have it be searchable without having the perform a sequential search, the searching is
//done through search keys that act as shortcuts to the locations of relavent information.
//table.h has the purpose of containing the algorithms to add to the table, search and return
//matching values, printing the whole table or just mathing entries in the tabler, removing 
//by name from the table. Also the ADT reads in data from a file in its constructor.
//*****************************************************************************************
#include "table.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;



//Has the purpose of reading in a data file and creating an entry object to be sent to the add function
//Inputs a data file outputs an entry
int table::read()
{
	entry insert;
	ifstream inFile;
	char name[64], number[24], key[64], address[128], description[500], information[500];

	inFile.open("very_large.txt");
	while(!inFile.eof() && inFile.getline(name, 64, '|'))
	{
		//name
		insert.name = new char[strlen(name) + 1];
		strcpy(insert.name, name);

		//Number of keys
		inFile.getline(number, 24, '|');
		insert.number_of_keys = atoi(number);

		//grab all search keys based on above number
		insert.search_keys = new char*[insert.number_of_keys];
		for(int i = 0; i < insert.number_of_keys; ++i)
		{
			inFile.getline(key, 64, '|');
			insert.search_keys[i] = new char[strlen(key) + 1];
			strcpy(insert.search_keys[i], key);	
		}
		
		//Address
		inFile.getline(address, 128, '|');
		insert.address = new char[strlen(address) + 1];
		strcpy(insert.address, address);

		//Description
		inFile.getline(description, 500, '|');
		insert.description = new char[strlen(description) + 1];
		strcpy(insert.description, description);

		//Information
		inFile.getline(information, 500, '|');
		insert.information = new char[strlen(information) + 1];
		strcpy(insert.information, information);

		//Rating
		inFile.getline(number, 24);
		insert.rating = atoi(number);
		addTable(insert);
		delete_entry(insert);
	}
	inFile.close();
	return 1;
}



//Constructor used to create the table data structure and initialize array elements to NULL
table::table()
{
	hash_table = new node*[hash_size];
	for(int i = 0; i < hash_size; ++i)
	{
		hash_table[i] = nullptr;
	}
	read();
}



//Constructor used to create the table data structure and initialize array but allows user
//to specificy size of table they want. Takes input in form of an int
table::table(int size)
{
	hash_size = size;
	hash_table = new node*[hash_size];
	for(int i = 0; i < hash_size; ++i)
	{
		hash_table[i] = nullptr;
	}
	read();
}



//Deconstructor used to deallocate memory in the table
table::~table()
{
	entry * ptr = nullptr;
	int hash = 0;
	for(int i = 0; i < hash_size; ++i)
	{
		if(hash_table[i])
		{
			while(hash_table[i])
			{
				ptr = hash_table[i]->data;
				for(int j = 0; j < ptr->number_of_keys; ++j)
				{
					hash = hash_function(ptr->search_keys[j]);
					delete_node(hash_table[hash], ptr);
				}
				delete_entry(*ptr);
				delete ptr;
				ptr = nullptr;
			}
		}
		hash_table[i] = nullptr;
	}
	delete [] hash_table;
}



//Function that has the purpose of creating the index for the hash table by creating a
//number based on numerical values of characters. Takes in a char array and outputs a 
//integer of type unsigned long long.
unsigned long long table::hash_function(char * search_key)
{
	unsigned long long hash = int(search_key[0]);
	for(unsigned int i = 1; i < strlen(search_key); ++i)
	{
		hash *= int(search_key[i]);
	}
	return hash % hash_size;
}



//Function used to copy data from one entry to another, the first input is a const entry
//thats is the entry to copy from and the second input is a pass by reference copy that 
//can be accessed and changed. The output is a pass by reference entry type.
int table::copy_entry(const entry to_copy, entry &copy)
{
	if(to_copy.name)
	{
		//Name
		copy.name = new char[strlen(to_copy.name) + 1];
		strcpy(copy.name, to_copy.name);

		//Keys and number of keys
		copy.number_of_keys = to_copy.number_of_keys;
		copy.search_keys = new char*[copy.number_of_keys];
		for(int i = 0; i < copy.number_of_keys; ++i)
		{
			copy.search_keys[i] = new char[strlen(to_copy.search_keys[i]) + 1];
			strcpy(copy.search_keys[i], to_copy.search_keys[i]);
		}

		//Address
		copy.address = new char[strlen(to_copy.address) + 1];
		strcpy(copy.address, to_copy.address);

		//Description
		copy.description = new char[strlen(to_copy.description) + 1];
		strcpy(copy.description, to_copy.description);

		///Information
		copy.information = new char[strlen(to_copy.information) + 1];
		strcpy(copy.information, to_copy.information);

		//Rating
		copy.rating = to_copy.rating;
		return 1;
	}	
	return 0;
}



//Print_entry has the purpose of printing off an entry, there is a function for printing to save
//space in the file so there wouldn't be repeated code for print. Takes in an imput of type struct
//entry and outputs/prints the data listed in the struct.
int table::print_entry(const entry to_print)
{
	if(!to_print.name) return 0;
	cout << "Name: " << to_print.name << endl;
	cout << "Search Keys: ";
	for(int i = 0; i < to_print.number_of_keys; ++i)
	{
		cout << to_print.search_keys[i];
		if(i != (to_print.number_of_keys - 1))
		{
			cout << ", ";
		}
	}
	cout << endl;
	cout << "Number of Search Keys: " << to_print.number_of_keys << endl;
	cout << "Address: " << to_print.address << endl;
	cout << "Description: " << to_print.description << endl;
	cout << "information: " << to_print.information << endl;
	cout << "Rating: " << to_print.rating << endl;
	cout << endl;
	return 1;
}



//Used to delete an entry, this function was written so that there wouldn't be additional delete
//functions everywhere. Takes in a pass by reference struct entry that can be edited and deleted.
int table::delete_entry(entry &to_del)
{
	int result = 0;
	if(to_del.name)
	{
		delete [] to_del.name;
		for(int i = 0; i < to_del.number_of_keys; ++i)
		{
			delete [] to_del.search_keys[i];
			to_del.search_keys[i] = nullptr;
		}
		delete [] to_del.search_keys;
		to_del.number_of_keys = 0;
		delete [] to_del.address;
		delete [] to_del.description;
		delete [] to_del.information;
		to_del.rating = 0;
		result = 1;
	}
	return result;
}



//The purpose of the function is to take in a const struct entry from the client program and 
//use the copy function to copy data into an allocated struct entry. The function this places
//this entry into nodes that are create at search key values written in the struct entry. The 
//new nodes are added at the top of the list at each index. The functions takes in a const 
//struct entry.
int table::addTable(const entry to_insert)
{
	int done = 0, hash = 0;
	if(!hash_table) return done;

	node * newNode = nullptr;
	entry * newData = new entry;
	copy_entry(to_insert, *newData);

	for(int i = 0; i < to_insert.number_of_keys; ++i)
	{
		hash = hash_function(to_insert.search_keys[i]);
		if(hash_table[hash] == nullptr)
		{
			hash_table[hash] = new node;
			hash_table[hash]->data = newData;
			hash_table[hash]->next = nullptr;
		}
		else
		{
			newNode = new node;
			newNode->data = newData;
			newNode->next = hash_table[hash];
			hash_table[hash] = newNode;
		}
		done = 1;
		++a;
	}
	return done;
}



//The purpose of this function is to search the table for the key provided by the client
//function, search the table for the key and then grab all values that match that key
//at that index. Takes in a char array for the search key and also takes in a pass by 
//reference array of struct entries, there is also a pass by reference int variable
//to record the size of the array so the client doesn't access memory locations that aren't
//printed. Outputs an array and int variable passed by reference.
int table::searchTable(char * search_key, entry ** &to_search, int &count)
{
	int found = 0, hash = 0;
	node * curr = nullptr;
	if(!hash_table) return found;
	hash = hash_function(search_key); //Find location for key
	if(hash_table[hash] == nullptr)
	{
		found = 0;
		count = 0;
	}
	else
	{
		curr = hash_table[hash]; //Access location
		while(curr)
		{
			for(int i = 0; i < curr->data->number_of_keys; ++i) //Loop through list of keys at each node to check for search key match
			{
				if(strcmp(curr->data->search_keys[i], search_key) == 0) //If the key in data match input key add to the array
				{
					to_search[count] = new entry;
					copy_entry(*(curr->data), *to_search[count]);
					++count;
					found = 1;
				}
			}
			curr = curr->next;
		}
	}
	return found;
}



//Function used to delete a node from the list. Written to recursively remove a node if 
//the data pointer matches the input data pointer. Takes in a pass by reference node pointer 
//and a pass by reference entry pointer.
int table::delete_node(node * &head, entry * &ptr)
{
	int result = 0;
	if(!head) return result;
	result = delete_node(head->next, ptr);
	if(head->data == ptr)
	{
		--a;
		node * temp = head->next;
		head->data = nullptr;
		delete head;
		head = temp;
		result = 1;
	}
	return result;
	
}



//Function that has the purpose of removing all nodes in a list that match the char array
//value passed from the client. The function search for the name, if it matches it then 
//removes that node at every search key value, until that name is no longer present in
//the list. Takes in an input of const char array.
int table::removeTable(const char * name)
{
	int done = 0, hash = 0;
	node * curr = nullptr;
	entry * ptr = nullptr;
	if(!hash_table) return done;
	for(int i = 0; i < hash_size; ++i)
	{
		if(hash_table[i])
		{
			
			curr = hash_table[i];
			while(curr)
			{
				if(strcmp(curr->data->name, name) == 0)
				{
					ptr = curr->data;
					for(int j = 0; j < ptr->number_of_keys; ++j)
					{
						hash = hash_function(ptr->search_keys[j]);
						done = delete_node(hash_table[hash], ptr);
					}
					delete_entry(*ptr);
					delete ptr;
					curr = hash_table[i];
				}
				else
				{
					curr = curr->next;
				}
			}
		}
	}
	return done;
}



//Function used to display entries in the table that match the search key. Takes in an input of char array
int table::displaySearch(char * key)
{
	int empty = 0;
	node * curr = nullptr;
	if(!hash_table) return empty;
	int hash = hash_function(key);
	if(hash_table[hash] == nullptr)
	{
		empty = 0;
	}
	else
	{
		curr = hash_table[hash];
		while(curr)
		{
			for(int i = 0; i < curr->data->number_of_keys; ++i)
			{
				if(strcmp(curr->data->search_keys[i], key) == 0)
				{
					print_entry(*(curr->data));
				}
                        }
			curr = curr->next;
		}
		empty = 1;	
	}
	return empty;
}



//Function that has the purpose of displaying the whole table at once. Only outputs/displays entry data.
int table::displayTable()
{
	int empty = 0;
	node * curr = nullptr;
	if(!hash_table) return empty;
	cout << "News - Entries: " << a << endl;
	for(int i = 0; i < hash_size; ++i)
	{
		if(hash_table[i])
		{
			cout << "Location [" << i << "]: " << endl;
			cout << "----------------------------------------------" << endl;
			curr = hash_table[i];
			while(curr)
			{
				print_entry(*(curr->data));
				curr = curr->next;
			}
			empty = 1;
		}	
	}
	cout << "----------------------------------------------" << endl;
	cout << "     - Entries: " << a << endl;
	
	return empty;
} 

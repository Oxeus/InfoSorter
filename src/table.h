//*****************************************************************************************
//Program by: Adam LaFleur
//Date: February 22, 2019
//Class: CS163 - Data Structures
//Program: #3
//File: table.h
//Purpose: The purpose of this program is to keep track of information pertaining to a topic
//and have it be searchable without having the perform a sequential search, the searching is
//done through search keys that act as shortcuts to the locations of relavent information.
//table.h has the purpose of defining the ADT structure and defining the entry structure
//*****************************************************************************************

struct entry {
	char * name;
	char ** search_keys;
	int number_of_keys;
	char * address;
	char * description;
	char * information;
	int rating;
};



class table
{
	public:
		table();
		table(int size);
		~table();
		int addTable(const entry to_insert);
		int searchTable(char * search_key, entry ** &to_search, int &count);
		int removeTable(const char * name);
		int displaySearch(char * key);
		int displayTable();
	private:
		struct node {
			entry * data;
			node * next;
		};
		node ** hash_table;
		int hash_size = 541;
		
		int a = 0; //Used to keep track of total amount of nodes for testing purposes

		int read();
		int copy_entry(const entry to_copy, entry &copy);
		int print_entry(const entry to_print);
		int delete_entry(entry &to_del);
		int delete_node(node * &head, entry * &ptr);
		unsigned long long hash_function(char * search_key);
};

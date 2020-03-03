#include <iostream>
#include <numeric>
#include <vector>
#include <fstream>
#include <math.h>
#include <chrono>
#include <ctime>

#define SIZE 10
#define STR_SIZE 100

using namespace std;

class HashTable {
	private:
		string arr[SIZE];

		int hash(string); //Hash input string into index
		int linear_probing(string, string); //return indexOfArray for inserting
		int quadratic_probing(string, string); //return indexOfArray for inserting
		bool is_desired(int, string);

	public:
		void insert(string);
		bool search(string);
		void remove(string);
};

bool HashTable::is_desired(int index, string key) {
	//if insertion key will be empty
	if(key == "") {
		// so we can insert at tombstone
		if(arr[index] == "tombstone")
			return true;
	} 

	return arr[index] == key;
}

int HashTable::hash(string key) {
    int ind = 0;
    int size = key.size();
    for(char &c : key) {
        int n = c - 'a' + 1;

        ind += ((unsigned long int) pow(27, --size) % SIZE);
    }

    return ind % SIZE;
}

int HashTable::linear_probing(string key, string search_term = "") {
	int ind = hash(key);

	if(!is_desired(ind, search_term)) {
		int old = ind++;
		while(old != ind && !is_desired(ind, search_term)) {
			//Searching
			if(search_term == "")
				if(arr[ind] == "")
					return -1;

			ind = ++ind % SIZE;
		}

		if(old == ind) return -1;
	}

	return ind % SIZE;
}

/*
* Insert into appropiate index based on algorithm
*/
void HashTable::insert(string s) {
	int ind = linear_probing(s);

	if(ind == -1)
		return;

	arr[ind] = s;
}

/*
* If found return true, else false
*/
bool HashTable::search(string s) {
	int ind = linear_probing(s, s);

	if (ind == -1)
		return false;

	return arr[ind] == s;
}

void HashTable::remove(string s) {
	int ind = linear_probing(s, s);

	if(ind == -1)
		return;

	arr[ind] = "tombstone";
	// cerr << "Removed " << s << " at: " << ind << endl;
}

//
string random_word(int num_chars) {
	string str;
	str.reserve(num_chars);

	for (int i = 0; i < num_chars; ++i)
		str += rand() % (127 - 32) + 32;
	
	return str;
}

vector<string> test_insertion(HashTable *ht) {
	vector<string> v;
	v.reserve(SIZE);
	for (int i = 0; i < SIZE; ++i){
		string str = random_word(STR_SIZE);
		ht->insert(str);
		v.push_back(str);
	}

	return v;
}

void test_searching(HashTable *ht, vector<string> *v) {
	for (int i = 0; i < SIZE; ++i)
		ht->search((*v)[i]);
}

void test_deletion(HashTable *ht, vector<string> *v) {
	for (int i = 0; i < SIZE; ++i)
		ht->remove((*v)[i]);
}

int main()
{
	srand(time(0));

	HashTable hash_table;
	
	double ms = 0.0;
	for (int i = 0; i < 3; ++i)
	{
		// auto v = test_insertion(&hash_table);
    	auto start = chrono::high_resolution_clock::now();
		// test_deletion(&hash_table, &v);
		test_insertion(&hash_table);
    	auto stop = chrono::high_resolution_clock::now();
   		chrono::duration<double> result = stop - start;

		ms += result.count() * 1000;
	}
	ms /= 3.0;
	
	ofstream write("result_probing.txt", ios::app);
	write << "Insertion with words of size " << STR_SIZE 
		<< " and hash table of size " << SIZE << '\n';
	write << ms << " ms\n";
	write.close();

	cout << ms << "ms DONE\n";

	return 0;
}
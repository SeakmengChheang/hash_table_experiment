#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <fstream>
#include <math.h>
#include <chrono>
#include <ctime>

#define SIZE 5000
#define STR_SIZE 100

using namespace std;

class HashTable {
	private:
		string arr[SIZE];

		int hash(string); //Hash input string into index
		int linear_probing(string, string); //return indexOfArray for inserting
		bool is_desired(int, string);

	public:
		void insert(string);
		void remove(string);
		bool search(string);
		void print();
};

bool HashTable::is_desired(int index, string key) {
	if(key == "" && arr[index] == "tombstone") return true;

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
	int step = 7 - (ind % 7);
	// if(search_term == "")
	// 	cerr << "original index of " << key <<" : " << ind << endl;

	if(!is_desired(ind, search_term)) {
		int old = ind;
		ind += step;
		while(old != ind && !is_desired(ind, search_term))
			ind = ++ind % SIZE;

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

	// cerr << "Inserted " << s << " at: " << ind << endl;
}

void HashTable::remove(string s) {
	int ind = linear_probing(s, s);

	if(ind == -1)
		return;

	arr[ind] = "tombstone";
	// cerr << "Removed " << s << " at: " << ind << endl;
}

/*
* If found return true, else false
*/
bool HashTable::search(string s) {
	int ind = linear_probing(s, s);

	if (ind == -1)
		return false;

	// cerr << "Search " << s << " founded at: " << ind << endl;

	return arr[ind] == s;
}

void HashTable::print() {
	for (int i = 0; i < SIZE; ++i)
		if(arr[i] != "")
			cout << i << ") " << arr[i] << ' ';
			
	cout << endl;
}

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

    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now();
    chrono::duration<double> result = stop - start;

	double ms = 0.0;
	for (int i = 0; i < 3; ++i)
	{
		auto v = test_insertion(&hash_table);
    	start = chrono::high_resolution_clock::now();
		test_deletion(&hash_table, &v);
    	stop = chrono::high_resolution_clock::now();
   		result = stop - start;

		ms += result.count() * 1000;
	}
	ms /= 3.0;
	
	ofstream write("double_hashing.txt", ios::app);
	write << "Insertion with words of size " << STR_SIZE 
		<< " and hash table of size " << SIZE << '\n';
	write << ms << " ms\n";
	write.close();

	cout << ms << " ms DONE\n";

	return 0;
}
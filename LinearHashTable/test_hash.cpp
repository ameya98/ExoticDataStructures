/*
	Tests for linear_hashing.cpp - a Linear Hash Table implementation.
    Author: Ameya Daigavane
*/

#include <iostream>
#include <cstdlib>
#include <cassert>
#include "linear_hashing.cpp"

using namespace std;

int main(){
	LinearHashTable<int> hash_table(10);

    hash_table.print();
    cout << "\n";

    /* Insert random keys. */
    int num_keys = 1000;
    for(int i = 0; i < num_keys; ++i){
        hash_table.insert(rand() % 1000);
    }

	hash_table.print();
    cout << "\n";

    cout << "Insertion tests passed!" << "\n";

    /* Search some keys. */
    assert(hash_table.count(449) == false);
    assert(hash_table.count(439) == true);
    assert(hash_table.count(278) == false);
    assert(hash_table.count(176) == true);

    cout << "Search tests passed!" << "\n";

    /* Delete some keys. */
    hash_table.del(176);
    assert(hash_table.count(176) == false);

    cout << "Deletion tests passed!" << "\n";

    cout << "All tests passed!" << "\n";

}

/*
	Tests for linear_hashing.cpp - a Linear Hash Table implementation.
    Author: Ameya Daigavane
*/

#include <iostream>
#include <cstdlib>
#include <cassert>
#include "linear_hashing.cpp"

int main(){
	LinearHashTable<int> hash_table(10);

    hash_table.print();
    std::cout << "\n";

    /* Insert random keys. */
    int num_keys = 1000;
    for(int i = 0; i < num_keys; ++i){
        hash_table.insert(rand() % 1000);
    }

	hash_table.print();
    std::cout << "\n";

    std::cout << "Insertion tests passed!" << "\n";

    /* Search some keys. */
    assert(hash_table.count(449) == false);
    assert(hash_table.count(439) == true);
    assert(hash_table.count(278) == false);
    assert(hash_table.count(176) == true);

    std::cout << "Search tests passed!" << "\n";

    /* Delete some keys. */
    hash_table.del(176);
    assert(hash_table.count(176) == false);
    hash_table.del(439);
    assert(hash_table.count(439) == true);
    hash_table.del(439);
    assert(hash_table.count(439) == true);
    hash_table.del(439);
    assert(hash_table.count(439) == false);

    hash_table.print();
    std::cout << "\n";

    std::cout << "Deletion tests passed!" << "\n";

    std::cout << "All tests passed!" << "\n";

}

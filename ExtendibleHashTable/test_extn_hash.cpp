/*
	Tests for extendible_hashing.cpp - an Extendible Hash Table implementation.
    Author: Ameya Daigavane
*/

#include <iostream>
#include <cstdlib>
#include <cassert>
#include "extendible_hashing.hpp"

int main(){
	ExtendibleHashTable<int> hash_table(0, 3);

    hash_table.print();
    std::cout << "\n";

    /* Insert random keys. */
    int num_keys = 1000;
    for(int i = 0; i < num_keys; ++i){
        hash_table.insert(rand() % 100000);
    }

	// hash_table.print();
    // std::cout << "\n";

    std::cout << "Insertion tests passed!" << "\n";

    /* Search some keys. */
    assert(hash_table.count(0) == false);
    assert(hash_table.count(19801) == true);
    assert(hash_table.count(1) == false);
    assert(hash_table.count(12164) == true);

    std::cout << "Search tests passed!" << "\n";

    /* Delete some keys. */
    hash_table.del(12164);
    assert(hash_table.count(12164) == false);

    std::cout << "Deletion tests passed!" << "\n";
    std::cout << "All tests passed!" << "\n";
}

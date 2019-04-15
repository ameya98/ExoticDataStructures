/*
	Tests for extendible_hashing.cpp - an Extendible Hash Table implementation.
    Author: Ameya Daigavane
*/

#include <iostream>
#include <cstdlib>
#include <cassert>
#include "extendible_hashing.hpp"

int main(){
	ExtendibleHashTable<int, int> hash_table(0, 3);

    hash_table.print();
    std::cout << "\n";

    /* Insert random keys. */
    int num_keys = 1000;
    for(int i = 0; i < num_keys; ++i){
        hash_table.insert(rand() % 100000, rand() % 10);
    }

    hash_table.print();
    std::cout << "\n";

    std::cout << "Insertion tests passed!" << "\n";

    /* Search some keys. */
    assert(hash_table.count(0) == false);
    assert(hash_table.count(27432) == true);
    assert(hash_table.count(1) == false);
    assert(hash_table.count(4860) == true);

    std::cout << "Search tests passed!" << "\n";

    /* Delete some keys. */
    hash_table.remove(4860);
    assert(hash_table.count(4860) == false);

    hash_table.remove(27432);
    assert(hash_table.count(27432) == true);

    std::cout << "Deletion tests passed!" << "\n";
    std::cout << "All tests passed!" << "\n";
}

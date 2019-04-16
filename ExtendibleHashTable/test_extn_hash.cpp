/*
	Tests for extendible_hashing.hpp - an Extendible Hash Table implementation.
    Author: Ameya Daigavane
*/

#include <iostream>
#include <cstdlib>
#include <cassert>
#include "extendible_hashing.hpp"

/* Custom hash for the hashtable. */
size_t custom_hash(int x){
    return x + 1;
}

int main(){

    /* Hash table parameters. */
    int global_depth_initial = 0;
    int num_slots_per_bucket = 3;

	ExtendibleHashTable<int, int> eht(global_depth_initial, num_slots_per_bucket);

    /* Set a custom hash function. */
    eht.set_hash(custom_hash);

    /* Should be empty. */
    eht.print();
    std::cout << "\n";

    /* Insert random keys. */
    int num_keys = 1000;
    for(int i = 0; i < num_keys; ++i){
        eht.insert(rand() % 100000, rand() % 10);
    }

    eht.print();
    std::cout << "\n";

    std::cout << "Insertion tests passed!" << "\n";

    /* Search some keys. */
    assert(eht.count(0) == false);
    assert(eht.count(27432) == true);
    assert(eht.count(1) == false);
    assert(eht.count(4860) == true);

    std::cout << "Search tests passed!" << "\n";

    /* Delete some keys. */
    eht.remove(4860);
    assert(eht.count(4860) == false);

    eht.remove(27432);
    assert(eht.count(27432) == false);

    std::cout << "Deletion tests passed!" << "\n";
    std::cout << "All tests passed!" << "\n";
}

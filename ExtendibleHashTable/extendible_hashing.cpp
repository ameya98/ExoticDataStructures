/*
    Extendible Hashing in C++.
    Author: Ameya Daigavane
*/


#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

template <typename T>
class HashBucket {

	public:
	int local_depth;
	int max_slots;
	std::vector <T> slots;

	HashBucket(int local_depth, int max_slots){
        this -> local_depth = local_depth;
		this -> max_slots = max_slots;

        slots.reserve(max_slots);
	}

	/* Insert into this bucket. */
	void insert(T key){
        /* Should not be full. */
        assert(!isFull());

		slots.push_back(key);

        /* Invariant. */
        assert(slots.size() <= max_slots);
	}

	/* Remove the last element in this bucket and return it. */
	T pop(){
		assert((slots.size() > 0) && (slots.size() <= max_slots));

		T key;
		key = slots.back();
		slots.pop_back();

        /* Invariant. */
        assert(slots.size() <= max_slots);

        return key;
	}

    /* Checks if key is present in this bucket. */
    bool count(T key){
        for(int i = 0; i < slots.size(); ++i){

            /* Check slots. */
            if(slots[i] == key){
                return true;
            }
        }

        return false;
    }

    /* Delete key from bucket. */
    void del(T key){
        assert(count(key));

        /* Check slots. */
        for(int i = 0; i < slots.size(); ++i){
            if(slots[i] == key){
                slots.erase(slots.begin() + i);
                return;
            }
        }
    }

	/* Get the number of keys in this bucket. */
	int size(){
		return slots.size();
	}

	/* True if the bucket has no keys. */
	bool isEmpty(){
		return slots.size() == 0;
	}

	/* True if the bucket is full. */
	bool isFull(){
		return slots.size() == max_slots;
	}

	/* Print the contents of this bucket. */
	void print(){
		if(isEmpty()){
			std::cout << "Empty." << "\n";
            return;
		}

		for(int i = 0; i < slots.size(); ++i){
			std::cout << slots[i] << " ";
		}
		std::cout << "\n";
	}
};

template <typename T>
class ExtendibleHashTable {

    int global_depth;
    int num_slots;
    std::vector<int> directory;
	std::vector<HashBucket<T>> buckets;

	public:
	ExtendibleHashTable(int global_depth = 0, int num_slots = 3){
        this -> global_depth = global_depth;
        this -> num_slots = num_slots;

        int num_buckets = 1 << global_depth;
		HashBucket<T> new_bucket(global_depth, num_slots);
        for(int i = 0; i < num_buckets; ++i){
            directory.push_back(i);
            (this -> buckets).push_back(new_bucket);
        }
	}

	/* Main hash function used. Currently identity. */
	int hash(T x){
		return x;
	}

	/* Get bucket index corresponding to this key. */
	int get_bucket(T key){
		return hash(key) & ((1 << global_depth) - 1);
	}

	/* Rehash between buckets with indices. */
	void rehash(int index1, int index2, int mask){
        HashBucket<T>& bucket1 = buckets[directory[index1]];
        HashBucket<T>& bucket2 = buckets[directory[index2]];

        bucket1.local_depth += 1;
        bucket2.local_depth = bucket1.local_depth;

        std::vector<T> keys;
		T key;
		int index;

		while (!bucket1.isEmpty()){
			key = bucket1.pop();
			keys.push_back(key);
		}

        // std::cout << "Keys to be rehashed:";
        // for(int i = 0; i < keys.size(); ++i){
        //     std::cout << keys[i] << " ";
        // }
        // std::cout << "\n";
        //
        // bucket1.print();
        // bucket2.print();
        //
        // print();

        assert(bucket1.isEmpty());
        assert(bucket2.isEmpty());

		for (int i = 0; i < keys.size(); ++i){
			key = keys[i];

            /* Recompute bucket index, based on mask. */
            if(hash(key) & mask){
                index = index2;
            } else {
                index = index1;
            }

            // buckets[directory[index]].insert(key);

            // cout << "Inserting key " << key << " into bucket indexed " << directory[index] << "\n";

            /* Reinsert based on this hash value. */
			buckets[directory[index]].insert(key);
		}
	}

    /* Double directory, and increase global depth by 1. */
    void double_directory(){
        int curr_size = directory.size();
        for(int i = 0; i < curr_size; ++i){
            directory.push_back(directory[i]);
        }
        global_depth += 1;
    }

	/* Insert into the hash table according to the hash function. */
	void insert(T key){
		int index = get_bucket(key);
        // cout << "Trying to insert " << key << " at index " << index << "\n";

        if (buckets[directory[index]].isFull()){
            // cout << "Bucket Full! \n";
            if(buckets[directory[index]].local_depth == global_depth){
                // cout << "Directory doubled!" << "\n";
                double_directory();
                //print();
            }
            split_bucket(index);
            insert(key);
        } else {
            // cout << "Bucket Not Full! \n";
            buckets[directory[index]].insert(key);
            // print();
        }
	}

    /* Split bucket with index. */
    void split_bucket(int index){

        int local_depth = buckets[directory[index]].local_depth;
        int index1 = index & ((1 << local_depth) - 1);
        int index2 = index1 + (1 << local_depth);

        HashBucket<T> new_bucket(local_depth, num_slots);
        buckets.push_back(new_bucket);

        directory[index2] = buckets.size() - 1;

        // cout << index1 << " " << index2 << " considered \n";

        // std::cout << "Before rehashing: " << "\n";
        // buckets[directory[index1]].print();
        // buckets[directory[index2]].print();

        /* Rehash values for this split bucket. */
        rehash(index1, index2, (1 << local_depth));

        // std::cout << "After rehashing: " << "\n";
        // buckets[directory[index1]].print();
        // buckets[directory[index2]].print();

        /* Point to correct buckets. */
        for(int i = index2 + (2 << local_depth); i < (1 << global_depth); i += (2 << local_depth)){
            directory[i] = directory[index2];
        };

    }

    /* Searches for the key in the hash table. Returns the index of the bucket if found, and -1 if not. */
    int search(T key){
        int index = get_bucket(key);

        if(buckets[directory[index]].count(key)){
            return index;
        } else {
            return -1;
        }
    }

    /* Delete a key from the hash table. */
    void del(T key){

        /* Key must be present in the hash table. */
        int index = search(key);
        assert(index != -1);

        /* Delete from corresponding bucket. */
        buckets[directory[index]].del(key);
    }

    /* Checks if key is present in the hash table. */
    bool count(T key){
        return (search(key) != -1);
    }


	/* Print the hash table. */
	void print(){
        print_directory();
		for(int i = 0; i < buckets.size(); ++i){
			std::cout << "Bucket " << i << " (local depth " << buckets[i].local_depth << ") : ";
			buckets[i].print();
		}
	}

    /* Print the hash table directory. */
    void print_directory(){
        std::cout << "Directory:" << "\n";
		for(int i = 0; i < directory.size(); ++i){
			std::cout << "dir[" << i << "] = " << directory[i] << "\n";
		}
	}
};

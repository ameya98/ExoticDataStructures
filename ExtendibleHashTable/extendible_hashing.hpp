/*
    Extendible Hashing in C++.
    Author: Ameya Daigavane
*/

#include <vector>
#include <iostream>
#include <cassert>
#include <utility>
#include <functional>

template <typename KEY, typename VAL>
class HashBucket {

	public:
	size_t local_depth;
	size_t max_slots;
	std::vector<std::pair<KEY, VAL>> slots;

	HashBucket(size_t local_depth, size_t max_slots){
        this -> local_depth = local_depth;
		this -> max_slots = max_slots;

        slots.reserve(max_slots);
	}

	/* Insert into this bucket. */
	void insert(std::pair<KEY, VAL> keyval){

        /* Should not be full. */
        assert(!isFull());

        for(size_t i = 0; i < slots.size(); ++i){
            /* Check slots. Overwrite old key-value pair. */
            if(slots[i].first == keyval.first){
                slots[i].second = keyval.second;
                return;
            }
        }

        /* Key not found already. */
		slots.push_back(keyval);

        /* Invariant. */
        assert(slots.size() <= max_slots);
	}

	/* Remove the last element in this bucket and return it. */
	std::pair<KEY, VAL> pop(){
		assert((slots.size() > 0) && (slots.size() <= max_slots));

		std::pair<KEY, VAL> deleted;
		deleted = slots.back();
		slots.pop_back();

        /* Invariant. */
        assert(slots.size() <= max_slots);

        return deleted;
	}

    /* Checks if key is present in this bucket. */
    bool count(KEY key){
        for(size_t i = 0; i < slots.size(); ++i){

            /* Check slots. */
            if(slots[i].first == key){
                return true;
            }
        }

        return false;
    }

    /* Delete key from bucket. */
    void remove(KEY key){
        assert(count(key));

        /* Check slots. */
        for(size_t i = 0; i < slots.size(); ++i){
            if(slots[i].first == key){
                slots.erase(slots.begin() + i);
                return;
            }
        }
    }

	/* Get the number of keys in this bucket. */
	size_t size(){
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

		for(size_t i = 0; i < slots.size(); ++i){
			std::cout << slots[i].first << ":" << slots[i].second << " | ";
		}
		std::cout << "\n";
	}
};

template <typename KEY, typename VAL>
class ExtendibleHashTable {

    size_t global_depth;
    size_t num_slots;
    size_t num_keys;
    std::vector<int> directory;
	std::vector<HashBucket<KEY, VAL>> buckets;
    std::function<size_t (KEY)> hash_func;

    /* Main hash function used. Initially, the identity function. */
	size_t hash(KEY key) {
        if(hash_func){
            return hash_func(key);
        } else {
            return key;
        }
	}

    /* Split bucket with index. */
    void split_bucket(size_t index) {

        size_t local_depth = buckets[directory[index]].local_depth;
        size_t index1 = index & ((1 << local_depth) - 1);
        size_t index2 = index1 + (1 << local_depth);

        /* Create new bucket for the split. */
        HashBucket<KEY, VAL> new_bucket(local_depth, num_slots);
        buckets.push_back(new_bucket);
        directory[index2] = buckets.size() - 1;

        /* Rehash values for this split bucket. */
        rehash(index1, index2, (1 << local_depth));

        /* Posize_t to correct buckets. */
        for(size_t i = index2 + (2 << local_depth); i < (1 << global_depth); i += (2 << local_depth)){
            directory[i] = directory[index2];
        };
    }

    /* Rehash between buckets with indices. */
	void rehash(size_t index1, size_t index2, size_t mask) {
        HashBucket<KEY, VAL>& bucket1 = buckets[directory[index1]];
        HashBucket<KEY, VAL>& bucket2 = buckets[directory[index2]];

        bucket1.local_depth += 1;
        bucket2.local_depth = bucket1.local_depth;

        std::vector<std::pair<KEY, VAL>> items;
		std::pair<KEY, VAL> item;
		size_t index;

		while (!bucket1.isEmpty()){
			item = bucket1.pop();
			items.push_back(item);
		}

        assert(bucket1.isEmpty());
        assert(bucket2.isEmpty());

		for (size_t i = 0; i < items.size(); ++i){
			item = items[i];

            /* Recompute bucket index, based on mask. */
            if(hash(item.first) & mask){
                index = index2;
            } else {
                index = index1;
            }

            /* Reinsert based on this hash value. */
			buckets[directory[index]].insert(item);
		}
	}

    /* Double directory, and increase global depth by 1. */
    void double_directory() {
        size_t curr_size = directory.size();
        for(size_t i = 0; i < curr_size; ++i){
            directory.push_back(directory[i]);
        }
        global_depth += 1;
    }

	public:
	ExtendibleHashTable(size_t global_depth = 0, size_t num_slots = 3) {

        this -> global_depth = global_depth;
        this -> num_slots = num_slots;
        this -> num_keys = 0;

        size_t num_buckets = 1 << global_depth;
		HashBucket<KEY, VAL> new_bucket(global_depth, num_slots);
        for(size_t i = 0; i < num_buckets; ++i){
            directory.push_back(i);
            (this -> buckets).push_back(new_bucket);
        }
	}

    /* Sets the hash function. */
	void set_hash(const std::function<size_t (KEY)>& func) {
		hash_func = func;
	}

	/* Get bucket index corresponding to this key. */
	size_t get_bucket_index(KEY key) {
		return hash(key) & ((1 << global_depth) - 1);
	}

	/* Insert into the hash table according to the hash function. */
	void insert(KEY key, VAL val) {

        /* Get the index of the bucket we're supposed to insert this key-value pair at. */
		size_t index = get_bucket_index(key);

        /* Check if the bucket is full. */
        if (buckets[directory[index]].isFull()){

            /* If the bucket's local depth matches the global depth, then we have to double the directory first. */
            if(buckets[directory[index]].local_depth == global_depth){
                double_directory();
            }

            /* Split only this bucket. */
            split_bucket(index);

            /* Try inserting again, recursively. */
            insert(key, val);
        } else {

            int old_bucket_size = buckets[directory[index]].size();
            buckets[directory[index]].insert(std::make_pair(key, val));
            int new_bucket_size = buckets[directory[index]].size();

            num_keys += new_bucket_size - old_bucket_size;
        }
	}

    /* Searches for the key in the hash table. Returns the index of the bucket if found, and -1 if not. */
    int search(KEY key) {
        size_t index = get_bucket_index(key);

        if(buckets[directory[index]].count(key)){
            return (int) index;
        } else {
            return -1;
        }
    }

    /* Delete a key from the hash table. */
    void remove(KEY key) {

        /* Key must be present in the hash table. */
        size_t index = search(key);
        assert(index != -1);

        /* Delete from corresponding bucket. */
        buckets[directory[index]].remove(key);
    }

    /* Checks if key is present in the hash table. */
    bool count(KEY key) {
        return (search(key) != -1);
    }

    /* Returns the total number of key-value pairs. */
    size_t size() {
        return num_keys;
    }

	/* Print the hash table. */
	void print(bool print_dir = false) {
        if(print_dir){
            print_directory();
        }

        std::cout << "Global Depth: " << global_depth << "\n";
		for(size_t i = 0; i < buckets.size(); ++i){
			std::cout << "Bucket " << i << " (Local Depth " << buckets[i].local_depth << ") : ";
			buckets[i].print();
		}
	}

    /* Print the hash table directory. */
    void print_directory(){
        std::cout << "Directory:" << "\n";
		for(size_t i = 0; i < directory.size(); ++i){
			std::cout << "dir[" << i << "] = " << directory[i] << "\n";
		}
	}
};

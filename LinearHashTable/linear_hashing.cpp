/*
	Linear Hashing in C++
    Author: Ameya Daigavane
*/

#include <vector>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

template <typename T>
class HashBucket {

	public:
	int num_keys;
	int num_slots;
	vector <T> slots;
	vector <T> overflows;

	HashBucket(int num_slots){
		this -> num_slots = num_slots;
		this -> num_keys = 0;
	}

	/* Insert into this bucket. */
	void insert(T key){
		if(!isFull()){
			slots.push_back(key);
		} else {
			overflows.push_back(key);
		}

		num_keys += 1;

        /* Invariant. */
        assert(num_keys == slots.size() + overflows.size());
	}

	/* Remove the last element in this bucket and return it. */
	T pop(){
		assert(num_keys > 0);

		T key;
		if(num_keys <= num_slots){
			key = slots.back();
			slots.pop_back();
		} else {
			key = overflows.back();
			overflows.pop_back();
		}

        num_keys -= 1;

        /* Invariant. */
        assert(num_keys == slots.size() + overflows.size());

        return key;
	}

    /* Checks if key is present in this bucket. */
    bool count(T key){
        for(int i = 0; i < slots.size(); ++i){
            if(slots[i] == key){
                return true;
            }
        }

        for(int i = 0; i < overflows.size(); ++i){
            if(overflows[i] == key){
                return true;
            }
        }

        return false;
    }

    /* Delete key from bucket. */
    void del(T key){
        assert(count(key));

        /* First check overflow slots. */
        for(int i = 0; i < overflows.size(); ++i){
            if(overflows[i] == key){
                overflows.erase(overflows.begin() + i);
                return;
            }
        }

        /* Then check original slots. */
        for(int i = 0; i < slots.size(); ++i){
            if(slots[i] == key){
                slots.erase(slots.begin() + i);
                return;
            }
        }
    }

	/* Get the number of keys in this bucket. */
	int size(){
		return num_keys;
	}

	/* True if the bucket has no keys. */
	bool isEmpty(){
		return slots.size() == 0;
	}

	/* True if the bucket is full. */
	bool isFull(){
		return num_keys >= num_slots;
	}

	/* Print the contents of this bucket. */
	void print(){
		if(isEmpty()){
			cout << "Empty." << "\n";
            return;
		}

		for(int i = 0; i < slots.size(); ++i){
			cout << slots[i] << " ";
		}

		if(overflows.size() > 0){
			cout << " ";
			cout << "Overflows: ";

			for(int i = 0; i < overflows.size(); ++i){
				cout << overflows[i] << " ";
			}
		}
		cout << "\n";
	}
};

template <typename T>
class LinearHashTable {

	int num_buckets;
	int init_buckets;
	int level;
	int split_index;
	int overflow_limit;
	vector<HashBucket<T>> buckets;

	public:
	LinearHashTable(int num_buckets = 10, int overflow_limit = 3){
		this -> num_buckets = num_buckets;
		this -> init_buckets = num_buckets;
		this -> level = 0;
		this -> split_index = 0;
		this -> overflow_limit = overflow_limit;

		HashBucket<T> new_bucket(overflow_limit);
		for(int i = 0; i < num_buckets; ++i){
			(this -> buckets).push_back(new_bucket);
		}
	}

	/* Main hash function used. Currently identity. */
	int hash(T x){
		return x;
	}

	/* Hash function for each level. */
	int hash_level(T x, int level){
		return hash(x) % split_demarcation(level);
	}

	/* Indicates when to reset split index. */
	int split_demarcation(int level){
		return (1 << level) * init_buckets;
	}

	/* Rehash between buckets with indices. */
	void rehash(int index1, int index2){

		HashBucket<T>& bucket1 = buckets[index1];
		HashBucket<T>& bucket2 = buckets[index2];

		T key;
		int hash_val;

		vector<T> keys;

		while (!bucket1.isEmpty()){
			key = bucket1.pop();
			keys.push_back(key);
		}

        // cout << "Keys to be rehashed: ";
        // for(int i = 0; i < keys.size(); ++i){
        //     cout << keys[i] << " ";
        // }
        // cout << "\n";

		for (int i = 0; i < keys.size(); ++i){
			key = keys[i];

            /* Recompute hash value with higher level hash. */
			hash_val = hash_level(key, level + 1);

            /* Reinsert based on this hash value. */
			buckets[hash_val].insert(key);
		}
	}

	/* Insert into the hash table according to the hash function. */
	void insert(T key){
		int hash_val = hash_level(key, level);

		if(hash_val < split_index){
			hash_val = hash_level(key, level + 1);
		}

		bool overflow = false;
		if(buckets[hash_val].isFull()){
			overflow = true;
		}

        /* Insert into corresponding bucket. */
		buckets[hash_val].insert(key);

        /* Split only if overflow occurs. */
		if(overflow){
			split_bucket();
		}
	}

    /* Searches for the key in the hash table. Returns the index of the bucket if found, and -1 if not. */
    int search(T key){
        int hash_val = hash_level(key, level);

        if(hash_val < split_index){
            hash_val = hash_level(key, level + 1);
        }

        if(buckets[hash_val].count(key)){
            return hash_val;
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
        buckets[index].del(key);
    }

    /* Checks if key is present in the hash table. */
    bool count(T key){
        return (search(key) != -1);
    }

    /* Add a new bucket. */
    void add_bucket(){
        HashBucket<T> new_bucket(overflow_limit);
		buckets.push_back(new_bucket);
        num_buckets += 1;
    }

	/* Split bucket at split_index. */
	void split_bucket(){

        /* Add a new bucket for the split. */
        add_bucket();

        // cout << "Before rehashing: " << "\n";
        // buckets[split_index].print();
        // buckets[buckets.size() - 1].print();

		/* Rehash values for this split bucket. */
		rehash(split_index, buckets.size() - 1);

        // cout << "After rehashing: " << "\n";
        // buckets[split_index].print();
        // buckets[buckets.size() - 1].print();

		/* Reset split index if reached the end for this level, otherwise increment. */
		if(split_index == split_demarcation(level)){
			split_index = 0;
			level += 1;
		} else {
			split_index += 1;
		}
	}

	/* Print the hash table. */
	void print(){
		cout << "Split pointer at bucket " << split_index << ".\n";
		for(int i = 0; i < num_buckets; ++i){
			cout << "Bucket " << i << ": ";
			buckets[i].print();
		}
	}
};

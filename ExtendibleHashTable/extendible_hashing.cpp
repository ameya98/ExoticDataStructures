/*
    Extendible Hashing in C++.
    Author: Ameya Daigavane
*/


#include <vector>
#include <iostream>
#include <cassert>

template <typename T>
class HashBucket {

	public:
	int local_depth;
	int num_slots;
    int num_keys;
	std::vector <T> slots;

	HashBucket(int local_depth, int num_slots){
        this -> local_depth = local_depth;
		this -> num_slots = num_slots;
		this -> num_keys = 0;
	}

	/* Insert into this bucket. */
	void insert(T key){
        /* Should not be full. */
        assert(!isFull());

		slots.push_back(key);
		num_keys += 1;

        /* Invariant. */
        assert(num_keys == slots.size());
	}

	/* Remove the last element in this bucket and return it. */
	T pop(){
		assert((num_keys > 0) && (num_keys <= num_slots));

		T key;
		key = slots.back();
		slots.pop_back();
        num_keys -= 1;

        /* Invariant. */
        assert(num_keys == slots.size());

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
		return num_keys;
	}

	/* True if the bucket has no keys. */
	bool isEmpty(){
		return slots.size() == 0;
	}

	/* True if the bucket is full. */
	bool isFull(){
		return num_keys == num_slots;
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
	int num_buckets;
    int num_slots;
	std::vector<HashBucket<T>> buckets;

	public:
	ExtendibleHashTable(int global_depth = 0, int num_slots = 3){
        this -> global_depth = global_depth;
		this -> num_buckets = 1 << global_depth;

		HashBucket<T> new_bucket(global_depth, num_slots);
        for(int i = 0; i < num_buckets; ++i){
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
        HashBucket<T>& bucket1 = buckets[index1];
        HashBucket<T>& bucket2 = buckets[index2];

        bucket1.local_depth += 1;
        bucket2.local_depth = bucket1.local_depth;

		T key;
		int index;

		std::vector<T> keys;

		while (!bucket1.isEmpty()){
			key = bucket1.pop();
			keys.push_back(key);
		}

        // std::cout << "Keys to be rehashed: ";
        // for(int i = 0; i < keys.size(); ++i){
        //     std::cout << keys[i] << " ";
        // }
        // std::cout << "\n";

		for (int i = 0; i < keys.size(); ++i){
			key = keys[i];

            /* Recompute bucket index, based on mask. */
            if(hash(key) & mask){
                index = index2;
            } else {
                index = index1;
            }

            /* Reinsert based on this hash value. */
			buckets[index].insert(key);
		}
	}

    /* Double buckets (directory), and increase global depth by 1. */
    void double_buckets(){
        std::vector<HashBucket<T>> temp = buckets;
        temp.insert(temp.end(), buckets.begin(), buckets.end());
        buckets = temp;
        global_depth += 1;
        num_buckets *= 2;
    }

	/* Insert into the hash table according to the hash function. */
	void insert(T key){
		int index = get_bucket(key);

        if (buckets[index].isFull()){
            if(buckets[index].local_depth == global_depth){
                double_buckets();
            }
            split_bucket(index);
            insert(key);
        } else {
            buckets[index].insert(key);
        }
	}

    /* Searches for the key in the hash table. Returns the index of the bucket if found, and -1 if not. */
    int search(T key){
        int index = get_bucket(key);

        if(buckets[index].count(key)){
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
        buckets[index].del(key);
    }

    /* Checks if key is present in the hash table. */
    bool count(T key){
        return (search(key) != -1);
    }

	/* Split bucket with index. */
	void split_bucket(int index){

        int local_depth = buckets[index].local_depth;
        int index1 = index & ((1 << local_depth) - 1);
        int index2 = index + (1 << local_depth);

        // std::cout << "Before rehashing: " << "\n";
        // buckets[split_index].print();
        // buckets[buckets.size() - 1].print();

		/* Rehash values for this split bucket. */
		rehash(index1, index2, (1 << local_depth));

        // std::cout << "After rehashing: " << "\n";
        // buckets[split_index].print();
        // buckets[buckets.size() - 1].print();

		/* Point to correct buckets. */
        for(int i = index2 + (2 << local_depth); i < (1 << global_depth); i += (2 << local_depth)){
            buckets[i] = buckets[index2];
        };

	}

	/* Print the hash table. */
	void print(){
		for(int i = 0; i < num_buckets; ++i){
			std::cout << "Bucket " << i << ": ";
			buckets[i].print();
		}
	}
};

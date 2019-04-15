/*
Skiplists in C++
Author: Ameya Daigavane
Date: 15th May 2018
A C++ implementation of skiplists, a randomized data structure supporting logarithmic expected-time insert, search and delete operations.
*/

#include <stdexcept>
#include <limits>
#include <cstdlib>
#include <ctime>

// The max-height of the skiplist.
// Note: the skiplist's bottomost list is at height 0
const size_t skiplist_max_height = 50;

// Note: range of values the int skiplist supports is (-INT_MIN, INT_MAX)
template <typename KEY, typename VAL>
struct skiplist_node {

    KEY key;
    VAL val;
    size_t height;
    skiplist_node ** next;

    skiplist_node(KEY inp_key, VAL inp_val, size_t inp_height){
        key = inp_key;
        val = inp_val;
        height = inp_height;
        next = new skiplist_node* [height + 1];
    }

};

template <typename TYPE>
class Skiplist;

template <typename TYPE>
std::ostream& operator<< (std::ostream& os, const Skiplist<TYPE>& sl);

template <typename TYPE>
class Skiplist {

  private:
    // Sentinel nodes - start and end.
    skiplist_node<TYPE> * start;
    skiplist_node<TYPE> * end;

    // Number of nodes/items currently stored
    size_t skiplist_size;

    // search_helper(val, min_level):
    // if val is found in the levels >= minlevel, returns a pointer to the node with value val,
    // else, returns a pointer to the node just before where it should be.
    //
    // Idea:
    // If current key == val, done.
    // Go right if next key <= val
    // Go down if next key > val
    skiplist_node<TYPE> * search_helper(TYPE val, size_t min_level) {

        skiplist_node<TYPE> * curr;
        skiplist_node<TYPE> * next;
        size_t curr_height = skiplist_max_height;

        curr = start;
        next = curr -> next[curr_height];

        while(curr -> val != val){
            // next value is also lesser, we can go right
            if(next -> val <= val){
                curr = next;
                next = curr -> next[curr_height];
            }
            // next value is smaller, go to lower list
            else{
                if(curr_height > min_level){
                    curr_height -= 1;
                    next = curr -> next[curr_height];
                }
                else {
                    break;
                }
            }
        }

        return curr;
    }

    // Insert val from levels 0 upto level.
    void insert_upto_level(TYPE val, size_t level) {

        skiplist_node<TYPE> * curr;
        skiplist_node<TYPE> * new_node = new skiplist_node<TYPE>(val, level);

        for(size_t i = 0; i <= level; ++i){
            curr = search_helper(val, i);
            new_node -> next[i] = curr -> next[i];
            curr -> next[i] = new_node;
        }
    }

  public:
    Skiplist() {

        // Seed rand() according to the current time.
        srand(time(NULL));

        // Initialize the sentinel nodes.
        start = new skiplist_node<TYPE>(std::numeric_limits<TYPE>::min(), skiplist_max_height);
        end = new skiplist_node<TYPE>(std::numeric_limits<TYPE>::max(), skiplist_max_height);

        for(size_t i = 0; i <= skiplist_max_height; ++i)
        {
            start -> next[i] = end;
        }

        for(size_t i = 0; i <= skiplist_max_height; ++i)
        {
            end -> next[i] = NULL;
        }

        // Initialize size.
        skiplist_size = 0;
    }

    // Search for a value.
    bool search(TYPE val){
        if (search_helper(val, 0) -> val == val) return true;
        else return false;
    }

    // Insert a value.
    void insert(TYPE val){

        if(val == std::numeric_limits<TYPE>::min()){
            throw std::invalid_argument("Cannot insert std::numeric_limits<TYPE>::min() as key. Use any other value.");
        }

        size_t level = 0;
        // keep flipping a coin!
        while (level < skiplist_max_height and rand() % 2 == 1){
            level += 1;
        }

        insert_upto_level(val, level);
        skiplist_size += 1;
    }

    // Remove a value - one copy only.
    void remove(TYPE val){

        skiplist_node<TYPE> * curr = search_helper(val, 0);

        // val exists in the skiplist.
        if(curr -> val == val){

            int height = curr -> height;
            int curr_height = height;
            skiplist_node<TYPE> * prev[height + 1];
            skiplist_node<TYPE> * prev_node;

            prev_node = start;
            while(curr_height >= 0){

                while(prev_node -> next[curr_height] != curr){
                    prev_node = prev_node -> next[curr_height];
                }

                while((curr_height >= 0) and (prev_node -> next[curr_height]  == curr)){
                    prev[curr_height] = prev_node;
                    curr_height -= 1;
                }
            }

            // Update the previous nodes at every level.
            for(size_t i = 0; i <= height; ++i){
                prev[i] -> next[i] = curr -> next[i];
            }

            delete curr;
            skiplist_size -= 1;

        }
        // val is not present in the skiplist
        else throw std::invalid_argument("Value not found in skiplist. Cannot delete.");
    }

    size_t size(){
        return skiplist_size;
    }

    friend std::ostream& operator<< <>(std::ostream& os, const Skiplist& sl);

};

// Overload the << operator for printing
template <typename TYPE>
std::ostream &operator<<(std::ostream &os, Skiplist<TYPE> const &sl)
{
    skiplist_node<TYPE> * curr;

    os << "---\n";
    for(size_t level = 0; level <= skiplist_max_height; ++level)
    {
        os << "Skiplist Level " << level << ":" << '\n';

        curr = sl.start -> next[level];
        if(curr == sl.end)
        {
            os << "Skiplist empty from here and above." << "\n---\n";
            break;
        }

        while(curr != sl.end)
        {
            os << curr -> val << " ";
            curr = curr -> next[level];
        }

        os << "\n\n";
    }

    return os;
}

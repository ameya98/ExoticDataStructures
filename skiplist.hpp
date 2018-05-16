/*
Skiplists in C++
Author: Ameya Daigavane
Date: 15th May 2018
A C++ implementation of skiplists, a randomized data structure.
*/

#include <stdexcept>
#include <climits>
#include <cstdlib>
#include <ctime>

const int skiplist_max_height = 50;
const int infinity = INT_MAX;
const int neg_infinity = INT_MIN;

// note: range of values the int skiplist supports is (-INT_MIN, INT_MAX)
template <typename TYPE>
struct skiplist_node
{
    TYPE val;
    int height;
    skiplist_node ** next;

    skiplist_node(TYPE inp_val, int inp_height)
    {
        val = inp_val;
        height = inp_height;
        next = new skiplist_node * [height + 1];
    }

};

template <class TYPE>
class skiplist;

template<typename TYPE>
std::ostream& operator<<(std::ostream& os, const skiplist<TYPE>& sl);

template <class TYPE>
class skiplist
{
private:
    // sentinel nodes - start and end.
    skiplist_node<TYPE> * start;
    skiplist_node<TYPE> * end;

    // number of nodes/items currently stored
    int skiplist_size;

    // search_helper(val, min_level), if val is found in the levels >= minlevel, returns a pointer to the node with value val,
    // else, returns a pointer to the node just before where it should be.
    // Idea:
    // If current key == val, done.
    // Go right if next key <= val
    // Go down if next key > val
    skiplist_node<TYPE> * search_helper(TYPE val, int min_level)
    {
        skiplist_node<TYPE> * curr;
        skiplist_node<TYPE> * next;
        int curr_height = skiplist_max_height;

        curr = start;
        next = curr -> next[curr_height];

        while(curr -> val != val)
        {
            // next value is also lesser, we can go right
            if(next -> val <= val)
            {
                curr = next;
                next = curr -> next[curr_height];
            }
            // next value is smaller, go to lower list
            else
            {
                if(curr_height > min_level)
                {
                    curr_height -= 1;
                    next = curr -> next[curr_height];
                }
                else
                {
                    break;
                }
            }
        }

        return curr;
    }

    // insert val from levels 0 upto 'level'
    void insert_upto_level(TYPE val, int level)
    {
        skiplist_node<TYPE> * curr;
        skiplist_node<TYPE> * new_node = new skiplist_node<TYPE>(val, level);

        for(int i = 0; i <= level; ++i)
        {
            curr = search_helper(val, i);
            new_node -> next[i] = curr -> next[i];
            curr -> next[i] = new_node;
        }
    }

public:
    skiplist()
    {
        // seed rand() according to the time
        srand(time(NULL));

        // initialize the sentinel nodes
        start = new skiplist_node<TYPE>(neg_infinity, skiplist_max_height);
        end = new skiplist_node<TYPE>(infinity, skiplist_max_height);

        for(int i = 0; i <= skiplist_max_height; ++i)
        {
            start -> next[i] = end;
        }

        for(int i = 0; i <= skiplist_max_height; ++i)
        {
            end -> next[i] = NULL;
        }

        // initialize size
        skiplist_size = 0;
    }

    // search for a value
    bool search(TYPE val)
    {
        if (search_helper(val, 0) -> val == val) return true;
        else return false;

    }

    void insert(TYPE val)
    {
        int level = 0;
        // keep flipping a coin!
        while (level < skiplist_max_height and rand() % 2 == 1)
        {
            level += 1;
        }

        insert_upto_level(val, level);
        skiplist_size += 1;
    }

    void remove(TYPE val)
    {
        skiplist_node<TYPE> * curr = search_helper(val, 0);

        // val exists in the skiplist
        if(curr -> val == val)
        {
            int height = curr -> height;
            skiplist_node<TYPE> * prev;

            for(int i = 0; i <= height; ++i)
            {
                prev = start;
                while(prev -> next[i] != curr)
                {
                    prev = prev -> next[i];
                }

                prev -> next[i] = curr -> next[i];
            }

            delete curr;
            skiplist_size -= 1;
        }
        // val is not present in the skiplist
        else
        {
            throw std::invalid_argument("value not found in skiplist - cannot delete.");
        }
    }

    int size()
    {
        return skiplist_size;
    }

    friend std::ostream& operator<< <>(std::ostream& os, const skiplist<TYPE>& sl);

};

// overload the << operator for printing
template <typename TYPE>
std::ostream &operator<<(std::ostream &os, skiplist<TYPE> const &sl)
{
    skiplist_node<TYPE> * curr;

    os << "---\n";
    for(int level = 0; level <= skiplist_max_height; ++level)
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

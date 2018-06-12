/*
Author: Ameya Daigavane
Date: 11th June, 2018
A C++ implementation of a Fenwick Tree (a Binary Indexed Tree) that allows logarithmic time range-sum queries, point queries, as well as range and point updates.
*/

#include <stdexcept>

template <typename TYPE>
class FenwickTree
{
    private:
        TYPE * add;
        TYPE * mul;
        int length;

        // performs the actual range update
        void range_update_helper(int pos, TYPE mulfact, TYPE addfact)
        {
            pos += 1;
            while(pos <= length)
            {
                mul[pos - 1] += mulfact;
                add[pos - 1] += addfact;
                pos += pos & -pos;
            }
        }

    public:
        // constructor with vector
        FenwickTree(std::vector<TYPE> v)
        {
            length = v.size();
            add = new TYPE[length];
            mul = new TYPE[length];

            // initialize
            for(int i = 0; i < length; ++i)
            {
                add[i] = 0;
                mul[i] = 0;
            }

            // point-wise updates
            for(int i = 0; i < length; ++i)
            {
                point_update(i, v[i]);
            }
        }

        // constructor with an array v[] of given size input_length
        FenwickTree(TYPE v[], int input_length)
        {
            length = input_length;
            add = new TYPE[length];
            mul = new TYPE[length];

            // initialize
            for(int i = 0; i < length; ++i)
            {
                add[i] = 0;
                mul[i] = 0;
            }

            // point-wise updates
            for(int i = 0; i < length; ++i)
            {
                point_update(i, v[i]);
            }
        }

        // add value 'val' to position i in a[0..length - 1]
        void point_update(int i, TYPE val)
        {
            range_update(i, i, val);
        }

        // add value 'val' to postions a[i..j]
        void range_update(int i, int j, TYPE val)
        {
            if(i < 0 or j < i or j >= length)
            {
                throw std::invalid_argument("subarray range invalid.");
            }
            else
            {
                range_update_helper(i, val, -val * (i - 1));
                range_update_helper(j, -val, val * j);
            }
        }

        // find sum of range a[i..j] (both inclusive)
        TYPE range_sum(int i, int j)
        {
            if(i < 0 or j < i or j >= length)
            {
                throw std::invalid_argument("subarray range invalid.");
            }
            else
            {
                return subarray_sum(j) - subarray_sum(i - 1);
            }
        }

        // find sum of range a[0..i] (both inclusive)
        TYPE subarray_sum(int i)
        {
            TYPE mulfact = 0;
            TYPE addfact = 0;

            TYPE startindex = i;
            i += 1;

            while(i > 0)
            {
                addfact += add[i - 1];
                mulfact += mul[i - 1];
                i = i & (i - 1);
            }

            return startindex * mulfact + addfact;
        }

        // returns the value of a[i]
        TYPE point_sum(int i)
        {
            if(i < 0 or i >= length)
            {
                throw std::invalid_argument("invalid array index.");
            }
            else
            {
                return range_sum(i, i);
            }
        }

};

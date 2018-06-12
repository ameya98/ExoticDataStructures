/*
Author: Ameya Daigavane
Date: 11th June, 2018
A C++ implementation Fenwick Tree (or a Binary Indexed Tree) that allows logarithmic time range sum queries and point-wise updates.
*/

#include <stdexcept>
class FenwickTree
{
    private:
        int * add;
        int length;

    public:
        // constructor with vector
        FenwickTree(std::vector<int> v)
        {
            length = v.size();
            add = new int[length];

            // initialize
            for(int i = 0; i < length; ++i)
            {
                add[i] = 0;
            }

            // point-wise updates
            for(int i = 0; i < length; ++i)
            {
                point_update(i, v[i]);
            }
        }

        // constructor with an array
        FenwickTree(int v[], int input_length)
        {
            length = input_length;
            add = new int[length];

            // initialize
            for(int i = 0; i < length; ++i)
            {
                add[i] = 0;
            }

            // point-wise updates
            for(int i = 0; i < length; ++i)
            {
                point_update(i, v[i]);
            }
        }

        // add value 'val' to position i in a[0..length - 1]
        void point_update(int i, int val)
        {
            i += 1;
            while(i <= length)
            {
                add[i - 1] += val;
                i += i & -i;
            }
        }

        // find sum of range a[i..j] (both inclusive)
        int range_sum(int i, int j)
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
        int subarray_sum(int i)
        {
            int sum = 0;
            i += 1;

            while(i > 0)
            {
                sum += add[i - 1];
                i = i & (i - 1);
            }

            return sum;
        }

        // returns the value of a[i]
        int point_sum(int i)
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

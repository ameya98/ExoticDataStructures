/*
Skiplists in C++: A Driver Program
Author: Ameya Daigavane
Date: 15th May 2018
A driver program for the 'skiplist.hpp' library.
*/

#include <iostream>
#include <cstdlib>
#include "skiplist.hpp"

using namespace std;
int main()
{
    skiplist<int> sl_integer;
    skiplist<double> sl_double;

    // insert a 100 random integers in the range 0 to 999.
    int integers[100];
    double doubles[100];
    for(int i = 0; i < 100; ++i)
    {
        integers[i] = (rand() % 1000);
        doubles[i] = (double) rand() / (double) RAND_MAX;

        sl_integer.insert(integers[i]);
        sl_double.insert(doubles[i]);
    }

    // print the list - the '<<' operator has been overloaded!
    cout << sl_integer;
    cout << sl_double;

    // delete the first 50 integers
    for(int i = 0; i < 50; ++i)
    {
        sl_integer.remove(integers[i]);
    }

    // how many elements in our skiplist now?
    cout << sl_integer.size() << " elements in skiplist currently." << '\n';

    // search for the 51st integer
    if(sl_integer.search(integers[50]))
    {
        cout << integers[50] << " is in the skiplist." << '\n';
    }
    else
    {
        cout << "Something's wrong." << '\n';
    }

    // print the skiplist again
    cout << sl_integer;

    // delete all the remaining integers
    for(int i = 50; i < 100; ++i)
    {
        sl_integer.remove(integers[i]);
    }

    // print the empty skiplist
    cout << sl_integer;

}

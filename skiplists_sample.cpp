/*
Skiplists in C++: A Driver Program
Author: Ameya Daigavane
Date: 15th May 2018
A driver program for the 'skiplist.hpp' library.
*/

#include <iostream>
#include "skiplist.hpp"

using namespace std;
int main()
{
    skiplist sl;

    // insert a 100 random integers in the range 0 to 999.
    int a[100];
    for(int i = 0; i < 100; ++i)
    {
        a[i] = (rand() % 1000);
        sl.insert(a[i]);
    }

    // print the list - the '<<' operator has been overloaded!
    cout << sl;

    // delete the first 50 integers
    for(int i = 0; i < 50; ++i)
    {
        sl.remove(a[i]);
    }

    // how many elements in our skiplist now?
    cout << sl.size() << " elements in skiplist currently." << '\n';

    // search for the 51st integer
    if(sl.search(a[50]))
    {
        cout << a[50] << " is in the skiplist." << '\n';
    }
    else
    {
        cout << "Something's wrong." << '\n';
    }

    // print the skiplist again
    cout << sl;

    // delete all the remaining integers
    for(int i = 50; i < 100; ++i)
    {
        sl.remove(a[i]);
    }

    // print the empty skiplist
    cout << sl;

}

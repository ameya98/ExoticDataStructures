# ExoticDataStructures
Exotic data-structures in C++14.

## Skiplists
A randomised data-structure with logarithmic expected-time for search, delete and insert operations.  
Use the library in your own source code with:
```cpp
#include "skiplist.hpp"
```
and then, declare a skiplist with:
```cpp
skiplist sl;
```
Currently, only int elements supported.
### Class Methods
Insertion:  __void insert(int val)__
```cpp
sl.insert(18);
```
Deletion:  __void remove(int val)__
```cpp
sl.remove(11);
```
Search:  __void search(int val)__
```cpp
sl.search(29);
```
Size:  __void size()__
```cpp
sl.size();
```

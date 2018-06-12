# ExoticDataStructures
Exotic data-structures in C++14. Supported for suitable types through templates.  
Currently implemented here:
* Fenwick Trees (Binary Indexed Trees)
* Skiplists

## Fenwick Trees
A flat-array based tree that allows calculation of prefix and subarray sums in logarithmic time.
Supports point-based and range-based updates (increase every element in a[i..j] by a certain value) in logarithmic time as well.  
Use the library in your own source code with:
```cpp
#include "fenwicktree.hpp"
```
There are two options to initialize the Fenwick Tree:
* Declare a __std::vector__ of type _typename_:
```cpp
std::vector<typename> v = {1, 5, 7, 8};
FenwickTree<typename> ft(v);
```
or
* Declare a standard array of type _typename_:  
```cpp
typename a[length] = {1, 2, 8};
FenwickTree<typename> ft(a, length);
```
Input the values in the containers before initializing the FenwickTree, or alternatively use the _point_update()_ method to fill in the values.  Note that the Fenwick Tree cannot change size once initialized, however.  
The "+" operator must be defined for _typename_.   
The standard types:  _int_, _double_, _float_, _long_ and _long long_ are all supported.

### Class Methods
In the descriptions below, the array a[..] represents the array over which sums and updates occur.  
This is the array to which the Fenwick Tree must be associated with, with the constructor above.

#### Updates
#### Range Update:  _void range_update(size_t left, size_t right, typename val)_
Increase all values in a[left..right], endpoints inclusive, by val.
```cpp
ft.range_update(0, 3, 2);
```
#### Point Update:  _void point_update(size_t pos, typename val)_
Increase a[pos] by val.
```cpp
ft.point_update(1);
```

#### Sums
#### Range Sum:  _typename range_sum(size_t left, size_t right)_
Returns the sum of all elements within the range a[left..right], endpoints inclusive.
```cpp
ft.range_sum(0, 3);
```

#### Prefix Sum:  _typename prefix_sum(size_t pos)_
Returns the sum of all elements within the range a[0..pos], endpoints inclusive.
```cpp
ft.prefix_sum(2);
```

#### Point Sum:  _typename point_sum(size_t pos)_
Returns the current value of a[pos].
```cpp
ft.point_sum(4);
```


## Skiplists
A randomized data-structure with logarithmic expected-time for search, delete and insert operations.  
Use the library in your own source code with:
```cpp
#include "skiplist.hpp"
```
and then, declare a skiplist of type _typename_ with:
```cpp
Skiplist<typename> sl;
```
Note,  _typename_ must be one of the types defined in __std::numeric_limits__, which includes the common _int_, _double_, _float_, _long_ and _long long_ datatypes.

### Class Methods
#### Insertion:  _void insert(int val)_
```cpp
sl.insert(18);
```
#### Deletion:  _void remove(int val)_
```cpp
sl.remove(11);
```
#### Search:  _bool search(int val)_
```cpp
sl.search(29);
```
#### Size:  _size_t size()_
```cpp
sl.size();
```

The '<<' operator has been overloaded, so you can do this:
```cpp
std::cout << sl;
```
to show the contents of the skiplist.

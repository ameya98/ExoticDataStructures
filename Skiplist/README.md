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

At this moment, __std::numeric_limits__<_typename_>::_min()_ cannot be used as a key, for example, INT_MIN when using int.

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

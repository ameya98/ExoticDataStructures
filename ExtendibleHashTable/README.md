## Extendible Hash Tables
Extendible Hash Tables are dynamic hashing structures that resize with the insertion of more data. Rehashing all keys on resizing would be too expensive. Extendible hash tables approach this by only splitting a bucket that has overflowed, rehashing keys from this bucket to split the keys between the old and split bucket. The directory maintains for every bucket which keys should be placed into them, based on the last digits of the hash. How many digits exactly is controlled by the _local_depth_ parameter of the bucket. The _global_depth_ parameter is the maximum over all bucket _local_depth_ parameters.

For a more thorough explanation of extendible hashing, see the original [paper](https://dl.acm.org/citation.cfm?doid=320083.320092) by R. Fagin et al.

Use the library in your own source code with:
```cpp
#include "extendible_hashing.hpp"
```
and then, declare a hashtable of key type _KEY_ and value type _VAL_ with:
```cpp
ExtendibleHashTable<KEY, VAL> eht(global_depth_initial, num_slots_per_bucket);
```
By default, the initial global depth and number of slots per bucket are set as 0 and 3 respectively, if not passed to the constructor above.

### Class Methods
The following examples assume a declaration of the form:
```cpp
ExtendibleHashTable<int, string> eht;
```

Note that duplicate keys are not supported. Previous values are overwritten when _insert()_ is called on a key already present.

#### Insertion:  _void insert(KEY key, VAL val)_
```cpp
eht.insert(18, "ameya");
```
#### Deletion:  _void remove(KEY key)_
```cpp
eht.remove(18);
```
This causes a runtime assertion failure if the key is not present in the hashtable.
#### Search:  _int search(KEY key)_
```cpp
eht.search(18);
```
Returns the bucket index (a nonnegative value) corresponding to the key if found. Otherwise, returns -1.
#### Count:  _bool count(KEY key)_
```cpp
eht.count(18);
```
Returns true if the key is present in the hashtable, false otherwise.
#### Size:  _size_t size()_
```cpp
eht.size();
```
Returns the number of keys present in the hashtable.
#### Printing: _void print(bool print_dir = false)_
```cpp
eht.print();
```
Prints the contents of the hashtable, bucket-by-bucket. Passing true would print the directory structure as well. Printing just the directory structure can be performed with _print_directory()_.

#### Custom Hash Functions: _void set_hash(const std::function<size_t (KEY)>& func)_
An additional feature is the ability to set your own custom hash function!
Declare a function returning a _size_t_ value of the form:
```cpp
size_t custom_hash(KEY k){
    ....
}
```

Then,
```cpp
eht.set_hash(custom_hash);
```
will change the hash function used by the hashtable. However, only call this when there are no keys present in the hashtable - for example, right after the declaration of the hashtable. Otherwise, a runtime assertion will fail.

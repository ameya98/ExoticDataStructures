## Extendible Hash Tables
Extendible Hash Tables are dynamic hashing structures that resize with the insertion of more data. Rehashing all keys on resizing would be too expensive. Extendible hash tables approach this by only splitting a bucket that has overflowed, rehashing keys from this bucket to split the keys between the old and split bucket. The directory maintains for every bucket which keys should be placed into them, based on the last digits of the hash. How many digits is controlled by the _local_depth_ parameter of the bucket.

For a more thorough explanation of extendible hashing, see the original [paper](https://dl.acm.org/citation.cfm?doid=320083.320092) by R. Fagin et al.

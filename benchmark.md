# v1

```
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          2.809809 s          35.589608 Mop/s
hash_map insert               7.430581 s          13.457897 Mop/s

unordered_map contains        420.021916 ms       238.082815 Mop/s
hash_map contains             320.732333 ms       311.786464 Mop/s

unordered_map at              160.887459 ms       621.552485 Mop/s
hash_map at                   439.573209 ms       227.493391 Mop/s

unordered_map erase           6.305776 s          15.858475 Mop/s
hash_map erase                446.482583 ms       223.972902 Mop/s
```
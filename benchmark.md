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

# v2

```
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          2.341352 s          42.710369 Mop/s
hash_map insert               8.348306 s          11.978478 Mop/s

unordered_map contains        162.986667 ms       613.547119 Mop/s
hash_map contains             178.241417 ms       561.036832 Mop/s

unordered_map at              167.293000 ms       597.753642 Mop/s
hash_map at                   205.411875 ms       486.826772 Mop/s

unordered_map erase           6.723283 s          14.873685 Mop/s
hash_map erase                289.002875 ms       346.017319 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  29.068907 s         3.440102 Mop/s
hash_map<string> insert       98.742848 s         1.012732 Mop/s
```

# v3

```
N = 10M
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          199.885709 ms       50.028589 Mop/s
hash_map insert               129.520875 ms       77.207632 Mop/s

unordered_map contains        15.833084 ms        631.588893 Mop/s
hash_map contains             9.182500 ms         1.089028 Gop/s

unordered_map at              15.537250 ms        643.614539 Mop/s
hash_map at                   9.196000 ms         1.087429 Gop/s

unordered_map erase           151.968167 ms       65.803255 Mop/s
hash_map erase                10.560750 ms        946.902445 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  2.149721 s          4.651767 Mop/s
hash_map<string> insert       959.896583 ms       10.417789 Mop/s

unordered_map<string> contains741.275209 ms       13.490266 Mop/s
hash_map<string> contains     539.308917 ms       18.542249 Mop/s

unordered_map<string> at      764.683833 ms       13.077300 Mop/s
hash_map<string> at           547.965667 ms       18.249319 Mop/s

unordered_map<string> erase   2.018851 s          4.953314 Mop/s
hash_map<string> erase        566.344416 ms       17.657100 Mop/s

N = 100M
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          1.833356 s          54.544790 Mop/s
hash_map insert               3.399212 s          29.418580 Mop/s

unordered_map contains        207.086458 ms       482.890098 Mop/s
hash_map contains             119.345708 ms       837.901938 Mop/s

unordered_map at              208.925000 ms       478.640661 Mop/s
hash_map at                   117.047458 ms       854.354308 Mop/s

unordered_map erase           1.579168 s          63.324490 Mop/s
hash_map erase                128.832125 ms       776.203917 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  22.997923 s         4.348219 Mop/s
hash_map<string> insert       71.971958 s         1.389430 Mop/s
```

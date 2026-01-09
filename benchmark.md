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

# v4

```
N = 10M
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          1.659672 s          6.025286 Mop/s
hash_map insert               674.187500 ms       14.832669 Mop/s

unordered_map contains        330.700916 ms       30.238803 Mop/s
hash_map contains             210.409375 ms       47.526399 Mop/s

unordered_map at              318.021250 ms       31.444440 Mop/s
hash_map at                   146.911708 ms       68.068094 Mop/s

unordered_map erase           1.779606 s          5.619220 Mop/s
hash_map erase                179.716333 ms       55.643245 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  2.221157 s          4.502159 Mop/s
hash_map<string> insert       1.034762 s          9.664055 Mop/s

unordered_map<string> contains775.430083 ms       12.896069 Mop/s
hash_map<string> contains     387.937208 ms       25.777368 Mop/s

unordered_map<string> at      798.094625 ms       12.529843 Mop/s
hash_map<string> at           409.162500 ms       24.440167 Mop/s

unordered_map<string> erase   1.768466 s          5.654617 Mop/s
hash_map<string> erase        457.653375 ms       21.850598 Mop/s

N = 100M
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          17.425751 s         5.738634 Mop/s
hash_map insert               8.508454 s          11.753017 Mop/s

unordered_map contains        3.590102 s          27.854358 Mop/s
hash_map contains             2.100415 s          47.609649 Mop/s

unordered_map at              3.590789 s          27.849034 Mop/s
hash_map at                   1.983101 s          50.426085 Mop/s

unordered_map erase           20.230912 s         4.942931 Mop/s
hash_map erase                2.248018 s          44.483636 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  25.058368 s         3.990683 Mop/s
hash_map<string> insert       16.585449 s         6.029382 Mop/s

unordered_map<string> contains8.440034 s          11.848293 Mop/s
hash_map<string> contains     4.913465 s          20.352235 Mop/s

unordered_map<string> at      8.511603 s          11.748669 Mop/s
hash_map<string> at           5.170944 s          19.338827 Mop/s

unordered_map<string> erase   20.061128 s         4.984764 Mop/s
hash_map<string> erase        5.390873 s          18.549870 Mop/s
```

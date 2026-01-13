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
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          4.601822 s          2.173053 Mop/s
hash_map insert               1.075015 s          9.302195 Mop/s

unordered_map contains        3.554295 s          2.813497 Mop/s
hash_map contains             465.127292 ms       21.499491 Mop/s

unordered_map at              3.623374 s          2.759859 Mop/s
hash_map at                   457.757042 ms       21.845650 Mop/s

unordered_map erase           4.436984 s          2.253783 Mop/s
hash_map erase                488.688791 ms       20.462921 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  5.660587 s          1.766601 Mop/s
hash_map<string> insert       3.167850 s          3.156715 Mop/s

unordered_map<string> contains3.396400 s          2.944294 Mop/s
hash_map<string> contains     1.947289 s          5.135343 Mop/s

unordered_map<string> at      3.588880 s          2.786385 Mop/s
hash_map<string> at           1.936490 s          5.163982 Mop/s

unordered_map<string> erase   4.572494 s          2.186990 Mop/s
hash_map<string> erase        1.966515 s          5.085139 Mop/s
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

# v5

```
N = 10M
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          1.663693 s          6.010723 Mop/s
hash_map insert               326.313166 ms       30.645408 Mop/s

unordered_map contains        317.982375 ms       31.448284 Mop/s
hash_map contains             153.998333 ms       64.935768 Mop/s

unordered_map at              305.224458 ms       32.762774 Mop/s
hash_map at                   131.961542 ms       75.779654 Mop/s

unordered_map erase           1.694110 s          5.902804 Mop/s
hash_map erase                164.099917 ms       60.938483 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  2.135181 s          4.683444 Mop/s
hash_map<string> insert       676.896875 ms       14.773299 Mop/s

unordered_map<string> contains764.325667 ms       13.083428 Mop/s
hash_map<string> contains     511.162167 ms       19.563263 Mop/s

unordered_map<string> at      759.795958 ms       13.161428 Mop/s
hash_map<string> at           574.450125 ms       17.407952 Mop/s

unordered_map<string> erase   1.692597 s          5.908080 Mop/s
hash_map<string> erase        442.353625 ms       22.606348 Mop/s

N=100M
Operation                     Time                Throughput
----------------------------------------------------------------------
unordered_map insert          18.403746 s         5.433676 Mop/s
hash_map insert               4.682977 s          21.353939 Mop/s

unordered_map contains        3.647120 s          27.418899 Mop/s
hash_map contains             1.845463 s          54.186949 Mop/s

unordered_map at              3.886949 s          25.727120 Mop/s
hash_map at                   1.729194 s          57.830407 Mop/s

unordered_map erase           20.098529 s         4.975488 Mop/s
hash_map erase                2.122788 s          47.107867 Mop/s

========== STRING KEY BENCHMARKS ==========

unordered_map<string> insert  29.088983 s         3.437728 Mop/s
hash_map<string> insert       12.178048 s         8.211497 Mop/s

unordered_map<string> contains8.436851 s          11.852763 Mop/s
hash_map<string> contains     6.113648 s          16.356845 Mop/s

unordered_map<string> at      8.479677 s          11.792902 Mop/s
hash_map<string> at           7.555446 s          13.235487 Mop/s

unordered_map<string> erase   19.965768 s         5.008573 Mop/s
hash_map<string> erase        5.753715 s          17.380075 Mop/s
```

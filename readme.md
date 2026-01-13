# I have made a serious attempt to make a hash map

## v1. 
naive chaining hash map, works well enough, did not beat unordered map
## v2. 
still chaining, attempted to store hashes so I don't have to recomupte. Slower on benchmarks, but probably because the string benchmark used were small strings and integers. I will be circling back to this idea later

### v3. 
flat map! Note that I am choosing to use a C-style array over std::vector not because I think it is better, but because I want to have full control and actually resize the array. Also note the convenience of using type aliasing and good programming practices, very little has to change when changing the underlying data structure.

Now outperforms unordered_map on datasets of size n <= 10M, still struggles with >=100M, most likely due to overhead of storing hashes.

## v4
Getting serious now - I am using specilizations for slots based on type, only storing the hash if it is a non numeric type, as they hash in O(1) anyway.

I am also changing the hashing algorithm, again with two separate algorithms for numeric and non-numeric types. This version requires rapidhash, so I have included it as is

Note that the numeric hashing algorithm IS SLOWER than what the older versions use however it aviod clustering, so at larger workloads and on random number values it is significantly better :)

I have also switched to using random values for benchmarking, which does technically invalidate other benchmarks, but I am going to let it go because they were worse anyway, so doesn't really matter.

This marks my final version, as I have now outperformed unordered_map in all benchmarks at N=100M, which was my goal to begin with. I understand that there are multiple ways to improve beyond what I have done, but I have acheived my original target of outperforming unordered_map.

## v5.1

I lied before, I kept going. This time I am using control bytes for efficient searching / probing, in preparation for using SIMD instructions to that it will be actually fast and just a slightly worse version of the abseil flat hash map.

The map actually performs worse than a regular linear probing map due to the computational overhead of individually checking each control byte


## v5.2

I have now implemente SMID instructions, that allow for simulatenous searching of a whole group at a time. 

Now even faster again. and tying together all the concepts I have learnt so far, this is a very cool implementation of a hash map! Hopefully more to come

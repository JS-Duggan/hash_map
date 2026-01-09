# I have made a serious attempt to make a hash map

## v1. 
naive chaining hash map, works well enough, did not beat unordered map
## v2. 
still chaining, attempted to store hashes so I don't have to recomupte. Slower on benchmarks, but probably because the string benchmark used were small strings and integers. I will be circling back to this idea later
### v3. 
flat map! Note that I am choosing to use a C-style array over std::vector not because I think it is better, but because I want to have full control and actually resize the array. Also note the convenience of using type aliasing and good programming practices, very little has to change when changing the underlying data structure.

Now outperforms unordered_map on datasets of size n <= 10M, still struggles with >=100M, most likely due to overhead of storing hashes.

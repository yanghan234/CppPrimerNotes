- [Associative containers](#associative-containers)
  - [Using an associative container](#using-an-associative-container)
    - [Using a map](#using-a-map)
    - [Using a set](#using-a-set)
  - [Overview of the associative containers](#overview-of-the-associative-containers)
    - [Initialize an associative container](#initialize-an-associative-container)
    - [Requirements on key type](#requirements-on-key-type)
    - [The ```pair``` type](#the-pair-type)
  - [Operations on associative containers](#operations-on-associative-containers)
    - [Type alias](#type-alias)
    - [Associative container iterators](#associative-container-iterators)
    - [Adding elements](#adding-elements)
    - [Erasing elements](#erasing-elements)
    - [Subscripting a ```map```](#subscripting-a-map)
    - [Accessing elements](#accessing-elements)
  - [The unordered containers](#the-unordered-containers)
# Associative containers
Associative containers support efficient loopup and retrieval by a key. The two primary associative containers are ```set``` and ```map```.
* Elements ordered by key:
  1. ```map```: associative array; holds key-value pair;
  2. ```set```: container in which the key is the value
  3. ```multimap```: ```map``` in which a key can appear multiple times
  4. ```multiset```: ```set``` in which a key can appear multiple times
* Unordered collections:
  1. ```unordered_map```: ```map``` organized by a hash function
  2. ```unordered_set```: ```set``` organized by a hash function
  3. ```unordered_multimap```: hashed ```map```; keys can appear multiple times
  4. ```unordered_multiset```: hashed ```set```; keys can appear multiple times

## Using an associative container
### Using a map
```c++
map<string,int> m;
m[key]++; // use keys as subscript

// map contains a data structure named pair
// pair has two public data elements, first -> key, second -> val.
for ( const auto &p: m )
    p.first;
    p.second;
```
### Using a set
```c++
set<string> s;
s.find(key) == s.end(); // true, if not found
```

## Overview of the associative containers
Associative containers do not support sequential-container position-specific operations, such as ```push_back``` or ```back```.

### Initialize an associative container
* Using initializer list
    ```c++
    set<string> s = {"123","124"};
    map<string,int> m = { { "123", 123 },
                          { "125", 12 } };
    ```
* Initialize from a sequential container
    ```c++
    vector<string> svec;
    // ...
    set<string> s1(svec.begin(),svec.end());
    ```
### Requirements on key type
For ordered containers, the key type must be comparable, i.e. they must support < operator.

We can use a comparison function for the key type when define a container.
```c++
bool compare(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}
set<string,decltype(compare)*> s1(compare);
```

### The ```pair``` type
A ```pair``` holds two data members.
```c++
pair<string, string> anon; // holds two strings
pair<string, size_t> word_count; // holds a string and a size_t
pair<string, vector<string>> line; // holds string and vector<int>

// In general 
pair<T1,T2> p;
pair<T1,T2> p(v1,v2);
pair<T1,T2> p = {v1,v2};
auto p = make_pair(v1,v2); // type is inferred from arguments 
```

## Operations on associative containers
### Type alias
| Name | Description |
| --- | :--- |
| ```key_type``` | Type of key for this container |
| ```mapped_type``` | Type associated with each key; ```map``` type only |
| ```value_type``` | For ```set```s, the same as ```key_type```; for maps, ```pair<const key_type,mapped_type>``` |

### Associative container iterators
When we dereference an iterator, we get a reference to a value of the container's ```value_type```. 
```c++
set<string> s;
*(s.begin()); // string
map<string,int> m;
*(m.begin()); // pair<string,int>
```
Notes:
1. Iterators of ```set```s are ```const```.
2. Iterators of sets and maps can also be used to traverse the container.
3. We typically do not use generic algorithms on associative containers.

### Adding elements
* Insert to sets
    ```c++
    s.insert(vec.begin(),vec.end());
    s.insert({1,2,3});
    ```
* Insert to maps
    ```c++
    m.insert({key,val});
    m.insert(make_pair(key,val));
    m.insert(pair<key_type,mapped_type>(key,val));
    m.insert(map<key_type,mapped_type>::value_type(key,val));
    ```
* Return from ```insert```
    The return value contains two members. The first is the iterator of the inserted element, if successfully inserted. The second is a boolean to indicate if the element is successfully inserted.
* Inserting to ```multiset``` and ```multimap``` is similar

### Erasing elements
```c++
// remove key from sets or maps
s.erase(key);
m.erase(key);
```

### Subscripting a ```map```
```set``` does not support subscripting, ```map``` does.
If a key does not exist in map, a new one is created.
```c++
map<string,int> m;
m["abc"] = 1; // add new one
m["abc"] = 2; // update value
// equivalent to m.at(key)
```

### Accessing elements
We can use ```find``` or ```count``` to check if a key exists in a ```set``` or ```map```.
```c++
s.find(key); // return an iterator; found if s.find(key) != s.end()  
s.count(key); // found if s.count(key) != 0;
```
If we want to check if a key exists in ```map```s, we should use ```find``` rather than subscripting. Because, subscripting may create unwanted elements in maps.

**I skipped a lot here**

## The unordered containers





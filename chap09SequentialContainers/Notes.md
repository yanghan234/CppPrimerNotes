- [Sequential containers](#sequential-containers)
  - [Overview of sequential containers](#overview-of-sequential-containers)
  - [Container Library Overview](#container-library-overview)
    - [Iterators](#iterators)
    - [Container type members](#container-type-members)
    - [```begin``` and ```end``` members](#begin-and-end-members)
    - [Defining and initializing container](#defining-and-initializing-container)
    - [Assignment ans ```swap```](#assignment-ans-swap)
    - [Container size operation](#container-size-operation)
    - [Relational operations](#relational-operations)
  - [Sequential container operations](#sequential-container-operations)
    - [Adding elements](#adding-elements)
    - [Accessing elements](#accessing-elements)
    - [Erasing elements](#erasing-elements)
    - [Resizing a container](#resizing-a-container)
    - [Operations may invalidate iterators](#operations-may-invalidate-iterators)
  - [How a ```vector``` grows](#how-a-vector-grows)
  - [Additional ```string``` operations](#additional-string-operations)
    - [Other ways to construct strings](#other-ways-to-construct-strings)
    - [```substr``` operation](#substr-operation)
    - [insert, erase, assign, append, replace](#insert-erase-assign-append-replace)
    - [Search in string](#search-in-string)
    - [```compare``` function](#compare-function)
    - [Conversions between strings to numbers](#conversions-between-strings-to-numbers)
  - [Container adaptors](#container-adaptors)
    - [```stack``` API](#stack-api)
    - [```queue``` and ```priority_queue``` API](#queue-and-priority_queue-api)
# Sequential containers
## Overview of sequential containers
A container holds a collection of objects of a specified type. **Sequential containers** store elements by the order of insertion. By contrast, ordered and unordered associative containers store elements by the order of keys.
| Containers | Description |
| --- | :--- |
| ```vector``` | Flexible-size array. Random access. Inserting or deleting other than at the back may be slow |
| ```deque```  | Double-ended queue. Random access. Fast insert/delete at front or back |
| ```list```   | Doubly lined list. Supports only bidirectional sequential access. Fast insert/delete at any point in the list |
| ```forward_list``` | Singly linked list. Supports only sequential access in on direction. Fast insert/delete at ny point in the list |
| ```array```  | Fixed-size array. Random access. Cannot add or remove elements |
## Container Library Overview
Containers can store almost all types, but if we want to give an initial size of the container, the type must have a default constructor. 
```c++
// snippet01.cpp
class A
{
    private:
        int _a;
        string _s;
    public:
        A(int a) : _a(a), _s(""){}
        A(string s) : _a(0), _s(s){}
};

int main()
{
    A a1(1);
    A a2("12");
    vector<A> v1;              // ok, empty container
    // vector<A> v2(10);       // error, class A has no default constructor
    vector<A> v3(10,1);        // ok, using implicit class type conversion
    // vector<A> v4(10,"123"); // error, because "123" -> string("123") -> A(string("123"))
                               // requires two-step conversion
    vector<A> v5(10,string("123")); // ok, using implicit class type conversion
    vector<A> v6(10,A(12));         // ok, explicitly call constructor
    vector<A> v7(10,A("123"));      // ok, explicitly call constructor
}
```

### Iterators
An **iterator range** is denoted by a pair of iterators, one refers an element (say first one) and the other one usually points to one step after last element. In most cases, the pair is ```iterator.begin()``` and ```iterator.end()```. The element range is left-inclusive and right-exclusive, i.e. iterators within $[\mathrm{begin},\mathrm{end})$ are valid.
```c++
vector<int> a;
// add something to a
vector<int>::iterator it = a.begin();
while ( it != a.end() ) // iterstors support !=, ==, but do not support <, >
    it++;
```

### Container type members
| Type member | Description |
| --- | :--- |
| ```iterator``` | Container iterator |
| ```const_iterator``` | Iterator type the can read but not change its element |
| ```size_type``` | unsigned integral type big enough to hold the size of the largest possible container of this type |
| ```difference_type``` | Signed integral type big enough to hold the distance between two elements |
| ```value_type``` | Element type |
| ```reference_type``` | Element's lvalue type; synonym for ```value_type&``` |
| ```const_reference``` | Element's ```const``` lvalye type, ```const value_type&``` |

### ```begin``` and ```end``` members
The ```begin``` and ```end``` operators yield iterators that refer to the first and one past the last element in the container. There are several version of the operators.
```c++
list<string> a = {"123","dfa"};
// begin and cbegin, ++operator moves to next one;
auto it1 = a.begin();   // list<string>::iterator
auto it2 = a.cbegin();  // list<string>::const_iterator, c++11
// rbegin and crbegin, ++operator moves to previous one;
auto it3 = a.rbegin();  // list<string>::reverse_iterator
auto it4 = a.crbegin(); // list<string>::const_reverse_iterator, c++11
```

### Defining and initializing container
All containers but ```array``` have a default constructor that creates an empty container. Because the size of ```array``` can not be changed, so its size must be provided.
* Initializing a container as a copy of another
    ```c++
    C c1(c2);  // copy C2 to C1
    C c1 = c2; // copy C2 to C1 
    C c1(iterator of c2, iterator of c2); // copy elements in C2 between two iterators
    ```
* List initialization
    ```c++
    list<string> l = {"123","2234"};
    ```
* Sequential container size-related constructor
    ```c++
    C c(size);        // initialize container with size elements
    C c(size, init);  // initialize container with size elements, each has value of init
    ```
* ```array``` container is a little bit different. Its size is fixed, so the size of an array should be included in type.
    ```c++
    array<int,43> a1;
    array<string,10> a2;
    ```

### Assignment ans ```swap```
Assignment and swap operators
```c++
c1 = c2;     // assign c2 to c1
swap(c1,c2); // swap c1 and c2
c1.swap(c2); // swap c1 and c2
c1.assign(b,e); // two iterators of another container
c1.assign(il);  // replace leading elements with the initializer list
c1.assign(n,t); // replace n elements with value t
```

### Container size operation
```c++
c.size();  // return size of container
c.empty(); // return if the container is empty
```

### Relational operations
Every container type supports the equality operators (== and !=); all the containers except unordered associative containers also support the relational operators (>, >=, <, <=). If the data type in the container is user-defined, it must support relational operators. See [snippet02](./snippet02.cpp).

## Sequential container operations
### Adding elements
* Using ```push_back```: add to the end
* Using ```push_front```: add to the beginning
* Using ```insert```:
    ```c++
    c.insert(iter,n,t); // insert n elements with value of t from iter
    c.insert(iter,it1,it2); // insert from iter elements between it1 and it2
    c.insert(iter,li); // li is a list of elements
    ```
    ```insert``` method returns the location of insertion, i.e. iter.
* Using emplace methods: ```emplace_back```, ```emplace_front``` and ```emplace``` can put element to the back, front or a specified location in the container. ```emplace``` methods take the arguments, pass them to the constructor of its data type and put the created object in container.

### Accessing elements
* subscript operator([]): valid for ```string```, ```vector```, ```deque```, and ```array```, but invalid for ```list``` and ```forward_list```.
* using iterator: valid for all sequential containers
* ```back``` and ```front```: back and front return the last and first element.

The members that access elements in a container return references. If the container is a ```const``` object, the return is a reference to ```const```. Otherwise, the return is an ordinary reference.

### Erasing elements
* ```pop_front``` and ```pop_back``` remove the first and last element in the container. ```forward_list``` does not support ```pop_back```.
* ```erase(iter)``` removes the element at iterator iter. ```erase(it1,it2)``` removes elements between it1 and it2.
* ```clear()``` delete all elements in the container.

### Resizing a container
Using ```resize(newsize)``` to set the container to newsize.

### Operations may invalidate iterators
If elements in a container are removed or added during a loop using iterators, the iterators may be invalid due to the fact that the container itself has been modified. 
* Using returned iterator: ```vector```, ```string``` and ```deque``` can return valid iterators after modification.
* Do not try to cache end iterator, because the end may also be changed. Always use the method ```end()``` to access end iterator.

## How a ```vector``` grows
```vector``` reserves some space beforehand. If the space is used up, it reserves a new chunk of space for future usage. If the space used is smaller than a certain threshold, the reserved space is shrinked. By default, ```vector``` size is expanded as 0, 1, 2, 4, ..., 2^n.
The resizing process is done automatically, but it can also be done per the request of users with API
| Method | Description |
| --- | :--- |
| ```reserve(n)``` | Allocate space for at least n elements |
| ```capacity()``` | Number of elements can have before reallocation is necessary |
| ```shrink_to_fit()``` | Request to reduce capacity() to equal size() |
| ```size()``` | The space that has been used |

## Additional ```string``` operations
### Other ways to construct strings
```c++
string s(cp,n); // cp is a char* array, s convert n characters to c++ string
string s(s2,pos); // s copy characters in s2 from pos
string s(s2,pos,len); // s copy len characters in s2 from pos
```

### ```substr``` operation
```c++
string s2 = s.substr(pos,n);  // copy n characters in s from pos
string s2 = s.substr(pos);  // copy characters in s from pos
```

### insert, erase, assign, append, replace

### Search in string
```c++
char c;
string s, t;
s.find(c,pos=0);
s.find(t,pos=0);  // find the first occurance of t in s from pos
s.find(t,pos,n);  // find the first occurance of t in s from pos in the next n characters;
s.rfind(t); // find the last occurance of t in s
```

### ```compare``` function
```c++
char* cp; // c-style string
string s1, s2;
s1.compare(s2); // compare s1 to s2
s1.compare(pos1,n1,s2); // compare s1.substr(pos1,n1) to s2
s1.compare(pos1,n1,s2,pos2,n2); // compare s1.substr(pos1,n1) and s2.substr(pos2,n2);
// s2 can be replaced with cp
```

### Conversions between strings to numbers
```c++
to_string(val);   // val to string

stoi(s,p,b=10);   // convert string s from p to int, default base is 10
stol(s,p,b=10);   // convert string s from p to long, default base is 10
stoul(s,p,b=10);  // convert string s from p to unsigned long, default base is 10
stoll(s,p,b=10);  // convert string s from p to long long, default base is 10
stoull(s,p,b=10); // convert string s from p to unsigned long long, default base is 10
stof(s,p)         // string to float
stod(s,p)         // string to double
stold(s,p)        // string to long double
```

## Container adaptors
In addition to sequential containers, the library defines three sequential container adaptors, ```stack```, ```queue``` and ```priority_queue```. **Container adaptors** take seqential containers and make them work as if they are stack, queue or priority queue. As example,
```c++
stack<int,vector<int>> svec; // stack implemented with vector
queue<int,vector<int>> qvec; // queue implemented with vector
priority_queue<int,vector<int>> pqvec; // priority queue implemented with vector
```
By default, ```stack``` and ```queue``` use ```deque``` while ```priority_queue``` uses ```vector``` to implement the adaptor.
### ```stack``` API
```c++
s.pop();         // removes, but does not return, the top element
s.push(item);    // push to stack
s.emplace(args); // pass args to element constructor
s.top();         // access last element without removing
```

### ```queue``` and ```priority_queue``` API
```c++
q.pop();        // removes front element, but does not return, the front element
q.front();      // returns, but does not remove, the front element
q.back();       // Valid only for queue
q.top();        // Valid only for priority_queue
q.push(item);   // add element to queue / priority_queue
q.emplace(args);
```







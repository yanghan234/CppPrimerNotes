- [Generic algorithms](#generic-algorithms)
  - [Overview](#overview)
  - [A fisrt look at the algorithms](#a-fisrt-look-at-the-algorithms)
    - [Read-only algorithms](#read-only-algorithms)
    - [Algorithms that write container elements](#algorithms-that-write-container-elements)
    - [Algorithms that reorder container elements](#algorithms-that-reorder-container-elements)
  - [Customizing operations](#customizing-operations)
    - [Predicates](#predicates)
    - [Lambda expressions](#lambda-expressions)
    - [Lambda captures and returns](#lambda-captures-and-returns)
    - [Binding arguments](#binding-arguments)
  - [Revisiting iterators](#revisiting-iterators)
    - [Insert iterators](#insert-iterators)
    - [```iostream``` iterators](#iostream-iterators)
    - [Reverse iterators](#reverse-iterators)
  - [Structure of generic algorithms](#structure-of-generic-algorithms)
    - [The five iterator categories](#the-five-iterator-categories)
    - [Algorithm parameter patterns](#algorithm-parameter-patterns)
    - [Algorithm naming conventions](#algorithm-naming-conventions)
  - [Container-specific algorithms](#container-specific-algorithms)
    - [The ```splice``` members](#the-splice-members)
    - [List-specific operations change the containers](#list-specific-operations-change-the-containers)
# Generic algorithms
**Generic algorithms** are a set of functions that can be used to generic data type.
## Overview
Most of the algorithms are defined in ```algorithm``` header; with some other algorithms defined in ```numeric``` header.

In general, the algorithms do not work directly on a container. Instread, they operate by traversing a range of elements bounded by two iterators.
Keynotes:
* Iterators make the algorithms decoupled with containers.
* Algorithms do depend on element-type operations: e.g. ```sort()``` requires element type with relationship operators implemented.

## A fisrt look at the algorithms
With only a few exceptions, the algorithms operate over a range of elements, denoted with iterators.

### Read-only algorithms
1. ```accumulate``` defined in ```numeric```.
    ```c++
    accumulate(it1, it2, inital_value);
    ```
    This function returns sum of elements between iterators ```it1``` and ```it2``` and ```initial_value```. This requires that the element type must be able to converted to the type of ```initial_value```.
2. ```equal``` determines if elements in two containers are the same
    ```c++
    equal(v1.cbegin(),v1.cend(),v2.cbegin());
    ```
    The element type in container must implement ```==``` operator.

### Algorithms that write container elements
1. ```fill``` function fills a range with provided value
    ```c++
    fill(vec.begin(),vec.end(),1);
    ```
2. ```fill_n``` function fills n elements
    ```c++
    fill_n(vec.begin(),n,1);
    ```
**Note**: Algorithms that write to a destination iterator assume the desination is large enough to hold the number of elements being written. One way to ensure that an algorithm has enough elements to hold the output is to use an **insert iterator**.
**back_inserter**: ```back_inserter``` is defined in the ```iterator``` header and is used for inserting to the end of a container.
    ```c++
    vector<int> vec;
    fill_n(back_inserter(vec),n,0);
    ```
When a ```back_inserter``` is passed to an algorithm, each assignment will call ```push_back```.

3. Copy algorithms:
    ```c++
    copy(v1.it1, v1.it2, v2.it); // values in v1 to v2
    replace(v.begin(),v.end(),old_value,new_value);
    replace_copy(v1.it1, v1.it2, back_insert(v2),old_value,new_value); // copy v1 to v2 and replace old with new value.
    ```

### Algorithms that reorder container elements
1. sort(): sort container
2. unique(): rearrange the input range to eliminate adjacent duplicated entries

## Customizing operations
### Predicates
A predicate is an expression that can be called and that returns a value that can be used as a condition. The predicates used by library algorithms are either **unary predicates** or **binary predicates**. For example, sort() algorithm takes a binary predicate, 
```c++
bool compare(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}
vector<string> vec;
sort(vec.begin(),vec.end(),compare);
```

### Lambda expressions
The predicates can be lamda expressions. A lambda expression represents a callable unit of code. It can be thought as an unnamed, inline function.
```c++
[capture list](parameter list) -> return type { function body }
```
Capture list gives access of local variables out of the lambda expression.

### Lambda captures and returns
When we define a lambda, the compiler generates a new unnamed class type that correspond to that lambda and an object to that type.
* Capture by value
    ```c++
    [v](parameter list)->return type { function body }; // v is captured by value
    ```
* Capture by reference
    ```c++
    [&v](parameter list)->return type { function body }; // v is captured by reference
    ```
    When we capture a variable by reference, we must make sure the variable must exist at the time lambda being executed.
* Implicit captures
    ```c++
    [=](parameter list)-> return type { function body}; // capture all local variables by value
    [&](parameter list)-> return type { function body}; // capture all local variables by reference
    [=,&v](parameter list)-> return type { function body}; // capture all local variables by value except v, v is captured by reference
    [&,v](parameter list)-> return type { function body}; // capture all local variables by reference except v, v is captured by value
    ```
* Mutable lambdas
    Mutable lambda expression can change values of varaibles even if they are passed by value.
    ```c++
    []() mutable ->return type{};
    ```

### Binding arguments
The function ```bind```, defined in ```functional``` header, takes a callable object and generates a new callable that adapts the parameter list of the original object.
```c++
auto newCallable = bind(oldCallable, arg_list);
```
The arg_list specifies the arguments passed to oldCallable. The arg_list may include names of the form ```_n```, where n is an integer. These arguments are placeholders represnting the n-th argument of newCallable.

## Revisiting iterators
In addition to the iterators for each of containers, the library defines several additional kinds of iterators in the ```iterator``` header.
* Insert iterators: These iterators are bound to a container and can be used to insert elements into the container
* Stream iterators: These iterators are bound to input or output streams and can be used to iterate through the associated IO stream
* Reverse iterators: These iterators move backward, rather than forward. The library containers, other than ```forward_list```, have reverse iterators.
* Move iterators: The special-purpose iterators move rather than copy their elements.
### Insert iterators
An inserter is an iterator adaptor that takes a container and yields an iterator that adds elements to the specifies container. Note, inserters call container operations.
* ```back_inserter```: creates an iterator that uses ```push_back```
* ```front_inserter```: creates an iterator that uses ```push_front```
* ```inserter```: creates an iterator that uses ```insert```
```back_inserter``` and ```front_inserter``` take a container as argument, while ```inserter``` takes a container and a regular iterator of that container.

### ```iostream``` iterators
Even though ```iostream``` types are not containers, there are iterators that can be used with objects of the IO types, ```istream_iterator``` and ```ostream_iterator```.

**I don't understand why this is necessary though.**

### Reverse iterators
1. The containers, except ```forward_list```, have reverse iterators, ```rbegin```, ```rend```, ```crbegin``` and ```crend```.
2. Incrementing(++) a reverse iterator moves the iterator to the previous element. Decrementing(--) a reverse iterator moves to the next element.

## Structure of generic algorithms
### The five iterator categories
| Iterator | Description |
| --- | :--- |
| Input iterator | Read, but not write; single-pass, increment only |
| Output iterator | Write, but not read; single-pass, increment only |
| Forward iterator | Read and write; multi-pass, increment only |
| Bidirectional iterator | Read and write; multi-pass, increment and decrement |
| Random-acess iterator | Read and write; multi-pass, full iterator arithmetic |

### Algorithm parameter patterns
Most of the algorithms have one of the following forms
```c++
alg(beg, end, other args);
alg(beg, end, dest, other args);
alg(beg, end, beg2, other args);
alg(beg, end, beg2, end2, other args);
```

### Algorithm naming conventions
* Some algorithms use overloading to pass a predicate, e.g. ```sort()```.
* Some algorithms have a second named version with ```_if``` suffix to accept predicate, e.g. ```find(beg, end, val)``` v.s. ```find_if(beg, end, predicate)```.
* Some algorithms have a version with ```_copy``` suffix. By default, algorithms manipulate containers in-place. Those with ```_copy``` suffix copy results to a new container. e.g. ```reverse(beg, end)``` v.s. ```reverse_copy(beg, end, dest)```.

## Container-specific algorithms
Unlike other containers, ```list``` and ```forward_list``` define several algorithms as members. Because these two containers do not support random access. Some generic algorithm depends on random access.
```c++
lst.sort();
lst.unique();
lst.reverse();
// ...
```
### The ```splice``` members
Lists use ```splice``` to insert.
```c++
lst.splice(p,lst2); // Move elements from lst2 to lst before p. Elements in lst2 are removed.
```

### List-specific operations change the containers
Unlike generic algorithms, list-spcific operations change the elements in containers. For example, ```merge``` or ```splice```, move elements from on list to another and delete elements in the source container.


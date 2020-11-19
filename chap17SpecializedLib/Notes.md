- [Specialized library facilities](#specialized-library-facilities)
  - [The ```tuple``` type](#the-tuple-type)
    - [Define a tuple](#define-a-tuple)
    - [```tuple``` as a return type](#tuple-as-a-return-type)
  - [The ```bitset``` type](#the-bitset-type)
    - [Define and initialize ```bitset```s](#define-and-initialize-bitsets)
    - [Operations on ```bitset```s](#operations-on-bitsets)
  - [Regular expression](#regular-expression)
  - [Random numbers](#random-numbers)
  - [The IO library revisited](#the-io-library-revisited)
# Specialized library facilities
## The ```tuple``` type
### Define a tuple
A ```tuple``` is a template that is similar to ```pair``` (from C++11). ```pair``` must contain exactly two elements, while ```tuple``` can contain multiple elements. It is useful when we want to deal with several objects of different types without defining a class.
```c++
tuple<int,int,int> t1;
tuple<int,string,double> t2;
auto t2 = make_tuple(1,"sdf",20.0);
```
* Access tuple members
```c++
// item is a tuple
auto a = get<0>(item);
auto b = get<1>(item);
auto c = get<2>(item);
// elements in tuple are o-indexed
```
* Relational and equality operators
Tuples can be compared elementwise, but the element types must overload < or == operator.
```c++
tuple<string,string> t1("1","2");
tuple<string,string> t2("123","2");
bool b1 = ( t1 < t2 );
bool b2 = ( t1 == t2 );
```

### ```tuple``` as a return type
We can use ```tuple``` to return several objects from a function, espcially when the objects have different types.
```c++
typdef tuple<string,string,int> tupleType;
tupleType func1();
vector<tupleType> func2();
```

## The ```bitset``` type
The ```bitset``` class is useful when we want to deal with a set of bits.
### Define and initialize ```bitset```s
```c++
bitset<16> bitvec; // all zero
bitset<16> bitvec(2); // initialize bitset with 0x0002
```
The rightmost bits are low-order bits, and the leftmost bits are high-order bits. Bits are indexed from 0 from rightmost bits to leftmost bits.
### Operations on ```bitset```s
```c++
bitvec[pos]; // pos index from right to left
```
For other operations, refer to the documentation.

## Regular expression
TBA

## Random numbers
TBA

## The IO library revisited
TBA





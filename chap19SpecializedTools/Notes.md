- [Specialized tools and techniques](#specialized-tools-and-techniques)
  - [Contolling memory allocation](#contolling-memory-allocation)
  - [Run-time type identification(RTTI)](#run-time-type-identificationrtti)
# Specialized tools and techniques
## Contolling memory allocation
Operators ```new``` and ```delete``` can be overloaded.
* ```new``` operator has to allocate enough memory, calls proper constructor and return the pointer to the object constructed.
* ```delete``` operator calls destructor and frees the space allocated.

When we overload ```new``` and ```delete``` operators, we need to call ```malloc``` and ```free``` to allocate space and free spaces.
* ```malloc``` function takes a size that says how many bytes to allocate and returns a pointer to the beginning of the memory allocated. If allocation fails, 0 is returned.
* ```free``` function takes a ```void``` that is a copy of a pointer that was returned from ```malloc``` and returns the associated memory to system.

## Run-time type identification(RTTI)
RTTI is provided through two operators:
1. The ```typeid``` operator, which returns the type of a given expression
2. The ```dynamic_cast``` operator, which safely converts a pointer or reference to a base type into a pointer or reference to a derived type

RTTI can cause bugs, so we must be careful when using these operators.

* Using ```dynamic_cast```
```c++
dynamic_cast<type*>(e); // e must be a valid pointer
dynamic_cast<type&>(e); // e must be a lvalue
dynamic_cast<type&&>(e); // e must not be an lvalue
```
* Using ```typeid```
The ```typeid``` operator returns the type of an object.
```c++
Derived *dp = new Derived;
Base *bp = dp;
if ( typeid(*bp) == typeid(*dp) )
{
    // ...
}
if ( typeid(*bp) == typeid(Derived) )
{
    // ...
}
```

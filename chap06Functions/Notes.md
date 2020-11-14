# Functions
## Function Basics
A typical function is defined as following,
```c++
return_type func_name(para_type1 para_name1, ...)
{
    // function body
}
```
### Local Objects
Objects defined within a function are not visible to the outside, with exception of static objects.
```c++
int func()
{
    string s;
    return s; // error, local objects will be destroyed when function returns
}
```
* Automatic objects: Objects that exist only while a block is executing are known as automatic objects, e.g. function parameters are automatic objects.
* Local static objects: Local static objects are initialized before the function is called for the first time. See snippet [here](./snippet01.cpp)
```c++
void func()
{
    // i will be initialized as 0, automatically;
    static int i;
    cout << i << endl;
    i++;
}
```

### Function Declaration
A function declaration defines return type, function name and types of parameter list. Function declarations are also known as **function prototype**(函数原型).
Function declarations usually go in header files.

### Separate Compilation
C++ codes can be written in separate files and can be compiled separately.

## Argument passing
There are three ways to pass arguments to a function:
1. Pass by reference: if a parameter is reference. When the function is called, the reference is binded to the argument and thus the argument does not need to be copied.
2. Pass by pointer: if a parameter is a pointer
3. Pass by value: if a parameter is not a reference or pointer. The arguments need to be copied to the parameters in the function. 
```c++
void func(Type a); // pass by value
void func(Type &a); // pass by reference
void func(Type *a); // pass by pointer
```
### Using references to avoid copies
References are just alias, so the arguments themselves do not need to be copied, thus making references are more efficient compared to passing by values. For primitive types, these copies might not make too much difference, but for user-defined types, the copies cost might not be negligible. See [snippet02](./snippet02.cpp)

### ```const```s in argument list
* If the passing by value, the ```const``` modifier outside of the function is ignored.
```c++
void func1(int i);
void func2(const int i);
int main()
{
    int i = 0;
    const int j = 0;
    func1(i); // legal
    func2(i); // legal
    func1(j); // legal
    /* The last function call func1(j) is legal.
     * func1 will create a temperay copy for j,
     * and thus does not affect j.
     * So, the const modifier of j in ignored.
     */ 
    func2(j); // legal
}
```
Due to what we discussed above, it is illegal to overload function by changing only ```const``` modifier when passing values.
```c++
void func(int);
void func(const int); // illegal, compiler thinks this is a re-definition
```

* If passing by references or pointers, the ```const``` modifier does make a difference, as one can pass non```const``` reference pointers or references to ```const``` pointers or references, but cannot do vice versa.
```c++
void func1(int &i)
{}
void func2(const int &i)
{}
int main()
{
    int i = 0;
    func1(i); // legal, passing nonconst ref to nonconst ref
    func2(i); // legal, passing nonconst ref to const ref
    const int j = 0;
    func1(j); // illegal, passing const ref to nonconst ref
    func2(j); // legal, passing const ref to const ref
}
```
As a result, it is allowed to overload functions by changing only non```const``` references to ```const``` references.
```c++
void func(const int &);
void func(int &); // overload; Compiler can find the best match.
```
* Use references to ```const``` when possible
When the value of a parameter will not be modified in a function, references to ```const``` are highly recommended, as this can reduce subtle errors.

### Array Parameters
Arrays cannot be passed by values, but can be passed by pointers.
```c++
// the following definitions are equivalent
void print(const int*);
void print(const int[]);
void print(const int[10]);

int i = 0;
int j[2] = {1,2};
print(&i); // legal, &i is int*
print(j);  // legal, j is int*
```
We can also use references to arrays.
```c++
// arr is a refernce to an array of 10 elements
print(int (&arr)[10]);

int a[10], b[5];
print(a); // legal
print(b); // illegal, different size
```
When we use references to arrays, the arguments must have the same dimesion as defined in parameter list. This is different from pointer, because the dimension of a pointer is ignored. See [snippet03](snippet03.cpp).

However, one must be careful to distinguish the following two forms
```c++
void print(int (&arr)[10]); // arr is a reference to an array of 10 integers
void print(int &arr[10]); // arr is an array of references to integers, size unknown
```
### Passing multidimensional arrays
```c++
void print(int (*matrix)[10]); // matrix is a 2D integer array, with 10 columns
void print(int *matrix[10]); // matrix is an array of 10 pointers to integer
```

### main function arguments
Conventionally, if one wants pass commandline arguments to a program, the ```main``` function should be defined as
```c++
// argc: number of strings
// argv: 0 -> the executable name, from 1 -> commandline arguments
int main(int argc, char **argv ){}
```

### Functions with varying parameters
* Parameters of same type --> using ```initializer_list``` (std11)
* Parameters of different types --> write a special kind of funtion, known as variadic template
* Another special type : ellipsis --> intended for coupling C and C++ codes.
```c++
// parameters of the same type
void error_msg(initializer_list<string> list)
{
    for ( auto it = list.begin(); it != list.end(); ++it )
        cout << *it << endl;
}
// the rightmost parameters of the same type
void error_msg(ErrCode e, initializer_list<string> list)
{
    cout << e.msg() << endl;
    for ( auto it = list.begin(); it != list.end(); ++it )
        cout << *it << endl;
}
```

## Return Type
A function can return void, value, references or pointers. **Never return a reference or a pointer to a local object.**
```c++
string& func()
{
    string s;
    return s; // illegal, reference to a local object
}
string& func2()
{
    return "123"; // illegal, reference to a temperary object
}
```

### Return initializing list
From C++11, functions can return a braced list of values, if the return type has a constructor that takes the returned list. The list will be used to create a temperary object of the funtion's return type and return the temperary object. 
```c++
vector<string> func()
{
    return {"123","456"};
}
```

### Return value of main function
```main``` function returns an error code to commandline/system. If the return value is 0, it means the program exited gracefully.

### Returning a pointer to an array
Because we cannot copy an array, a function can only return a pointer or reference to an array.
To simplify declaration, we can use alias to pointer to array
```c++
typedef int arrType[10]; // arrType of 10 integers
using arrType = int[10]; // equivalent to previous one
arrType* func(int i); // a function that returns a point to array of 10 integers
```
If we do not use alias, the definition is still possible but more complicated.
```c++
// general form
Type (*function(parameter_list))[dimension]
// e.g.
// func takes an integer and return a pointer to an array of 10 integers
int (*func(int i))[10];
```
### Trailing return type
From C++11, we can declare functions with trailing return types. This is especially useful when the return type is complicated.
```c++
// This function returns a pointer to an array of 10 integers.
auto func(int i) -> int (*)[10];
// This is equivalent to
int (*func(int i))[10];
```
[Snippet04](snippet04.cpp) contains several functions of the types discussed above.

### Using ```decltype```
From C++ 11, ```decltype``` can be used to get the type of an object and may help us simplify declaration of functions
```c++
int odd[] = {1,3};
int even[] = {2,4};

// decltype(odd) gives the type of array of 2 integers, and the func returns a pointer to it
decltype(odd) *func() {}
```

## Overload functions
Here I summarized the rules to overload functions
1. Variable names in declaration are not important
```c++
void func(int a);
void func(int b); // the same as above
```
2.  If passing by values, ```const``` is the same as non```const```; if passing by references or pointers, ```const``` overloads non```const```.
```c++
void func0(int);
void func0(const int); // illegal, redeclare above

void func1(int&);
void func1(const int&); // overloads above

void func2(int*);
void func2(const int*); // overloads above
```
**Tip**: When overloading ```const``` references, we may use ```const_cast``` and avoid rewriting functions
```c++
const int &func(const int &i);
int &func(int &i) // overloads above
{
    auto& ii = const_cast<const int&>(i);
    auto& r = func(ii); // call const int& version
    return const_cast<int&>(r);
}
```
### Overloading and scope
If we overload outer functions within a local scope, the outer functions are hidden
```c++
string read();
void print(string);
void print(double);
void write(int);
void write(string);
void foo()
{
    bool read = false;
    // string s = read(); // error, outside function read() is hidden

    void print(int){}; // outside declarations are no longer available
    print("abs"); // error, outside print(string) is hidden 
    print(1); // legal, use local declaration print(int);
    print(2.1); // legal, but calls local declaration print(int), not outside print(double), so the printing will be 2

    write("abc"); // there is not local definition, so use outside declarations
    write(1);
}
```

## Features for specialized uses
### Default arguments
1. We can give default values to parameters in declaration (recommended) or definition of a function. But we can not give default values to the same paramters to the same parameter in both declaration and definition.
```c++
// declaration
void func1(int a, int b = 0 ); // ok, give default value to b
// definition
void func1(int a = 0, int b) // ok, b already has default value, now give default value to a
{
    // function body
}

// declaration
void func2(int a = 0); // ok
// definition
void func2(int a = 0) // error, we cannot give default values at both declaration and definition
{}
```
2. Parameters with default values must be put to the right in parameter list.
### ```Inline``` functions
```Inline``` functions avoid function call overhead. The compiler expands every inline function and thus there is no function call overhead. **But, inline is just a request for compiler, and compiler may decide to ignore the request if the function is complicated**
```c++
inline int max(int a, int b)
{
    return a < b ? b : a;
}
```
### ```constexpr``` functions ( C++ 11 )
A ```constexpr``` function is a function that can be used in a const expression. The ```return``` type and the type of each parameter must be a literal type, and the function can only contain exactly one ```return``` statement.
```c++
constexpr int new_size() { return 4; }
```
**```constexpr``` functions are implicitly inline.***
**```inline``` and ```constexpr``` functions are recommended to be put in header files.**

### Aids for debugging
* ```assert``` is a preprocessor macro.
```c++
#include <cassert>
assert(expr);
```
The program will complain if the expression in ```assert``` is ```false```, at runtime.
* ```NDEBUG``` (not debug) is a preprocessor variable that helps us run some codes only when debugging.
```c++
#ifndef NDEBUG
assert(expr); // this part will only be executed when debugging
#endif
```
To let ```NDEBUG``` take effect, one must add the option to the compiler
```bash
$ gcc -D NDEBUG # following by files 
```
In addition, there are several other useful macros that help us debug.
| Macro name | Description                                 |
| ---        | :---                                        |
| \_\_func__ | string, name of current debugging function  |
| \_\_FILE__ | string, name of current file                |
| \_\_LINE__ | integer, current line number                |
| \_\_TIME__ | string, time of current file being compiled |
| \_\_DATE__ | string, time of current file being compiled |

## Function matching
This part looks very detailed and I decide to skip this part for now.

## Pointers to functions
A function pointer is a pointer that denotes a function rather than any object. Functions pointers can be used as arguments to some other functions. **Question: What does it looks like in memory? Where is the function in memory? These are not covered in this part.**
```c++
// Function pointer examples
bool func();
bool (*pf)(); // a function pointer to some functions, that should take no arguments and return bool, pf is uninitialized yet.
// initialize
pf = func;
pf = &func; // Two expressions are equivalent
```
### Using function pointers
Function pointers can be used the same as function names.
```c++
int min(int,int){}; 
int (*pf)(int,int) = min; // or, &min

// use it
pf(1,2); 
// or, equivalently
(*pf)(1,2);
```

### Function pointer arguments
```c++
const Type &min(const Type &a, const Type &b, int (*compare)(const Type&,const Type&))
{
    // compare is a function pointer
    // Can be a regular function, or a lambda expression
    return compare(a,b) < 0 ? a : b;
}
```

### Function pointer to overloading functions
Compiler with math the function automatically, if no match found, the compiler complains. 
```c++
void ff(int*);
void ff(const int*);
void (*pf1)(int*) = ff; // compiler will find the first one
void (*pf2)(const int*) = ff; // compiler will find the second one
```

### Return function pointer
Here, we introduce 3 approaches.
1. Alias or regular declaration
```c++
using F = int(int*,int); // F is a function type
using PF = int(*)(int*,int); // PF is a pointer type

F f1(int); // error, F is a function type and function f1 CANNOT returns a function
F *f1(int); // function f1 returns a function pointer to a function of type F
PF f1(int); // equivalent to previous one

// regular definition is again very complicated
inf (*f1(int)) (int*,int);
```
2. Trailing return type
```c++
auto f1(int) -> int (*)(int*,int);
```
3. Using ```decltype```
Suppose we already a function ```f0```, and we want to return a function pointer to a function of the type of ```f0```, we can do the following,
```c++
decltype(f0) *f1(int);
```
**In my opinion, trailing return type is more friendly.**

- [Templates and Generic Programming](#templates-and-generic-programming)
  - [Defining a template](#defining-a-template)
    - [Function templates](#function-templates)
    - [Class templates](#class-templates)
    - [Template parameters](#template-parameters)
  - [Member templates](#member-templates)
    - [Controlling instantiations](#controlling-instantiations)
  - [Template argument deduction](#template-argument-deduction)
    - [Conversions and template type parameters](#conversions-and-template-type-parameters)
    - [Function-template explicit arguments](#function-template-explicit-arguments)
    - [Trailing return types and type transformation](#trailing-return-types-and-type-transformation)
    - [Function pointers and argument deduction](#function-pointers-and-argument-deduction)
    - [Template argument deduction and references](#template-argument-deduction-and-references)
    - [Understanding ```std::move```](#understanding-stdmove)
    - [Forwarding](#forwarding)
  - [Overloading and templates](#overloading-and-templates)
  - [Variadic templates](#variadic-templates)
    - [Writing a variadic function template](#writing-a-variadic-function-template)
    - [Pack expansion](#pack-expansion)
    - [Forwarding parameter packs](#forwarding-parameter-packs)
  - [Template specializations 模板特化](#template-specializations-模板特化)
# Templates and Generic Programming
## Defining a template
### Function templates
```c++
template <typename T>
return_type function_name(parameter_list)
{
    // function body
}

// as an example
template <typename T>
int compare(const T &v1, const T &T2)
{
    if ( v1 < v2 ) return -1;
    if ( v2 < v1 ) return 1;
    return 0;
}
```
* Instatiating a function template
    ```c++
    compare(1,0);
    compare(obj1,obj2); // obj1 & obj2 must be objects of the same class
    ```
In this example, the arguments must have overloaded ```<``` operator.

When invoking a function template, we might not need to specify the type of arguments. Because the compiler can deduce its type ```T```.
* Template type parameters
We can provide multiple type parameters, each of them must starts from a ```typename``` or ```class``` keyword. The two words can be used interchangably in template programing.
    ```c++
    template <typename T1, typename T2>
    return_type func(/* ... */);
    // equivalent to use class
    template <typename T1, class T2>
    return_type func(/* ... */);
    ```
* Nonetype template parameters
In addition to defining type parameters, we can define templates that take nontype parameters. Usually, a nontype parameter represents a value. 
    ```c++
    // define a function to compare int arrays of N elements
    template <unsigned N>
    int compare(int (&a)[N], int (&b)[N])
    {}

    // invoke
    int a[10];
    int b[10];
    compare(a,b);
    ```
Nonetype parameters can be mixed with type parameters in template definition.
    ```c++
    template <typename T, unsigned N> // this is allowed
    ...
    ```
See this ready-for-compilation code [NoneTypeTemplate](./NoneTypeTemplate.cpp)
* ```inline``` or ```constexpr```
Template functions are allowed to be ```inline``` or ```constexpr```.
    ```c++
    tempalte <typename T, class C, unsigned N>
    inline return_type func_name(parameter_list); // ok
    ```
* Reduce requirement for input type
The ```compare``` examples above implicit assume that the type ```T``` overloads operator ```<```. But we can lift this assumption by using ```less``` function in standard library.
    ```c++
    template <typename T>
    int compare(const T &t1, const T &t2)
    {
        if ( less<T>()(t1,t2) ) return -1;
        if ( less<T>()(t2,t1) ) return 1;
        return 0;
    }
    ```
**Template programs should try to minimize the number of requirements placed on the argument types.**
* Template compilation
When the compiler sees the definition of a template, it does not generate code. It generates code only wehn we instantiate a specific instance of the template.

Definitions of function templates and member functions of class templates are ordinarily put into header files.
* Compilation errors
Compilation erroes are mostly reported during instantiation.

###  Class templates
* Defining a class templates
    ```c++
    template <typename T>
    class Base
    {};
    ```
* Instantiating a class template
When we use a class template, we must supply extra information: a list of explicit template arguments.
```c++
vector<int> vec;
```
* References to a template type in the scope of the template
```c++
template <typename T>
class Foo
{
    public:
        T &func();
        vector<T> vec;
};
```
* Simplifying use of a template class name
```c++
template <typename T>
class Foo
{
    // inside the template class
    Foo& func(); // no need to write Foo<T>
};

// outside of the template
Foo<int> doSomthing();
```
* Class templates and friends
Friendship is one-on-one. 
```c++
template <typename> class FFoo; // forward declaration
template <typename T>
class Foo
{
    friend class FFoo<T>;
};
```
```FFoo<int>``` is only a friend to ```Foo<int>```.
* Befriending the template's own type parameter
```c++
template <typename Type>
class Foo
{
    friend Type; // grants access to the type used to instantiate Foo
};
```
This is useful when instantiate the template with a class.
* Template type alias
```c++
typdef Foo<string> strFoo;
```
* ```static``` members of class template
Each instanitation of a template class has its own copy of the ```static``` members.
```c++
template <typename T>
class Foo
{
    public:
        static int count;
};
```
```Foo<int>``` and ```Foo<string>``` have different copies of ```count```.

### Template parameters
* The names used for template parameters are not important.
```c++
template <typename T> class Foo{};
template <typename U> class Foo{}; // completely equivalent
```
* Template parameters and scope
* A template parameter hides any declaration out that name in an outer scope.
```c++
typedef double A;
template <typename A, typename B>
void f(A a, B b)
{
    A tmp = a; // A hides the A in outer scope
}
```
* Template declaration
Forward declaration,
```c++
template <typename T> int compare(const T&, const T&);
template <typename> class Foo; // T can be negelected in forward declaration
```
* Using class members that are types
If we write something like ```T::size_type```, the compiler does not know if ```T::size_type``` is a value or a type. Thus, we nhave to tell the compiler when we use a class member that is a type.
```c++
template <typename T>
class Foo
{
    public:
        vector<T> vec;
        // We must tell compiler vector<T>::size_type is a type, not a varaible
        typename vector<T>::size_type count;
};
```
* Default template arguments
```c++
// default arguments for template functions
template <typename T, typename F = less<T>>
int compare(const T &v1, const T &v2, F f = F() )
{
    // ... 
}

// default arguments for template class
template <typename T = int>
class Foo
{};
Foo fint;
Foo<string> fstring;
```
## Member templates
A class, either an ordinary or template class, may have a member function that is itself a template.
* Member templates of ordinary class
```c++
class Foo
{
    public:
        template<typename T>
        void func(T t);
};
```
* Member templates of class templates
```c++
template <typename T> 
class Foo
{
    public:
    template <typename It>
    void Func(It a, It b); // template member function;
    template <typename It>
    Foo(It a, It b); // template constructor
};

// define functions outside of the template
template<typename T>
    template <typename It>
    Foo<T>::Foo(It a, It b) { /* */ }
template<typename T>
    template <typename It>
    void Foo<T>::Func(It a, It b) { /* */ }
```

### Controlling instantiations
The fact that instantiations are generated when a template is used means that the same instantiation may appear in multiple object files. This makes the total size of the objects very large. 

We can use an explicit instantiation,
```c++
extern template declaration; // declaration
template declaration; // definition
```
When the compiler sees an ```extern``` template declaration, it will not generate code for that instantiation in that file. There can be multiple ```extern``` declaration in the same project, but there should be only one definition of the template.

## Template argument deduction
The process of determining the template arguments from the function arguments is known as **template argument deduction**.

### Conversions and template type parameters
Top-level consts in either the parameter or the argument are ignored. The only other conversions performed in a call to a function template are
1. ```const``` conversion: A function parameter that is a reference/pointer to a ```const``` can be passed a reference/pointer to a non```const``` object
2. Array- or function-to-pointer conversions: If the function parameter is not a reference type, then the normal pointer conversion will be applied to arguments of array or function type. An array argument will be converted to a pointer to its fisrt element. Similarly, a function argument will be converted to a pointer to the function's type.
   
Other conversion, such as arithmetic conversions, derived-to-base and user-defined conversion, are no performed.
```c++
template <typename T> T fobj(T,T);
template <typename T> T fref(const T&, const T&);
string s1("a string");
const string s2("another string");
fobj(s1,s2); // ok, copy by value, const is ignored
fref(s1,s2); // ok, both convert to const string&
int a[10], b[2];
fobj(a,b); // ok, arrays converted to pointer to first element, fobj(int*,int*)
fref(a,b); // error, conversion not allowed
```
* Function parameters that use the same template parameter type
```c++
template <typename T>
int compare(const T &a, const T &b)
{ /* ... */ }
long lng;
compare(lng,1024); // error, 1024 is int, arithmetric conversion is not allowed
compare(lng,2014L); // ok, both are long
```
Checkout [TypeConversion](./TypeConversion.cpp) for details.

### Function-template explicit arguments
We can explicitly specify the type when calling a template funciton,
```c++
template <typename T1, typename T2, typename T3>
T1 sum(T2, T3);

sum<int>(1,2);         // deduce T2, T3
sum<int,int>(1,2);     // deduce T3
sum<int,int,int>(1,2); // no deduction needed
```
Normal conversions apply for explicitly specified arguments,
```c++
template <typename T>
int compare(const T&, const T&)
{ /* ... */ }

long lng;
compare(lng,1024); // error, arguments have different types
compare(lng,1024L); // ok, two arguments have the same type
compare<int>(lng,20); // ok, normal conversion applies
compare<long>(lng,20); // ok, normal conversion applies
```
### Trailing return types and type transformation
Compiler can automatically deduce return types of template functions,
```c++
template <Typename It>
??? &fcn(It beg, It end)
{
    return *beg;
}
auto &i = fcn(vec.begin(),vec.end()); // automatically deduce return type
```
another example written with trailing return type,
```c++
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
    return *beg;
}
```
* Type transformation library template classes
In template programming, sometimes we want to know the type of an element passed by users, rather than references.

To obtain the element type, we can use a library **type transformation** template.
```c++
template <typename It>
auto fcn(It beg, It end) ->
    typename remove_reference<decltype(*beg)>::type
{
    // return by value, not reference
    return *beg;
}
```
Type transformation templates, like ```remove_reference``` used in the example above, are defined in ```type_traits``` header. Type traits (类型萃取)

### Function pointers and argument deduction
When we initialize or assign a function pointer from a function template, them compiler uses the type of the pointer to deduce the template arguments.
```c++
template <typename T>
int compare(const T&, const T&)
{ /* ... */ }
// pf1 points to the instantiation int compare(const int&, const int&)
int (*pf1)(const int&, const int&) = compare;
```
We can also passing template function instantiationgs as arguments to other functions,
```c++
void func(int (*)(const int&, const int&));
func(compare); // error, compiler does not know which instantiation of compare
func(compare<int>); // ok
```
### Template argument deduction and references
* Type deduction from Lvalue reference function parameters
When a function parameter is an ordinary reference to a template type parameter ```T&```, the binding rules say that we can pass only an lvalue. 
```c++
template <typename T>
void f1(T&);
f1(i); // i is int, T is int
f1(ci); // ci is a const int, T is const int
f1(5); // error, 5 is rvalue
```
If a function template has type ```const T&```, normal binding rules say that we can pass any kind of arugment.
```c++
template <typename T>
void f2(const T&);
f2(i); // i is int, T is int
f2(ci); // ci is const int, I is int
f2(5); // a const int& can be bound to an rvalue, T is int
```
* Type deduction from Rvalue reference function parameters
When a function parameter is an rvalue reference, normal binding rules say that we can pass an rvalue to this parameter.
```c++
template <typename T> void f3(T&&);
f3(42); // 42, as a literal, is a rvalue; T is int
```
* Reference collapsing and rvalue reference parameters
Normally, we cannot pass lvalues to an rvalue parameter in a function. But, the language defines two exceptions to normal binding rules that allow this kind of usage in template functions.
1. When we pass an lvalue ( e.g. i ) to a function parameter that is an rvalue reference to a template type parameter ( e.g. ```T&&&``` ), the compiler deduce the template type parameter as the argument's lvalue reference type. 
2. If we indirectly create a reference to a reference, then those references collapse. In all but one case, the references collapse to form an ordinary lvalue reference type. The only exception is, passing an rvalue reference to an rvalue reference collapses to an rvalue reference.
```c++
(X& ) &  // collapse to X&
(X& ) && // collapse to X&
(X&&) &  // collapse to X&
(X&&) && // collapse to X&&
```
Examples:
```c++
template <typename T> void f3(T&&);

int i;
int &ci = i;
int &&ri = std::make(ci);
f3(i);  // T -> int&
f3(ci); // T -> int&
f3(ri); // T -> int&&
```
There are two important consequences from these rules:
1. A function parameter that is an rvalue reference to a template type parameter (e.g. ```T&&```) can be bound to an lvalue.
2. If the argument is an lvalue, then tbe deduced template argument type will be an lvalue reference type and the function parameter will be instantiated as an ordinary lvalue reference parameter (```T&```).

This means, we can pass any type of argument to a ```T&&``` function parameter.

We can overload function templates as before,
```c++
template <typename T> void f(T&&);
template <typename T> void f(const T&);
```

### Understanding ```std::move```
* How ```std::move``` is defined
```c++
template <typename T>
typename remove_reference<T>::type &&move(T &&t)
{
    // remove reference to get the element type
    return static_cast<typename remove_reference<T>::type &&>(t);
}
```
* ```static_cast``` from an Lvalue to an Rvalue is permitted

### Forwarding
**I am confused about forwarding**

A function parameter that is an rvalue reference to a template type parameter (i.e., ```T&&```) preserves the ```const```ness and lvalue/rvalue property of its corresponding argument.
* Using ```std::forward``` to preserve type information in a call
```forward``` returns an rvalue reference to that explicit argument type. That is, the return type of ```forward<T>``` is ```T&&```.
```c++
template <typename Type>
intermediary(Type &&arg)
{
    finalFunc(std::Forward<Type>(arg));
}
```

## Overloading and templates
If there are more than one functions that are equally matched, then
* If there is only one nontemplate function in the set of equally good matches, the nontemplate function is called.
* If there are no nontemplate functions in the set, but there are multiple function templates, and one the these templates is more specialized than any of the others, the more specialized function template is called.

## Variadic templates
A **variadic template** is a template function or class that can take a varying number of parameters. The varying parameters are known as a **parameter pack**. There are two kinds of parameter packs: A **template parameter pack** represents zero or more template parameters and a **function parameter pack** represents zero or more funciton parameters.
```c++
// A function template that takes at least one argument
template <typename T, typename... Args>
void foo(const T& t, const Args&... rest);
```
* The ```sizeof...``` operator
```sizeof...``` operator returns the number of elements in a pack.
```c++
template <typename ...Args>
void f(Args ... args)
{
    sizeof...(Args); // number of type parameters
    sizeof...(args); // number of function parameters
}
```
### Writing a variadic function template
Unlike ```initializer_list```, variadic functions can take arguments of different types. Variadic functions are often recursive. For example,
```c++
template <typename T>
ostream &print(ostream &os, const T &t);

template <typename T, typename ... Args>
ostream &print(ostream &os, const T &t, const Args ... rest)
{
    os << t << " ";
    return print(os,rest...); // recusive, until rest has only one element
}
```
### Pack expansion
The second definition of ```print``` function above recursively expands argument list.

It is also allowed to apply a function on each of the element in the pack. For example,
```c++
template <typename... Args>
void doSomthing(Args... rest)
{
    // func is a single-argument function
    // print is a variadic function template
    // func applies on every elemenr in the pack
    print(os,func(rest)...);
}
```
### Forwarding parameter packs
The library function ```std::forward``` can be applied to every element in a parameter pack. For example, if we want to implement the ```emplace_back``` function in ```vector```, this might be the code
```c++
class myVector
{
    public:
        template <class... Args>
        void emplace_back(Args&&...);
};

template <class... Args>
void myVector::emplace_back(Args&&... rest)
{
    alloc.constructor(address,std::forward<Args>(rest)...);
    // By the way alloc.constructor must also be variadic, otherwise it will not be able to take a pack as argument.
}
```

## Template specializations 模板特化
We can defined a special implementation for a specific type parameter for a template, and this process is known as template specialization.
```c++
template <typename T>
int compare(const T&, const T&);

// special version of compare to handle pointers to character arrays
template <>
int compare(const char* const &p1, const char* const &p2)
{
    return strcmp(p1,p2);
}
```
* Function overloading versus template specialization
Specializations instantiate a template; they do not overload it. As a result, specializations do not affect function matching.
* Class template specializations
```c++
template <typename T>
class Foo
{
    public:
        void func(T);
};

// specialization for T = int
template <>
class Foo<int>
{
    public:
        void func(int);
};
```

* Class-template partial specializations
We can specify some, but not all, of the template parameters or some, but not all, aspects of the parameters. A class template partial specialization is itself a template. 
```c++
template <class T>
class Foo
{};

// partial specialization
template <class T>
class Foo<T&&> {}; // rvalue references
template <class T>
class Foo<I&> {};  // lvalue references
```
* Specializing members but not the class
We can specialize a member function for a specific type parameter, while keeping other members unchanged.
```c++
template <typename T>
class Foo
{
    public:
        void func();
};

// specialize a member function when T = in
template <>
void Foo<int>::func()
{ /* ... */}
```


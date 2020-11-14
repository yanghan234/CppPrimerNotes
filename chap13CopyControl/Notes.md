- [Copy control](#copy-control)
  - [Copy, assign and destroy](#copy-assign-and-destroy)
    - [The copy constructor](#the-copy-constructor)
      - [Synthesized copy constructor](#synthesized-copy-constructor)
      - [Copy initialization vs copy initialization](#copy-initialization-vs-copy-initialization)
      - [Parameters and return vales](#parameters-and-return-vales)
    - [The copy-assignment operator](#the-copy-assignment-operator)
      - [Introducing overloaded assignment](#introducing-overloaded-assignment)
    - [Destructor](#destructor)
      - [When a destructor is called](#when-a-destructor-is-called)
      - [Synthesized destructor](#synthesized-destructor)
    - [The rule of three/five](#the-rule-of-threefive)
    - [Using ```=default```](#using-default)
    - [Preventing copies](#preventing-copies)
      - [Define a function as deleted](#define-a-function-as-deleted)
  - [Copy control and resource management](#copy-control-and-resource-management)
    - [Classes that act like values](#classes-that-act-like-values)
    - [Classes that act like pointers](#classes-that-act-like-pointers)
      - [Reference counts](#reference-counts)
  - [Swap](#swap)
      - [Using ```swap``` in assignment operator](#using-swap-in-assignment-operator)
  - [A copy-control example](#a-copy-control-example)
  - [Classes that manage dynamic memory](#classes-that-manage-dynamic-memory)
    - [Example: ```StrVec```](#example-strvec)
  - [Moving objects](#moving-objects)
    - [Rvalue references](#rvalue-references)
      - [Lvalues persist; Rvalues are ephemeral](#lvalues-persist-rvalues-are-ephemeral)
      - [The library ```move``` function](#the-library-move-function)
    - [Move constructor and move assignment](#move-constructor-and-move-assignment)
      - [Move operations, library containers and exceptions](#move-operations-library-containers-and-exceptions)
      - [Move-assignment operator](#move-assignment-operator)
      - [A moved-from object must be destructible](#a-moved-from-object-must-be-destructible)
      - [Synthesized move operations](#synthesized-move-operations)
    - [Rvalue references and member functions](#rvalue-references-and-member-functions)
      - [Reference qualifier](#reference-qualifier)
# Copy control
When we define a class, we specify, implicitly or explicitly, what happens when objects of that class type are copied, assigned, moved and detroyed.  A class controls these operations by defining five special functions, **copy construcotr**, **copy assignment**, **move constructor**, **move assignment** and **destructor**. Collectively, these operations are called **copy control**.

## Copy, assign and destroy
### The copy constructor
A constructor is the copy contructor if its first parameter is a reference to the class type and any additional parameters have default values.
```c++
class Foo
{
    public:
        Foo(); // default constructor
        Foo(const Foo &); // copy constructor
        // it has to be a reference
}
```
#### Synthesized copy constructor
When we define a class, a copy constructor is synthesized even if we define other constructors. But a default constructor will no be synthesized, unless requested, if we have defined other constructors.

Synthesized copy constructors make memberwise copy of the members in the class. Primitive types and arrays are copied; objects of user-defined classes are copied using the copy constructors of their class.

#### Copy initialization vs copy initialization
```c++
string dots(10,'0');         // direct initialization
string s(dots);              // direct initialization
string s2 = dots;            // copy initialization
string s3 = "123";           // copy initialization
string s4 = string(100,'0'); // copy initialization
```
Copy initialization happens when
1. Define variables with =
2. Pass an object as an argument to a parameter of nonreference type
3. Return an object from a function that has a nonreference return type
4. Brace initialize the elements in an array or the members of an aggregate class

#### Parameters and return vales
During a function call, parameters that have a nonreference type are copy initialized. When a function has a nonreference return type, the return value is used to copy initialize the result of the call operator.

This is why the copy constructor must have a reference argument.

### The copy-assignment operator
Copy assignment controls what happens when assign one object to another object.
```c++
Foo f1, f2;
f1 = f2; // copy-assignment operator
```
If not provided, the compiler will synthesize a copy-assignment operator.
#### Introducing overloaded assignment
To write a copy-assignment operator, we need to overload assignment operator.
```c++
class Foo
{
    public:
        Foo& operator=(const Foo&); // assignmnet operator
}
```
The synthesized copy-assignment operator assign every non```static``` member of right object to left object.

### Destructor
Destructors do whatever needed to free the resources used by an object and detroy the non```static``` data members of the object.
```c++
class Foo
{
    public:
        ~Foo(); // destructor
};
```
Destructor can not be overloaded, as it has not input parameters nor return types.

#### When a destructor is called
1. Variables are detroyed when they go out of scope.
2. Members of an object are destroyed when the object of which they are a part is destroyed.
3. Elements in a container--whether a library container or an array--are destroyed when the container is destoyed.
4. Dynamically allocated objects are destroyed when the ```delete``` operator is applied to a pointer to the object.
5. Temporary objects are destroyed at the end of the full expression in which the temperary was created.

#### Synthesized destructor
If not provided, the compiler defines a synthesized destructor.

### The rule of three/five
Copy controls have five operators. They are not mandatory, but, in general, it is unusual to need one of them without needing to define them all. 
Rules:
1. **Classes that need destructors need copy and assignment.** Usually, a destructor is needed when we have to delete dynamically allocated spaces. This mean we have to write copy/copy-assignment construcotrs to allocate these spaces.
2. **Classes that need copy need assignment, and vice versa.**

### Using ```=default```
We can explicitly ask the compiler to generate the synthesized versions of the copy-control members.
```c++
class Foo
{
    public:
        Foo() = default; // synthesized default constructor
        Foo(const Foo&) = default; // synthesized copy
        Foo &operator=(const Foo&) = default; // synthesized copy assignment
        ~Foo() = default; // synthesized destructor
};
```

### Preventing copies
Although most classes should define a copy constructor and a copy-assignment operator, some classes prevent copies for purposes. For example, ```iostream``` are not allowed to copied or assigned.
#### Define a function as deleted
Recall that the compiler always generates synthesized copy/assignment constructors even if we have implemented other constructors. This means omitting copy/assignment constructors does not *delete* them. So, to disable copy/assignment constructors, we have to explicitly mark them as deleted,
```c++
class Foo
{
    public:
        Foo() = default; // synthesized default
        Foo(const Foo&) = delete; // disable copy
        Foo &operator=(const Foo&) = delete; // disable assignment
        ~Foo(); // synthesized destructor
};
```
The ```=delete``` keyword must appear at declaration of the function. ```=delete``` keyword can also be used for other class function members to suppress some functions. But, ```=default``` can appear at declaration or definition of member functions. If it appears at declaration, the constructor is ```inline```. Also, ```=default``` can only be used for constructors/destructor.

**The destructor should not be a deleted member**. If the detructor is deleted, there is not way to detroy the object of that type. The compiler will not let us define variable or create temporaries of that class. We can dynamically allocate an object of a class with deleted destructor. But, we need take care of how they can be detroyed.
```c++
class Foo
{
    public:
        Foo() = default;
        ~Foo() = delete; // suppress destructor
};
Foo f1; // error, cannot not create a object without destructor
Foo *pf = new Foo(); // ok
delete pf; // error, object does not have destructor
```
**The copy-control memebers may be synthesized as deleted.** 
* The synthesized destructor is defined as deleted if the class has a member whose own destructor is deleted or inaccessbile (e.g. private);
* The synthesized copy constructor is defined as deleted if the class has a memebr whose own copy constructor is deleted or inaccessible. It is also deleted if the class has a member with a deleted or inaccessible destructor.
* The synthesized copy-assignment operator is defined as deleted if a member has deleted or inaccessible copy-assignment operator, of the class has a ```const``` or reference member;
* The synthesized default constructor is defined as deleted if the class has a member with a deleted default or inaccessible destructor; or has a reference member that does not have an in-class initializer; or has a const member whose type does not explicitly define a default constructor and that member does not have an in-class initializer.
Generally speaking, these rules mean that if a class has data member that cannot be default constructed, copied, assigned or detructed, then the corresponding member will be a deleted function.

## Copy control and resource management
Classes that manage resources that do not reside in the class must define the copy control members. For example, a class dynamically allocates space. Such classes require destructor to free these spaces and copy, assignment constructors to manage these resources.

In order to manage these spaces, we have two choices: We can define the copy operators to make the class behave like a value or a pointer. 

### Classes that act like values
To provide value-like behavior, each object has its own copy of the resources that the class manages.

The copy constructor creates a new object by copying all the resources of the old one.

The copy assignment firstly destroy the resources of left-hand object and the copy everything from the right-hand object. Note that, copy assignment must work if the both sides are the same object. So, the correct approach is to copy right hand to a temperary object and then delete the left-hand object.

### Classes that act like pointers
Pointer-like classes do not really copy the resources, instead, they create new pointers to the resources and maintain a counter to decide when to free the underlying resources. One example is ```share_ptr```.
#### Reference counts
Reference counting works as follows:
* In addition to initializing the object, each constructor creates a counter. The counter will keep track of how many objects share state with the object we are creating.
* The copy constructor does not allocate a new counter; instead, it copies the data member of its given object, including the counter.
* The destructor decrements the counter, indicating that there is one less user of the shared state. If the count goes to zero, the destructor deletes that state.
* The copy-assignment operator increments the right-hand object's reference counter and decrements the left-hand object's counter.

## Swap
In addition to defining the copy-control members, classes that manage resources often also define a function named ```swap```. Defining ```swap``` is particularly important for classes that we plan to use with algorithms that reorder elements, e.g. ```sort()```.

If a class does not define its own ```swap```, library function ```swap``` will be called. It may not cause issues, but necessary operations might need to be done. 

We can overrider the default behavior of ```swap``` by defining a ```swap``` as a ```friend``` function.
```c++
class Foo
{
    friend void swap(Foo &, Foo &);
}
```
Conventionally, if we call library ```swap```, we write ```std::swap```; otherwise, we call class-specific ```swap```.

#### Using ```swap``` in assignment operator
Classes that defined ```swap``` often use ```swap``` to define their assignment operator. These operators use a technique called **copy and swap**. 
```c++
// Note that, here we pass rhs by value,
// because after assignment, the rhs should not be changed.
// As it is passed by value, the copy constructor is called.
Foo& operator=(Foo rhs)
{
    swap(*this,rhs);
    return *this;
}
```
This technique can automatically handle self-assignment.


## A copy-control example
TBA

## Classes that manage dynamic memory
Some classes need to allocate a varying amount of storage at runtime. Such classes can often use library containers. However, this strategy does not always work for every class and those classes need to manage their own allocation.

### Example: ```StrVec```
TBA

## Moving objects
One of the major features from C++11 is the ability to move rather than copy an object. In cases where the copied-from objects are immediately destroyed after copy, moving rather than copying is more effecient. An example of such class is ```unique_ptr```.

### Rvalue references
To support move operations, C++ 11 introduced a new kind of reference: **rvalue reference**.
```c++
int i = 42; // ok
int &r = i; // ok, lvalue r refers to i
int &&rr = i; // error, i is lvalue, cannot bind an rvalue to lvalue

int &r2 = i * 42; // error, i*42 is rvalue
const int &r3 = i*42; // ok, we can bind a reference to const to rvalue
int &&rr2 = i * 42; // ok, bind rvalue reference to rvalue
```

#### Lvalues persist; Rvalues are ephemeral
Lvalues have persistent state, while rvalues are either literal or temporary objects. Because rvalues references can only be bound to temporaries, we know that
* The referred-to object is about to be destroyed
* There can be no other users of that object

**Rvalue references are referring to rvalues but they are lvalues themselves.** Thus, we can not assign a rvalue reference to a rvalue reference.
```c++
int &&rr1 = 4; //ok, binds to a literal
int &&rr2 = rr1; // error, rr1, as a rvalue reference, is a lvalue;
```

#### The library ```move``` function
The library ```move``` function, defined in ```utitlity``` header, returns rvalue reference of a lvalue.
```c++
int &&rr2 = std::move(rr1); // ok
```
* Please note, after being moved, the moved-from object may be meaningless, so we should not use it any more.
* Conventionally, we should use ```std::move``` instead of ```move``` even if we have ```using namespace std;```

### Move constructor and move assignment
Move and move assignment constructors are similar to copy and copy assignment, but they take over resources from their given objects instead of copying them.

Move constructor takes a rvalue reference, and move constructor must ensure that the moved-from object is left in a state such that destroying them is harmless.
```c++
// noexcept is needed
Foo::Foo(Foo &&) noexcept {}
```
#### Move operations, library containers and exceptions
Because a move operation executes by taking over resources, it ordinarily does not itself allocate any resources. As a result, move operations ordinarily will not throw any exception. But it is still possible that a move constructor may throw exception. So, why do we need ```noexcept``` keyword for move constructor. Because library containers assume its element type move constructor should not throw exceptions, otherwise the move process might be unsafe -- moved-from objects might be lost if exception appears. Instead, library container may decide to use copy constructor rather than move constructor unless it knows the move constructor does not throw exceptions.

#### Move-assignment operator
The move-assignment operator does the same work as the destructor and the move constructor. It must take care of self-move-assignment.
```c++
Foo &operator=(Foo &&) noexcept;
```

#### A moved-from object must be destructible
Moving from an object does not destroy that object, but somtimes after the move operation is done, the moved-from object will be destroyed. Therefore, when we write a move operation, we must ensure that the moved-from object is in a state in which the destructor can be run. In addition, move operations must guarantee that the object remains valid. One can still call its member functions and do whatever the class allows, but the results of member functions calls or the values of member variables are not meaningful.

#### Synthesized move operations
As it does for copy construcotr and copy-assignment operator, the compiler will, in some cases, synthesize the move constructor and move-assignment operator. 
* If a class defines its own copy, copy-assignment constructor or destructor, the move constructor and move-assignment constructor are **NOT** synthesized. 
* The compiler will synthesize a move constructor or a move-assignment constructor **ONLY** if the class does not define any of its copy-control members and if every non```static``` members can be moved.
* The compier can move built-in types and classes with move or move-assignment constructors defined.

Synthesized move operations are defined as ```delete```d,
* If a class member defines copy operations but does not define move operations, or if the class has a member that does not define its copy operations and for which the compiler is unable to synthesize a move constructor.
* The move constructor or move-assignment constructor is defined as deleted if the class has a memebr whose own move constructor or move-assignment constructor is deleted or inaccessible. 
* The move constructor is defined as deleted if the destructor is deleted or inaccessible.
* The mvoe-assignment operator is defined as deleted if the class has a ```const``` or reference memeber.

### Rvalue references and member functions
Move-enable memebrs typically use the same parameter patterns as the copy constructors.
```c++ 
void push_back(const X&); // copy
void push_back(X&&);  // move

// The following will not be used,
// so there is no need to define them.
void push_back(X&); // when we copy, we intend not to modify object, so it should be a reference to const
void push_back(const X&&); // when we move, we intend to modify object, so it can't be a reference to const

// For example
X x;
obj.push_back(x); // calling copy
obj.push_back(literal of X); // calling move
```
#### Reference qualifier
Reference qualifiers can be used in the same manner as we define ```const``` member function -- we can place a reference qualifier after the parameter list of a non```static``` member function. The reference qualifier can be either ```&``` and ```&&```, indicating that ```this``` may point to an lvalue or rvalue.
```c++
class Foo
{
    public:
        Foo &operator=(const Foo&) &; // this pointer must be modifiable lvalues
        Foo sorted() &&; // may run on modifiable rvalues
        Foo sorted() const &; // may run on any kind of Foo
};

// define two functions
Foo &refFoo(); // returns a lvalue reference
Foo valFoo();  // returns a value

refFoo().sorted(); // calling Foo sorted() &&
valFoo().sorted(); // calling Foo sorted() const &
```

If a member function has a reference qualifier, all the versions of that member with the same parameter list must have reference qualifiers.

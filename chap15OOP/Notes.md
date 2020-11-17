- [Object-oriented programming](#object-oriented-programming)
  - [Overview](#overview)
    - [Inheritance](#inheritance)
    - [Dynamic binding](#dynamic-binding)
  - [Define base and derived classes](#define-base-and-derived-classes)
    - [Define a base class](#define-a-base-class)
    - [Define a derived class](#define-a-derived-class)
    - [Conversions and inheritance](#conversions-and-inheritance)
  - [Virtual functions](#virtual-functions)
  - [Abstract base classes](#abstract-base-classes)
  - [Access control and inheritance](#access-control-and-inheritance)
  - [Class scope under inheritance](#class-scope-under-inheritance)
  - [Constructors and copy control](#constructors-and-copy-control)
    - [Virtual destructors](#virtual-destructors)
    - [Synthesized copy control and inheritance](#synthesized-copy-control-and-inheritance)
    - [Derived-class copy-control members](#derived-class-copy-control-members)
    - [Inherited constructors](#inherited-constructors)
  - [Containers and inheritance](#containers-and-inheritance)
# Object-oriented programming
## Overview
The key ideas in object-oriented programming are
* Data abstraction: We define classes that separate interface and implementation
* Inheritance: We can define classes that model the relationships among similar types
* Dynamic binding: We can use objects of similar types while ignoring the details of how they differ.
### Inheritance
Classes related by inheritance form a hierachy, including base class and derived class. Base classes define virtual functions so that derived classes can provide their own implementation for virtual functions.

A derived calss must specify the classes from which it intends to inherit.
```c++
/*
 * Note: derivation list should be written in definition.
 * Forward declaration should not include derivation list.
 */
// forward declaration
class Deriv;

// definition
class Deriv: public Base
{
};
```

### Dynamic binding
Through dynamic binding, if we call a virtual function via the pointer or reference to base class, the program will dynamically decide to call the implementation in base or derived class.
```c++
class Base
{
    public:
        virtual Foo();
};

class Deriv: public Base
{
    public;
        Foo() override;
};

Base *bptr1 = new Base();
bptr1->Foo(); // call implementation in base class
Base *bptr2 = new Deriv();
bptr2->Foo(); // call implementation in derived class
```
Because the decission as to which version to run depends on the type of argument, that decision can't be made until runtime. Therefore, dynamic binding is sometimes known as **run-time binding**.

## Define base and derived classes
### Define a base class
* Member functions
    * If a function is expected to be override in derived class, it should be ```virtual``` so that the function call be dynamically bound.
    * Any non```static``` functions other than constructors can be ```virtual```
    * Destructor of base function is highly recommended to be ```virtual```
* Access control and inheritance
    ```protected``` access control is useful for a base class, as ```protected``` members can be accessed from derived classes but can not be accessed from outside of base/derived class.

### Define a derived class
A derived class must specify from which class(es) it inherits by providing a **class derivation list**.

* Virtual functions
    * Derived classes do not necessarily write ```virtual```. The ```virtual``` functions remain ```virtaul``` in the whole hierachy.
    * From C++ 11, ```override``` specifier can be added after parameter list to guarantee that this function does override a virtual function in base class.
* Derived-class object and derived-to-base conversion
    * A derived-class object contains members inherited from base class and members defined by derived-class.
    * A pointer or reference to base class can be used to point a derived class, this is known as **derived-to-base** conversion.
* Derived-class constructor
    * A derived class should call base-class constructors to initialize members inherited from base class.
    * A derived-class constructor should call base-class constructors in its initialization list.
    ```c++
    Derived::Derived() : Base() {}
    ```
    * The base class is initialized first, and then the members of the derived class are initialized in the order in which they are declared in the class.
* Using members of base in derived: A derived class can use ```public``` and ```protected``` members of base class as if they are members of the derived class.
* Inheritance and ```static``` members: If a base class defines a ```static``` member, this member is shared by all classes of the hierachy.
* Declaration of derived classes: Forward declaration of derived classes should not include derivation list. 
```c++
// forward declaration
class Derived : public Base; // error
class Derived; // ok
```
* Classes used as a base class: **Classes must be defined instead of just being declared before they can be used as base functions**.
```c++
class Base; // forward declaration
class Derived: public Base // error, Base is not defined yet
{};

// define Base
class Base
{//...};
class Derived: public Base // ok, Base is defined
{//... };
```
* Preventing inheritance: From C++ 11, the ```final``` keyword can be used to prevent a class from being inherited further. 
```c++
class Derived final: public Base
{ /* ... */ };
class DDerived: public Derived // error, Derived is final
{ /* ... */ };
```

### Conversions and inheritance
We can bind a pointer (including smart pointers) or reference to a base-class type to an object of a type derived from that base class, known as **derived-to-base conversion**.  The implication of derived-to-base conversion is that, the actual type of a pointer or reference to base class is unknown until runtime.
```c++
Base *bptr = new Derived(); // ok, derived-to-base conversion
Derived dobj;
Base &bref = dobj;
```
* Static type and dynamic type: Static types are known at compile time; dynamic types are known at run time. The dynamic binding is supported via ```virtual``` function.
* No implicit conversion from base to derived.
```c++
Base bobj;
Derived *dptr = &bobj; // error
Derived &dref = bobj; // error

Derived dobj;
Base *bptr = &dobj; // ok, derived-to-base conversion
Derived *dptr = bptr; // error
```
* Derived-to-base conversion applies only to pointers and references, but not objects. When convering derived-class objects to base-class objects, the derived-class members in derived-class objects are lost, known as **sliced down**.
```c++
Derived dobj;
Base bobj = dobj; // call copy constructor of Base, information in dobj is lost
bobj = dobj; // call copy assignment operator of Base, information in dobj is lost
```

Three things to remember about conversions among classes,
1. The conversion from derived to base applies only to pointer or reference types.
2. There is no implicit conversion from base class to derived class.
3. Like any member, the derived-to-base conversion may be inaccessible due to access control.(```public``` inheritance)

## Virtual functions
When a virtual function is called by a pointer or reference, the compiler generates code to decide at run time which function to call. If the function is called by an object, the function called is decided at compile time.
* Virtual functions in a derived class
    * Once a function is declared as ```virtual```, it remains ```virtual``` in all the derived classes.
    * To override a virtual function, function name, return type and parameter list type must be exactly the same.
    * One exception is when the return type is reference or pointer to base type.
    ```c++
    class Base
    {
        public:
            virtual Base &func();
            virtual int func2(int,double,string);
    };
    class Derived: public Base
    {
        public:
            // return reference to derived type is ok to override
            Derived &func(); // overrides Base::func();
            int func2(int,double,string); // overrides Base::func2
    };
    ```
    * ```override``` specifier lets the compiler check if the declaration overrides a virtual function is base class
    ```c++
    class Base
    {
        public:
            virtual int func() const;
    };
    class Derived: public Base
    {
        public:
            int func() const override; // ok, override Base::func()
            int func() override; // error, no such function in base class
    }
    ```
    * ```final``` specifier disables a function from being overrided in its derived classes
    ```c++
    class Base
    {
        public:
            virtual int func();
    };
    class Derived: public Base
    {
        public:
            int func() final; // cannot be overrided
    }
    ```
    * If virtual functions base and derived classes have different default arguments. The defaults values used in a function call depend on to which class the pointer or reference points.
    ```c++
    class Base
    {
        public:
            virtual void func(int i = 0);
    };
    class Derived: public Base
    {
        public:
            void func(int i = 2);
    };

    Derived dobj;
    Base *bptr = &dobj;
    Derived *dptr = &dobj;
    bptr->func(); // call Derived::func(), but i = 0
    dptr->func(); // call Derived::func(), i = 2
    ```
    So the function being called is always the function in derived class, but the default values depend on the type of pointer. See also the snippet [DefaultValues](./DefaultValues.cpp).
    * Circumventing the virtual mechanism: We can force a pointer to base to call the version in base class by explicitly calling the function with the name of the base class.
    ```c++
    Base *bptr = new Derived();
    // func() is virtual
    bptr->Base::func(); // explicitly call func() in base class
    bptr->func(); // call func() in derived class
    ```

## Abstract base classes
**Abstract base classes** are those that contain **pure virtual functions**. Pure virtual functions do not need to be defined, thus abstract base classes can not be instatiated.
```c++
class ABase
{
    public:
        virtual void func() =0; // pure virtual function
};
```
Classes that inherit abstract base class must implement all pure virtual functions before they can be instatiated. Otherwise, they are also abstract base classes.

**Note**: It is allowed to give a default definition of a pure base class outside of the class, but it does not change the fact that the class is a abstract one. In such case, derived class can use the default implementation by explicitly using the name of abstract base class. However, this feature is not widely used. 
```c++
class ABase
{
    public:
        virtual void func()=0;
}

void ABase::func()
{ /* ... */ }

class Derived: public ABase
{
    public:
        void func()
        {
            ABase::func(); // explicit use the default implementation
        }
}
ABase abobj; // error, it is still abstract
Derived dobj; // ok
```

## Access control and inheritance
* ```protected``` members
    * Inaccessible to users of the class
    * Accessible to members and friends of classes derived from this class
* ```public```, ```private``` and ```protected``` inheritance
    * Derivation access specifier controls the access that users of the derived classes.
    * ```public``` inheritance keeps ```public``` and ```protected``` members ```public``` and ```protected``` in derived class. (No change)
    * ```protected``` inheritance makes ```public``` and ```protected``` members ```protected``` in derived class. 
    * ```private``` inheritance makes ```public``` and ```protected``` members ```private``` in derived class.
* Accessibility of derived-to-base conversion
    * Remember that a pointer/reference to a base object can only access public members of base class, otherwise compilation fails.
    * ```protected``` and ```private``` inheritances make ```public``` members in base class no longer accessible in derived class. So, derived-to-base conversion is only useful for ```public``` inheritance.
    * ```friend``` functions of derived classes can always use derived-to-base conversions, no matter what inheritance. This is because ```friend``` functions have access to all members of a derived class and a derived-to-base conversion can only access ```public``` members of base class.
    * Member functions and ```friend``` functions of a class derived (DD) from a derived class (D) can use derived-to-base conversion if D inherits from Base class (B) using either ```public``` or ```protected```.
* Friendship and inheritance
    * Friendship is **NOT** inherited: ```friend``` functions or classes of base class have no special access to members of derived classes.
* Exempting individual members
    * It is allowed to inherit specific memebrs differently from derivation control
    ```c++
    class Base
    {
        public:
            void func();
            /* ... */
    };
    class Derived: private Base
    {
        public:
            using Base::func();
            /* Other members are inherited as private,
             * but func() is inherited as public
             */
    };
    ```

## Class scope under inheritance
* Name lookup happens at compile time: The static type determines what members can be used.
```c++
class Base
{
    public:
        virtual void func1();
};
class Derived: public Base
{
    public:
        void func1() override;
        void func2();
};
Derived obj;
Derived *dptr = &obj;
Base *bptr = &obj;
obj.func1(); // calls overriden version
obj.func2(); // ok
dptr->func1(); // calls overriden version
dptr->func2(); // ok
bptr->func1(); // ok, but calls overriden version
bptr->func2(); // error, Base does not have func2()
```

* Name collision and inheritance: Derived classes can reuse names used in Base, but it hides the names used in Base. In such case, we can use base class name to explicitly use the members of base class being hidden.
```c++
class Base
{
    public:
        string me = "base";
};

class Derived: public Base
{
    public:
        string me = "derived"; // hides me in base;
        string describe() { return me; } // return "derived"

        string describe2() { return Base::me; } // return "derived"
};
```
Try the code [NameCollision](./NameCollision.cpp)

* Name lookup happens before type checking: Functions declared in an inner scope do not overload functions declared in an outer scope. Functions defined in a derived class do not overload members in base class. Instead, a member function in a derived class has the same name as a base-class member hides the base-class function. 
```c++
class Base
{
    public:
        int func();
};
class Derived: public Base
{
    public:
        int func(int); // hides Base::func();
};
```
This is why derived classes must use the same name and same parameter list to override a virtual function in base class. The ```override``` specifier helps us check if we really override a virtual function as we want, instead of defining a new function.
```c++
class Base
{
    public:
        virtual int func();
};
class D1: public Base
{
    public:
        int func(int); // define a new function, and hides Base::func()
        virtual int func2();
};
class D2: public D1
{
    public:
        int func(); // override Base::func()
        int func2(); // overried D1::func2()
};
```

## Constructors and copy control
### Virtual destructors
A base class should have a ```virtual``` destructor. Otherwise, dynamically allocated spaces allocated in derived objevts may not be deleted.
```c++
class Base
{
    public:
        Base()=default;
        virtual ~Base() = default;
};
class Derived: public Base
{
    public:
        Derived() { ptr = new int[100]; }
        ~Derived() { delete[] ptr; } // override destructor
    private:
        int *ptr;
};

Base *ptr = new Derived();
/*
 * If base-class destructor is not virtual,
 * derived-class destructor will not be called.
 * So Derived::ptr will not be freed -> memory leakage.
 */
delete ptr;
```
Try [VirtualDestructor](./VirtualDestructor.cpp)

Note: virtual destructors turn off synthesized move. Because, if any copy control is defined( copy or copy assignment or destructor), move and move assignment will not be automatically synthesized. 

### Synthesized copy control and inheritance
* If the default constructor, copy constructor, copy assignment operator or destructor is the base class is deleted or inaccessible, the corresponding member in the derived class is defined as delete.
* If the base class has an inaccessible or deleted destructor, the synthesized default and copy constructors in the derived classes are defined as deleted. Because, the base part cannot be deleted in a copy constructor of derived class.
* The compiler will not synthesize a deleted move operation. If we use ```=default``` to request for a move constructor, it will be marked as ```delete``` if base class destructor is deleted or corresponding move operation in base class is deleted or inaccessible. Because, the base-class of a derived-class object cannot be moved or destroyed.


### Derived-class copy-control members
The copy and move constructor for a derived class must copy/move the members of its base part as well as the members in the members in the derived.
* Defining a derived copy or move constructor: We ordinarily use the corresponding base-class constructor initialize the base part of the object.
```c++
class Base { /* ... */ };
class Derived: public Base
{
    public:
        Derived(): Base() {}
};
```
* Derived-class assignment operator: We explicitly call base class operator to assign base part in derived object.
```c++
class Base {};
class Derived: public Base
{
    public:
        Derived &operator=(const Derived &rhs)
        {
            Base::operator=(rhs);
            return *this;
        }
};
```
* Destructor in derived class: Destructor in derived class only needs to clean up derived part in the object and does not need to explicitly call base-class operator.
* Calls to virtuals in constructor and destructor: When base-class constructor calls a virtual function, the derived-class part of the object is uninitialized. So, the base-class constructor/destructor will use the implementation in itself. See [CallVirtualInBase](./CallVirtualsInBase.cpp).
### Inherited constructors
From C++ 11, a derived class is allowed to "inherit" constructors from base class. It asks the compiler to generate copy control for those that base class has. If derived class contains members that are not in base class, they are initialized by default values.
```c++
class Base { };
class Derived: public Base
{
    public:
        using Base::Base; // inherit base-class constructors
        /* Compiler automatically generates constructor similar to
         * Derived(args) : Base(args) {};
         */
};
```
1. Unlike ```using``` ordinary base-class functions, ```using``` base-class constructors does not change their access level.
2. ```using``` declarations can't specify ```explicit``` or ```constexpr```. The compiler generated constructors remain the same if base-class specifies ```explicit``` or ```constexpr```. 
## Containers and inheritance
We should not store objects in containers. Instead, we should put pointers/smart pointers, not objects, in containers.
See [ContainerOfInherited](./ContainerOfInherited.cpp)

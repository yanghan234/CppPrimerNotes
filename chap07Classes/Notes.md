- [Classes](#classes)
  - [Defining abstract data types](#defining-abstract-data-types)
    - [```this``` pointer](#this-pointer)
    - [```const``` member function](#const-member-function)
    - [Class scope](#class-scope)
    - [Definition of member function](#definition-of-member-function)
    - [Return ```this``` object](#return-this-object)
    - [Define class-related nonmember function](#define-class-related-nonmember-function)
    - [Constructors](#constructors)
  - [Access control and encapsulation](#access-control-and-encapsulation)
    - [```class``` vs ```struct``` keywords](#class-vs-struct-keywords)
    - [```friend``` classes and functions](#friend-classes-and-functions)
  - [Additional class features](#additional-class-features)
    - [Type member of class](#type-member-of-class)
    - [```inline``` member functions](#inline-member-functions)
    - [In-class initializer](#in-class-initializer)
    - [Returning ```*this``` from a ```const``` member function](#returning-this-from-a-const-member-function)
    - [Class types](#class-types)
    - [Revisit friendships](#revisit-friendships)
  - [Class scope](#class-scope-1)
    - [Name loopup](#name-loopup)
  - [Revisit constructors](#revisit-constructors)
    - [Initializer list](#initializer-list)
    - [Order of member initialization](#order-of-member-initialization)
    - [Delegating constructors](#delegating-constructors)
    - [Using default constructor](#using-default-constructor)
    - [Implicit class type conversions](#implicit-class-type-conversions)
    - [Aggregate classes](#aggregate-classes)
    - [Literal classes](#literal-classes)
  - [```static``` class members](#static-class-members)
    - [Declaration](#declaration)
    - [Usage](#usage)
    - [Definition](#definition)
    - [Initializing ```static``` member variables](#initializing-static-member-variables)
    - [```static``` members can be used in ways ordinary members can't](#static-members-can-be-used-in-ways-ordinary-members-cant)

# Classes
## Defining abstract data types
In C++, one can use either ```construct``` or ```class``` keywords to define classes. The difference is, ```construct``` members have public access while ```class``` members have private access, by default. 
### ```this``` pointer
Each object of a class implicitly contains a ```this``` pointer, which points to the beginning address of the object itself, see [snippet01](./snippet01.cpp).
```c++
// snippet01.cpp
class Book
{
    public:
        Book() { cout << this << endl; }
        ~Book(){};
};

int main()
{
    Book b; // print this pointer in constructor
    cout << &b << endl; // print initial address of the object
    // two addresses printed above are the same.
    return 0;
}
```
```this``` pointer has the type of ```class_type *const```, i.e. ```this``` can not point to another object but ```this``` can be used to modify the object.

### ```const``` member function
We put a ```const``` modifier after parameter list if we don't want the member function to modify class data. Each member function in the class implicitly contains ```this``` pointer as a parameter and ```this``` function can be used to modify data in the object. So, the ```const``` modifier after parameter list is used to change the type of ```this``` from ```class_type *const``` to ```const class_type *const```, so that ```this``` pointer can not change data in the object.
**Exception**: If a variable in the object is ```mutable```, ```const``` member function can still change these data
```c++
class Type
{
    public:
        return_type func(parameter_list) const;
    private:
        int a; // func cannot modify a, because it is const
        mutable int b; // func can still modify b, as b is mutable
}
```

### Class scope
For member functions and varaibles defined in a class, they can access each other and their orders of declarations in the class are not important. 
```c++
class Type
{
    public:
        void func1(){func2();}; // ok, func1 can call func2, even though func2 is declared after func1()
        void func2(){};
}
```

### Definition of member function
Typically, we put only declaration of member functions in class, and write the function body out of class.
```c++
class Type
{
    public :
        void func(); // declaration
}

void Type::func()
{
    // definition
}
```

### Return ```this``` object
This is very useful when we overload compound operators or intend to call member functions consecutively.
```c++
class Type
{
    public: 
        Type &operator+=(const Type& that)
        {
            return *this; // return reference to current object 
        }
};
```
See [snippet02](snippet02.cpp).

### Define class-related nonmember function
We sometimes need funcitons to handle objects of certain classes. It is recommended to define these functions within the same header file of the class. A useful example is overloading of ```<<``` operator of a class.
```c++
class Type
{
    public:
        friend ostream &operator<<(ostream &, const Type &);
};

// definition
ostream &operator<<(ostream &os, const Type &me)
{
    // print something to os
    // ...
    return os;
}
```
See [snippet02](./snippet02.cpp).

### Constructors
Basics of constructors
1. Constructors initialize objects of class.
2. Constructors have no return type.
3. Constructors can be overloaded.
4. Constructors can not be ```const```, otherwise how can they initialize variables?
5. If one creates a ```const``` object of a class, the object is ```const``` after constructor finished initialization.
6. If a class has no user-defined constructor, compiler will writes a default constructor, known as **synthesized default constructor**, that take no input. Synthesized default constructor initializes primitive types with default values and initializes user-defined types with their default constructor. **If at least one user-defined constructor is given, the compiler will no longer generate synthesized defulat constructor**.
7. ```=default``` explicitly asks the compiler to generate a default constructor.
```c++
class Type
{
    public:
        Type()=default; // compiler generated default constructor
};
```
8. Initializing list
```c++
class Type
{
    public:
        Type()=default;
        Type(int i, int j) : _i(i), _j(j) {}; // _i(i), _j(j) is initializing list
    private:
        int _i, _j;
};
```
9. Copy, assignment and destruction: 
```c++
class Type
{
    public:
        Type()=default; // default constructor
        Type(const Type &); // copy constructor
};
```

## Access control and encapsulation
* public members: accessible out of the class
* protected members: accessible to inheritance class
* private members: only accessible to class itself and ```friend``` functions and classes

### ```class``` vs ```struct``` keywords
Classes defined with ```class``` and ```struct``` keywords have private access level and public access level by default, respectively.

### ```friend``` classes and functions
```friend``` classes and functions can access private members of a funciton. They should be declared within a class, but they are not class members. **It is recommeded to group ```friend``` classes and functions at the beginning or end of class definition.**

## Additional class features
### Type member of class
Classes can define local names/alias for types. Definitions of alias must appear before it is fisrt used.
```c++
class Screen
{
    public:
        typedef std::string::size_type pos;
        // or
        using pos = std::string::size_type;
        
    private:
        // First usage
        pos cursor = 0;

};
```

### ```inline``` member functions
Member functions defined inside the class are automatically ```inline```, if possible. For functions defined outside of class, we need put ```inline``` keyword at declaration or definition.
```c++
class Type
{
    public:
        void getVal() { return _val;} // implicitly inline
        inline void getVal2();
        void getVal3();
    private:
        int _val;
};
void Type::getVal2() { return _val; } // no inline needed here
inline void Type::getVal3() { return _val; } // inline was not present at declaration
```

### In-class initializer
From C++ 11, in-class initializers are allowed. For there variables, it is not necessary for constructors to initialize them, because they have default values already.
```c++
class Type
{
    private:
        int a = 0; // in-class initializer
};
```

### Returning ```*this``` from a ```const``` member function
Recall that, in a ```const``` member function, ```this``` pointer has the type of ```const class_name *const```. If a ```const``` function returns ```*this```, the return type has to be ```const class_name &```. As a result, if we embed the result with another non```const``` function, the compiler will complain. Here is an example,
```c++ 
class Type
{
    public:
        const Type &func() const;
        Type& func(); // overloading
        void func2(); // nonconst, modify something 
};

int main( )
{
    Type t;
    t.func(); // call first definition
    t.func().func2(); // call second definition of func(), which returns nonconst reference
}
```
The way out is to overload the ```const``` member function with a non```const``` version, see the code block above. When embeded in consecutive calls, the compiler will match the non```const``` version.

### Class types
* Classes with exactly the same members but with different names are different types.
```c++
/*
 * Class A and B defined below are seen as completely different types,
 * and thus should not be converted to each other.
 */
class A
{
    int a;
    int func();
};
class B
{
    int a;
    int func();
}
```
* Classes are also allowed to be declared in advance and defined later. This is known as **forward declaration**. With forward declaration, one can use pointers and references to this class, but cannot instatiate the class before completely define this class.

### Revisit friendships
* Friend functions and classes can access private members.
* Friend functions defined within the body of a class is seen as ```inline```, if possible.
* Friend functions declared in class are **not** really declared yet. If functions in class want to call friend functions, the friend functions must be declared outside of class.

## Class scope
Every class defines its own new scope. Outside the class scope, variables and functions members can be accessed only through an object, a pointer or a reference. Type members in classes can be accessed using scope operator (```::```).
### Name loopup
* Functions and variables: Compiler firstly process ALL function declarations and then process body definitions of functions. This indicates that, the order of declaration in the class is not import. Functions can call functions declared later in the same class.
* Type member: When compiler processes declaration of functions or variable, the types of them must be already known. Thus, type members must be declared before they are fisrt used.
* Name loopup process:
    1. First, look for a name inside the member function;
    2. Second, look for a name within the same class;
    3. Third, out of the class, look for names **BEFORE** current class;
    4. A name is not found --> Compilation error.

## Revisit constructors
### Initializer list
When write a constructor, we can assign values to members in constructor body or use initializer list, for example,
```c++
class Type
{
    public:
        Type()=default;
        // using assignment
        Type(int a, int b)
        {
            // give default values to member variables
            _a = a;
            _b = b;
        }
        // using initializer list
        Type(int a, int b) : _a(a), _b(b){}
    private:
        int _a;
        int _b;
};
```
However, if a class has variables that are references or objects of classes without default constructors, assignments are not equivalent to initializer list. When the body of a constructor starts to execute, the object is already in memory. If the class contains references, an error occurs because the object is created in memory but the reference has not referred to any object. To solve this issue, we must initialize references in initializer list, which is executed before the class object is ever created in memory.
```c++
class Type
{
    public:
        Type(int a)
        {
            // error
            // when entering constructor, 
            // the object is already created,
            // but _r does not refer to any thing
            _a = a;
            _r = a;
        }
        Type(int a) : _a(a), _r(a) {} // legal, initializer list executes before object is created
    private:
        int _a;
        int &_r;
};
```

### Order of member initialization
Variables are initialized according to the order of declaration. It is recommended to write the initializer in the same order of declaration
```c++
class Type
{
    public:
        // error, rem initilizes first as it appears before base in the declaration list
        // To solve this issue, just reorder base and rem below.
        Type(int i, int j) : base(i), rem(base%j) {} 
    private:
        int rem;
        int base; 
};
```
### Delegating constructors
From C++ 11, a constructor is allowed to call another constructor. The former is called a delegating constructor.
```c++
class Type
{
    public:
        Type(int i, int j) : _i(i), _j(j){}
        Type(int i) : Type(i,0){}
    private:
        int _i;
        int _j;
};
```

### Using default constructor
It is a common mistake when one tries to use default constructor,
```c++
class Type
{
    // class body
};
int main()
{
    Type a(); // ok, but it defines a function, named a, with no input parameters and return type is Type.
    Type b; // using default constructor
}
```

### Implicit class type conversions
If a class has single-parameter constructors, this class allows implicit type conversion from the types of single-parameter constructors to the class type. 
As an example,
```c++
class Type
{
    public:
        Type(int i) : _i(i), _s("") { }
        Type(string s) : _i(0), _s(s) { } 
    private:
        int _i;
        string _s;
};
Type func1()
{
    return 1; // legal, a temperary object of class Type will be created and returned.
}
Type func2()
{
    string s = "123";
    return s; // legal, a temperary object of class Type will be created and returned.
}
Type func3()
{
    return "123"; // illegal, literal "123" needs to be converted to string and then a Type object can be created. 
                  // This requires two-step conversion, which is illegal.
}
```
Implicit type conversion can be suppressed, if an ```explicit``` modifier is put before single-parameter constructor. But, the side effect is that, assignment form initialization is also suppressed.
```c++
class Type
{
    public:
        explicit Type(int i) : _i(i) {}
    private:
        int _i;
};
Type func()
{
    Type t1(123);  // ok, explicitly call the constructor
    Type t2 = 123; // error, assignment form is suppressed due to explicit keyword
    return 1;      // error, implicit conversion is suppressed
    return t1;     // ok, explicitly return Type object
}
```

### Aggregate classes
An aggregate class gives users direct access to its members and has special initialization syntax. A class is aggregate if 
  * All data members are ```public```
  * It does not define any constructors
  * It has no in-class initializers
  * It has not base classes or virtual functions
```c++
struct Data {
    int val;
    string s;
};

Data d1 = { 0, "1234" }; // initializing
```

### Literal classes
Did not understand this part.
I have to review ```constexpr``` statement before coming back.

## ```static``` class members
### Declaration
```static``` members are independent of any objects of the class, thus ```static``` function does not have an implicit ```this``` argument and ```static``` function cannot use ```this``` pointer in function body. As a result, ```static``` member function cannot be declared as ```const```, because ```const``` applies constraint on ```this``` pointer. 

### Usage
* Use class name and scope operator
* Use object 

### Definition
* There is no need to rewrite ```static``` keyword if a ```static``` function is defined out of class.
* ```static``` function cannot accesss non```static``` member variables or functions in the class, because when the ```static function``` is called, there might be no object of the class created.
* ```static``` members are created before any object of the class is created. They exist until the program fully completes.

### Initializing ```static``` member variables
1. Ordinary ```static``` member variables can not be initialized in-class. Instead, they need to be initialized out of the class body.
```c++
class Type
{
    private:
        static int TypeVal; // by default, it is zero
};

int Type::TypeVal = 1; // initializing
```
2. ```static``` members that are ```constexpr``` can be initialized within the class body,
```c++
class Type
{
    private:
        static constexpr int TypeVal = 123; // ok, in-class initialization for constexpr
};
```

### ```static``` members can be used in ways ordinary members can't
1. A ```static``` data can have the same type as the class type of which it is a member.
```c++
class Type
{
    private:
        static Type t; // ok for static
};
```
2. ```static``` members can be used as default arguments
```c++
class Type
{
    public:
        Type& func( int i = TypeVal);
    private:
        static int TypeVal;
}
```









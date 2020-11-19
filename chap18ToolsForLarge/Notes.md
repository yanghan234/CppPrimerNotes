- [Tools for large programs](#tools-for-large-programs)
  - [Exception handling](#exception-handling)
  - [Namespaces](#namespaces)
  - [Multiple and virtual inheritance](#multiple-and-virtual-inheritance)
    - [Multiple inheritance](#multiple-inheritance)
    - [Conversions and multple base classes](#conversions-and-multple-base-classes)
    - [Class scope under multiple inheritance](#class-scope-under-multiple-inheritance)
    - [Virtual inheritance](#virtual-inheritance)
    - [Constructors and virtual inheritance](#constructors-and-virtual-inheritance)
# Tools for large programs
## Exception handling
TBA

## Namespaces
TBA

## Multiple and virtual inheritance
Multiple inheritance is the ability to derive a class from more than one direct base class.
### Multiple inheritance
```c++
class Bear: public ZooAnimal
{};

class Panda: public Bear, public Endangered
{};
```
* Derived classes inherit state from each base class
Under multiple inheritance, an object of a derived class contains a subobject for each of its base classes.
* Derived constructros initialize all base classes
```c++
Panda::Panda() : Bear(), Endangered() {}
```
* Inherited constructors and multiple inheritance
A derived class can inherit its constructors from one or more of its base classes. But, it is an error to inherit the same constructor from more than one base class;

* Destructors and multiple inheritance
Destructor in a drived class is responsible for cleaning up resources allocated by that class only.
Destructors are always invoked in the reverse order from which the constructors are run.

* Copy and move operations for multiply derived classes

### Conversions and multple base classes
A pointer/reference to any of an object's accessible base classes can be used to point or refer to the object.
* Lookup based on type of pointer or reference
The static type of a pointer/reference decide what functions can be called. (The same as single inheritance)

### Class scope under multiple inheritance
Under multiple inheritance, name lookup simultaneously among all the direct base classes. If a name is found through more than one base class, then the use of that name is ambiguous.

It is legal to inherit multiple members with the same name, but we must specify which version we want to use.
```c++
BaseA::func();
// or
BaseB::func();
```

### Virtual inheritance
A class can inherit from the same base class more than once. For example, A -> B, A->C, B->D, C->D. D indirectly inherits from A twice. So, an object of D contains two subobjects of A, even though the two parts should be the same. 

**Virtual inheritance** lets a class specify that it is willing to share its base class. The shared base-class subobject is called a **virtual base class**. Regardless of how often the same virtual base appears in an inheritance hierachy, the derived object contains only one, shared subobject for that virtual base class.

Virtual derivation affects the classes that subsequently derive from a class with a virtual base; it doesn't affect the derived class itself.
```c++
class Base
{};
class D1: virtual public Base
{};
class D2: virtual public Base
{};
class DD: public D1, public D2
{};
DD obj; // obj contains only one copy of Base
Base *ptr = new DD(); // ok
```

* Visibility of virtual base-class members
Because there is only one shared subobject of the virtual shared base, members in that base can be accessed directly without specifying its name. If a member function from the virtual base is overridden along only one derivation path, then that overridden member can be used directly. 

In general, if a function has only one implementation in the hierachy, it can be accessed directly. 

### Constructors and virtual inheritance
In a virtual derivation, the virtual base is initialized by the most derived constructor.

Virtual base classes are always constructed prior to nonvirtual classes regardless of where they appear in the inheritance hierachy.

* Constructor and destructor order
A class can have more than one virtual base class. In that case, the virtual subobjects are constructed in left-to-right order as they appear in the derivation list.

Destructors are called in the reversed order in which constructors are called.



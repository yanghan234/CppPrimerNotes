- [Dynamic memory](#dynamic-memory)
  - [Dynamics  memory and smart pointers](#dynamics-memory-and-smart-pointers)
    - [The ```shared_ptr``` class](#the-shared_ptr-class)
      - [The ```make_shared``` function](#the-make_shared-function)
      - [Copy and assign ```shared_ptr```s](#copy-and-assign-shared_ptrs)
    - [Managing memory directly](#managing-memory-directly)
      - [Using ```new``` to dynamically allocate and initialize objects](#using-new-to-dynamically-allocate-and-initialize-objects)
      - [Dynamically allocated ```const``` objects](#dynamically-allocated-const-objects)
      - [Exhausted memory](#exhausted-memory)
      - [Free dynamic memory](#free-dynamic-memory)
    - [Using ```shared_ptr```s with ```new```](#using-shared_ptrs-with-new)
    - [Smart pointers and exceptions](#smart-pointers-and-exceptions)
      - [Smart pointers and dumb classes](#smart-pointers-and-dumb-classes)
    - [Using ```unique_ptr```](#using-unique_ptr)
      - [Passing and returning ```unique_ptr```](#passing-and-returning-unique_ptr)
      - [Passing a deleter to ```unique_ptr```](#passing-a-deleter-to-unique_ptr)
    - [Using ```weak_ptr```](#using-weak_ptr)
  - [Dynamic arrays](#dynamic-arrays)
    - [```new``` and ```delete```](#new-and-delete)
    - [Smart pointers and dynamic arrays](#smart-pointers-and-dynamic-arrays)
    - [The ```allocator``` class](#the-allocator-class)
# Dynamic memory
Static memory is used for local ```static``` objects, for class ```static``` data members and for variables defined outside any functions. ```static``` objects are allocated before they are used, and they are destroyed when the program ends. Stack memory is used for non```static``` objects defined inside functions. Stack objects exist only while the block in which they are defined is executing.

Objects allocated in static or stack memory are automatically created and detroyed by the compiler.

In addition to static and stack memory, there is another chunk of memory, known as the **free store** or **heap**. Programs use the heap for objects that they **dynamically allocate** at runtime.

## Dynamics  memory and smart pointers
In C++, dynamic memory is managed through a pair of operators: ```new``` and ```delete```. ```new``` allocates and optionally initializes an object in dynamic memory and returns a pointer to the object. ```delete``` takes a pointer to a dynamic object, destroys the object and frees the associated memory.

The new library procides two **smart pointer**s. A smart pointer acts like a regular pointer with the import exception that is automatically deletes the object to which it points.
1. ```shared_ptr```: allows multiple pointers to refer to the same object
2. ```unique_ptr```: owns the object to which it points
3. ```weak_ptr```: weak reference to an object managed by a ```shared_ptr```
All three are defined in ```memory``` header.

### The ```shared_ptr``` class
Smart pointers are templates, thus we must provide the type to which it points at declaration. For example,
```c++
shared_ptr<string> p1; // shared_ptr that can point at a string
shared_ptr<list<int>> p2; // shared_ptr that can point at a list of integers
// By default, these pointers are nullptr
```
#### The ```make_shared``` function
```make_shared``` function allocates and initializes an object and return a ```shared_ptr```. It passes its arguments to the constructor of the type to which it points.
```c++
shared_ptr<int> p = make_shared<int>(42); // p points to an int with value of 42;
shared_ptr<string> p = make_shared<string>(10,'9');
auto p = make_shared<vector<string>>();
```

#### Copy and assign ```shared_ptr```s
When we copy or assign a ```shared_ptr```, each ```shared_ptr``` keeps track of how many other ```shared_ptrs``` point to the same object.
```c++
auto p = make_shared<int>(42); // object to which p points has one user
auto q(p); // copy p to q; object to which p and q point has two users
```
We can think of a ```shared_ptr``` as if it has a counter. When we copy a ```shared_ptr```, the couter is incremented; when we assign a new value to the ```shared_ptr``` or destroy the pointer, the counter is decremented. Once a ```shared_ptr```'s counter goes to zero, the ```share_ptr``` automatically frees the object that it manages.

The implementation of ```shared_ptr``` might be different across platforms, for different compilers, or for different versions of the libraries. So, some implementation might not really maintain a counter, but we can think of it as if there is a counter.

### Managing memory directly
#### Using ```new``` to dynamically allocate and initialize objects
When we allocate dynamial memory with ```new```, the objects are initialized with the default constructor.
```c++
int *pi = new int;
string *ps = new string;
vector<int> *pv = new vector<int>();
```
#### Dynamically allocated ```const``` objects
It is legal to allocate ```const``` objects with ```new``` and ```const``` objects must be initialized.
```c++
const int *pci = new const int(4);
const string *pcs = new const string();
```
#### Exhausted memory
If there is no enough memory available, the ```new``` process will fail. By default, it raises a ```bad_alloc``` exception.
```c++
int *p1 = new int; // if allocation fails, new throws std::bac_alloc
```
**Placement new**: A placement new expression lets us pass additional arguments to ```new```. In the following example, it suppresses ```bad_alloc``` exception and return a ```nullptr``` instead.
```c++
int *p2 = new (nothrow) int;
```
#### Free dynamic memory
We can free dynamic memory using ```delete``` expression,
```c++
delete p; // it calls the destructor of the type to which p points
```
The pointer we pass to ```delete``` must either point to dynamically allocated memory or be a null pointer. Deleting a pointer to memory that was not allocated by ```new```, or deleting the same pointer value more than once, is undefined.
```c++
int i, *pi = &i;
delete i; // error, i is not a pointer
delete pi; // undefined, not dynamically allocated

double *pd1 = new double;
double *pd2 = pd1;
delete pd1; // ok
delete pd2; // error, the memory has already been freed

/* Even though const memory cannot be modified,
 * they can be freed with delete.
 */ 
const string *pcs = new const string;
delete pcs; // ok
```
Dynamically allocated objects exist unitl they are freed. If smart pointers are used, manual deleting is not needed.

**Common problems** using ```new``` and ```delete```
1. Neglecting to delete dynamic memory is known as memory leak. 
2. Using an object after it has been deleted.
3. Deleting the same memory twice.

### Using ```shared_ptr```s with ```new```
The smart pointer constructors that take pointers are ```explicit```. Hence, we cannot implicitly convert a built-in pointer to a pointer; we must use the direct form of initialization,
```c++
shared_ptr<double> p1; // ok, nullptr
shared_ptr<int> p2(new int(42)); // p2 points to an integer
shared_ptr<int> p3 = new int(1024); // error, must use direct initialization

shared_ptr<int> clone(int p)
{
    return new int(p); // error, implicit conversion to shared_ptr<int>
}
share_ptr<int> clone(int p)
{
    return shared_ptr<int>(new int(p)); // ok, explicit form of constructor
}
```
* Don't mix ordinary pointers and smart pointers --> Smart pointers may free the object to which the ordinary pointer points. This creates dangling pointer.
* Don't use ```get``` memeber function of smart functgion to initialize or assign another smart pointer. Smart pointers have a ```get()``` member function that returns a built-in pointer to the same object. This also mixes up build-in pointers and smart pointers and result in the same problem.

### Smart pointers and exceptions
When the program throws an exception, smart pointers can still free the dynamic memory while ordinary pointers can not.
```c++
void func()
{
    shared_ptr<int> p;
    // throws exception
    // the space to which p points will still be freed
}
void func()
{
    int *p = new int;
    // throws exception
    delete p; // this will not be executed when the program throws exception
}
```
#### Smart pointers and dumb classes
When a smart pointer tries to free allocated object, the destructor of that type will be called. However, some classes may not have a well-defined destructor. In this case, we can pass a **deleter** to the constructor of smart pointer. When smart pointer tries to free the space, the deleter will be called.
```c++
shared_ptr<T> p(new T, deleter);
```

### Using ```unique_ptr```
A ```unique_ptr``` owns the object to which it points. Unlike ```shared_ptr```, only one ```unique_ptr``` at a time can point to a given object. The object to which a ```unique_ptr``` points is destroyed when the ```unique_ptr``` is destroyed.
```c++
unique_ptr<double> p1;
unique_ptr<int> p2(new int(n));
```
Because ```unique_ptr``` owns the object to which it points, copying or assignment between ```unique_ptr```s are not supported.
```c++
unique_ptr<string> p1(new string("fdas"));
unique_ptr<string> p2(p1); // error, can not copy
unique_ptr<string> p3 = p2; // error, can not assign
```
If a ```unique_ptr``` gives up its ownership of the object, another ```unique_ptr``` can pick up the ownership.
```c++
unique_ptr<string> p2(p1.release()); // transfer ownership from p1 to p2; as a result, p1 becomes nullptr 
unique_ptr<string> p3(new string("123"));
p2.reset(p3.release()); // delete p2's old object and transfer ownership from p3 to p2, p3 becomes nullptr
```
#### Passing and returning ```unique_ptr```
There is one exception to the rule that we cannot copy a ```unique_ptr```: we can copy or assign a ```unique_ptr``` that is about to be destroyed. The most common example is when we return a ```unique_ptr``` from a function.
```c++
unique_ptr<int> clone(int p)
{
    return unique_ptr<int>(new int(p));
}
// alternatively
unique_ptr<int> clone(int p)
{
    unique_ptr<int> ret(new int(p));
    return ret;
}
```
#### Passing a deleter to ```unique_ptr```
Overridding the deleter in a ```unique_ptr``` affects the ```unique_ptr``` type.
```c++
unique_ptr<objT,delT> p(new objT, deleter);
```

### Using ```weak_ptr```
A ```weak_ptr``` is a smart pointer that does not control the lifetime of the object to which it points. Instead, a ```weak_ptr``` points to an object that is managed by a ```shared_ptr```. Binding a ```weak_ptr``` does not change the counter in ```shared_ptr```. If ```shared_ptr``` decrements to zero, the object will be detroyed even if there are still ```weak_ptr```s pointing to the same object.
```c++
auto p = make_shared<int>(42);
weak_ptr<int> wp(p);
```
Since ```weak_ptr``` does not guarantee the existence of the object it pointing to, we must check if the object exists when using ```weak_ptr```. The ```lock()``` function of ```weak_ptr``` returns the object if it exists.
```c++
if ( shared_ptr<int> p = wp.lock() ) // true if p is not null;
{
}
```

## Dynamic arrays
### ```new``` and ```delete```
We ask ```new``` to allocate an array of objects by specify the number of objects.
```c++
int *pia = new int[size];
delete[] pia; // free dynamically allocated arrays
```
### Smart pointers and dynamic arrays
```c++
unique_ptr<intp[]> up(new int[10]);
up.reset(); // automatically uses delete[] to destroy its pointer
```

### The ```allocator``` class
The library ```allocator``` class decouples the allocation of memory and construction of objects.
```c++
allocator<string> alloc;          // object that can allocate strings
auto const p = alloc.allocate(n); // allocate n strings
alloc.construct(p,"");
```
The space allocated are not unconstructed, and we have to call the ```construct()``` function to create the objects.
We then call ```destroy()``` function to destroy objects.
We call ```deallocate()``` function to deallocate space.






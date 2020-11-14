/* 2020-11-05
 * In this snippet, we write functions with complicated return type
 */
#include <iostream>
using namespace std;

// regular definition to return a pointer to array
int (*func1())[2]
{
    static int arr[] = {1,2};
    return &arr;
}

// using trailing return type to return a pointer to array
auto func2() -> int (*)[2]
{
    static int arr[] = {1,2};
    return &arr;
}

// regular definition to return reference to int[2]
int (&func3())[2]
{
    static int arr[] = {1,2};
    return arr;
}

// trailing definition to return reference to int[2]
auto func4() -> int (&)[2]
{
    static int arr[2] = {1,2};
    return arr;
}

void print(int (*pt)[2])
{
    // *pt gives us int[2]
    cout << (*pt)[0] << endl;
    cout << (*pt)[1] << endl;
}

void print(int (&ref)[2])
{
    // ref itself is int[2]
    cout << ref[0] << endl;
    cout << ref[1] << endl;
}

int main()
{
    int (*pt1)[2] = func1();
    print(pt1);
    int (*pt2)[2] = func2();
    print(pt2);
    int (&ref1)[2] = func3();
    print(ref1);
    int (&ref2)[2] = func4();
    print(ref2);
}
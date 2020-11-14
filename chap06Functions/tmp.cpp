#include <iostream>
#include <vector>
using namespace std;

vector<int> func()
{
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    return v;
}

int main()
{
    vector<int> v = func();
    for ( size_t i = 0; i < v.size(); ++i )
        cout << v[i] << endl;
    return 0;
}
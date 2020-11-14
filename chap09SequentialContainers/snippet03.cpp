#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v;
    int n = 100;
    for ( int i = 0; i < n; ++i )
    {
        cout << " capacity = " << v.capacity() << ", size = " << v.size() << endl;
        v.push_back(i);
    }

    while ( !v.empty() )
    {
        cout << " capacity = " << v.capacity() << ", size = " << v.size() << endl;
        v.pop_back();
    }
    return 0;
}

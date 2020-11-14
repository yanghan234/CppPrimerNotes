#include <iostream>
#include <string>
using namespace std;

class StrVec
{
    private:
        string *begin;
        string *end;
        string *first_free;
        size_t _capacity;
        size_t _size;
        allocator<string> alloc;
    public:
        StrVec() = default;
        bool empty() const { return _size == 0; }
        size_t size() const { return _size; }
        size_t capacity() const { return _capacity; }
        void push_back(const string &);
        void push_back(string&&);
};

void StrVec::push_back(const string &s)
{
    cout << "Push back lvalue" << endl;
    if ( empty() )
    {
        begin = end = first_free = alloc.allocate(1);
        alloc.construct(first_free,s);
    }
}

void StrVec::push_back(string &&s)
{
    cout << "Push back rvalue" << endl;
    if ( empty() )
    {
        begin = end = first_free = alloc.allocate(1);
        alloc.construct(first_free,s);
    }
}


int main()
{
    string s("123");
    StrVec svec;
    svec.push_back(std::move(s));
    return 0;
}
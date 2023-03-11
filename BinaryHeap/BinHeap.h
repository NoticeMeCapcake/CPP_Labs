#ifndef Bin
#define Bin

#include <iostream>
#include <vector>
#include <string>
#include <exception>

using namespace std;

template<typename T1>
class NotExistException : public exception {
private:
    string except;
    T1 _key;
public:
    NotExistException (const T1 &key1) {
        _key = key1;
        except = "Element does not exist";
    }
    const char* what() const noexcept {
        // cout << except << ": key = " << _key << endl;
        return except.c_str();
    };
};
template<typename T1>
class AlreadyExistException : public exception {
private:
    string except;
    T1 _key;
public:
    AlreadyExistException (const T1 &key1) {
        _key = key1;
        except = "Element already exist";
    }
    const char* what() const noexcept {
        // cout << except << ": key = " << _key << endl;
        return except.c_str();
    };
};
/////////////////

template <typename T1, typename T2>
void prt(const T1 &_key, const T2 &val, const int &depth){
    cout << "Depth: " << depth << "; Key = " << _key << "; value = " << val << endl;
}

template <typename T1, typename T2>
class Node {
public:
    Node(T1 _key, T2 _value) {
        key = _key;
        value = _value;
    }
    T1 key;
    T2 value;
    ~Node() {}
};

template <typename T>
class Comparator {
public:
    virtual int compare(const T& left, const T& right) const = 0;
    virtual ~Comparator() {}
};

class CmpInt : public Comparator<int> {
public:
    virtual int compare(const int& left, const int& right) const {
        return (left > right) ? 1 : ((left < right) ? -1 : 0);
    }
};


class CmpStr : public Comparator<string> {
public:
    virtual int compare(const string& left, const string& right) const {
        return (left > right) ? 1 : ((left < right) ? -1 : 0);
    }
};

template <typename T1, typename T2>
class Heap {
protected:
    vector<Node<T1, T2>> heap;
    Comparator<T1> *cmp;
public:
    virtual void insert(const T1 &key1, const T2 &val) = 0;
    virtual T2 &get() = 0;
    virtual T2 pop() = 0;
    virtual Heap<T1, T2> *merge(Heap<T1, T2> * tree) = 0;
    virtual ~Heap(){};
};

template <typename T1, typename T2>
class HeapFabric {
public:
    virtual Heap<T1, T2> *create(Comparator<T1> *) = 0;
};


template <typename T1, typename T2>
class BinHeapFabric: HeapFabric<T1, T2> {
public:
    Heap<T1, T2> *create(Comparator<T1> *cmp);
};


template <typename T1, typename T2>
class BinHeap: public Heap<T1, T2> {
protected:
    friend class BinHeapFabric<T1, T2>;
    void shift_up(unsigned long long i);
    void shift_down(unsigned long long i);
    BinHeap(Comparator<T1> *comp);
public:
    virtual void insert(const T1 &key1, const T2 &val);
    virtual T2 &get();
    virtual T2 pop();
    virtual Heap<T1, T2> *merge(Heap<T1, T2> * tree);
    virtual ~BinHeap(){};
    virtual void traverse();
};

#endif

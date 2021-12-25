#ifndef Bin
#define Bin

#include <iostream>
#include <vector>
#include <string>
#include <exception>

using namespace std;

////////////
template<typename T1>
class NotExistException : public exception {
private:
    string except;
    T1 _key;
public:
    NotExistException (const T1 key1) {
        _key = key1;
        except = "Element does not exist";
    }
    const char* what() const noexcept {
        cout << except << ": key = " << _key << endl;
        return except.c_str();
    };
};
template<typename T1>
class AlreadyExistException : public exception {
private:
    string except;
    T1 _key;
public:
    AlreadyExistException (const T1 key1) {
        _key = key1;
        except = "Element already exist";
    }
    const char* what() const noexcept {
        cout << except << ": key = " << _key << endl;
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
    Node(T1 _key, T2 _value);
    T1 key;
    T2 value;
    Node *left;
    Node *right;
    Node *&chld(const int var) {
        if (var <= 0)
            return left;
        else
            return right;
    }
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
class AC {
public:
    virtual T2 pop(const T1&) = 0;
    virtual void push(const Node<T1, T2>&) = 0;
    virtual T2& search(const T1&) = 0;
    virtual void prefix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const = 0;
    virtual void infix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const = 0;
    virtual void postfix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const = 0;
    virtual ~AC(){};
};


template <typename T1, typename T2>
class BinaryTree : public AC <T1, T2> {
protected:
    BinaryTree() {}
    Comparator<T1> *cmp = nullptr;
    void copy_traverse(const Node<T1, T2> *_root, vector<Node<T1, T2>> &vec) const;
    void del_traverse(const Node<T1, T2> *_root, vector<T1> &vec) const;
    void prefix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const;
    void infix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const;
    void postfix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const;
    class Pop {
    public:
        int pop_node(T1 &_key, T2 &val, vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> * tree);
    protected:
        virtual void hook1(Node<T1, T2> *&removable, Node<T1, T2> *&tmp_par, int param, vector<Node<T1, T2> (*)> v, BinaryTree<T1, T2> *tree);
        virtual void hook2(vector<Node<T1, T2> (*)> &vec, const T1 &_key, BinaryTree<T1, T2> * tree);
    };
    class Push {
    public:
        int push_node(const Node<T1, T2> &_node, vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> * tree);
    protected:
        virtual void hook1(vector<Node<T1, T2> (*)> &vec, const Node<T1,T2> &_node, BinaryTree<T1, T2> * tree);
        virtual void hook2(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *tree);
    };
    class Search {
    public:
        int find_node(const T1 &, Node<T1, T2> *&, vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *);
    protected:
        virtual void hook(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *);
    };

    Pop *popper = nullptr;
    Push *pusher = nullptr;
    Search *searcher = nullptr;
    Node<T1, T2> *root = nullptr;
    void clean();
public:

    BinaryTree(Comparator<T1> *_cmp);
    BinaryTree(const BinaryTree<T1, T2> &tree); // copy
    BinaryTree& operator =(const BinaryTree<T1, T2> &tree);
    virtual T2 pop(const T1 &);
    virtual void push(const Node<T1, T2> &);
    virtual T2& search(const T1&);
    virtual void prefix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const;
    virtual void infix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const;
    virtual void postfix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const;
    ~BinaryTree();
};


template <typename T1, typename T2>
Node<T1, T2>:: Node(T1 _key, T2 _value) {
    this->key = _key;
    this->value = _value;
    this->left = nullptr;
    this->right = nullptr;
}

#endif

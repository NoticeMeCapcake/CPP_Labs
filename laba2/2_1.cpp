#include <iostream>
#include <vector>
#include <string>
#include <exception>

using namespace std;



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
        else if (var > 0)
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
private:
    Node<T1, T2> *root;
    Comparator<T1> *cmp;
    void copy_traverse(const Node<T1, T2> *_root, vector<Node<T1, T2>> &vec) const;
    void del_traverse(const Node<T1, T2> *_root, vector<T1> &vec) const;
    void prefix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const;
    void infix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const;
    void postfix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const;
    void clean();
    class Pop {
    public:
        int pop_node(Node<T1, T2> *&root, const T1&, T2 &, Comparator<T1> *);  // protected + virtual
    protected:
        virtual void hook();
    };
    class Push {
    public:
        int push_node(Node<T1, T2> *&root, const Node<T1, T2> &, Comparator<T1> *);
    protected:
        virtual void hook();
    };
    class Search {
    public:
        int find_node(Node<T1, T2> *&root, const T1 &, Node<T1, T2> *&, Comparator<T1> *) const;
    protected:
        virtual void hook() const;
    };

    Pop *popper;
    Push *pusher;
    Search *searcher;
public:
    ////////////
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


template <typename T1, typename T2>
BinaryTree<T1,T2>:: BinaryTree(Comparator<T1> *_cmp) {
    this->cmp = _cmp;
    popper = new Pop;
    pusher = new Push;
    searcher = new Search;
    root = nullptr;
}

template <typename T1, typename T2>
void BinaryTree<T1,T2>:: copy_traverse(const Node<T1, T2> *_root, vector<Node<T1, T2>>& vec) const {
    if (_root == nullptr)
        return;
    vec.push_back(*_root);
    copy_traverse(_root->left, vec);
    copy_traverse(_root->right, vec);
}

template <typename T1, typename T2>
void BinaryTree<T1,T2>:: del_traverse(const Node<T1, T2> *_root, vector<T1> &vec) const {
    if (_root == nullptr)
        return;
    del_traverse(_root->left, vec);
    del_traverse(_root->right, vec);
    vec.push_back(_root->key);
}

template <typename T1, typename T2>
BinaryTree<T1,T2>:: BinaryTree(const BinaryTree<T1, T2> &tree) {
    this->cmp = tree.cmp;
    popper = new Pop;
    pusher = new Push;
    searcher = new Search;
    vector<Node<T1, T2>> _tree;
    copy_traverse(tree.root, _tree);
    for (auto node : _tree) {
        this->push(node);
    }
}

template <typename T1, typename T2>
T2 BinaryTree<T1, T2>::pop(const T1 &_key) {

    T2 popped_val;
    int ex_code = popper->pop_node(root, _key, popped_val, cmp);
    if (ex_code) {
        throw NotExistException(_key);;
    }
    return popped_val;
}


template <typename T1, typename T2>
int BinaryTree<T1, T2>:: Pop:: pop_node(Node<T1, T2> *&root, const T1 &_key, T2 &val, Comparator<T1> *cmp) {

    Node<T1, T2> *tmp_par = root;
    Node<T1, T2> *removeable = root;
    int param = 0;
    while (int compared = cmp->compare(_key, removeable->key)) {
            if (removeable->chld(compared) == nullptr)
                return 1;

            param = compared;
            tmp_par = removeable;
            removeable = removeable->chld(compared);
    }

    val = removeable->value;

    if (removeable->left == nullptr and removeable->right == nullptr) {
        if (param == 0)
            root = nullptr;
        else
            tmp_par->chld(param) = nullptr;

        delete removeable;

        hook();
        return 0;
    }
    else if ((removeable->left != nullptr and removeable->right == nullptr) or
             (removeable->left == nullptr and removeable->right != nullptr)) {
        if (param == 0){
            if (removeable->left != nullptr)
                root = removeable->left;
            else
                root = removeable->right;
        }
        else {
            if (removeable->left != nullptr)
                tmp_par->chld(param) = removeable->left;
            else
                tmp_par->chld(param) = removeable->right;
        }
        delete removeable;
        return 0;
    }
    else {
        Node <T1, T2> *tmp = removeable->right;
        tmp_par = removeable;
        param = 1;
        while (tmp->left != nullptr) {
            param = -1;
            tmp_par = tmp;
            tmp = tmp->left;
        }
        removeable->key = tmp->key;
        removeable->value = tmp->value;
        tmp_par->chld(param) = tmp->right;
        delete tmp;
        hook();
        return 0;
    }
}



template <typename T1, typename T2>
T2& BinaryTree<T1,T2>:: search(const T1 &_key) {

    Node<T1, T2> *req_node;
    int ex_code = searcher->find_node(root, _key, req_node, cmp);
    if (ex_code) {
        throw NotExistException(_key);
    }
    return req_node->key;
}


template<typename T1, typename T2>
int BinaryTree<T1, T2>:: Search:: find_node(Node<T1, T2> *&root, const T1 &_key, Node<T1, T2> *&req_node, Comparator<T1> *cmp) const {
    hook();
    Node<T1, T2> *tmp = root;
    while (int res = cmp->compare(_key, tmp->key)) {
        tmp = tmp->chld(res);
        if (tmp == nullptr) {
            hook();
            return 1;
        }
    }
    req_node = tmp;
    hook();
    return 0;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Search:: hook() const{
    return;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Push:: hook() {
    return;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Pop:: hook() {
    return;
}


template <typename T1, typename T2>
BinaryTree<T1,T2>& BinaryTree<T1,T2>:: operator =(const BinaryTree<T1, T2> &tree) {
    clean();
    vector<Node<T1, T2>> _tree;
    copy_traverse(tree.root, _tree);
    for (auto node : _tree) {
        this->push(node);
    }
    return *this;
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: push(const Node<T1, T2> &_node) {

    int ex_code = pusher->push_node(root, _node, cmp);
    if (ex_code) {
        throw AlreadyExistException(_node.key);
    }
}


template<typename T1, typename T2>
int BinaryTree<T1, T2>:: Push:: push_node(Node<T1, T2> *&root, const Node<T1, T2> &_node, Comparator<T1> *cmp) {
    hook();
    Node<T1, T2> *node = new Node<T1, T2>(_node.key, _node.value);
    Node<T1, T2> *tmp = root;
    if (tmp == nullptr) {
        root = node;
        hook();
        return 0;
    }
    while (tmp != nullptr) {
        switch (cmp->compare(node->key, tmp->key)) {
        case 1:
            if (tmp->right != nullptr) {
                tmp = tmp->right;
                continue;
            }
            else {
                tmp->right = node;
                hook();
                return 0;
            }
        case 0:
            delete node;
            hook();
            return 1;
        case -1:
            if (tmp->left != nullptr) {
                tmp = tmp->left;
                continue;
            }
            else {
                tmp->left = node;
                hook();
                return 0;
            }
        }
    }
    return 1;
}

template <typename T1, typename T2>
void BinaryTree<T1,T2>:: prefix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const {
    prefix_traverse(root, func, 0);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: prefix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const {
    if (_root == nullptr)
        return;
    func(_root->key, _root->value, _depth);
    prefix_traverse(_root->left, func, _depth + 1);
    prefix_traverse(_root->right, func, _depth + 1);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: infix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const {
    infix_traverse(root, func, 0);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: infix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const {
    if (_root == nullptr)
        return;
    infix_traverse(_root->left, func, _depth + 1);
    func(_root->key, _root->value, _depth);
    infix_traverse(_root->right, func, _depth + 1);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: postfix(void (*func)(const T1 &_key, const T2 &val, const int &depth)) const {
    postfix_traverse(root, func, 0);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: postfix_traverse(const Node<T1, T2> *_root, void (*func)(const T1 &_key, const T2 &val, const int &depth), int _depth) const {
    if (_root == nullptr)
        return;
    postfix_traverse(_root->left, func, _depth + 1);
    postfix_traverse(_root->right, func, _depth + 1);
    func(_root->key, _root->value, _depth);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: clean() {
    if (root != nullptr) {
        this->pop(root->key);
    }
}


template <typename T1, typename T2>
BinaryTree<T1,T2>:: ~BinaryTree() {
    clean();
    delete popper;
    delete pusher;
    delete searcher;
}

int main() {
    CmpInt compar;
    BinaryTree<int, int> tree1(&compar);
    Node<int, int> node1(4, 16), node2(23, 11), node3(1, 44);
    tree1.push(node1);
    tree1.prefix(prt);
    tree1.push(node2);
    tree1.push(node3);
    tree1.prefix(prt);
    tree1.pop(4);
    tree1.prefix(prt);
    try {
        tree1.pop(323);
    }
    catch(BinaryTree<int, int>::NotExistException error) {
        error.what();
    }

    BinaryTree<int, int> tree2(tree1);

    tree2.prefix(prt);

    return 0;
}

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
        int pop_node(Node<T1, T2> *&root, T1 &_key, T2 &val, Comparator<T1> *cmp, vector<Node<T1, T2> (*)> &v);  // protected + virtual
        virtual void hook(Node<T1, T2> *&root, vector<Node<T1, T2> (*)> &vec, const T1 &_key, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree);
    };
    class Push {
    public:
        int push_node(Node<T1, T2> *&root, const Node<T1, T2> &_node, Comparator<T1> *cmp, vector<Node<T1, T2> (*)> &v);
        virtual void hook(Node<T1,T2> *&root, vector<Node<T1, T2> (*)> &vec, const Node<T1,T2> &_node, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree);
    };
    class Search {
    public:
        int find_node(Node<T1, T2> *&root, const T1 &, Node<T1, T2> *&, Comparator<T1> *) const;
        virtual void hook() const;
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
    T1 key1 = _key;
    vector<Node<T1, T2> (*)> nodes_to_do_smth;
    int ex_code = popper->pop_node(this->root, key1, popped_val, cmp, nodes_to_do_smth);
    if (ex_code) {
        throw NotExistException<T1>(_key);;
    }
    return popped_val;
}


template <typename T1, typename T2>
int BinaryTree<T1, T2>:: Pop:: pop_node(Node<T1, T2> *&root, T1 &_key, T2 &val, Comparator<T1> *cmp, vector<Node<T1, T2> (*)> &v) {

    Node<T1, T2> *tmp_par = root;
    Node<T1, T2> *removeable = root;
    int param = 0;
    while (int compared = cmp->compare(_key, removeable->key)) {
            if (removeable->chld(compared) == nullptr)
                return 1;
            v.push_back(removeable);
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
        v.push_back(tmp_par);
        param = 1;
        while (tmp->left != nullptr) {
            param = -1;
            tmp_par = tmp;
            v.push_back(tmp_par);
            tmp = tmp->left;
        }
        removeable->key = tmp->key;
        removeable->value = tmp->value;
        tmp_par->chld(param) = tmp->right;
        delete tmp;
        return 0;
    }
}



template <typename T1, typename T2>
T2& BinaryTree<T1,T2>:: search(const T1 &_key) {

    Node<T1, T2> *req_node;
    int ex_code = searcher->find_node(root, _key, req_node, cmp);
    if (ex_code) {
        throw NotExistException<T1>(_key);
    }
    return req_node->key;
}


template<typename T1, typename T2>
int BinaryTree<T1, T2>:: Search:: find_node(Node<T1, T2> *&root, const T1 &_key, Node<T1, T2> *&req_node, Comparator<T1> *cmp) const {
    Node<T1, T2> *tmp = root;
    while (int res = cmp->compare(_key, tmp->key)) {
        tmp = tmp->chld(res);
        if (tmp == nullptr) {
            return 1;
        }
    }
    req_node = tmp;
    return 0;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Search:: hook() const{
    return;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Push:: hook(Node<T1,T2> *&root, vector<Node<T1, T2> (*)> &vec, const Node<T1,T2> &_node, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree) {
    if (vec.size() == 0)
        root = new Node<T1, T2>(_node.key, _node.value);
    else {
        Node<T1, T2> *father = vec[vec.size() - 1];
        father->chld(cmp->compare(_node.key, father->key)) = new Node<T1, T2>(_node.key, _node.value);
    }
    return;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Pop:: hook(Node<T1, T2> *&root, vector<Node<T1, T2> (*)> &vec, const T1 &_key, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree) {
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
    vector<Node<T1, T2> (*)> v;
    int ex_code = pusher->push_node(root, _node, cmp, v);
    if (ex_code) {
        throw AlreadyExistException<T1>(_node.key);
    }
    pusher->hook(root, v, _node, cmp, this);
}


template<typename T1, typename T2>
int BinaryTree<T1, T2>:: Push:: push_node(Node<T1, T2> *&root, const Node<T1, T2> &_node, Comparator<T1> *cmp, vector<Node<T1, T2> (*)> &v) {
    Node<T1, T2> *tmp = root;
    if (tmp == nullptr) {
        return 0;
    }
    while (tmp != nullptr) {
        v.push_back(tmp);
        int compared = cmp->compare(_node.key, tmp->key);
        if (!compared)
            return 1;

        if (!(tmp->chld(compared)))
            return 0;
        tmp = tmp->chld(compared);
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
    while (root != nullptr) {
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



template <typename T1, typename T2>
class AVL_Node : public Node<T1, T2> {
public:
    AVL_Node(T1 _key, T2 _val) : Node<T1, T2>(_key, _val) {
    }
    int b_f = 0;
};


template <typename T1, typename T2>
class AVL : public BinaryTree<T1, T2> {
public:
    AVL(Comparator<T1> *comp);
    AVL(const AVL<T1,T2> &tree);
    AVL &operator=(const AVL<T1,T2> &tree);
    virtual T2 pop(const T1&);
    virtual void push(const Node<T1, T2>&);
    ~AVL();

protected:
    class AVL_Push : public BinaryTree<T1, T2>::Push {
        virtual void hook(Node<T1,T2> *&root, vector<Node<T1, T2> (*)> &vec, const Node<T1,T2> &_key, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree);
    };
    class AVL_Pop : public BinaryTree<T1, T2>::Pop {
        virtual void hook(Node<T1, T2> *&root, vector<Node<T1, T2> (*)> &vec, const T1 &_key, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree);
    };
    class AVL_Search : public BinaryTree<T1, T2>::Search { // РАБОТАЕТ ТАК ЖЕ КАК И В БСТ
    };
private:
    void small_rotate(Node<T1, T2> ** rootative);
    void big_rotate(Node<T1, T2> ** rootative);
};

template <typename T1, typename T2>
AVL<T1, T2>::AVL(Comparator<T1> *comp){
    this->cmp = comp;
    this->popper = new AVL_Pop;
    this->pusher = new AVL_Push;
    this->searcher = new AVL_Search;
}


template <typename T1, typename T2>
AVL<T1, T2>::AVL(const AVL<T1,T2> &tree) {
    this->cmp = tree.cmp;
    this->popper = new AVL_Pop;
    this->pusher = new AVL_Push;
    this->searcher = new AVL_Search;
    vector<Node<T1, T2>> _tree;
    copy_traverse(tree.root, _tree);
    for (auto node : _tree) {
        this->push(node);
    }
}


template <typename T1, typename T2>
AVL<T1, T2> &AVL<T1, T2>::operator=(const AVL<T1,T2> &tree) {
    this->clean();
    vector<Node<T1, T2>> _tree;
    copy_traverse(tree.root, _tree);
    for (auto node : _tree) {
        this->push(static_cast<AVL_Node<T1, T2>>(node));
    }
    return *this;
}


template <typename T1, typename T2>
T2 AVL<T1, T2>::pop(const T1 &_key) {

    T2 popped_val;
    T1 key1 = _key;
    vector<Node<T1, T2> (*)> nodes_to_balance;
    int ex_code = this->popper->pop_node(this->root, key1, popped_val, this->cmp, nodes_to_balance);
    if (ex_code) {
        throw NotExistException<T1>(_key);
        // throw -1;
    }
    if (this->root != nullptr)
        this->popper->hook(this->root, nodes_to_balance, _key, this->cmp, this);
    return popped_val;
}


template <typename T1, typename T2>
void AVL<T1, T2>::small_rotate(Node<T1, T2> ** rootative) {
    AVL_Node<T1, T2> *rooty = static_cast<AVL_Node<T1, T2>*>(*rootative);
    int factor = rooty->b_f / (((rooty->b_f > 1) or (rooty->b_f < -1)) ? 2 : 1);
    AVL_Node<T1, T2> *tmp = static_cast<AVL_Node<T1, T2>*>((*rootative)->chld(factor));

    if (tmp->b_f != 0) {
        rooty->b_f = 0;
        tmp->b_f = 0;
    }
    else {
        rooty->b_f = -factor;
        tmp->b_f = factor;
    }

    rooty->chld(factor) = tmp->chld(-factor);
    tmp->chld(-factor) = *rootative;
    *rootative = static_cast<Node<T1, T2> *>(tmp);

}


template <typename T1, typename T2>
void AVL<T1, T2>::big_rotate(Node<T1, T2> ** rootative) {
    AVL_Node<T1, T2> *tmp_a, *tmp_b, *rooty = static_cast<AVL_Node<T1, T2>*>(*rootative);
    int factor = rooty->b_f / (((rooty->b_f > 1) or (rooty->b_f < -1)) ? 2 : 1);
    tmp_a = static_cast<AVL_Node<T1, T2>*>((*rootative)->chld(factor));
    tmp_b = static_cast<AVL_Node<T1, T2>*>(tmp_a->chld(-factor));

    tmp_a->chld(-factor) = tmp_b->chld(factor);
    tmp_b->chld(factor) = tmp_a;
    rooty->chld(factor) = tmp_b->chld(-factor);
    tmp_b->chld(-factor) = *rootative;

    rooty->b_f = (-factor - tmp_b->b_f) / 2;
    tmp_a->b_f = (factor - tmp_b->b_f) / 2;
    tmp_b->b_f = 0;

    *rootative = tmp_b;
}


template <typename T1, typename T2>
void AVL<T1, T2>:: AVL_Pop::hook(Node<T1, T2> *&root, vector<Node<T1, T2> (*)> &vec, const T1 &_key, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree) {
    T1 tmp_key = _key;
    long long len = vec.size();
    for (long long i = len - 1; i >= 0; i--) {
        AVL_Node<T1, T2> *node = static_cast<AVL_Node<T1, T2>*>(vec[i]);
        int res = cmp->compare(tmp_key, node->key);
        if (res > 0)
            node->b_f--;
        else
            node->b_f++;
        if (node->b_f == 1 or node->b_f == -1){
            return;
        }
        else if (node->b_f == 2 or node->b_f == -2) {
            if ((static_cast<AVL_Node<T1, T2>*>(node->chld(node->b_f/2))->b_f == node->b_f/2) or (static_cast<AVL_Node<T1, T2>*>(node->chld(node->b_f/2))->b_f == 0)) { // SMALL ROTATE
                static_cast<AVL<T1, T2> *>(tree)->small_rotate(&(vec[i]));
            }
            else { // BIG ROTATE
                static_cast<AVL<T1, T2> *>(tree)->big_rotate(&(vec[i]));
            }
            if (i) {
                (vec[i - 1])->chld(cmp->compare((vec[i])->key, (vec[i-1])->key)) = vec[i];
            }
            else {
                root = vec[i];
            }
            if ((static_cast<AVL_Node<T1, T2>*>(node))->b_f != 0)
                return;
        }
        tmp_key = node->key;
    }
}


template<typename T1, typename T2>
void AVL<T1, T2>::AVL_Push::hook(Node<T1,T2> *&root, vector<Node<T1, T2> (*)> &vec, const Node<T1,T2> &_node, Comparator<T1> *cmp, BinaryTree<T1, T2> * tree) {
    T1 tmp_key = _node.key;
    long long len = vec.size();
    if (len == 0)
        root = new AVL_Node<T1, T2>(_node.key, _node.value);
    else {
        Node<T1, T2> *father = vec[len - 1];
        father->chld(cmp->compare(_node.key, father->key)) = new AVL_Node<T1, T2>(_node.key, _node.value);
    }
    for (long long i = len - 1; i >= 0; i--) {
        AVL_Node<T1, T2> *node = static_cast<AVL_Node<T1, T2>*>(vec[i]);
        int res = cmp->compare(tmp_key, node->key);
        if (res < 0)
            node->b_f--;
        else
            node->b_f++;
        if (node->b_f == 0){
            return;
        }
        else if (node->b_f == 2 or node->b_f == -2) {
            if ((static_cast<AVL_Node<T1, T2>*>(node->chld(node->b_f/2))->b_f == node->b_f/2) or (static_cast<AVL_Node<T1, T2>*>(node->chld(node->b_f/2))->b_f == 0)) { // SMALL ROTATE
                static_cast<AVL<T1, T2> *>(tree)->small_rotate(&(vec[i]));
            }
            else { // BIG ROTATE
                static_cast<AVL<T1, T2> *>(tree)->big_rotate(&(vec[i]));
            }
            if (i) {
                (vec[i - 1])->chld(cmp->compare((vec[i])->key, (vec[i-1])->key)) = vec[i];
            }
            else {
                root = vec[i];
            }
            if ((static_cast<AVL_Node<T1, T2>*>(node))->b_f == 0)
                return;
        }
        tmp_key = vec[i]->key;
    }
    return;
}



template <typename T1, typename T2>
void AVL<T1, T2>:: push(const Node<T1, T2> &_node) {
    vector<Node<T1, T2> (*)> v;
    int ex_code = this->pusher->push_node(this->root, _node, this->cmp, v);
    if (ex_code) {
        throw AlreadyExistException<T1>(_node.key);
        // throw -1;
    }
    this->pusher->hook(this->root, v, _node, this->cmp, this);
}


template <typename T1, typename T2>
AVL<T1, T2>::~AVL() {
    this->clean();
    delete this->popper;
    delete this->pusher;
    delete this->searcher;
}






int main() {
    CmpInt compar;
    AVL<int, int> tree1(&compar);
    Node<int, int> node1(6, 16), node2(23, 11), node3(8, 44);
    tree1.push(node1);
    tree1.prefix(prt);
    cout << endl;
    tree1.push(node2);
    tree1.push(node3);
    tree1.prefix(prt);

    return 0;
}



// template <typename T1, typename T2>
// int &AVL<T1, T2>:: AVL_Push:: push(Node<T1, T2> *&root, const Node<T1, T2> &_node, Comparator<T1> *cmp) {
    //     AVL_Node<T1, T2> *t = root; // родитель lmp
    //     AVL_Node<T1, T2> *s = root; // место перебалансировки
    //     AVL_Node<T1, T2> *p = root; // продвижение по дереву
    //     AVL_Node<T1, T2> *q = root;
    //     int res = 0;
    //     while (q != nullptr) {
        //         res = cmp->compare(_node.key, tmp->key);
        //         if (!res) {
            //             return 1;
            //         }
            //         q = p->chld(res)
            //         if (!q) {
                //             break;
                //         }
                //         if (q->b_f) {
                    //             t = p;
                    //             s = q
                    //         }
                    //         p = q
                    //     }
                    //
                    //     q = new AVL_Node<T1, T2>(_node.key, _node.val);
                    //     if (!root) {
                        //         root = q;
                        //         return;
                        //     }
                        //     p->chld(res) = q;
                        //     int a = (_node->key < s->key) ? -1 : 1;
                        //     p = s->chld(a);
                        //     AVL_Node<T1, T2> *r = p;
                        //     while (p != q) {
                            //         res = cmp->compare(_node->key, p->key)
                            //         if (!res) break;
                            //         p->b_f = res;
                            //         p = p.chld(res);
                            //     }
                            //
                            //     if (s->b_f != a) {
                                //         s->b_f += a;
                                //         return 0;
                                //     }
                                //
                                //     if (r->b_F == a) { // SMALL ROTATE
                                    //         p = r;
                                    //         s->chld(a) = r->chld(-a);
                                    //         r->chld(-a) = s;
                                    //         s->b_f = 0;
                                    //         r->b_f = 0;
                                    //     }
                                    //     else { // TWIN ROTATE
                                        //         p = r->chld(-a);
                                        //         r->chld(-a) = p->chld(a);
                                        //         p->chld(a) = r;
                                        //         s->chld(a) = p->chld(-a);
                                        //         p->chld(-a) = s;
                                        //         s->b_f = (-a - p->b_f) / 2;
                                        //         r->b_f = (a - p->b_f) / 2;
                                        //         p->b_f = 0;
                                        //     }
                                        //     if (s == t->right) {
                                            //         t->right = p;
                                            //     }
                                            //     else {
                                                //         t->left = p
                                                //     }
                                                //     return 0;
                                                // }

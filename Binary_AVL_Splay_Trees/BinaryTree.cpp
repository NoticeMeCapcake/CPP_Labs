#ifndef BinR
#define BinR

#include "BinaryTree.h"

using namespace std;

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
    int ex_code = popper->pop_node(key1, popped_val, nodes_to_do_smth, this);
    if (ex_code) {
        throw NotExistException<T1>(_key);;
    }
    return popped_val;
}


template <typename T1, typename T2>
int BinaryTree<T1, T2>:: Pop:: pop_node(T1 &_key, T2 &val, vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> * tree) {

    Node<T1, T2> *tmp_par = tree->root;
    Node<T1, T2> *removeable = tree->root;
    int param = 0;
    while (int compared = tree->cmp->compare(_key, removeable->key)) {
            if (removeable->chld(compared) == nullptr)
                return 1;
            v.push_back(removeable);
            param = compared;
            tmp_par = removeable;
            removeable = removeable->chld(compared);
    }

    val = removeable->value; // всё, что ниже запихнуть в

    hook1(removeable, tmp_par, param, v, tree);

    hook2(v, _key, tree);
    return 0;
}



template <typename T1, typename T2>
void BinaryTree<T1, T2>:: Pop:: hook1(Node<T1, T2> *&removeable, Node<T1, T2> *&tmp_par, int param, vector<Node<T1, T2> (*)> v, BinaryTree<T1, T2> *tree) {
    if (removeable->left == nullptr and removeable->right == nullptr) {
        if (param == 0)
            tree->root = nullptr;
        else
            tmp_par->chld(param) = nullptr;

        delete removeable;
    }
    else if ((removeable->left != nullptr and removeable->right == nullptr) or
             (removeable->left == nullptr and removeable->right != nullptr)) {
        if (param == 0){
            if (removeable->left != nullptr)
                tree->root = removeable->left;
            else
                tree->root = removeable->right;
        }
        else {
            if (removeable->left != nullptr)
                tmp_par->chld(param) = removeable->left;
            else
                tmp_par->chld(param) = removeable->right;
        }
        delete removeable;
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
    }
}


template <typename T1, typename T2>
T2& BinaryTree<T1,T2>:: search(const T1 &_key) {

    Node<T1, T2> *req_node;
    vector<Node<T1, T2> (*)> vec;
    int ex_code = searcher->find_node(_key, req_node, vec, this);
    if (ex_code) {
        throw NotExistException<T1>(_key);
    }
    return req_node->key;
}


template<typename T1, typename T2>
int BinaryTree<T1, T2>:: Search:: find_node(const T1 &_key, Node<T1, T2> *&req_node, vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *tree) {
    Node<T1, T2> *tmp = tree->root;
    if (!tmp) return 1;
    while (int res = tree->cmp->compare(_key, tmp->key)) {
        v.push_back(tmp);
        tmp = tmp->chld(res);
        if (tmp == nullptr) {
            return 1;
        }
    }
    v.push_back(tmp);
    req_node = tmp;
    this->hook(v, tree);
    return 0;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Search:: hook(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *) {
    return;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Push:: hook1(vector<Node<T1, T2> (*)> &vec, const Node<T1,T2> &_node, BinaryTree<T1, T2> * tree) {
    Node<T1, T2> *node_to_add = new Node<T1, T2>(_node.key, _node.value);
    if (vec.size() == 0)
        tree->root = node_to_add;
    else {
        Node<T1, T2> *father = vec[vec.size() - 1];
        father->chld(tree->cmp->compare(_node.key, father->key)) = node_to_add;
    }
    vec.push_back(node_to_add);
    return;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Push:: hook2(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *tree) {
    return;
}


template<typename T1, typename T2>
void BinaryTree<T1, T2>:: Pop:: hook2(vector<Node<T1, T2> (*)> &vec, const T1 &_key, BinaryTree<T1, T2> * tree) {
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
    int ex_code = pusher->push_node(_node, v, this);
    if (ex_code) {
        throw AlreadyExistException<T1>(_node.key);
    }
}


template<typename T1, typename T2>
int BinaryTree<T1, T2>:: Push:: push_node(const Node<T1, T2> &_node, vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> * tree) {
    Node<T1, T2> *tmp = tree->root;
    if (tmp == nullptr) {
        hook1(v, _node, tree);
        hook2(v, tree);
        return 0;
    }
    while (tmp != nullptr) {
        v.push_back(tmp);
        int compared = tree->cmp->compare(_node.key, tmp->key);
        if (!compared)
            return 1;

        if (!(tmp->chld(compared))) {
            hook1(v, _node, tree);
            hook2(v, tree);
            return 0;
        }
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
    while (this->root != nullptr) {
        this->pop(this->root->key);
    }
}


template <typename T1, typename T2>
BinaryTree<T1,T2>:: ~BinaryTree() {
    // cout<<"just in case\n";
    this->clean();
    delete this->popper;
    delete pusher;
    delete searcher;
}


#endif /* end of include guard: BinR */

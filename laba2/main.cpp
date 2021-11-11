#include <iostream>
#include <vector>
#include <exception>
#include <functional>
#include <time.h>

using namespace std;


class NotExistException : public exception {
private:
    string except;
public:
    NotExistException () {
        except = "Element does not exist";
    }
    const char* what() const noexcept {
        return except.c_str();
    };
};


class AlreadyExistException : public exception {
private:
    string except;
public:
    AlreadyExistException () {
        except = "Element has already existed";
    }
    const char* what() const noexcept {
        return except.c_str();
    };
};


template <typename T>
class Comparator {
public:

    Comparator() {}
    virtual int compare(const T& left, const T& right) const = 0;
    virtual ~Comparator() {}
};

class CmpInt : public Comparator<int> {
public:
    CmpInt() {}
    virtual int compare(const int& left, const int& right) const {
        return (left > right) ? 1 : ((left < right) ? -1 : 0);
    }
    virtual ~CmpInt(){}

};


class CmpStr : public Comparator<string> {
public:
    CmpStr() {}
    virtual int compare(const string& left, const string& right) const {
        return (left > right) ? 1 : ((left < right) ? -1 : 0);
    }
    virtual ~CmpStr(){}
};


template <typename T1, typename T2>
class Node {
public:
    Node(T1 _key, T2 _value);
    T1 key;
    T2 value;
    Node *left_child;
    Node *right_child;
    ~Node() {}
};

template <typename T1, typename T2>
class BinaryTree {
private:
    Node<T1, T2> *root;
    Comparator<T1> *cmp;
    void add_to_tree(const Node<T1, T2> &_node);
    void romove_from_tree(const T1 &_key);
    void replace_in_tree(const Node<T1, T2> &_node);
    T2& find_value_by_key(const T1 &_key);
    void prefix_traverse(Node<T1, T2> *_root, function<void (const T1 &_key, const T2 &val, const int &depth)> func, int _depth);
    void infix_traverse(Node<T1, T2> *_root, function<void (const T1 &_key, const T2 &val, const int &depth)> func, int _depth);
    void postfix_traverse(Node<T1, T2> *_root, function<void (const T1 &_key, const T2 &val, const int &depth)> func, int _depth);
public:
    BinaryTree(Comparator<T1> *_cmp);
    BinaryTree(const BinaryTree<T1, T2> &tree); // copy
    BinaryTree& operator =(const BinaryTree<T1, T2> &tree);
    BinaryTree& operator +=(const Node<T1, T2> &_node); // add value by key
    BinaryTree& operator *=(const Node<T1, T2> &_node); // replace value by key
    BinaryTree& operator -=(const T1 &_key); // del value by key
    T2& operator [](const T1 &_key);
    void prefix(function<void (const T1 &_key, const T2 &val, const int &depth)> func);
    void infix(function<void (const T1 &_key, const T2 &val, const int &depth)> func);
    void postfix(function<void (const T1 &_key, const T2 &val, const int &depth)> func);
    ~BinaryTree();
protected:
    virtual void add_node(Node<T1, T2> *&_root, const Node<T1, T2> &_node);
    virtual void remove_node(Node<T1, T2> *&_root, const T1 &_key);
    virtual void replace_node(Node<T1, T2> *&_root, const Node<T1, T2> &_node);

};

template <typename T1, typename T2>
void callBack(const T1 &_key, const T2 &val, const int &depth) {
    cout << _key << " : " << val << " : " << depth << endl;
}

//void callBack(const string &_key, const int &val, const int &depth);


template <typename T1, typename T2>
Node<T1, T2>:: Node(T1 _key, T2 _value) {
    this->key = _key;
    this->value = _value;
    this->left_child = nullptr;
    this->right_child = nullptr;
}


template <typename T1, typename T2>
BinaryTree<T1,T2>:: BinaryTree(Comparator<T1> *_cmp) {
    this->cmp = _cmp;
    root = nullptr;
}

template <typename T1, typename T2>
BinaryTree<T1,T2>:: BinaryTree(const BinaryTree<T1, T2> &tree) {
    this->cmp = tree.cmp;
    vector<Node<T1, T2>> _tree;
    tree.prefix([&_tree](const T1 &_key, const T2 &_value, const int &smth){_tree.push_back(Node<T1, T2> (_key, _value));});
    for (auto node : _tree) {
        *this += node;
    }
}

template <typename T1, typename T2>
BinaryTree<T1,T2>& BinaryTree<T1,T2>::operator *=(const Node<T1, T2> &_node) {
    replace_in_tree(_node);
}

template <typename T1, typename T2>
void BinaryTree<T1,T2>:: replace_in_tree(const Node<T1, T2> &_node) {
    try {
        replace_node(root, _node);
    }
    catch (NotExistException error) {
        cerr << error.what();
        exit(-1);
    }

}

template <typename T1, typename T2>
void BinaryTree<T1,T2>::replace_node(Node<T1, T2> *&_root, const Node<T1, T2> &_node) {
    NotExistException exception;
    Node<T1, T2> *tmp = _root;
    if (tmp == nullptr) {
        throw exception;
    }
    while (tmp != nullptr) {
        switch (this->cmp->compare(_node.key, tmp->key)) {
        case 1:
            if (tmp->right_child != nullptr) {
                tmp = tmp->right_child;
                continue;
            }
            else {
                throw exception;
            }
        case 0:
            tmp->value = _node.value;
            return;
        case -1:
            if (tmp->left_child != nullptr) {
                tmp = tmp->left_child;
                continue;
            }
            else {
                throw exception;
            }
        }
    }
}


template <typename T1, typename T2>
BinaryTree<T1,T2>& BinaryTree<T1,T2>::operator +=(const Node<T1, T2> &_node) {
    this->add_to_tree(_node);
    return *this;
}


template <typename T1, typename T2>
BinaryTree<T1,T2>& BinaryTree<T1,T2>::operator -=(const T1 &_key) {
    this->romove_from_tree(_key);
    return *this;
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: romove_from_tree(const T1 &_key) {
    try {
        this->remove_node(this->root, _key);
    }
    catch(NotExistException error) {
        cerr << error.what() << endl;
        exit(-1);
    }
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: remove_node(Node<T1, T2> *&_root, const T1 &_key) {
    NotExistException exception;
    Node<T1, T2> *tmp_par = _root;
    Node<T1, T2> *removeable = _root;
//    cout << "need remove: " << _key << endl;
//    cout << "adres in remove: " << tmp_par << endl;
    while (int compared = this->cmp->compare(_key, removeable->key)) {
//        cout << "key now " << removeable->key << endl;
        switch (compared) {
        case 1:
            if (removeable->right_child == nullptr) {
                throw exception;
            }
            tmp_par = removeable;
            removeable = removeable->right_child;
            break;
        case -1:
            if (removeable->left_child == nullptr) {
                throw exception;
            }
            tmp_par = removeable;
            removeable = removeable->left_child;
            break;
        }
    }
//    cout << "found key " << removeable->key << endl;
    while (removeable != nullptr) {
//        cout << removeable->key << endl;
        if (removeable->left_child == nullptr && removeable->right_child == nullptr) {

            if (tmp_par->left_child == removeable)
                tmp_par->left_child = nullptr;

            else
                tmp_par->right_child = nullptr;

            delete removeable;
            return;
        }
        else if ((removeable->left_child != nullptr && removeable->right_child == nullptr) ||
                 (removeable->left_child == nullptr && removeable->right_child != nullptr)) {
            if (removeable == _root){
                if (removeable->left_child != nullptr)
                    _root = removeable->left_child;
                else
                    _root = removeable->right_child;
            }
            else if (tmp_par->right_child == removeable) {
                if (removeable->left_child != nullptr)
                    tmp_par->right_child = removeable->left_child;
                else
                    tmp_par->right_child = removeable->right_child;
            }
            else if (tmp_par->left_child == removeable) {
                if (removeable->left_child != nullptr)
                    tmp_par->left_child = removeable->left_child;
                else
                    tmp_par->left_child = removeable->right_child;
            }
            delete removeable;
            return;
        }
        else {
            Node <T1, T2> *tmp = removeable->right_child;
            bool left_is_here = false;
            while (tmp->left_child != nullptr) {
                tmp_par = tmp;
                tmp = tmp->left_child;
                left_is_here = true;
            }
            removeable->key = tmp->key;
            removeable->value = tmp->value;
            if (!left_is_here) {
                removeable->right_child = tmp->right_child;
                delete tmp;
                return;
            }
            removeable = tmp;
        }
    }
}


template <typename T1, typename T2>
T2& BinaryTree<T1,T2>:: operator [](const T1 &_key) {
    return find_value_by_key(_key);
}


template <typename T1, typename T2>
T2& BinaryTree<T1,T2>:: find_value_by_key(const T1 &_key) {
    NotExistException exception;
    Node<T1, T2> *tmp = root;
    while (int res = cmp->compare(_key, tmp->key)) {
        if (res == -1) {
            tmp = tmp->left_child;
        }
        else {
            tmp = tmp->right_child;
        }
        if (tmp == nullptr) {
            throw exception;
        }
    }
    return tmp->value;
}


template <typename T1, typename T2>
BinaryTree<T1,T2>& BinaryTree<T1,T2>:: operator =(const BinaryTree<T1, T2> &tree) {
    this->cmp = tree.cmp;
    vector<Node<T1, T2>> _tree;
    tree.prefix([&_tree](const T1 &_key, const T2 &_value, const int &smth){_tree.push_back(Node<T1, T2> (_key, _value));});
    for (auto node : _tree) {
        *this += node;
    }
    return *this;
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: add_to_tree(const Node<T1, T2> &_node) {
    try {
        this->add_node(this->root, _node);
    }
    catch(AlreadyExistException error) {
        cerr << error.what() << endl;
        exit(-2);
    }
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: add_node(Node<T1, T2> *&_root, const Node<T1, T2> &_node) {
    AlreadyExistException exception;
    Node<T1, T2> *node = new Node<T1, T2>(_node.key, _node.value);
    Node<T1, T2> *tmp = _root;
//    cout << "to add: " << node->key << endl;
    if (tmp == nullptr) {
        _root = node;
        return;
    }
    while (tmp != nullptr) {
//        cout << "want to compare with: " << tmp->key << endl;
        switch (this->cmp->compare(node->key, tmp->key)) {
        case 1:
            if (tmp->right_child != nullptr) {
                tmp = tmp->right_child;
                continue;
            }
            else {
                tmp->right_child = node;
//                tmp = nullptr;
                return;
            }
        case 0:
            delete node;
            throw exception;
        case -1:
            if (tmp->left_child != nullptr) {
                tmp = tmp->left_child;
                continue;
            }
            else {
                tmp->left_child = node;
//                tmp = nullptr;
                return;
            }
        }
    }
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: prefix(function<void (const T1 &_key, const T2 &val, const int &depth)> func) {
    prefix_traverse(root, func, 0);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: prefix_traverse(Node<T1, T2> *_root, function<void (const T1 &_key, const T2 &val, const int &depth)> func,
                                         int _depth) {
    if (_root == nullptr)
        return;
    func(_root->key, _root->value, _depth);
    prefix_traverse(_root->left_child, func, _depth + 1);
    prefix_traverse(_root->right_child, func, _depth + 1);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: infix(function<void (const T1 &_key, const T2 &val, const int &depth)> func) {
    infix_traverse(root, func, 0);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: infix_traverse(Node<T1, T2> *_root, function<void (const T1 &_key, const T2 &val, const int &depth)> func,
                                         int _depth) {
    if (_root == nullptr)
        return;
    prefix_traverse(_root->left_child, func, _depth + 1);
    func(_root->key, _root->value, _depth);
    prefix_traverse(_root->right_child, func, _depth + 1);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: postfix(function <void (const T1 &_key, const T2 &val, const int &depth)> func) {
    postfix_traverse(root, func, 0);
}


template <typename T1, typename T2>
void BinaryTree<T1,T2>:: postfix_traverse(Node<T1, T2> *_root, function<void (const T1 &_key, const T2 &val, const int &depth)> func,
                                         int _depth) {
    if (_root == nullptr)
        return;
    prefix_traverse(_root->left_child, func, _depth + 1);
    prefix_traverse(_root->right_child, func, _depth + 1);
    func(_root->key, _root->value, _depth);
}


template <typename T1, typename T2>
BinaryTree<T1,T2>:: ~BinaryTree() {
    vector<T1> keys;
    if (root != nullptr) {
        this->postfix([&keys](const T1 &_key, const T2 &_value, const int &smth){keys.push_back(_key);});
        for (auto _key : keys) {
            *this -= _key;
        }
    }
    if (cmp != nullptr)
        delete cmp;
}



int main()
{
    srand(time(0));
    BinaryTree <int, string> tree1(new CmpInt());
    vector<Node<int, string>> nodes;
    vector<int> keys_int;
    cout << "\n\n\nnew made int-string tree:\n\n";
    for (int i = 0; i < 10; i++) {
        int tmp = rand() % 1001;
        while (find(keys_int.begin(), keys_int.end(), tmp) != keys_int.end()) {
            tmp = rand() % 1001;
        }
        keys_int.push_back(tmp);
        nodes.push_back(Node<int, string>(keys_int[i], to_string(rand() % 551)));
    }
    for (auto node : nodes)
        tree1 += node;
    tree1.prefix(callBack<int, string>);
    cout << "\n\nwill remove by key: " << keys_int[2] << endl;
    tree1 -= keys_int[2];
    cout << "\n tree after operation: \n";
    tree1.prefix(callBack<int, string>);
    cout << endl << "\nwill insert by key: " << keys_int[5] << endl;
    tree1 *= Node<int, string>(keys_int[5], "1313");
    cout << "\n tree after operation: \n";
    tree1.prefix(callBack<int, string>);

    BinaryTree <string, int> tree2(new CmpStr());
    vector<Node<string, int>> nodes2;
    vector<string> keys_str;
    cout << "\n\n\nnew made string-int tree:\n\n";
    for (int i = 0; i < 10; i++) {
        string tmp = to_string(rand() % 1001);
        while (find(keys_str.begin(), keys_str.end(), tmp) != keys_str.end()) {
            tmp = to_string(rand() % 1001);
        }
        keys_str.push_back(tmp);
        nodes2.push_back(Node<string, int>(keys_str[i], rand() % 551));
    }
    for (auto node : nodes2)
        tree2 += node;
    tree2.prefix(callBack<string, int>);
    cout << "\n\nwill remove by key: " << keys_str[2] << endl;
    tree2 -= keys_str[2];
    cout << "\n tree after operation: \n";
    tree2.prefix(callBack<string, int>);
    cout << endl << "\nwill insert by key: " << keys_str[5] << endl;
    tree2 *= Node<string, int>(keys_str[5], 1313);
    cout << "\n tree after operation: \n";
    tree2.prefix(callBack<string, int>);
    return 0;
}

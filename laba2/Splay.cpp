#include "Splay.h"

template <typename T1, typename T2>
Splay<T1, T2>::Splay(Comparator<T1> *_cmp) {
    this->cmp = _cmp;
    this->pusher = new SpPush();
    this->popper = new SpPop();
    this->searcher = new SpSearch();
}


// template <typename T1, typename T2>
// Splay<T1, T2>::~Splay() {
//     delete this->searcher;
//     delete this->pusher;
//     delete this->popper;
// }


template <typename T1, typename T2>
void Splay<T1, T2>::splay(vector<Node<T1, T2> (*)> &vec) {
    if (!(vec.size())) return;
    Node<T1, T2> *cur = vec[vec.size() - 1];
    long long iter = vec.size() - 2;
    while (cur != this->root) {
        int direction_below = this->cmp->compare(cur->key, (vec[iter])->key);
        if (vec[iter] == this->root) {
            zig(vec[iter], direction_below);
            // cout << "zig -- " << direction_below << endl;
            this->root = cur;
            return;
        }
        else {
            int direction_upper = this->cmp->compare((vec[iter])->key, (vec[iter - 1])->key);
            if (direction_below == direction_upper) {
                zig_zig(vec[iter - 1], direction_upper);
                // cout << "zig_zig -- " << direction_upper << endl;
            }
            else {
                zig_zag(vec[iter - 1], direction_upper);
                // cout << "zig_zag -- " << direction_upper << endl;
            }
        }
        iter -= 2;
        if ((vec[iter + 1]) == this->root) {
            this->root = cur;
            return;
        }
        (vec[iter])->chld(this->cmp->compare(cur->key, (vec[iter])->key)) = cur;
    }
}

template <typename T1, typename T2>
void Splay<T1, T2>::zig(Node<T1, T2> *rooty, int dir) {
    Node<T1, T2> *cur = rooty->chld(dir);
    rooty->chld(dir) = cur->chld(-dir);
    cur->chld(-dir) = rooty;
}

template <typename T1, typename T2>
void Splay<T1, T2>::zig_zig(Node<T1, T2> *rooty, int dir) {
    Node<T1, T2> *par = rooty->chld(dir);
    Node<T1, T2> *cur = par->chld(dir);

    rooty->chld(dir) = par->chld(-dir);
    par->chld(-dir) = rooty;

    par->chld(dir) = cur->chld(-dir);
    cur->chld(-dir) = par;
}

template <typename T1, typename T2>
void Splay<T1, T2>::zig_zag(Node<T1, T2> *rooty, int dir) {
    Node<T1, T2> *par = rooty->chld(dir);
    Node<T1, T2> *cur = par->chld(-dir);

    // cout <<
    par->chld(-dir) = cur->chld(dir);
    cur->chld(dir) = par;

    rooty->chld(dir) = cur->chld(-dir);
    cur->chld(-dir) = rooty;
}

template <typename T1, typename T2>
void Splay<T1, T2>::SpSearch::hook(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *tree) {
    // cout << "hookSearch:  " << v.size() << endl;
    static_cast<Splay<T1, T2>*>(tree)->splay(v);
}

template <typename T1, typename T2>
void Splay<T1, T2>::SpPush::hook2(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *tree) {
    static_cast<Splay<T1, T2>*>(tree)->splay(v);
}


template <typename T1, typename T2>
Splay<T1, T2>::Splay(Node<T1, T2> *new_root, Comparator<T1> *_cmp) {
    this->cmp = _cmp;
    this->pusher = new SpPush();
    this->popper = new SpPop();
    this->searcher = new SpSearch();
    this->root = new_root;
}


template <typename T1, typename T2>
void Splay<T1, T2>::SpPop::hook1(Node<T1, T2> *&removable, Node<T1, T2> *&tmp_par, int param, vector<Node<T1, T2> (*)> v, BinaryTree<T1, T2> *tree) {
    v.push_back(removable);
    static_cast<Splay<T1, T2> *>(tree)->splay(v);
    Splay<T1, T2> *left = nullptr, *right = nullptr;

    // cout<< "in pop after splay\n";

    static_cast<Splay<T1, T2>*>(tree)->split(left, right);

    // cout<< "in pop after split\n";

    delete static_cast<Splay<T1, T2>*>(tree)->root;

    static_cast<Splay<T1, T2>*>(tree)->root = static_cast<Splay<T1, T2>*>(tree)->merge(left, right);


    left->root = nullptr;
    right->root = nullptr;
    // cout<< "in pop after merge\n";
    delete left;
    // cout<< "in pop after merge2\n";
    delete right;
}

template <typename T1, typename T2>
void Splay<T1, T2>::split(Splay<T1, T2> *&left, Splay<T1, T2> *&right) {
    left = new Splay<T1, T2>(this->root->left, this->cmp);
    right = new Splay<T1, T2>(this->root->right, this->cmp);
}

template <typename T1, typename T2>
Node<T1, T2> *Splay<T1, T2>::merge(Splay<T1, T2> *left, Splay<T1, T2> *right) {
    if (!(left->root)) {
        return right->root;
    }
    else if (!(right->root)) {
        return left->root;
    }
    Node<T1, T2> *tmp = left->root;


    vector<Node<T1, T2> (*)> v;
    while (tmp) {
        v.push_back(tmp);
        tmp = tmp->right;
    }
    left->splay(v);
    left->root->right = right->root;
    // cout<< "in merge 1\n";
    return left->root;
}


template <typename T1, typename T2>
Splay<T1, T2>::~Splay() {
    // this->clean();
    // cout<< "in dest after clean\n";
    // delete this->popper;
    //
    // delete this->pusher;
    // delete this->searcher;
    // cout<< "in dest after del popper\n";
}


int main() {

    CmpInt comp;

    Splay<int, int> tree(&comp);

    Node<int, int> a(1, 2), b(3, 4), c(5, 6), f(7, 8);

    tree.push(a);
    // tree.pop(1);
    tree.prefix(prt);

    // cout << "anomaly thing: " << tree.root->key << endl;

    tree.push(b);
    tree.prefix(prt);

    tree.push(c);
    tree.prefix(prt);
    tree.push(f);
    tree.prefix(prt);
    // Splay<int, int> tree2(tree);

    // tree2.prefix(prt);

    cout<<"search 1\n";
    tree.search(5);
    tree.prefix(prt);

    cout<<"pop 3\n";
    tree.pop(3);
    tree.prefix(prt);



    return 0;
}

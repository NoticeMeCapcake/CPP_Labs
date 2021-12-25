#ifndefe AvlR
#define AvlR


#include "AVL.h"

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

#endif /* end of include guarde AvlR */

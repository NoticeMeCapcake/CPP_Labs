#ifndef Splayh
#define Splayh

#include "BinaryTree.cpp"

template <typename T1, typename T2>
class Splay : public BinaryTree<T1, T2> {
protected:
    class SpPop : public BinaryTree <T1, T2>:: Pop {
    protected:
        virtual void hook1(Node<T1, T2> *&removable, Node<T1, T2> *&tmp_par, int param, vector<Node<T1, T2> (*)> v, BinaryTree<T1, T2> *tree);
        // virtual void hook2(vector<Node<T1, T2> (*)> &vec, const T1 &_key, BinaryTree<T1, T2> * tree);
    };
    class SpPush : public BinaryTree <T1, T2>:: Push {
    protected:
        // virtual void hook(vector<Node<T1, T2> (*)> &vec, const Node<T1,T2> &_node, BinaryTree<T1, T2> * tree);
        virtual void hook2(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *tree);
    };
    class SpSearch : public BinaryTree <T1, T2>:: Search {
    protected:
        virtual void hook(vector<Node<T1, T2> (*)> &v, BinaryTree<T1, T2> *tree);
    };

    Node<T1, T2> *merge(Splay<T1, T2> *left, Splay<T1, T2> *right);
    void split(Splay<T1, T2> *&left, Splay<T1, T2> *&right);
    void splay(vector<Node<T1, T2> (*)> &vec);
    void zig(Node<T1, T2> *rooty, int dir);
    void zig_zig(Node<T1, T2> *rooty, int dir);
    void zig_zag(Node<T1, T2> *rooty, int dir);


    // void clean();
    Splay(Node<T1, T2> *new_root, Comparator<T1> *_cmp);
public:

    Splay(Comparator<T1> *_cmp);
    // Splay(vector<Node<T1, T2> (*)> &v);
    Splay(const BinaryTree<T1, T2> &tree); // copy
    Splay& operator =(const BinaryTree<T1, T2> &tree);
    ~Splay();
};

#endif /* end of include guard: Splay */

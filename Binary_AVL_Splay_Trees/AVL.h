
#include "BinaryTree.cpp"


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

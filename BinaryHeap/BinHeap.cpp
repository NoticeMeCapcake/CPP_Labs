#ifndef BinR
#define BinR

#include "BinHeap.h"

using namespace std;
template <typename T1, typename T2>
Heap<T1, T2> * BinHeapFabric<T1, T2>::create(Comparator<T1> *cmp) {
    return new BinHeap<T1, T2>(cmp);
}

template <typename T1, typename T2>
BinHeap<T1,T2>:: BinHeap(Comparator<T1> *_cmp) {
    this->cmp = _cmp;
}

template <typename T1, typename T2>
void BinHeap<T1,T2>:: shift_down(unsigned long long i) {
    while (2 * i + 1 < this->heap.size()) {
        unsigned long long left = 2 * i + 1;
        unsigned long long right = 2 * i + 2;
        unsigned long long j = left;
        if ((right < this->heap.size()) and (this->cmp->compare((this->heap[right]).key, (this->heap[left]).key) < 0))
            j = right;
        if (this->cmp->compare((this->heap[i]).key, (this->heap[j]).key) < 0)
            break;
        Node<T1, T2> tmp = this->heap[i];
        this->heap[i] = this->heap[j];
        this->heap[j] = tmp;
        i = j;
    }
    return;
}


template <typename T1, typename T2>
void BinHeap<T1,T2>:: shift_up(unsigned long long i) {
    if (i < 1)
        return;

    while (this->cmp->compare((this->heap[i]).key, (this->heap[(i - 1) / 2]).key) < 0) {
        Node<T1, T2> tmp = this->heap[i];
        this->heap[i] = this->heap[(i - 1) / 2];
        this->heap[(i - 1) / 2] = tmp;
        i = (i - 1) / 2;
    }
    return;
}


template <typename T1, typename T2>
T2 &BinHeap<T1,T2>::get() {
    if (this->heap.size())
        return (this->heap[0]).key;
    else
        throw NotExistException<T1>(0);
}


template <typename T1, typename T2>
void BinHeap<T1,T2>::insert(const T1 &key, const T2 &val) {
    for (auto node : this->heap) {
        if (this->cmp->compare(node.key, key) == 0)
            throw AlreadyExistException<T2>(0);
    }
    this->heap.push_back(Node<T1, T2>(key, val));
    //cout << this->heap[0].key << endl;
    this->shift_up(this->heap.size() - 1);
    return;
}


template <typename T1, typename T2>
T2 BinHeap<T1,T2>::pop() {
    if (!(this->heap.size()))
        throw NotExistException<T1>(0);
    T2 val = this->heap[0].value;
    this->heap[0] = this->heap[this->heap.size() - 1];
    this->heap.pop_back();
    this->shift_down(0);
    return val;
}


template <typename T1, typename T2>
void BinHeap<T1,T2>::traverse() {
    for (auto node : this->heap) {
        cout << node.key << " ";
    }
    cout << endl;
}

template <typename T1, typename T2>
Heap<T1, T2> * BinHeap<T1, T2>:: merge(Heap<T1, T2> * tree) {
    for (auto node: static_cast<BinHeap<T1, T2> *>(tree)->heap) {
        this->insert(node.key, node.value);
    }
    return this;
}



int main() {
    BinHeapFabric<int, int> fabric;
    CmpInt comp;
    BinHeap<int, int> *heap = static_cast<BinHeap<int, int> *>(fabric.create(&comp));
    heap->insert(4, 6);
    heap->traverse();

    heap->insert(5, 1);
    heap->insert(8, 1);
    heap->insert(12, 1);
    heap->insert(1, 1);
    heap->traverse();

    heap->pop();
    heap->traverse();
    return 0;
}


#endif /* end of include guard: BinR */

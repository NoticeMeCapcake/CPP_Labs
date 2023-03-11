#ifndef Fabric
#define Fabric

template <typename T1, typename T2>
class HeapFabric {
public:
    virtual Heap *create(Comparator<T1, T2>) = 0;
};


template <typename T1, typename T2>
class BinHeapFabric: HeapFabric<T1, T2> {
public:
    Heap *create(Comparator<T1> *) {
        return new BinHeap<T1, T2>(Comparator<T1> *);
    }
};

#endif /* end of include guard: Fabric */

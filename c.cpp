#include <cassert>
#include <cstring>
#include <iostream>

#define _debug_mode_

const int kMaxHeap = 100'005;

struct member {
    int val = 0;
    bool on = true;
};

member** max_heap = new member*[kMaxHeap];
member** min_heap = new member*[kMaxHeap];
member* arr = new member[kMaxHeap];
int size = 0;

void OutputElement(member* x) {
    std::cout << (x->on ? '[' : '(')
                  << x->val
                  << (x->on ? ']' : ')');
}

void OutputHeap(member** heap) {
    int i = 0;
    int count = 1;
    while (i < size) {
        for (int j = 0; j < count && i + j < size; ++j) {
            OutputElement(heap[i + j]);
            std::cout << (j == count - 1 ? "\n" : " ");
        }
        i += count;
        count *= 2;
    }
}

void Swap(member*& a, member*& b) {
    member* temp = a;
    a = b;
    b = temp;
}

bool MinCmp(member* lhs, member* rhs) {
    if (!lhs->on) {
        return false;
    } else if (!rhs->on) {
        return true;
    }

    return lhs->val < rhs->val;
}

bool MaxCmp(member* lhs, member* rhs) {
    if (!lhs->on) {
        return false;
    } else if (!rhs->on) {
        return true;
    }

    return lhs->val > rhs->val;
}

void SiftDown(member** heap, int i, bool compare(member*,member*)) {
    if (i >= size) {
        return;
    }

    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left >= size) {
        return;
    }

    int j = left;

    if (right < size && compare(heap[right], heap[left])) {
        j = right;
    }

    if (compare(heap[j], heap[i])) {
        Swap(heap[i], heap[j]);
    }

    SiftDown(heap, j, compare);
}

int SiftUp(member** heap, int i, bool compare(member*,member*)) {
    if (i == 0) {
        return 0;
    }

    int parent = (i - 1) / 2;
    if (compare(heap[i], heap[parent])) {
        Swap(heap[parent], heap[i]);
        return SiftUp(heap, parent, compare);
    }

    return i;
}

void HeapInsert(member** heap, member* x, bool compare(member*,member*)) {
    heap[size] = x;
    size++;
    SiftUp(heap, size - 1, compare);
    size--;
}

member* Remove(member** heap, int i, bool compare(member*,member*)) {
    assert(i >= 0 && i < size);
    member* removed = heap[i];
    removed->on = false;
    SiftDown(heap, i, compare);
    return removed;
}

member* GetTop(member** heap, bool compare(member*,member*)) {
    return Remove(heap, 0, compare);
}

void InterfaceInsert(int x) {
    arr[size] = {x, true};
    HeapInsert(min_heap, &arr[size], MinCmp);
    HeapInsert(max_heap, &arr[size], MaxCmp);
    
    size++;
}

int InterfaceGetMin() {
    return GetTop(min_heap, MinCmp)->val;
}

int InterfaceGetMax() {
    return GetTop(max_heap, MaxCmp)->val;
}

int main(int argc, char** argv) {
    #ifndef _debug_mode_
    int q;
    std::cin >> q;

    for (int k = 0; k < q; ++k) {
    #endif
    #ifdef _debug_mode_
    while (true) {
    #endif
        char cmd[7] = {0};
        for (int c = 0; c < 6; ++c) {
            std::cin >> cmd[c];
        }

        #ifdef _debug_mode_
        if (strcmp(cmd, "Finish") == 0) {
            break;
        }
        #endif
        if (strcmp(cmd, "Insert") == 0) {
            std::cin >> cmd[0]; // skip (
            int x;
            std::cin >> x;
            std::cin >> cmd[0]; // skip )
            
            InterfaceInsert(x);
        } else if (strcmp(cmd, "GetMin") == 0) {
            #ifndef _debug_mode_
            std::cout << InterfaceGetMin() << (k == q - 1 ? "" : "\n");
            #endif
            #ifdef _debug_mode_
            std::cout << InterfaceGetMin() << std::endl;
            #endif
        } else if (strcmp(cmd, "GetMax") == 0) {
            #ifndef _debug_mode_
            std::cout << InterfaceGetMax() << (k == q - 1 ? "" : "\n");
            #endif
            #ifdef _debug_mode_
            std::cout << InterfaceGetMax() << std::endl;
            #endif
        }

        #ifdef _debug_mode_
        std::cout << "Min heap:" << std::endl;
        OutputHeap(min_heap);
        std::cout << std::endl << std::endl;
        
        std::cout << "Max heap:" << std::endl;
        OutputHeap(max_heap);
        std::cout << std::endl << std::endl;
        #endif
    }

    delete[] min_heap;
    delete[] max_heap;
    delete[] arr;
    return 0;
}

/*
Insert(6)
Insert(5)
Insert(4)
Insert(3)
Insert(2)
Insert(1)
Insert(0)

Insert(4)
Insert(0)
Insert(5)
Insert(3)
Insert(1)
Insert(6)
Insert(2)
*/
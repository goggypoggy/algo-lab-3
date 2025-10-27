#include <cassert>
#include <cstring>
#include <iostream>

const int kMaxHeap = 100'000;

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
    int sifted_up_x = SiftUp(heap, size - 1, compare);
    SiftDown(heap, sifted_up_x, compare);
    size--;
}

member* Remove(member** heap, int i, bool compare(member*,member*)) {
    assert(i >= 0 && i < size);
    member* removed = heap[i];
    removed->on = false;
    Swap(heap[i], heap[size - 1]);
    //size--;
    int sifted_up_i = SiftUp(heap, i, compare);
    SiftDown(heap, sifted_up_i, compare);
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
    
    int q;
    std::cin >> q;

    for (int i = 0; i < q; ++i) {
    //while (true) {
        char cmd[7] = {0};
        for (int c = 0; c < 6; ++c) {
            std::cin >> cmd[c];
        }

        /*
        if (strcmp(cmd, "Finish") == 0) {
            break;
        }
        */
        if (strcmp(cmd, "Insert") == 0) {
            std::cin >> cmd[0]; // skip (
            int i;
            std::cin >> i;
            std::cin >> cmd[0]; // skip )
            
            InterfaceInsert(i);
        } else if (strcmp(cmd, "GetMin") == 0) {
            std::cout << InterfaceGetMin() << std::endl;
        } else if (strcmp(cmd, "GetMax") == 0) {
            std::cout << InterfaceGetMax() << std::endl;
        }

        /*
        std::cout << "Min heap:" << std::endl;
        OutputHeap(min_heap);
        std::cout << std::endl << std::endl;
        
        std::cout << "Max heap:" << std::endl;
        OutputHeap(max_heap);
        std::cout << std::endl << std::endl;
        */
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
*/
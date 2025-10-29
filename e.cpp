#include <iostream>

struct memblk {
    bool active = false;
    int due_date = -1;
    int id;
};

const int kMaxBlk = 30'000;

memblk* memblk_arr;

// Heap of active memory blocks
memblk** busy_heap;
int busy_heap_size = 0;

// Heap of free memory blocks
memblk** free_heap;
int free_heap_size = kMaxBlk;

int L = 600;

// lhs < rhs date-wise
bool DateCmp(memblk*& lhs, memblk*& rhs) {
    return lhs->due_date < rhs->due_date;
}

// lhs < rhs id-wise
bool IdCmp(memblk*& lhs, memblk*& rhs) {
    return lhs->id < rhs->id;
}

void OutputElement(memblk* x) {
    std::cout << x->id + 1
            << "::"
            << x->due_date;
}

void OutputMas(memblk* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].active) {
            OutputElement(&arr[i]);
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void OutputHeap(memblk** heap, int size) {
    int i = 0;
    int count = 1;
    while (i < size) {
        for (int j = 0; j < count && i + j < size; ++j) {
            OutputElement(heap[i + j]);
            std::cout << (j == count - 1 || i + j == size - 1 ? "\n" : " ");
        }
        i += count;
        count *= 2;
    }
}

int SiftDown(memblk** heap, int size, int i, bool cmp(memblk*&,memblk*&)) {
    while (i < size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left >= size) {
            break;
        }

        int j = left;

        if (right < size && cmp(heap[right], heap[left])) {
            j = right;
        }

        if (cmp(heap[j], heap[i])) {
            std::swap(heap[i], heap[j]);
        }

        i = j;
    }
    return i;
}

int SiftUp(memblk** heap, int size, int i, bool cmp(memblk*&,memblk*&)) {
    while (i > 0 && !cmp(heap[(i - 1) / 2], heap[i])) {
        std::swap(heap[(i - 1) / 2], heap[i]);
        i = (i - 1) / 2;
    }

    return i;
}

void BuildHeap(memblk** heap, int size, bool cmp(memblk*&,memblk*&)) {
    int start = size / 2;
    for (int i = start; i >= 0; i--) {
        SiftDown(heap, size, i, cmp);
    }
}

void Insert(memblk** heap, memblk* new_memblk, int& size, bool cmp(memblk*&,memblk*&)) {
    heap[size] = new_memblk;
    size++;
    SiftUp(heap, size, size - 1, cmp);
}

void RemoveTop(memblk** heap, int& size, bool cmp(memblk*&,memblk*&)) {
    std::swap(heap[0], heap[size - 1]);
    size--;
    SiftDown(heap, size, 0, cmp);
}

void UpdateHeap(int cur_time) {
    while (busy_heap_size > 0 &&
        busy_heap[0]->due_date <= cur_time) {
        busy_heap[0]->active = false;
        busy_heap[0]->due_date = -1;
        Insert(free_heap, busy_heap[0], free_heap_size, IdCmp);
        RemoveTop(busy_heap, busy_heap_size, DateCmp);
    }
}

bool AccessBlock(int cur_time, int id) {
    if (memblk_arr[id - 1].active) {
        memblk_arr[id - 1].due_date = cur_time + L;
        //BuildHeap(busy_heap, busy_heap_size, DateCmp);
        for (int i = 0; i < busy_heap_size; ++i) {
            if (busy_heap[i]->id == id - 1) {
                SiftDown(busy_heap, busy_heap_size, i, DateCmp);
                break;
            }
        }    
        return true;
    }

    return false;
}

int GetBlock(int cur_time) {
    free_heap[0]->due_date = cur_time + L;
    free_heap[0]->active = true;
    
    int added = free_heap[0]->id;
    
    Insert(busy_heap, free_heap[0], busy_heap_size, DateCmp);
    RemoveTop(free_heap, free_heap_size, IdCmp);

    return added + 1;
}

int main(int argc, char** argv) {
    memblk_arr = new memblk[kMaxBlk];
    busy_heap = new memblk*[kMaxBlk];
    free_heap = new memblk*[kMaxBlk];

    for (int i = 0; i < kMaxBlk; ++i) {
        memblk_arr[i].id = i;
        free_heap[i] = &memblk_arr[i];
    }

    BuildHeap(free_heap, free_heap_size, IdCmp);

    int time;
    char cmd;
    while (std::cin >> time >> cmd) {
        UpdateHeap(time);

        /*
        std::cout << "before:\n";
        OutputHeap(busy_heap, busy_heap_size);
        //OutputMas(memblk_arr, kMaxBlk);
        std::cout << "------\n";
        /**/

        if (cmd == '+') {
            std::cout << GetBlock(time) << std::endl;
        } else if (cmd == '.') {
            int x;
            std::cin >> x;
            std::cout << (AccessBlock(time, x) ? '+' : '-') << std::endl;
        }

        /*
        std::cout << "after:\n";
        OutputHeap(busy_heap, busy_heap_size);
        //OutputMas(memblk_arr, kMaxBlk);
        std::cout << "------\n";
        /**/
    }

    delete[] busy_heap;
    delete[] free_heap;
    delete[] memblk_arr;
    return 0;
}
#include <iostream>

struct memblk {
    bool active = false;
    int due_date = -1;
    int id;
};

memblk* memblk_arr;

// Heap of active memory blocks
memblk** memblk_heap;
int memblk_heap_size = 0;

int next_blk = 0;

int L = 600;

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

int SiftDown(memblk** heap, int size, int i) {
    while (i < size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left >= size) {
            break;
        }

        int j = left;

        if (right < size && heap[right]->due_date < heap[left]->due_date) {
            j = right;
        }

        if (heap[j]->due_date < heap[i]->due_date) {
            std::swap(heap[i], heap[j]);
        }

        i = j;
    }
    return i;
}

int SiftUp(memblk** heap, int size, int i) {
    while (i > 0 && heap[(i - 1) / 2]->due_date >= heap[i]->due_date) {
        std::swap(heap[(i - 1) / 2], heap[i]);
        i = (i - 1) / 2;
    }

    return i;
}

void BuildHeap(memblk** heap, int size) {
    int start = size / 2;
    for (int i = start; i >= 0; i--) {
        SiftDown(heap, size, i);
    }
}

void Insert(memblk** heap, memblk* new_memblk, int& size) {
    heap[size] = new_memblk;
    size++;
    SiftUp(heap, size, size - 1);
}

void RemoveTop(memblk** heap, int& size) {
    std::swap(heap[0], heap[size - 1]);
    size--;
    SiftDown(heap, size, 0);
}

void UpdateHeap(int cur_time) {
    while (memblk_heap_size > 0 &&
        memblk_heap[0]->due_date <= cur_time) {
        memblk_heap[0]->active = false;
        memblk_heap[0]->due_date = -1;
        if (memblk_heap[0]->id < next_blk) {
            next_blk = memblk_heap[0]->id;
        }
        RemoveTop(memblk_heap, memblk_heap_size);
    }
}

bool AccessBlock(int cur_time, int id) {
    if (memblk_arr[id - 1].active) {
        memblk_arr[id - 1].due_date = cur_time + L;
        BuildHeap(memblk_heap, memblk_heap_size);    
        return true;
    }

    return false;
}

int GetBlock(int cur_time) {
    memblk_arr[next_blk].due_date = cur_time + L;
    memblk_arr[next_blk].active = true;
    memblk* new_blk = &memblk_arr[next_blk];
    int added = next_blk;
    
    Insert(memblk_heap, new_blk, memblk_heap_size);
    
    while (memblk_arr[next_blk].active) {
        next_blk++;
    }

    return added + 1;
}

int main(int argc, char** argv) {
    memblk_arr = new memblk[30'000];
    memblk_heap = new memblk*[30'000];

    for (int i = 0; i < 30'000; ++i) {
        memblk_arr[i].id = i;
    }

    int time;
    char cmd;
    while (std::cin >> time >> cmd) {
        // escape sequence for debugging
        if (time <= 0) {
            break;
        }

        UpdateHeap(time);

        /*
        std::cout << "before:\n";
        OutputHeap(memblk_heap, memblk_heap_size);
        //OutputMas(memblk_arr, 30'000);
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
        OutputHeap(memblk_heap, memblk_heap_size);
        //OutputMas(memblk_arr, 30'000);
        std::cout << "------\n";
        /**/
    }

    delete[] memblk_heap;
    delete[] memblk_arr;
    return 0;
}
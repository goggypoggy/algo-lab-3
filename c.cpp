#include <cassert>
#include <cstring>
#include <iostream>

const int kMaxHeap = 100'000;

void Output(int* mas, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << mas[i] << (i == size - 1 ? "\n" : " ");
    }
}

void Swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void SiftDown(int* heap, int size,  int element) {
    if (element >= size) {
        return;
    }

    int left = 2 * element + 1;
    int right = 2 * element + 2;

    if (left >= size) {
        return;
    }

    int j = left;

    if (right < size && heap[right] > heap[left]) {
        j = right;
    }

    if (heap[element] < heap[j]) {
        Swap(heap[element], heap[j]);
    }

    SiftDown(heap, size, j);
}

int SiftUp(int* heap, int size, int element) {
    if (element == 0) {
        return 0;
    }

    int parent = (element - 1) / 2;
    if (heap[parent] < heap[element]) {
        Swap(heap[parent], heap[element]);
        return SiftUp(heap, size, parent);
    }

    return element;
}

void Insert(int* heap, int &size, int x) {
    heap[size] = x;
    size++;
    int sifted_up_x = SiftUp(heap, size, size - 1);
    SiftDown(heap, size, sifted_up_x);
}

int Remove(int* heap, int& size, int i) {
    assert(i >= 0 && i < size);
    int removed = heap[i];
    Swap(heap[i], heap[size - 1]);
    size--;
    int sifted_up_i = SiftUp(heap, size, i);
    SiftDown(heap, size, sifted_up_i);
    return removed;
}

int GetMin(int* heap, int& size) {
    if (size == 1) {
        return Remove(heap, size, 0);
    }
    int min = (size - 2) / 2 + 1;

    for (int i = min + 1; i < size; ++i) {
        if (heap[i] < heap[min]) {
            min = i;
        }
    }

    return Remove(heap, size, min);
}

int GetMax(int* heap, int& size) {
    return Remove(heap, size, 0);
}

int main(int argc, char** argv) {
    int q;
    int* heap = new int[kMaxHeap];
    int size = 0;
    
    std::cin >> q;

    for (int i = 0; i < q; ++i) {
        char cmd[7] = {0};
        for (int c = 0; c < 6; ++c) {
            std::cin >> cmd[c];
        }

        if (strcmp(cmd, "Insert") == 0) {
            std::cin >> cmd[0]; // skip (
            int i;
            std::cin >> i;
            std::cin >> cmd[0]; // skip )

            Insert(heap, size, i);
        } else if (strcmp(cmd, "GetMin") == 0) {
            std::cout << GetMin(heap, size) << std::endl;
        } else if (strcmp(cmd, "GetMax") == 0) {
            std::cout << GetMax(heap, size) << std::endl;
        }

        //Output(heap, size);
    }

    delete[] heap;
    return 0;
}
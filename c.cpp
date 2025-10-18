#include <iostream>
#include <cstring>

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

void BuildHeap(int* heap, int size) {
    for (int i = (size / 2) - 1; i >= 0; --i) {
        SiftDown(heap, size, i);
    }
}

int main(int argc, char** argv) {
    int q;
    int* heap = new int[100'000];

    std::cin >> q;

    for (int i = 0; i < q; ++i) {
        char cmd[7] = {0};
    }

    

    delete[] heap;
    return 0;
}
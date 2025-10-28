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
    while (element > 0 && heap[element] > heap[(element - 1) / 2]) {
        Swap(heap[element], heap[(element - 1) / 2]);
        element = (element - 1) / 2;
    }

    return element;
}

void Insert(int* heap, int &size, int x) {
    heap[size] = x;
    size++;
    int sifted_up_x = SiftUp(heap, size, size - 1);
    SiftDown(heap, size, sifted_up_x);
}

int GetMin(int* heap, int& size) {
    int min = (size - 2) / 2 + 1;
    if (size == 1) {
        min = 0;
    }

    for (int i = min + 1; i < size; ++i) {
        if (heap[i] < heap[min]) {
            min = i;
        }
    }

    int removed = heap[min];
    Swap(heap[min], heap[size - 1]);
    size--;
    SiftUp(heap, size, min);
    return removed;
}

int GetMax(int* heap, int& size) {
    int removed = heap[0];
    Swap(heap[0], heap[size - 1]);
    size--;
    SiftDown(heap, size, 0);
    return removed;
}

int main(int argc, char** argv) {
    int q;
    int* heap = new int[kMaxHeap];
    int size = 0;
    
    std::cin >> q;

    for (int k = 0; k < q; ++k) {
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
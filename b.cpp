#include <iostream>

void Output(int* mas, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << mas[i] << (i == size - 1 ? "" : " ");
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

void BuildHeap(int* heap, int size) {
    for (int i = (size / 2) - 1; i >= 0; --i) {
        SiftDown(heap, size, i);
    }
}

void SortHeap(int* heap, int size) {
    while (size > 0) {
        Swap(heap[0], heap[size - 1]);
        size--;
        SiftDown(heap, size, 0);
    }
}

void HeapSort(int* mas, int size) {
    BuildHeap(mas, size);
    SortHeap(mas, size);
}

int main(int argc, char** argv) {
    int n;
    int* heap;

    std::cin >> n;
    heap = new int[n];

    for (int i = 0; i < n; ++i) {
        std::cin >> heap[i];
    }

    HeapSort(heap, n);
    Output(heap, n);

    delete[] heap;
    return 0;
}
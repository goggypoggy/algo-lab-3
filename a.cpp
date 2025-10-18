#include <iostream>

bool CheckHeap(int* heap, int size, int parent) {
    if (parent >= size) {
        return true;
    }
    
    int left = 2 * parent + 1;
    if (left >= size) {
        return true;
    }

    int right = 2 * parent + 2;
    bool parent_is_heap;
    if (right >= size) {
        parent_is_heap = heap[parent] <= heap[left];
    } else {
        parent_is_heap = (heap[parent] <= heap[left]) && (heap[parent] <= heap[right]);
    }

    return parent_is_heap && CheckHeap(heap, size, left) && CheckHeap(heap, size, right);
}

int main(int argc, char** argv) {
    int n;
    int heap[100'000];

    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> heap[i];
    }

    std::cout << (CheckHeap(heap, n, 0) ? "YES" : "NO");
    return 0;
}
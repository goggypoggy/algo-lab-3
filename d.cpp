#include <iostream>

struct core {
    int cost;
    int finish_time = -1;
};

// lhs < rhs cost-wise
bool CostCmp(core*& lhs, core*& rhs) {
    return lhs->cost < rhs->cost;
}

// lhs < rhs finish_time-wise
bool FinishCmp(core*& lhs, core*& rhs) {
    if (lhs->finish_time == -1) {
        return false;
    }
    if (rhs->finish_time == -1) {
        return true;
    }
    return lhs->cost < rhs->cost;
}

int SiftDown(core** heap, int size, int i, bool cmp(core*&,core*&)) {
    while (i < size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left >= size) {
            break;
        }

        int j = left;

        if (right < size && cmp(heap[left], heap[right])) {
            j = right;
        }

        if (cmp(heap[i], heap[j])) {
            std::swap(heap[i], heap[j]);
        }

        i = j;
    }
    return i;
}

int SiftUp(core** heap, int size, int i, bool cmp(core*&,core*&)) {
    while (i > 0 && cmp(heap[(i - 1) / 2], heap[i])) {
        std::swap(heap[(i - 1) / 2], heap[i]);
        i = (i - 1) / 2;
    }

    return i;
}

void BuildHeap(core** heap, int size, bool cmp(core*&,core*&)) {
    int start = size / 2;
    for (int i = start; i >= 0; i--) {
        SiftDown(heap, size, i, cmp);
    }
}

int main(int argc, char** argv) {
    int n, m;
    std::cin >> n >> m;
    core* core_arr = new core[n];
    core** cost_heap = new core*[n];
    core** finish_heap = new core*[n];

    for (int i = 0; i < n; ++i) {
        std::cin >> core_arr[i].cost;
        cost_heap[i] = &core_arr[i];
        finish_heap[i] = &core_arr[i];
    }

    BuildHeap(cost_heap, n, CostCmp);

    

    return 0;
}
#include <iostream>

struct task {
    long start;
    long length;
};

struct core {
    long cost;
    int finish_time = -1;
    task cur_task = {0, 0};
};

core* core_arr;
    
core** cost_heap;
int cost_heap_size;

core** finish_heap;
int finish_heap_size;

long total_cost = 0;

void OutputElement(core* x) {
    std::cout << x->cost
            << "::"
            << x->finish_time
            << "::("<<x->cur_task.start<<";"<<x->cur_task.length<<")";
}

void OutputHeap(core** heap, int size) {
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

// lhs > rhs cost-wise
bool CostCmp(core*& lhs, core*& rhs) {
    return lhs->cost > rhs->cost;
}

// lhs > rhs finish_time-wise
bool FinishCmp(core*& lhs, core*& rhs) {
    // -1 <=> infinity
    // if a core is already available,
    // it's finish_time is infinity
    if (lhs->finish_time == -1) {
        return true;
    }
    if (rhs->finish_time == -1) {
        return false;
    }
    return lhs->finish_time > rhs->finish_time;
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

void Insert(core** heap, core* new_core, int& size, bool cmp(core*&,core*&)) {
    heap[size] = new_core;
    size++;
    SiftUp(heap, size, size - 1, cmp);
}

void AssignTask(task& new_task) {
    if (cost_heap[0]->finish_time > new_task.start) {
        //std::cout << "No cores available!\n";
        return;
    }

    // The top element of the cost heap
    // is the core with the smallest cost
    // Assign that core a new finish time
    // Remove top of cost heap
    //std::cout << "Core " << cost_heap[0]->cost << " in use\n";
    cost_heap[0]->finish_time = new_task.start + new_task.length;
    cost_heap[0]->cur_task = new_task;
    std::swap(cost_heap[0], cost_heap[cost_heap_size - 1]);
    cost_heap_size--;
    SiftDown(cost_heap, cost_heap_size, 0, CostCmp);

    // Since we updated a core's finish time
    // we have to assure the finish heap stays a heap
    BuildHeap(finish_heap, finish_heap_size, FinishCmp);
}

void RefreshCores(int cur_time) {
    // refresh until all cores are free
    // or can't refresh anymore
    while (true) {
        if (finish_heap[0]->finish_time != -1 && 
            cur_time >= finish_heap[0]->finish_time) {
            Insert(cost_heap, finish_heap[0], cost_heap_size, CostCmp);
            finish_heap[0]->finish_time = -1;
            total_cost += finish_heap[0]->cost * finish_heap[0]->cur_task.length;
            finish_heap[0]->cur_task = {0, 0};
            SiftDown(finish_heap, finish_heap_size, 0, FinishCmp);
        } else {
            break;
        }
    }

    //std::cout << "Total cost: " << total_cost << std::endl;
}

int main(int argc, char** argv) {
    int n, m;
    std::cin >> n >> m;
    
    core_arr = new core[n];
    
    cost_heap = new core*[n];
    cost_heap_size = n;
    
    finish_heap = new core*[n];
    finish_heap_size = n;

    for (int i = 0; i < n; ++i) {
        std::cin >> core_arr[i].cost;
        cost_heap[i] = &core_arr[i];
        finish_heap[i] = &core_arr[i];
    }

    BuildHeap(cost_heap, cost_heap_size, CostCmp);

    /*
    std::cout << "cost heap:\n";
    OutputHeap(cost_heap, cost_heap_size);
    std::cout << "finish heap:\n";
    OutputHeap(finish_heap, finish_heap_size);
    std::cout << "------\n\n";
    */

    for (int i = 0; i < m; ++i) {
        task new_task;
        std::cin >> new_task.start >> new_task.length;
        RefreshCores(new_task.start);

        /*
        std::cout << "refreshed, time = " << new_task.start << ":\n";
        std::cout << "cost heap:\n";
        OutputHeap(cost_heap, cost_heap_size);
        std::cout << "finish heap:\n";
        OutputHeap(finish_heap, finish_heap_size);
        std::cout << "------\n\n";
        */

        AssignTask(new_task);

        /*
        std::cout << "cost heap:\n";
        OutputHeap(cost_heap, cost_heap_size);
        std::cout << "finish heap:\n";
        OutputHeap(finish_heap, finish_heap_size);
        std::cout << "------\n\n";
        */
    }

    for (int i = 0; i < n; i++) {
        total_cost += core_arr[i].cost * core_arr[i].cur_task.length;
    }
    std::cout << total_cost;
    
    delete[] cost_heap;
    delete[] finish_heap;
    delete[] core_arr;

    return 0;
}
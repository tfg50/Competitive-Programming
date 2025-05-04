#ifndef PARTITION_SOLVER_H
#define PARTITION_SOLVER_H
#include "Biblioteca/template_basics.hpp"

template<const int N>
class PartitionSolver {
public:
    PartitionSolver() {
        std::vector<int> a;
        partitions.push_back(a);
        gen(1, N, a);
        std::sort(partitions.begin(), partitions.end());
        to.assign(partitions.size(), std::vector<int>(N+1, -1));
        for(int i = 0; i < (int) partitions.size(); i++) {
            int sum = 0;
            auto arr = partitions[i];
            for(auto x : arr) {
                sum += x;
            }
            for(int j = 1; j + sum <= N; j++) {
                arr = partitions[i];
                arr.push_back(j);
                std::sort(arr.begin(), arr.end());
                to[i][j] = getIndex(arr);
            }
        }
    }

    int size() const { return (int) partitions.size(); }
    int getIndex(const std::vector<int> &arr) const { return std::lower_bound(partitions.begin(), partitions.end(), arr) - partitions.begin(); }
    int add(int id, int num) const { return to[id][num]; }
    std::vector<int> getPermutation(int id) const { return partitions[id]; }
private:
    std::vector<std::vector<int>> partitions, to;
    void gen(int i, int sum, std::vector<int> &a) {
        if(i > sum) { return; }
        a.push_back(i);
        partitions.push_back(a);
        gen(i, sum - i, a);
        a.pop_back();
        gen(i+1, sum, a);
    }
};

#endif

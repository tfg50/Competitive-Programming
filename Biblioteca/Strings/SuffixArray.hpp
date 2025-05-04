#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H
#include "Biblioteca/template_basics.hpp"

class SuffixArray {
public:
    template<class T>
    std::vector<int> buildSuffix(const T &array) {
        int n = (int) array.size();
        std::vector<int> sa(n);
        for(int i = 0; i < n; i++) {
            sa[i] = i;
        }
        std::sort(sa.begin(), sa.end(), [&](int a, int b) { return array[a] < array[b]; });
        int cur = 0;
        std::vector<int> inv(n);
        std::vector<int> nxt(n);
        inv[sa[0]] = 0;
        for(int i = 1; i < n; i++) {
            inv[sa[i]] = (array[sa[i - 1]] != array[sa[i]] ? ++cur : cur);
        }
        cur++;
        for(int k = 0; cur < n && (1 << k) < n; k++) {
            cur = 0;
            auxSort(sa, inv, 1 << k);
            for(int l = 0, r = 0; l < n; l = r, cur++) {
                while(r < n && getPair(inv, sa[l], 1 << k) == getPair(inv, sa[r], 1 << k)) {
                    nxt[sa[r++]] = cur;
                }
            }
            nxt.swap(inv);
        }
        return sa;
    }

    template<class T>
    std::vector<int> buildLCP(const std::vector<int> &sa, const T &array) {
        int n = sa.size();
        std::vector<int> inv(n);
        for(int i = 0; i < n; i++) {
            inv[sa[i]] = i;
        }
        std::vector<int> lcp(n, 0);
        for(int i = 0, k = 0; i < n; i++) {
            if(inv[i] + 1 == n) {
                k = 0;
                continue;
            }
            int j = sa[inv[i] + 1];
            while(i + k < n && j + k < n && array[i + k] == array[j + k]) {
                k++;
            }
            lcp[inv[i]] = k;
            if(k > 0) {
                k--;
            }
        }
        return lcp;
    }
private:
    void auxSort(std::vector<int> &sa, const std::vector<int> &inv, int offset) {
        // O(nlogn) step, O(nlog^2n) total
        // std::sort(sa.begin(), sa.end(), [&](int a, int b) { return getPair(inv, a, offset) < getPair(inv, b, offset); });
        // O(n) step, O(nlogn) total
        int n = (int) sa.size();
        std::vector<int> cnt(n+2, 0);
        std::vector<int> tmp(n, -1);
        for(auto pos : sa) {
            // int id = pos + offset < (int) inv.size() ? inv[pos + offset] + 1 : 0;
            int id = inv[(pos+offset)%n];
            cnt[id]++;
        }
        for(int i = 1; i <= n+1; i++) {
            cnt[i] += cnt[i-1];
        }
        // TODO: LOOK INTO REUSING THE INFORMATION OF +offset IN ORDER TO CUT THIS CODE IN HALF
        for(auto pos : sa) {
            // int id = pos + offset < (int) inv.size() ? inv[pos + offset] + 1 : 0;
            int id = inv[(pos+offset)%n];
            tmp[--cnt[id]] = pos;
        }
        cnt.assign(n+1, 0);
        for(auto pos : tmp) {
            int id = inv[pos];
            cnt[id]++;
        }
        for(int i = 1; i <= n+1; i++) {
            cnt[i] += cnt[i-1];
        }
        std::reverse(tmp.begin(), tmp.end());
        for(auto pos : tmp) {
            int id = inv[pos];
            sa[--cnt[id]] = pos;
        }
    }
    std::pair<int, int> getPair(const std::vector<int> &inv, int pos, int offset) {
        //return std::pair<int, int>(inv[pos], pos + offset < (int) inv.size() ? inv[pos + offset] : -1);
        return std::pair<int, int>(inv[pos], inv[(pos + offset) % (int) inv.size()]);
    }
};

#endif
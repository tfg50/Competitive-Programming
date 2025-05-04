#ifndef SUFFIX_AUTOMATON_H
#define SUFFIX_AUTOMATON_H
#include "Biblioteca/template_basics.hpp"

template<class T, class M = std::map<T, int>>
struct SuffixAutomaton {
    struct Node {
        int link, len;
        M to;
    };

    SuffixAutomaton() {
        clear();
    }

    void clear() {
        nodes.clear();
        nodes.push_back({-1, 0});
    }

    int direct(int last, T ch) { return nodes[last].to.count(ch) ? nodes[last].to[ch] : -1; }
    int next(int last, T ch) {
        while(last && !nodes[last].to.count(ch)) {
            last = nodes[last].link;
        }
        return nodes[last].to.count(ch) ? nodes[last].to[ch] : 0;
    }

    int extend(int last, T ch) {
        int cur = (int) nodes.size();
        if(!nodes[last].to.count(ch)) {
            nodes.push_back({0, nodes[last].len + 1});
            while(last != -1 && !nodes[last].to.count(ch)) {
                nodes[last].to[ch] = cur;
                last = nodes[last].link;
            }
        } else {
            cur = nodes[last].to[ch];
            if(nodes[cur].len == nodes[last].len + 1) {
                return cur;
            }
            cur = -1;
        }
        if(last != -1) {
            int q = nodes[last].to[ch];
            if(nodes[q].len == nodes[last].len + 1) {
                nodes[cur].link = q;
            } else {
                int clone = (int) nodes.size();
                nodes.push_back(nodes[q]);
                nodes[clone].len = nodes[last].len + 1;
                while(last != -1 && nodes[last].to[ch] == q) {
                    nodes[last].to[ch] = clone;
                    last = nodes[last].link;
                }
                nodes[q].link = clone;
                if(cur == -1) {
                    return clone;
                }
                nodes[cur].link = clone;
            }
        }
        return cur;
    }

    std::vector<Node> nodes; // doing .reserve here should make it faster
};

int reindex(char);

template<class Key, class Val, const int ms>
struct ArrayMap {
    Val data[ms];

    ArrayMap() { memset(data, 0, sizeof data); }

    int count(Key k) {
        return data[reindex(k)] != 0 ? 1 : 0;
    }

    Val& operator [](Key k) {
        return data[reindex(k)];
    }
};

int reindex(char ch) { return ch - '0'; }

#endif

#ifndef AHO_H
#define AHO_H
#include "Biblioteca/template_basics.hpp"

template<const int ALPHA = 26, class T = std::string, const int off = 'a'>
struct Aho {
    struct Node {
        int to[ALPHA];
        int size;
        int fail, pfail;
        bool present;

        Node() {
            for(int i = 0; i < ALPHA; i++) {
                to[i] = 0;
            }
            size = 0;
            pfail = fail = 0;
            present = false;
            // maybe initialize some other stuff here
        }

        // maybe add some other stuff here
    };

    Aho() {
        nodes.push_back(Node());
    }

    template<class F>
    void goUp(int on, F f) {
        for(on = nodes[on].present ? on : nodes[on].pfail; on > 0; on = nodes[on].pfail) {
            f(nodes[on]);
        }
    }

    template<class CH>
    int nextState(int on, CH ch) const {
        return nodes[on].to[ch - off];
    }

    int addString(const T &str) {
        int on = 0;
        for(auto ch : str) {
            if(nodes[on].to[ch-off] == 0) {
                nodes[on].to[ch-off] = (int) nodes.size();
                nodes.push_back(Node());
                nodes.back().size = 1 + nodes[on].size;
            }
            on = nodes[on].to[ch-off];
        }
        // makes this node present
        nodes[on].present = true;
        return on;
    }

    void build() {
        std::queue<int> que;
        que.push(0);
        while(!que.empty()) {
            int on = que.front();
            que.pop();
            nodes[on].pfail = nodes[nodes[on].fail].present ? nodes[on].fail : nodes[nodes[on].fail].pfail;
            // do stuff that carries over with fail here
            for(int i = 0; i < ALPHA; i++) {
                int &to = nodes[on].to[i];
                if(to) {
                    nodes[to].fail = on == 0 ? 0 : nodes[nodes[on].fail].to[i];
                    que.push(to);
                } else {
                    to = nodes[nodes[on].fail].to[i];
                }
            }
        }
    }

    std::vector<Node> nodes;
};

#endif
#ifndef PERSISTENT_SEGTREE_H
#define PERSISTENT_SEGTREE_H

struct Node{
    int v = 0;
    Node *l = this, *r = this;
};

int CNT = 1;
Node buffer[ms * 20];

Node* update(Node *root, int l, int r, int idx, int val){
    Node *node = &buffer[CNT++];
    *node = *root;
    int mid = (l + r) / 2;
    node->v += val;
    if(l+1 != r){
        if(idx < mid) node->l = update(root->l, l, mid, idx, val);
        else node->r = update(root->r, mid, r, idx, val);
    }
    return node;
}

int query(Node *node, int tl, int tr, int l, int r){
    if(l <= tl && tr <= r) return node->v;
    if(tr <= l || tl >= r) return 0;
    int mid = (tl+tr) / 2;
    return query(node->l, tl, mid, l, r) + query(node->r, mid, tr, l, r);
}

#endif

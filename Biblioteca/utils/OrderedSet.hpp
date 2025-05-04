#ifndef ORDERED_SET_H
#define ORDERED_SET_H
#include "Biblioteca/template_basics.hpp"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

template<class T>
using OrderedSet = tree<T,null_type,std::less<T>,rb_tree_tag,tree_order_statistics_node_update>;
#endif

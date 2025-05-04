#ifndef CONTAINER_H
#define CONTAINER_H
template <typename Value>
using Container = std::conditional_t<(std::is_trivial<Value>::value && std::is_standard_layout<Value>::value && !std::is_array<Value>::value), std::basic_string<Value>, std::vector<Value>>;
#endif

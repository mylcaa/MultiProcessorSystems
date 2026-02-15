#include "node.hpp"

Node::Node(int id, Console& console): 
    cpu(), 
    cache(console), 
    cc(id, cache, console),
    id(id) {}
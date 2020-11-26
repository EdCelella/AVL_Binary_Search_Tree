# AVL Binary Search Tree

This project contains my implementation of an AVL binary search tree in C++ 11. The data structure is implemented in the [avl.h](headers/avl.h) file as a class, which is templated to allow for any key-value data type combination.

## Prerequisites

A C++ compiler which is capable of compiling the C++ 11 standard (e.g. [GNU](https://gcc.gnu.org)).

## How To Use

The AVL binary tree is implemented entirely within the [avl.h](headers/avl.h) file as a class. Thus, to use this data structure within any project the header file simply needs to be included.

```c++
#include "path_to_file/avl.h"
```

In order to declare an AVL tree object simply use the following constructor (where `key_type` and `item_type` can be any data types):

```c++
AVL<key_type, item_type> obj;
```

Move and copy operators have also been implemented, and can be used to instantiate/create objects.

Once an object has been created, the following implemented class functions can be used:

- `insert(key, item)` : Inserts a new node into the tree, and if required rebalances the tree.
- `lookup(key)` : Searches for a node containing the `key` value. If found, a pointer to the nodes `item` is returned. If no node in the tree contains the `key` being searched then `nullptr` is returned.
- `remove(key)` : Searches for a node containing the `key` value. If found, the node is removed and the tree is rebalanced.
- `displayEntries()` : Outputs all nodes in key order to console _(uses std::cout)_. The `<<` operator is also overloaded, and provides the same functionality.
- `displayTree()` : Outputs a visual representation of the tree to console _(uses std::cout)_.

### Testing

Included within the src directory is a Makefile and the file [main.cpp](src/main.cpp). This file reads the [names.txt](names.txt) file and inserts each key-item pair into an AVL object. To build and run this file (on UNIX) simply execute the following commands:

```
make all
./avlbinarytree
```

## License

This project is licensed under the terms of the [Creative Commons Attribution 4.0 International Public license](License.md).




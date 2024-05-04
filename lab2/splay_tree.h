#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct SplayTreeNode {
    struct SplayTreeNode* _parent;
    struct SplayTreeNode* _left;
    struct SplayTreeNode* _right;
    int _val;
} SplayTreeNode;

typedef struct SplayTree {
    SplayTreeNode* root;
} SplayTree;

void setParent(SplayTreeNode* child, SplayTreeNode* parent) {
    if (child != NULL) {
        child->_parent = parent;
    }
}

// when double rotation is used
void _ReassignParent(SplayTreeNode* child) {
    SplayTreeNode* parent = child->_parent->_parent;
    SplayTreeNode* parentparent = parent->_parent;
    if (parentparent != NULL) {
        if (parentparent->_left == parent) {
            child->_parent = parentparent;
            parentparent->_left = child;
        } else {
            child->_parent = parentparent;
            parentparent->_right = child;
        }
    }
}

void _LeftRotation(SplayTreeNode* parent) {
    SplayTreeNode* child = parent->_right;
    if (parent->_parent != NULL) {
        if (parent == parent->_parent->_left) {
            parent->_parent->_left = child;
        } else {
            parent->_parent->_right = child;
        }
    }
    child->_parent = parent->_parent;
    // work with parent
    parent->_parent = child;
    parent->_right = child->_left;
    setParent(child->_left, parent);
    // work with child
    child->_left = parent;
}

void _RightRotation(SplayTreeNode* parent) {
    SplayTreeNode* child = parent->_left;
    if (parent->_parent != NULL) {
        if (parent == parent->_parent->_left) {
            parent->_parent->_left = child;
        } else {
            parent->_parent->_right = child;
        }
    }
    child->_parent = parent->_parent;
    // work with parent
    parent->_parent = child;
    parent->_left = child->_right;
    setParent(child->_right, parent);
    // work with child
    child->_right = parent;
}


void __SingleRotation(SplayTreeNode* node) {
    if (node->_parent->_left == node) {
        _RightRotation(node->_parent);
    } else {
        _LeftRotation(node->_parent);
    }
}

void __DoubleRotation(SplayTreeNode* node) {
    SplayTreeNode* parent = node->_parent;
    SplayTreeNode* parentparent = parent->_parent;
    _ReassignParent(node);
    if (parentparent->_left == parent) {
        if (parent->_left == node) {
            _RightRotation(parentparent);
            _RightRotation(parent);
        } else {
            _LeftRotation(parent);
            parentparent->_left = node;
            _RightRotation(parentparent);
        }
    } else {
        if (parent->_left == node) {
            _RightRotation(parent);
            parentparent->_right = node;
            _LeftRotation(parentparent);
        } else {
            _LeftRotation(parentparent);
            _LeftRotation(parent);
        }
    }
}

void _Splay(SplayTreeNode* node) {
    while (node->_parent != NULL) {
        __SingleRotation(node);
    }
}

// Finds parent of node with value `val`.
// If root is NULL or root->value == val behaviour is undefined&
SplayTreeNode* _Find(SplayTreeNode* root, int val) {
    SplayTreeNode* child = root->_val > val ? root->_left : root->_right;
    while (child != NULL && child->_val != val) {
        root = child;
        child = root->_val > val ? root->_left : root->_right;
    }
    return root;
}

bool Contains(SplayTree* tree, int val) {
    if (tree->root == NULL) {
        return false;
    }
    if (tree->root->_val == val) {
        return true;
    }
    SplayTreeNode* f = _Find(tree->root, val);
    if (f->_left && f->_left->_val == val) {
        tree->root = f->_left;
        _Splay(f->_left);
        return true;
    } else if (f->_right && f->_right->_val == val) {
        tree->root = f->_right;
        _Splay(f->_right);
        return true;
    }

    return false;
}

SplayTreeNode* _AllocateNode(int val) {
    SplayTreeNode* node = (SplayTreeNode*) malloc(sizeof(SplayTreeNode));
    node->_left = node->_right = node->_parent = NULL;
    node->_val = val;

    return node;
}

void Insert(SplayTree* tree, int val) {
    if (tree->root == NULL) {
        tree->root = _AllocateNode(val);
        return;
    }

    if (tree->root->_val == val) {
        return;
    }

    SplayTreeNode* parent = _Find(tree->root, val);

    bool side = parent->_val > val;

    if (side && parent->_left != NULL || !side && parent->_right != NULL) {
        return;
    }

    SplayTreeNode* node = _AllocateNode(val);

    if (side) {
        parent->_left = node;
    } else {
        parent->_right = node;
    }
    node->_parent = parent;

    _Splay(node);
    tree->root = node;
}

SplayTreeNode* __FindMostLeft(SplayTreeNode* node) {
    while (node->_left != NULL) {
        node = node->_left;
    }

    return node;
}

void __Erase(SplayTreeNode* node) {
    if (node == NULL) {
        return;
    }
    bool left = node->_left != NULL;
    bool right = node->_right != NULL;
    if (left && right) {
        SplayTreeNode* swap = __FindMostLeft(node->_right);
        node->_val = swap->_val;
        if (swap->_parent->_left == swap) {
            swap->_parent->_left = NULL;
        } else {
            swap->_parent->_right = NULL;
        }
        free(swap);
    } else if (!left && !right) {
        if (node->_parent->_left == node) {
            node->_parent->_left = NULL;
        } else {
            node->_parent->_right = NULL;
        }

        free(node);
    } else {
        SplayTreeNode* swap = node->_left != NULL ? node->_left : node->_right;
        swap->_parent = node->_parent;
        if (node->_parent->_left == node) {
            node->_parent->_left = swap;
        } else {
            node->_parent->_right = swap;
        }

        free(node);
    }
}

void __EraseRoot(SplayTree* tree) {
    bool left = tree->root->_left != NULL;
    bool right = tree->root->_right != NULL;
    if (left && right) {
        __Erase(tree->root);
    } else if (!left && !right) {
        free(tree->root);
        tree->root = NULL;
    } else {
        SplayTreeNode* swap = tree->root->_left != NULL ? tree->root->_left : tree->root->_right;
        swap->_parent = NULL;
        free(tree->root);
        tree->root = swap;
    }
}

void Erase(SplayTree* tree, int val) {
    if (tree->root == NULL) {
        return;
    }
    if (tree->root->_val == val) {
        __EraseRoot(tree);
    } else {
        SplayTreeNode* parent = _Find(tree->root, val);
        __Erase(parent->_val > val ? parent->_left : parent->_right);
    }
}

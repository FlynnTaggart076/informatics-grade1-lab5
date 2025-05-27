#ifndef SKIPLIST_H
#define SKIPLIST_H

#include<vector>
#include<cmath>
#include<string>
#include<iostream>
#include <sstream>

using namespace std;

template<typename T>
class SkipListNode {
public:
    SkipListNode<T>* forward;
    SkipListNode<T>* down;

    SkipListNode(T value, int layer) : data(value), layer(layer), forward(nullptr), down(nullptr) {}

    T get_value() {
        return data;
    }

    int get_layer() {
        return layer;
    }

    SkipListNode<T>* get_down() {
        return down;
    }

    ~SkipListNode() {}
private:
    T data;
    int layer;
};



template<typename T>
class SkipList {
public:
    SkipList(T* data, int n);

    void Insert (T value, bool resize = true);

    bool Contains(const T& value);
    SkipListNode<T>* Search(const T& value);

    bool Erase(const T& value);
    void Clear();

    void PrintFull();
    void Print();

    string ToString() const;
    string ToFullString() const;

    int Size() const {
        return size;
    }

    bool Empty() const {
        return size == 0;
    }

    ~SkipList() {
        Clear();
    }

private:
    void UpdateRoot();

    SkipListNode<T>* root;
    int size;
    int max_layer = 1;
};


template <typename T>
SkipList<T>::SkipList(T* data, int n) {
    size = n;
    max_layer = (n > 1 ? static_cast<int>(floor(log2(n))) + 1 : 1);

    root = new SkipListNode<T>(data[0], max_layer - 1);

    SkipListNode<T>* cur = root;
    for (int level = max_layer - 2; level >= 0; --level) {
        auto node = new SkipListNode<T>(data[0], level);
        cur->down = node;
        cur = node;
    }

    for (int i = 1; i < n; ++i) {
        Insert(data[i], false);
    }
}

template <typename T>
void SkipList<T>::Insert (T value, bool resize) {
    if (resize)
        size++;
    UpdateRoot();

    /*  pushFront case  */
    if (value < root->get_value()) {
        SkipListNode<T>* currentRoot = root;
        SkipListNode<T>* newRoot = new SkipListNode<T>(value, currentRoot->get_layer());
        root = newRoot;

        newRoot->forward = currentRoot;
        SkipListNode<T>* currentNewRoot = newRoot;
        currentRoot = currentRoot->down;

        while (currentRoot) {
            SkipListNode<T>* newCurrentRoot = new SkipListNode<T>(value, currentRoot->get_layer());
            currentNewRoot->down = newCurrentRoot;

            newCurrentRoot->forward = currentRoot;
            currentNewRoot = newCurrentRoot;
            currentRoot = currentRoot->down;
        }

        return;
    }

    /*  pushAnywhere case  */
    vector<SkipListNode<T>*> last_in_layer(max_layer, nullptr);
    SkipListNode<T>* current = root;

    for (int lvl = root->get_layer(); lvl >= 0; lvl--) {
        while (current->forward && current->forward->get_value() < value) {
            current = current->forward;
        }
        last_in_layer[lvl] = current;
        current = current->down;
    }

    SkipListNode<T>* newNode = new SkipListNode<T>(value, 0);
    newNode->forward = last_in_layer[0]->forward;
    last_in_layer[0]->forward = newNode;

    SkipListNode<T>* currentDown = newNode;

    for (int lvl = 1; lvl < max_layer; lvl++) {
        if ((float)rand() / RAND_MAX >= 0.5f) {
            break;
        }
        SkipListNode<T>* upNode = new SkipListNode<T>(value, lvl);
        upNode->forward = last_in_layer[lvl]->forward;
        last_in_layer[lvl]->forward = upNode;

        upNode->down = currentDown;
        currentDown = upNode;
    }
}

template <typename T>
bool SkipList<T>::Contains(const T& value) {
    SkipListNode<T>* current = root;
    while (true) {
        if (current->get_value() == value) {
            return true;
        }
        if (current->forward && current->forward->get_value() <= value) {
            current = current->forward;
        } else {
            if (current->down) {
                current = current->down;
            } else {
                return false;
            }
        }
    }
}

template <typename T>
SkipListNode<T>* SkipList<T>::Search(const T& value) {
    SkipListNode<T>* current = root;
    while (true) {
        if (current->get_value() == value) {
            return current;
        }
        if (current->forward && current->forward->get_value() <= value) {
            current = current->forward;
        } else {
            if (current->down) {
                current = current->down;
            } else {
                return nullptr;
            }
        }
    }
}

template <typename T>
bool SkipList<T>::Erase(const T& value) {
    /* root delete case */
    if (value == root->get_value()) {
        vector<SkipListNode<T>*> nexts(max_layer, nullptr);
        SkipListNode<T>* current = root;
        while (current) {
            nexts[current->get_layer()] = current->forward;
            current = current->down;
        }

        SkipListNode<T>* succ0 = nexts[0];
        if (!succ0) {
            current = root;
            while (current) {
                SkipListNode<T>* down = current->down;
                delete current;
                current = down;
            }
            root = nullptr;
            size--;
            return true;
        }

        vector<SkipListNode<T>*> bottom(max_layer, nullptr);
        for (int lvl = 0; lvl < max_layer; lvl++) {
            SkipListNode<T>* node = nexts[lvl];
            if (!node) continue;
            while (node->down) {
                node = node->down;
            }
            bottom[lvl] = node;
        }

        int highest = 0;
        for (int lvl = max_layer - 1; lvl >= 0; lvl--) {
            if (bottom[lvl] == bottom[0]) {
                highest = lvl;
                break;
            }
        }

        SkipListNode<T>* newRoot = nexts[highest];

        SkipListNode<T>* promo = newRoot;
        for (int lvl = promo->get_layer() + 1; lvl < max_layer; lvl++) {
            SkipListNode<T>* upNode = new SkipListNode<T>(promo->get_value(), lvl);
            upNode->forward = nexts[lvl];
            upNode->down = promo;
            promo = upNode;
        }

        current = root;
        while (current) {
            SkipListNode<T>* down = current->down;
            delete current;
            current = down;
        }

        root = promo;
        size--;
        return true;
    }

    /* another delete case */
    vector<SkipListNode<T>*> update(max_layer, nullptr);
    SkipListNode<T>* current = root;

    for (int lvl = root->get_layer(); lvl >= 0; lvl--) {
        while (current->forward && current->forward->get_value() < value) {
            current = current->forward;
        }
        update[lvl] = current;
        if (current->down) {
            current = current->down;
        }
    }

    SkipListNode<T>* target = update[0]->forward;
    if (!target || target->get_value() != value) {
        return false;
    }

    for (int lvl = 0; lvl < max_layer; lvl++) {
        if (update[lvl]->forward && update[lvl]->forward->get_value() == value) {
            SkipListNode<T>* nodeToDelete = update[lvl]->forward;
            update[lvl]->forward = nodeToDelete->forward;
            delete nodeToDelete;
        }
    }

    --size;
    return true;
}

template <typename T>
void SkipList<T>::Clear() {
    if (!root) return;

    vector<SkipListNode<T>*> heads;
    for (SkipListNode<T>* cur = root; cur; cur = cur->down) {
        heads.push_back(cur);
    }

    for (auto head : heads) {
        SkipListNode<T>* node = head;
        while (node) {
            SkipListNode<T>* next = node->forward;
            delete node;
            node = next;
        }
    }

    root = nullptr;
    size = 0;
    max_layer = 1;
}

template <typename T>
void SkipList<T>::PrintFull() {
    SkipListNode<T>* current = root;
    while (current) {
        SkipListNode<T>* current2 = current;
        while (current2) {
            std::cout << current2->get_value() << " ";
            current2 = current2->forward;
        }
        current = current->down;
        std::cout << '\n';
    }
}

template <typename T>
void SkipList<T>::Print() {
    SkipListNode<T>* current = root;
    while (current->down) {
        current = current->down;
    }
    while (current) {
        std::cout << current->get_value() << " ";
        current = current->forward;
    }
    std::cout << '\n';
}

template<typename T>
string SkipList<T>::ToString() const {
    ostringstream oss;
    SkipListNode<T>* current = root;
    while (current && current->down) {
        current = current->down;
    }
    bool first = true;
    while (current) {
        if (!first) oss << " ";
        oss << current->get_value();
        first = false;
        current = current->forward;
    }
    return oss.str();
}

template<typename T>
string SkipList<T>::ToFullString() const {
    ostringstream oss;
    SkipListNode<T>* level = root;
    bool firstLevel = true;
    while (level) {
        if (!firstLevel) oss << "\n";
        SkipListNode<T>* node = level;
        bool first = true;
        while (node) {
            if (!first) oss << " ";
            oss << node->get_value();
            first = false;
            node = node->forward;
        }
        firstLevel = false;
        level = level->down;
    }
    return oss.str();
}

template <typename T>
void SkipList<T>::UpdateRoot() {
    int new_max_layer = (size > 1 ? static_cast<int>(floor(log2(size))) + 1 : 1);
    if (new_max_layer == max_layer) { return; }

    max_layer = new_max_layer;
    SkipListNode<T>* newRoot = new SkipListNode<T>(root->get_value(), max_layer - 1);
    SkipListNode<T>* tmp = root;
    newRoot->down = tmp;
    root = newRoot;
};

#endif //SKIPLIST_H

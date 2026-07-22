#ifndef DLLIST_HPP
#define DLLIST_HPP

#include <stdexcept>
#include <iterator>     // для std::forward_iterator_tag и т.д.
#include <cstddef>      // для std::ptrdiff_t

template<typename T>
class DLList { // Doubly Linked List
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& v) : data(v), prev(nullptr), next(nullptr) {}
        Node(T&& v) : data(std::move(v)), prev(nullptr), next(nullptr) {}
    };

    Node* sntl;      // sentinel node
    std::size_t sz;

    void _init() {
        this->sntl = new Node(T{});
        this->sntl->next = this->sntl;
        this->sntl->prev = this->sntl;
        this->sz = 0;
    }

    void _dstr() {
        Node* cur = this->sntl->next;
        while (cur != this->sntl) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        delete this->sntl;        // важно! sentinel тоже был выделен через new
        this->sntl = nullptr;
        this->sz = 0;
    }

    Node* _idx_node(std::size_t idx) const {
        Node* cur = nullptr;
        if (idx < sz / 2) {
            cur = this->sntl->next;
            for (std::size_t i = 0; i < idx; ++i) cur = cur->next;
        } else {
            cur = this->sntl->prev;
            for (std::size_t i = sz - 1; i > idx; --i) cur = cur->prev;
        }
        return cur;
    }

public:
    // ==================== Итераторы ====================

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator() : node(nullptr) {}
        explicit iterator(Node* n) : node(n) {}

        reference operator*() const { return node->data; }
        pointer operator->() const { return &(node->data); }

        iterator& operator++() {
            node = node->next;
            return *this;
        }
        iterator operator++(int) { 
            iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        bool operator==(const iterator& other) const { return node == other.node; }
        bool operator!=(const iterator& other) const { return node != other.node; }

    private:
        Node* node;
        friend class DLList;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        const_iterator() : node(nullptr) {}
        explicit const_iterator(Node* n) : node(n) {}
        const_iterator(const iterator& it) : node(it.node) {}  // из не-const

        reference operator*() const { return node->data; }
        pointer operator->() const { return &(node->data); }

        const_iterator& operator++() {
            node = node->next;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        bool operator==(const const_iterator& other) const { return node == other.node; }
        bool operator!=(const const_iterator& other) const { return node != other.node; }

    private:
        Node* node;
        friend class DLList;
    };

    // ==================== begin / end ====================

    iterator begin()                { return iterator(sntl->next); }
    iterator end()                  { return iterator(sntl); }

    const_iterator begin() const    { return const_iterator(sntl->next); }
    const_iterator end() const      { return const_iterator(sntl); }

    const_iterator cbegin() const   { return const_iterator(sntl->next); }
    const_iterator cend() const     { return const_iterator(sntl); }

    // ==================== Конструкторы, деструктор и т.д. ====================

    DLList() { _init(); }
    ~DLList() { _dstr(); }

    DLList(const DLList& other) {
        _init();
        for (Node* cur = other.sntl->next; cur != other.sntl; cur = cur->next)
            push_back(cur->data);
    }

    DLList(DLList&& other) noexcept {
        sntl = other.sntl;
        sz = other.sz;
        other._init();  // other становится пустым
    }

    DLList& operator=(DLList other) noexcept {
        swap(other);
        return *this;
    }

    void swap(DLList& other) noexcept {
        std::swap(sntl, other.sntl);
        std::swap(sz, other.sz);
    }

    // ===== Capacity =====
    bool empty() const noexcept { return sz == 0; }
    std::size_t size() const noexcept { return sz; }

    // ===== Accessors =====
    T& front() {
        if (empty()) throw std::out_of_range("front() on empty list");
        return sntl->next->data;
    }
    const T& front() const {
        if (empty()) throw std::out_of_range("front() on empty list");
        return sntl->next->data;
    }
    T& back() {
        if (empty()) throw std::out_of_range("back() on empty list");
        return sntl->prev->data;
    }
    const T& back() const {
        if (empty()) throw std::out_of_range("back() on empty list");
        return sntl->prev->data;
    }

    T& operator[](std::size_t idx) {
        if (idx >= sz) throw std::out_of_range("index out of range");
        return _idx_node(idx)->data;
    }
    const T& operator[](std::size_t idx) const {
        if (idx >= sz) throw std::out_of_range("index out of range");
        return _idx_node(idx)->data;
    }

    // ===== Modifiers =====
    void insert(std::size_t pos, const T& val) {
        if (pos > sz) throw std::out_of_range("insert position out of range");
        Node* newNode = new Node(val);
        Node* posNode = (pos == sz) ? sntl : _idx_node(pos);
        Node* prevNode = posNode->prev;

        prevNode->next = newNode;
        posNode->prev = newNode;
        newNode->next = posNode;
        newNode->prev = prevNode;
        ++sz;
    }

    void erase(std::size_t pos) {
        if (pos >= sz) throw std::out_of_range("erase position out of range");
        Node* eraseNode = _idx_node(pos);
        Node* nextNode = eraseNode->next;
        Node* prevNode = eraseNode->prev;

        nextNode->prev = prevNode;
        prevNode->next = nextNode;
        delete eraseNode;
        --sz;
    }

    void push_front(const T& value) { insert(0, value); }
    void push_back(const T& value)  { insert(sz, value); }
    void pop_front()                { if (empty()) throw std::out_of_range("pop_front on empty list"); erase(0); }
    void pop_back()                 { if (empty()) throw std::out_of_range("pop_back on empty list"); erase(sz - 1); }

    void clear() {
        _dstr();
        _init();
    }
};

#endif // DLLIST_HPP

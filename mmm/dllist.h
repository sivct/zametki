#ifndef DLLIST_HPP
#define DLLIST_HPP

#include <stdexcept>


template<typename T>
class DLList { // Doubly Linked List

private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& v) : data(v), prev(nullptr), next(nullptr) {}
        Node(T&& v) : data(std::move(v)), prev(nullptr), next(nullptr) {} // конструктор перемещения
    };

    Node* sntl; // указатель на фиктивный (сторожевой, senitel) Node
    std::size_t sz; // текущий размер списка

    void _init() { // initialization
        this->sntl = new Node(T{}); // выделение памяти в куче (heap)
        this->sntl->next = this->sntl;
        this->sntl->prev = this->sntl;
        this->sz = 0;
    }

    void _dstr() { // ustroy destroy poryadok eto otstoy
        
        Node* cur = this->sntl->next;
        while (cur != this->sntl) {
            Node* next = cur->next;
            delete cur; // освобождение памяти в куче (heap)
            cur = next;
        }
        // выпендрёжный вариант:
        // for (Node* cur = this->sntl->next, next = cur->next; cur != this->sntl; cur = next, next = cur->next) {
        //     delete cur;  
        // }

        this->sntl = nullptr;
        this->sz = 0; 
    }

    // O(n) (C / 2)
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

    DLList()  { this->_init(); } // конструктор по умолчанию
    ~DLList() { this->_dstr(); } // деструктор

    // конструктор копирования
    DLList(const DLList& other) { 
        this->_init();
        for (Node* cur = other.sntl->next; cur != other.sntl; cur = cur->next) this->push_back(cur->data); 
    }

    // конструктор перемещения
    DLList(DLList&& other) noexcept {
        this->sntl = other.sntl;
        this->sz = other.sz;
        other._init();
    }


    void swap(DLList& other) noexcept {
        std::swap(this->sntl, other.sntl);
        std::swap(this->sz, other.sz);
    }

    // ===== Capacity =====

    bool empty() const noexcept { return this->sz == 0; }
    std::size_t size() const noexcept { return this->sz; }


    // ===== Accessors =====
    // Дублирование методов в данном случае необходимо для соблюдения const-концепции.

    T& front() { // чтобы l.front() можно было использовать как lvalue, напр. l.front() = 5;
        if (this->empty()) throw std::out_of_range("front() on empty list");
        return this->sntl->next->data;
    }

    const T& front() const {
        if (this->empty()) throw std::out_of_range("front() on empty list");
        return this->sntl->next->data;
    }

    T& back() { // назначение аналогично T& front()
        if (this->empty()) throw std::out_of_range("back() on empty list");
        return this->sntl->prev->data;
    }

    const T& back() const {
        if (this->empty()) throw std::out_of_range("back() on empty list");
        return this->sntl->prev->data;
    }

    const T& operator[](std::size_t idx) const {
        if (idx >= this->sz) throw std::out_of_range("index out of range");
        return (this->_idx_node(idx))->data;
    }

    T& operator[](std::size_t idx) { // назначение аналогично T& front()
        if (idx >= this->sz) throw std::out_of_range("index out of range");
        return this->_idx_node(idx)->data;
    }


    // ===== Modifiers =====

    void insert(std::size_t pos, const T& val) {
        if (pos > this->sz) throw std::out_of_range("insert position out of range");

        Node* newNode = new Node(val);
        Node* posNode = (pos == this->sz) ? this->sntl : this->_idx_node(pos);
        Node* prevNode = posNode->prev;

        prevNode->next = newNode;
        posNode->prev = newNode;

        newNode->next = posNode;
        newNode->prev = prevNode;

        ++(this->sz);
    }

    void erase(std::size_t pos) {
        if (pos >= this->sz) throw std::out_of_range("erase position out of range");

        Node* eraseNode = this->_idx_node(pos);
        Node* nextNode = eraseNode->next;
        Node* prevNode = eraseNode->prev;

        nextNode->prev = prevNode;
        prevNode->next = nextNode;

        delete eraseNode;
        --(this->sz);
    }

    void push_front(const T& value) { this->insert(0, value); } // O(1)

    void push_back(const T& value) { this->insert(this->sz, value); } // O(1)

    void pop_front() { // O (1)
        if (this->empty()) throw std::out_of_range("pop_front on empty list");
        this->erase(0);
    }

    void pop_back() { // O (1)
        if (this->empty()) throw std::out_of_range("pop_back on empty list");
        this->erase(this->sz - 1);
    }

    void clear() {
        this->_dstr();
        this->_init();
    }
};

#endif // DLLIST_HPP

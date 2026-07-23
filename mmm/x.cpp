#include <functional> // для std::hash
#include <list>       // используем готовый список для цепочек
#include <vector>
#include <optional>   

#include <iostream>
#include <stdexcept>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* topNode;
    size_t count;

public:
    Stack() : topNode(nullptr), count(0) {}

    ~Stack() {
        while (!isEmpty()) pop();
    }

    // Запрещаем копирование для простоты (Rule of 5 можно дописать)
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        ++count;
    }

    void pop() {
        if (isEmpty()) throw std::runtime_error("Stack is empty");
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        --count;
    }

    T& top() {
        if (isEmpty()) throw std::runtime_error("Stack is empty");
        return topNode->data;
    }

    bool isEmpty() const { return count == 0; }
    size_t size() const { return count; }
};

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* frontNode; // Отсюда удаляем
    Node* backNode;  // Сюда добавляем
    size_t count;

public:
    Queue() : frontNode(nullptr), backNode(nullptr), count(0) {}

    ~Queue() {
        while (!isEmpty()) dequeue();
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            frontNode = backNode = newNode;
        }
        else {
            backNode->next = newNode;
            backNode = newNode;
        }
        ++count;
    }

    void dequeue() {
        if (isEmpty()) throw std::runtime_error("Queue is empty");
        Node* temp = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) backNode = nullptr; // Если очередь стала пустой
        delete temp;
        --count;
    }

    T& front() {
        if (isEmpty()) throw std::runtime_error("Queue is empty");
        return frontNode->data;
    }

    bool isEmpty() const { return count == 0; }
    size_t size() const { return count; }
};

template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    ~Deque() {
        while (!isEmpty()) pop_front();
    }

    void push_front(const T& val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++count;
    }

    void push_back(const T& val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            head = tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        ++count;
    }

    void pop_front() {
        if (isEmpty()) throw std::runtime_error("Deque empty");
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        --count;
    }

    void pop_back() {
        if (isEmpty()) throw std::runtime_error("Deque empty");
        Node* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        --count;
    }
    T& front() { return head->data; }
    T& back() { return tail->data; }
    bool isEmpty() const { return count == 0; }
    size_t size() const { return count; }
};





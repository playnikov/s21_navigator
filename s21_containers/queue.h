#ifndef S21_QUEUE_H
#define S21_QUEUE_h

#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {

template <typename T>
class Queue {
 private:
  struct Node {
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
  };

  Node* head_;
  Node* tail_;
  size_t size_;

  void clear() {
    while (!empty()) {
      pop();
    }
  }

 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  Queue() : head_(nullptr), tail_(nullptr), size_(0) {}
  Queue(const Queue& other) : Queue() {
    Node* current = other.head_;
    while (current) {
      push(current->data);
      current = current->next;
    }
  }
  Queue(Queue&& other)
      : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }
  Queue(std::initializer_list<value_type> const& items) : Queue() {
    for (const auto& item : items) {
      push(item);
    }
  }

  ~Queue() { clear(); }

  Queue& operator=(const Queue& other) {
    if (this != &other) {
      Queue temp(other);
      swap(temp);
    }
    return *this;
  }
  Queue& operator=(Queue&& other) {
    if (this != &other) {
      clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  void push(const value_type& value) {
    Node* new_node = new Node(value);
    if (empty()) {
      head_ = tail_ = new_node;
    } else {
      tail_->next = new_node;
      tail_ = new_node;
    }
    ++size_;
  }

  void pop() {
    if (empty()) {
      throw std::out_of_range("Queue::pop(): queue is empty");
    }
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
    --size_;
    if (empty()) {
      tail_ = nullptr;
    }
  }

  void swap(Queue& other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  reference front() {
    if (empty()) {
      throw std::out_of_range("Queue::front(): queue is empty");
    }
    return head_->data;
  }
  const_reference front() const {
    if (empty()) {
      throw std::out_of_range("Queue::front(): queue is empty");
    }
    return head_->data;
  }
  reference back() {
    if (empty()) {
      throw std::out_of_range("Queue::back(): queue is empty");
    }
    return tail_->data;
  }

  const_reference back() const {
    if (empty()) {
      throw std::out_of_range("Queue::back(): queue is empty");
    }
    return tail_->data;
  }

  bool empty() const { return size_ == 0; }
  size_type size() const { return size_; }
};

}  // namespace s21

#endif
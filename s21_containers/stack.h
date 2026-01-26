#ifndef S21_STACK
#define S21_STACK

#include <initializer_list>
#include <stdexcept>

namespace s21 {

template <typename T>
class Stack {
 private:
  struct Node {
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
  };

  Node* top_;
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

  Stack() : top_(nullptr), size_(0) {}

  Stack(std::initializer_list<value_type> const& items) : Stack() {
    for (const auto& item : items) {
      push(item);
    }
  }

  Stack(const Stack& other) : Stack() {
    if (!other.empty()) {
      Stack temp;
      Node* current = other.top_;

      while (current) {
        temp.push(current->data);
        current = current->next;
      }

      while (!temp.empty()) {
        push(temp.top());
        temp.pop();
      }
    }
  }

  Stack(Stack&& other) noexcept : top_(other.top_), size_(other.size_) {
    other.top_ = nullptr;
    other.size_ = 0;
  }

  ~Stack() { clear(); }

  Stack& operator=(const Stack& other) {
    if (this != &other) {
      Stack temp(other);
      swap(temp);
    }

    return *this;
  }

  Stack& operator=(Stack&& other) noexcept {
    if (this != &other) {
      clear();
      top_ = other.top_;
      size_ = other.size_;
      other.top_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  void push(const value_type& value) {
    Node* new_node = new Node(value);
    new_node->next = top_;
    top_ = new_node;
    ++size_;
  }

  void pop() {
    if (empty()) {
      throw std::out_of_range("Stack::pop(): stack is empty");
    }
    Node* temp = top_;
    top_ = top_->next;
    delete temp;
    --size_;
  }

  void swap(Stack& other) {
    std::swap(top_, other.top_);
    std::swap(size_, other.size_);
  }

  reference top() {
    if (empty()) {
      throw std::out_of_range("Stack::top(): stack is empty");
    }
    return top_->data;
  }

  const_reference top() const {
    if (empty()) {
      throw std::out_of_range("Stack::top(): stack is empty");
    }
    return top_->data;
  }

  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }
};

}  // namespace s21

#endif
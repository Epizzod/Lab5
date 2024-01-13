#include <iostream>
#include <initializer_list>
#include <cassert>

using namespace std;

template<typename T>
class AbstractDeque {
public:
    virtual ~AbstractDeque() {};

    virtual unsigned GetSize() const = 0;
    virtual bool PushFront(const T& element) = 0;
    virtual bool PopFront(T& element) = 0;
    virtual bool PushBack(const T& element) = 0;
    virtual bool PopBack(T& element) = 0;
    virtual bool PeekFront(T& element) const = 0;
    virtual bool PeekBack(T& element) const = 0;
    virtual bool IsEmpty() const = 0;
};

template<typename T>
class DequeBasedOnArray : public AbstractDeque<T> {
private:
    T* elements;
    unsigned capacity;
    unsigned size;
    int frontIndex;
    int backIndex;

public:
    DequeBasedOnArray(unsigned capacity) : capacity(capacity), size(0), frontIndex(-1), backIndex(-1) {
        assert(capacity > 0);
        elements = new T[capacity];
    }

    DequeBasedOnArray(const DequeBasedOnArray<T>& other) : capacity(other.capacity), size(other.size), frontIndex(other.frontIndex), backIndex(other.backIndex) {
        elements = new T[capacity];
        for (unsigned i = 0; i < capacity; i++) {
            elements[i] = other.elements[i];
        }
    }

    DequeBasedOnArray(DequeBasedOnArray<T>&& other) noexcept {
        elements = other.elements;
        capacity = other.capacity;
        size = other.size;
        frontIndex = other.frontIndex;
        backIndex = other.backIndex;

        other.elements = nullptr;
        other.capacity = 0;
        other.size = 0;
        other.frontIndex = -1;
        other.backIndex = -1;
    }

    ~DequeBasedOnArray() {
        delete[] elements;
    }

    DequeBasedOnArray& operator=(const DequeBasedOnArray<T>& other) {
        if (this != &other) {
            delete[] elements;

            capacity = other.capacity;
            size = other.size;
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;

            elements = new T[capacity];
            for (unsigned i = 0; i < size; i++) {
                elements[i] = other.elements[i];
            }
        }
        return *this;
    }

    DequeBasedOnArray& operator=(DequeBasedOnArray<T>&& other) noexcept {
        if (this != &other) {
            delete[] elements;

            elements = other.elements;
            capacity = other.capacity;
            size = other.size;
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;

            other.elements = nullptr;
            other.capacity = 0;
            other.size = 0;
            other.frontIndex = -1;
            other.backIndex = -1;
        }
        return *this;
    }

    unsigned GetSize() const override {
        return size;
    }

    bool PushFront(const T& element) override {
        if (size == capacity) {
            return false;
        }

        if (IsEmpty()) {
            frontIndex = 0;
            backIndex = 0;
        }
        else {
            frontIndex = (frontIndex - 1 + capacity) % capacity;
        }

        elements[frontIndex] = element;
        size++;
        return true;
    }

    bool PopFront(T& element) override {
        if (IsEmpty()) {
            return false;
        }

        element = elements[frontIndex];

        if (frontIndex == backIndex) {
            frontIndex = -1;
            backIndex = -1;
        }
        else {
            frontIndex = (frontIndex + 1) % capacity;
        }

        size--;
        return true;
    }

    bool PushBack(const T& element) override {
        if (size == capacity) {
            return false;
        }

        if (IsEmpty()) {
            frontIndex = 0;
            backIndex = 0;
        }
        else {
            backIndex = (backIndex + 1) % capacity;
        }

        elements[backIndex] = element;
        size++;
        return true;
    }

    bool PopBack(T& element) override {
        if (IsEmpty()) {
            return false;
        }

        element = elements[backIndex];

        if (frontIndex == backIndex) {
            frontIndex = -1;
            backIndex = -1;
        }
        else {
            backIndex = (backIndex - 1 + capacity) % capacity;
        }

        size--;
        return true;
    }

    bool PeekFront(T& element) const override {
        if (IsEmpty()) {
            return false;
        }

        element = elements[frontIndex];
        return true;
    }

    bool PeekBack(T& element) const override {
        if (IsEmpty()) {
            return false;
        }

        element = elements[backIndex];
        return true;
    }

    bool IsEmpty() const override {
        return size == 0;
    }

    friend ostream& operator<<(ostream& os, const DequeBasedOnArray& deque) {
        for (unsigned i = 0; i < deque.GetSize(); i++) {
            os << deque[i] << " ";
        }
        return os;
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }

        int ind = (frontIndex + index) % capacity;
        return elements[ind];
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }

        int ind = (frontIndex + index) % capacity;
        return elements[ind];
    }
};

template<typename T>
void demonstrateAbstractDeque(DequeBasedOnArray<T>& deque) {
    deque.PushFront(1);
    deque.PushFront(2);
    deque.PushBack(3);
    deque.PushBack(4);

    cout << "Size: " << deque.GetSize() << endl;
    cout << "Deque: " << deque << endl;

    int frontElement;
    if (deque.PeekFront(frontElement)) {
        cout << "\nFront Element: " << frontElement << endl;
    }

    int backElement;
    if (deque.PeekBack(backElement)) {
        cout << "Back Element: " << backElement << endl;
    }

    int removedElement;
    if (deque.PopFront(removedElement)) {
        cout << "\nFront Removed Element: " << removedElement << endl;
    }

    if (deque.PopBack(removedElement)) {
        cout << "Back Removed Element: " << removedElement << endl;
    }

    cout << "\nSize: " << deque.GetSize() << endl;
    cout << "Deque: " << deque << endl;
}

template<typename T>
void demonstrateAbstractDeque(DequeBasedOnArray<T>* deque) {
    deque->PushFront(1);
    deque->PushFront(2);
    deque->PushBack(3);
    deque->PushBack(4);

    cout << "\nSize: " << deque->GetSize() << endl;
    cout << "Deque: " << *deque << endl;

    int frontElement;
    if (deque->PeekFront(frontElement)) {
        cout << "\nFront Element: " << frontElement << endl;
    }

    int backElement;
    if (deque->PeekBack(backElement)) {
        cout << "Back Element: " << backElement << endl;
    }

    int removedElement;
    if (deque->PopFront(removedElement)) {
        cout << "\nFront Removed Element: " << removedElement << endl;
    }

    if (deque->PopBack(removedElement)) {
        cout << "Back Removed Element: " << removedElement << endl;
    }

    cout << "\nSize: " << deque->GetSize() << endl;
    cout << "Deque: " << *deque << endl;
}

int main() {
    DequeBasedOnArray<int> deque1(5);
    demonstrateAbstractDeque(deque1);

    DequeBasedOnArray<int> deque2(5);
    demonstrateAbstractDeque(&deque2);

    return 0;
}
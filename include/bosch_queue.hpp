#pragma once

#include <mutex>
#include <chrono>
#include <condition_variable>
#include <thread>
#include <stdexcept>

template <class T>
class Queue {

public:
    /** Creates a queue
     *
     * @param size: size of queue to create
     */
    Queue(int size);


    /** Deletes queue
     *
     */
    virtual ~Queue();


    /** Pushes an element into the queue
     *
     * @param element: element to be inserted into queue
     */
    void Push(T element);


    /** Returns the oldest element in queue and removes it
     *
     * @return oldest element in the queue
     */
    T Pop();


    /** Returns the oldest element in queue and removes it with a timeout
     *
     * @param ms: time in milliseconds to wait for this request to be fulfilled
     *
     * @return oldest element in the queue
     */
    T PopWithTimeout(int ms);


    /** Returns the number of elements in the queue
     *
     * @return number of elements in the queue
     */
    int Count();


    /** Returns the maximum number of elements the queue can contain
     *
     * @return maximum number of elements the queue can contain
     */
    int Size();


private:
    T* queue;
    int capacity;
    int elements;
    int head;   // Before first position with data
    int tail;   // Last position with data
    std::mutex mtx;
    bool done;
};


template <class T>
Queue<T>::Queue(int size) : capacity(size), elements(0), head(-1), tail(-1), done(false) {
    
    queue = new T[size];
}

template <class T>
Queue<T>::~Queue() {
    
    delete[] queue;
}

template <class T>
void Queue<T>::Push(T element) {
    
    std::unique_lock<std::mutex> lock(mtx);
    // if full, drop oldest element
    if (elements == capacity) {
        head++;
        elements--;
    }

    queue[(++tail)%capacity] = element;
    elements++;
}
 
template <class T>
T Queue<T>::Pop() {

    while (!elements) {}

    std::unique_lock<std::mutex> lock(mtx);
    elements--;
    return queue[(++head)%capacity];
}

template <class T>
T Queue<T>::PopWithTimeout(int ms) {
    
    if (elements) {
        return Pop();  
    }

    T value;
    done = false;

    std::thread timeoutThread([&]() -> T {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        if (!done)
            throw std::runtime_error("Timeout waiting to Pop");
        return Pop();
    });

    value = Pop();
    done = true;
    return value;
}

template <class T>
int Queue<T>::Count() {
    
    return elements;
}

template <class T>
int Queue<T>::Size() {
    
    return capacity;
}
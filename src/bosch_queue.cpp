#include "bosch_queue.hpp"

#include <thread>
#include <iostream>
#include <condition_variable>
#include <mutex>

std::mutex signal_mtx;
std::condition_variable write_done, read_done;

int writingThread(Queue<int>& queue) {

    std::unique_lock<std::mutex> write_lock(signal_mtx);

    read_done.wait(write_lock);
    std::cout << "writingThread: Pushing 1" << std::endl;
    queue.Push(1);

    write_done.notify_all();
    read_done.wait(write_lock);

    std::cout << "writingThread: Pushing 2" << std::endl;
    queue.Push(2);
    std::cout << "writingThread: Pushing 3" << std::endl;
    queue.Push(3);
    std::cout << "writingThread: Pushing 4" << std::endl;
    queue.Push(4); // Element 2 dropped! 3,4

    write_done.notify_all();
    read_done.wait(write_lock);

    std::cout << "writingThread: Pushing 5" << std::endl;
    queue.Push(5);

    write_done.notify_all();
    std::cout << "writingThread: Returning" << std::endl;
    return 0;
}


int readingThread(Queue<int>& queue, int milliseconds) {

    std::unique_lock<std::mutex> read_lock(signal_mtx);

    write_done.wait(read_lock);
    read_done.notify_all();

    write_done.wait(read_lock);

    int temp = queue.Pop();
    std::cout << "readingThread: Popped " << temp << std::endl;

    read_done.notify_all();
    write_done.wait(read_lock);

    temp = queue.Pop();
    std::cout << "readingThread: Popped " << temp << std::endl;
    temp = queue.Pop();
    std::cout << "readingThread: Popped " << temp << std::endl;
    read_done.notify_all();

    write_done.wait(read_lock); // NOT
    temp = queue.PopWithTimeout(milliseconds);
    std::cout << "readingThread: Popped " << temp << std::endl;

    std::cout << "readingThread: Returning" << std::endl;
    return 0;
}

int main() {

    Queue<int> aQueue = Queue<int>(2);

    std::thread reader(readingThread, std::ref(aQueue), 500);
    std::thread writer(writingThread, std::ref(aQueue));

    std::cout << "Threads spawned" << std::endl;
    write_done.notify_all();

    writer.join();
    reader.join();

    return 0;
}
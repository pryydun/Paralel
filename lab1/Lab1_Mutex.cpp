
#include <iostream>
#include <thread>
#include <windows.h>
#include <list>
#include <mutex>
#include <algorithm>
#include <string>
#include <functional>

using namespace std;

#define TASK 6

// Завдання 1.2.1
#if TASK == 1

void Thread1() {
    cout << 1 << endl;
}

void Thread2() {
    cout << 2 << endl;
}

int main() {
    thread t1(Thread1);
    thread t2(Thread2);

    return 0;
}

// Завдання 1.2.2
#elif TASK == 2

void Thread1() {
    cout << 1 << endl;
}

void Thread2() {
    cout << 2 << endl;
}

int main() {
    thread t1(Thread1);
    thread t2(Thread2);

    t1.detach();
    t2.detach();
    Sleep(10);
    return 0;
}

// Завдання 1.2.3

#elif TASK == 3

 
list<int> l;

 
void AddToList(int number) {
    for (int i = 0; i < 10; i++) {
        int value = number + i;

        l.push_back(value);

        cout << "Added: " << value << endl;
    }
}

 
void ListContains(int number) {
    for (int i = 0; i < 10; i++) {

        bool found =
            find(l.begin(), l.end(), number) != l.end();

        if (found) {
            cout << number << " found" << endl;
        }
        else {
            cout << number << " not found" << endl;
        }
    }
}

int main() {
    int number = 5;

    thread t1(AddToList, number);
    thread t2(ListContains, number);

    t1.join();
    t2.join();

    return 0;
}


// Завдання 1.2.4


#elif TASK == 4

list<int> l;
mutex mtx;

void AddToList(int number) {
    for (int i = 0; i < 10; i++) {
        mtx.lock();

        l.push_back(number + i);
        cout << "Added: " << number + i << endl;

        mtx.unlock();
    }
}



void ListContains(int number) {
    for (int i = 0; i < 10; i++) {

        mtx.lock();

        bool found =
            find(l.begin(), l.end(), number) != l.end();

        if (found) {
            cout << number << " found" << endl;
        }
        else {
            cout << number << " not found" << endl;
        }

        mtx.unlock();
    }
}


int main() {
    int number = 5;

    thread t1(AddToList, number);
    thread t2(ListContains, number);

    t1.join();
    t2.join();

    return 0;
}


// Завдання 1.2.5

#elif TASK == 5

list<int> l;
mutex mtx;

void AddToList(int number) {
    lock_guard<mutex> lock(mtx);

    l.push_back(number);
    cout << "Added: " << number << endl;
}

void ListContains(int number) {
    lock_guard<mutex> lock(mtx);

    bool found = false;

    for (int x : l) {
        if (x == number) {
            found = true;
            break;
        }
    }

    cout << number << (found ? " found" : " not found") << endl;
}

int main() {
    int number = 5;

    for (int i = 0; i < 10; i++) {
        thread t1(AddToList, number + i);
        thread t2(ListContains, number);

        t1.detach();
        t2.detach();
    }

    Sleep(1000);

    return 0;
}

// Завдання 1.2.6
#elif TASK == 6

class someData {
public:
    string name;
    string surname;
    string address;
    int age = 0;

    void print() const {
        cout << name << " " << surname << ", "
            << address << ", " << age << endl;
    }
};

class exchangePerson {
private:
    someData data;
    mutex mtx;
    bool ready = false;

public:
    static void JohnDoe(exchangePerson& p) {
        lock_guard<mutex> lock(p.mtx);

        p.data = { "John", "Doe", "Unknown", 120 };
        p.ready = true;
    }

    static void JacobSmith(exchangePerson& p) {
        lock_guard<mutex> lock(p.mtx);

        p.data = { "Jacob", "Smith", "Known", 1 };
        p.ready = true;
    }

    static bool IsReady(exchangePerson& p) {
        lock_guard<mutex> lock(p.mtx);
        return p.ready;
    }

    static void Swap(exchangePerson& p1, exchangePerson& p2) {
        if (&p1 == &p2)
            return;

        std::lock(p1.mtx, p2.mtx);

        lock_guard<mutex> lock1(p1.mtx, adopt_lock);
        lock_guard<mutex> lock2(p2.mtx, adopt_lock);

        cout << "Before swap:" << endl;
        p1.data.print();
        p2.data.print();

        someData temp = p1.data;
        p1.data = p2.data;
        p2.data = temp;

        cout << "After swap:" << endl;
        p1.data.print();
        p2.data.print();
    }
};

int main() {
    exchangePerson p1, p2;

    thread t1(exchangePerson::JohnDoe, ref(p1));
    t1.detach();

    thread t2(exchangePerson::JacobSmith, ref(p2));
    t2.detach();

    while (!exchangePerson::IsReady(p1) ||
        !exchangePerson::IsReady(p2)) {
        Sleep(1);
    }

    thread t3(exchangePerson::Swap, ref(p1), ref(p2));
    t3.join();

    return 0;
}
// Завдання 1.2.7
#elif TASK == 7

class someData {
public:
    string name;
    string surname;
    string address;
    int age = 0;

    void print() const {
        cout << name << " " << surname << ", "
            << address << ", " << age << endl;
    }
};

class exchangePerson {
private:
    someData data;
    mutex mtx;
    bool ready = false;

public:
    static void JohnDoe(exchangePerson& p) {
        lock_guard<mutex> lock(p.mtx);

        p.data = { "John", "Doe", "Unknown", 120 };
        p.ready = true;
    }

    static void JacobSmith(exchangePerson& p) {
        lock_guard<mutex> lock(p.mtx);

        p.data = { "Jacob", "Smith", "Known", 1 };
        p.ready = true;
    }

    static bool IsReady(exchangePerson& p) {
        lock_guard<mutex> lock(p.mtx);
        return p.ready;
    }

    static void Swap(exchangePerson& p1, exchangePerson& p2) {
        if (&p1 == &p2)
            return;

        unique_lock<mutex> lock1(p1.mtx, defer_lock);
        unique_lock<mutex> lock2(p2.mtx, defer_lock);

        std::lock(lock1, lock2);

        cout << "Before swap:" << endl;
        p1.data.print();
        p2.data.print();

        someData temp = p1.data;
        p1.data = p2.data;
        p2.data = temp;

        cout << "After swap:" << endl;
        p1.data.print();
        p2.data.print();
    }
};

int main() {
    exchangePerson p1, p2;

    thread t1(exchangePerson::JohnDoe, ref(p1));
    t1.detach();

    thread t2(exchangePerson::JacobSmith, ref(p2));
    t2.detach();

    while (!exchangePerson::IsReady(p1) ||
        !exchangePerson::IsReady(p2)) {
        Sleep(1);
    }

    thread t3(exchangePerson::Swap, ref(p1), ref(p2));
    t3.join();

    return 0;
}
#endif

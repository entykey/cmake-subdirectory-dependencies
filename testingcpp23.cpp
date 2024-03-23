/*
// original c++ 23 code, ref: https://en.cppreference.com/w/cpp/memory/shared_ptr
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
 
using namespace std::chrono_literals;
 
struct Base
{
    Base() { std::cout << "Base::Base()\n"; }
 
    // Note: non-virtual destructor is OK here
    ~Base() { std::cout << "Base::~Base()\n"; }
};
 
struct Derived : public Base
{
    Derived() { std::cout << "Derived::Derived()\n"; }
 
    ~Derived() { std::cout << "Derived::~Derived()\n"; }
};
 
void print(auto rem, std::shared_ptr<Base> const& sp)
{
    std::cout << rem << "\n\tget() = " << sp.get()
              << ", use_count() = " << sp.use_count() << '\n';
}
 
void thr(std::shared_ptr<Base> p)
{
    std::this_thread::sleep_for(987ms);
    std::shared_ptr<Base> lp = p; // thread-safe, even though the
                                  // shared use_count is incremented
    {
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lk(io_mutex);
        print("Local pointer in a thread:", lp);
    }
}
 
int main()
{
    std::shared_ptr<Base> p = std::make_shared<Derived>();
 
    print("Created a shared Derived (as a pointer to Base)", p);
 
    std::thread t1{thr, p}, t2{thr, p}, t3{thr, p};
    p.reset(); // release ownership from main
 
    print("Shared ownership between 3 threads and released ownership from main:", p);
 
    t1.join();
    t2.join();
    t3.join();
 
    std::cout << "All threads completed, the last one deleted Derived.\n";
}
*/



/*
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define true 1
#define false 0

typedef struct Base {
    void (*destroy)(struct Base*);
} Base;

typedef struct Derived {
    Base base;
} Derived;

void Base_destroy(Base* self) {
    printf("Base::~Base()\n");
}

void Derived_destroy(Base* self) {
    printf("Derived::~Derived()\n");
}

Base* Base_create() {
    Base* self = (Base*)malloc(sizeof(Base));
    self->destroy = Base_destroy;
    printf("Base::Base()\n");
    return self;
}

Derived* Derived_create() {
    Derived* self = (Derived*)malloc(sizeof(Derived));
    self->base.destroy = Derived_destroy;
    printf("Derived::Derived()\n");
    return self;
}

void Base_destroy_shared(Base* self) {
    if (self != NULL) {
        self->destroy(self);
        free(self);
    }
}

typedef struct {
    pthread_t thread;
    Base* shared_ptr;
} ThreadArgs;

void* thr(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    usleep(987000);
    Base* local_ptr = args->shared_ptr;
    printf("Local pointer in a thread:\n\tget() = %p\n", (void*)local_ptr);
    return NULL;
}

int main() {
    Base* p = (Base*)Derived_create();

    printf("Created a shared Derived (as a pointer to Base)\n");
    printf("Shared ownership between 3 threads and released ownership from main:\n");

    pthread_t t1, t2, t3;
    ThreadArgs targs1 = {0}, targs2 = {0}, targs3 = {0};
    targs1.shared_ptr = p;
    targs2.shared_ptr = p;
    targs3.shared_ptr = p;

    pthread_create(&t1, NULL, thr, (void*)&targs1);
    pthread_create(&t2, NULL, thr, (void*)&targs2);
    pthread_create(&t3, NULL, thr, (void*)&targs3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    Base_destroy_shared(p);

    printf("All threads completed, the last one deleted Derived.\n");

    return 0;
}
*/
/*
Output:
Derived::Derived()
Created a shared Derived (as a pointer to Base)
Shared ownership between 3 threads and released ownership from main:
Local pointer in a thread:
        get() = 0x7fc0757059e0
Local pointer in a thread:
        get() = 0x7fc0757059e0
Local pointer in a thread:
        get() = 0x7fc0757059e0
Derived::~Derived()
All threads completed, the last one deleted Derived.
*/





#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Base {
    int ref_count;
    void (*destroy)(struct Base*);
} Base;

typedef struct Derived {
    Base base;
} Derived;

void Base_destroy(Base* self) {
    printf("Base::~Base()\n");
}

void Derived_destroy(Base* self) {
    printf("Derived::~Derived()\n");
    Base_destroy(self);
}

Base* Base_create() {
    Base* self = (Base*)malloc(sizeof(Base));
    self->ref_count = 1;
    self->destroy = Base_destroy;
    printf("Base::Base()\n");
    return self;
}

Derived* Derived_create() {
    Derived* self = (Derived*)malloc(sizeof(Derived));
    self->base.ref_count = 1;
    self->base.destroy = Derived_destroy;
    printf("Derived::Derived()\n");
    return self;
}

void Base_destroy_shared(Base* self) {
    if (self != NULL) {
        if (--self->ref_count <= 0) {
            self->destroy(self);
            free(self);
        }
    }
}

typedef struct {
    pthread_t thread;
    Base* shared_ptr;
} ThreadArgs;

void* thr(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    usleep(987000);
    Base* local_ptr = args->shared_ptr;
    printf("Local pointer in a thread:\n\tget() = %p\n", (void*)local_ptr);
    return NULL;
}

int main() {
    Base* p = (Base*)Derived_create();

    printf("Created a shared Derived (as a pointer to Base)\n");

    printf("Shared ownership between 3 threads and released ownership from main:\n");

    pthread_t t1, t2, t3;
    ThreadArgs targs1 = {0}, targs2 = {0}, targs3 = {0};
    targs1.shared_ptr = p;
    targs2.shared_ptr = p;
    targs3.shared_ptr = p;

    pthread_create(&t1, NULL, thr, (void*)&targs1);
    pthread_create(&t2, NULL, thr, (void*)&targs2);
    pthread_create(&t3, NULL, thr, (void*)&targs3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    Base_destroy_shared(p);

    printf("All threads completed, the last one deleted Derived.\n");

    return 0;
}
/*
Output:
Derived::Derived()
Created a shared Derived (as a pointer to Base)
Shared ownership between 3 threads and released ownership from main:
Local pointer in a thread:
        get() = 0x7f95957059e0
Local pointer in a thread:
        get() = 0x7f95957059e0
Local pointer in a thread:
        get() = 0x7f95957059e0
Derived::~Derived()
Base::~Base()
All threads completed, the last one deleted Derived.
*/


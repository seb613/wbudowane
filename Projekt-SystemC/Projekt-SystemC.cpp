#include <iostream>
#include <systemc.h>

SC_MODULE(hello_world) {
    SC_CTOR(hello_world) {
        SC_THREAD(main_thread);
    }
    void main_thread() {
        std::cout << "Hello, SystemC!" << std::endl;
    }
};

int sc_main(int argc, char* argv[]) {
    hello_world hw("HW");
    sc_start();
    return 0;
}


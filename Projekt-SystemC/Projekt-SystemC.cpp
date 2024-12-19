#include <iostream>
#include <systemc.h>

SC_MODULE(Module1) {
    sc_fifo<int> fifo;  // FIFO channel to communicate with Module2

    void task1() {
        while (true) {
            wait(10, SC_NS);  // Wait for 10 ns
            std::cout << "Task1: Sending data to Module2" << std::endl;
            fifo.write(1);  // Write to FIFO
        }
    }

    // Task2 of Module1
    void task2() {
        while (true) {
            wait(20, SC_NS);  // Wait for 20 ns
            std::cout << "Task2: Sending data to Module2" << std::endl;
            fifo.write(2);  // Write to FIFO
        }
    }

    // Task3 of Module1
    void task3() {
        while (true) {
            wait(30, SC_NS);  // Wait for 30 ns
            std::cout << "Task3: Sending data to Module2" << std::endl;
            fifo.write(3);  // Write to FIFO
        }
    }

    // Task4 of Module1
    void task4() {
        while (true) {
            wait(40, SC_NS);  // Wait for 40 ns
            std::cout << "Task4: Sending data to Module2" << std::endl;
            fifo.write(4);  // Write to FIFO
        }
    }

    // Nested task5 in Module1
    void task5() {
        while (true) {
            wait(50, SC_NS);  // Wait for 50 ns
            std::cout << "Task5: Sending data to Module2" << std::endl;
            fifo.write(5);  // Write to FIFO
        }
    }

    SC_CTOR(Module1) : fifo("fifo", 10) {  // FIFO with size 10
        SC_THREAD(task1);
        SC_THREAD(task2);
        SC_THREAD(task3);
        SC_THREAD(task4);
        SC_THREAD(task5);
    }
};

// Define the second module with 1 task (receiving data from FIFO)
SC_MODULE(Module2) {
    sc_fifo<int>* fifo;  // Pointer to the FIFO channel from Module1

    // Task of Module2: Receiving data from FIFO
    void receive_data() {
        while (true) {
            int data = fifo->read();  // Read from FIFO
            std::cout << "Module2 received: " << data << std::endl;
            wait(10, SC_NS);  // Simulate some processing delay
        }
    }

    SC_CTOR(Module2) {
        SC_THREAD(receive_data);
    }
};

// Top Module to instantiate and connect both modules
SC_MODULE(Top) {
    Module1* m1;  // Pointer to Module1
    Module2* m2;  // Pointer to Module2

    // Constructor
    SC_CTOR(Top) {
        m1 = new Module1("Module1");
        m2 = new Module2("Module2");

        // Connect the FIFO channel between Module1 and Module2
        m2->fifo = &m1->fifo;
    }
};

// Main function to start the simulation
int sc_main(int argc, char* argv[]) {
    Top top("Top");  // Instantiate the top module

    // Start the simulation
    sc_start();  // Run for 200 ns

    return 0;
}
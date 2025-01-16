#include <systemc.h>
#include <iostream>
#include <string>

using namespace std;

// Define traffic light states
#define RED "\t[\033[1;31mO\033[0m]"
#define YELLOW "\t[\033[1;33mO\033[0m]"
#define GREEN "\t[\033[1;32mO\033[0m]"
#define EMPTY "\t[O]\t[O]\n"
#define NONE "\t[O]"
#define EMERGENCY "\033[5mPOJAZD UPRZYWILEJOWANY\033[0m"

// Function to render traffic lights
void render_lights(int state)
{
	system("cls");
    switch (state)
    {
    case 1:
		cout << "Stan normalny task 1" << endl;
        cout << RED << NONE << endl
            << EMPTY << NONE << GREEN << endl;
        break;
    case 2:
		cout << "Stan normalny task 2" << endl;
        cout << RED << NONE << endl
            << YELLOW << YELLOW << endl
            << EMPTY;
        break;
    case 3:
		cout << "Stan normalny task 3" << endl;
        cout << NONE << RED << endl
            << EMPTY << GREEN << NONE << endl;
        break;
    case 4:
		cout << "Stan normalny task 4" << endl;
        cout << NONE << RED << endl
            << YELLOW << YELLOW << endl
            << EMPTY;
        break;
    case 5:
		cout << "Stan normalny task 5" << endl;
        cout << EMERGENCY << endl;
        cout << RED << NONE << endl
            << EMPTY
            << NONE << GREEN << endl;
        break;
    case 6:
		cout << "Stan awaryjny task6" << endl;
        cout << "Migajacy Pomaranczowy" << endl;
        cout << EMPTY
            << YELLOW << YELLOW << endl
            << EMPTY;
        break;
    default:
        cout << "Zabroniony stan" << endl;
    }
}

// Handle input from user
bool handle_input(sc_fifo<int>* fifo, sc_event* task_events, int current_state)
{
    cout << "Nacisnij Enter, aby wywolac nastepne zadanie, lub wpisz 'Q' aby zakonczyc program, 'A' dla trybu awarii (Pomaranczowe migajace), lub 'U' dla trybu uprzywilejowanego: ";
    string input;
    getline(cin, input);

    if (input == "Q" || input == "q")
    {
        cout << "Konczenie symulacji" << endl;
        sc_stop();
        exit(0);
    }
    if (input == "A" || input == "a")
    {
        if (fifo != nullptr)  // Check if fifo is initialized
        {
            fifo->write(6);  // Write to FIFO
        }
        else
        {
            throw std::runtime_error("FIFO is not initialized!");
        }
        return false; // Awaria -> wejście z klawiatury -> powrót do tego samego zadania
    }
    if (input == "U" || input == "u")
    {
        task_events[4].notify();
        return false; // Uprzywilejowany -> wejście z klawiatury -> powrót do tego samego zadania
    }

	if (input == "2")
	{
        if (current_state != 1) return true;
        task_events[1].notify();
		return false; 
	}
    if (input == "3")
    {
		if (current_state != 2) return true;
		task_events[2].notify();
		return false;
    }
	if (input == "4")
	{
		if (current_state != 3) return true;
		task_events[3].notify();
        return false;
	}
	if (input == "1")
	{
		if (current_state != 4) return true;
		task_events[0].notify();
        return false;
	}

    return true; // petla
}



//////////////////////////

// Submoduł dla Zadania 2 / task2
SC_MODULE(SubModule)
{
    sc_event* task_events;           // Wydarzenie do powiadomienia w nadrzędnym module (CPU1)
    sc_fifo<int>* fifo; // FIFO do komunikacji awaryjnej

    SC_CTOR(SubModule) 
    {
        SC_THREAD(task2);
    }

    void task2()
    {
        while (true)
        {
            wait(task_events[1]); // Czekaj na zdarzenie z nadrzędnego modułu
            cout << "CPU1 Zadanie 2 w SubModule: Wykonanie w czasie " << sc_time_stamp() << endl;
            render_lights(2);
            if (handle_input(fifo, task_events, 2))
            {
                task_events[1].notify(SC_ZERO_TIME);
            }
        }
    }
};

//////////////////////////////////////////////////////////////////

// CPU1
SC_MODULE(CPU1)
{
    sc_fifo<int>* fifo; // FIFO do komunikacji awaryjnej
    sc_event task_events[5];               // Wydarzenia dla każdego zadania
    SubModule submodule;                   // Submoduł dla Zadania 2

    SC_CTOR(CPU1) : submodule("SubModule")
    {
        submodule.task_events = task_events; // Przekazanie tablicy zdarzeń do Submodułu

        SC_THREAD(task1);
        SC_THREAD(task3);
        SC_THREAD(task4);
        SC_THREAD(task5);
    }

    void task1()
    {
        while (true)
        {
            wait(task_events[0]);
            cout << "CPU1 Zadanie 1: Wykonywanie w czasie " << sc_time_stamp() << endl;
            render_lights(1);
            if (handle_input(fifo, task_events, 1))
            {
                task_events[0].notify(SC_ZERO_TIME);
            }
        }
    }

    void task3()
    {
        while (true)
        {
            wait(task_events[2]);
            cout << "CPU1 Zadanie 3: Wykonywanie w czasie " << sc_time_stamp() << endl;
            render_lights(3);
			if (handle_input(fifo, task_events, 3))
            {
                task_events[2].notify(SC_ZERO_TIME);
            }
        }
    }

    void task4()
    {
        while (true)
        {
            wait(task_events[3]);
            cout << "CPU1 Zadanie 4: Wykonywanie w czasie " << sc_time_stamp() << endl;
            render_lights(4);
			if (handle_input(fifo, task_events, 4))
            {
                task_events[3].notify(SC_ZERO_TIME);
            }
        }
    }

    ////////////////////////////

    bool handle_emergency() {
		cout << "Nacisnij 1, aby powrocic do normalnego wykonania";
		string input;
		getline(cin, input);
		if (input == "1")
		{
			task_events[0].notify();
			return false;
		}
		return true;
	}

    void task5() // Pojazd uprzywilejowany
    {
        while (true)
        {
            wait(task_events[4]);
            cout << "CPU1 Zadanie 5: Wykonywanie w czasie " << sc_time_stamp() << endl;
            render_lights(5);
			if (handle_emergency())
			{
				task_events[4].notify(SC_ZERO_TIME);
			}
        }
    }
};

///////////////////////////////////////////////////////////////////


bool handle_interaction(sc_event* task_event)
{
    cout << "Nacisnij 1, aby powrocic do normalnego wykonania";
    string input;
    getline(cin, input);
	if (input == "1")
	{
		task_event->notify();
		return false;
	}
    return true;
}

// CPU2: Obsługuje stan awarii
SC_MODULE(CPU2)
{
    sc_fifo<int>* fifo; // FIFO do odbioru sygnałów awaryjnych
	sc_event* task_event; //powrót do normalnego stanu

    SC_CTOR(CPU2)
    {
        SC_THREAD(task6);
    }

    void task6() // Stan awaryjny (Pomarańczowe migające)
    {
        while (true)
        {
            int state = fifo->read();
            if (state == 6)
            {
                do{
                cout << "CPU2: Obsluguje stan awaryjny w czasie " << sc_time_stamp() << endl;
                render_lights(6);
                } while (handle_interaction(task_event));
                
            }
        }
    }
};

///////////////////////////////////////////////////////////////////

// Moduł najwyższego poziomu
SC_MODULE(Top)
{
    CPU1 cpu1;
    CPU2 cpu2;
    sc_fifo<int> fifo; //fifo o pojemności 1

    SC_CTOR(Top) : fifo(1), cpu1("CPU1"), cpu2("CPU2")
    {
        cpu1.fifo = &fifo; 
        cpu1.submodule.fifo = &fifo;
        cpu2.fifo = &fifo; 
		cpu2.task_event = &cpu1.task_events[0];


        SC_THREAD(start_simulation);
    }

    void start_simulation()
    {
        wait(1, SC_NS);
        cpu1.task_events[0].notify();
    }
};

int sc_main(int argc, char* argv[])
{
    Top top("Top");
    sc_start();
    return 0;
}

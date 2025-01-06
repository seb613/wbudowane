#include <systemc.h>
#include <iostream>
#include <string>

using namespace std;

// FIFO
sc_fifo<int> fifo(1);

// Stany świateł
#define RED "\t[\033[1;31mO\033[0m]"
#define YELLOW "\t[\033[1;33mO\033[0m]"
#define GREEN "\t[\033[1;32mO\033[0m]"
#define EMPTY "\t[O]\t[O]\n"
#define NONE "\t[O]"
#define EMERGENCY "\033[5mPOJAZD UPRZYWILEJOWANY\033[0m"

void render_lights(int state)
{
    switch (state)
    {
    case 1:
        cout << RED << NONE << endl
            << EMPTY << NONE << GREEN << endl;
        break;
    case 2:
        cout << RED << NONE << endl
            << YELLOW << YELLOW << endl
            << EMPTY;
        break;
    case 3:
        cout << NONE << RED << endl
            << EMPTY << GREEN << NONE << endl;
        break;
    case 4:
        cout << NONE << RED << endl
            << YELLOW << YELLOW << endl
            << EMPTY;
        break;
    case 5:
        cout << EMERGENCY << endl;
        cout << RED << NONE << endl
            << EMPTY 
            << NONE << GREEN << endl;
        break;
    case 6:
        cout << "Migajacy Pomaranczowy" << endl;
        cout << EMPTY 
            << YELLOW << YELLOW << endl
            << EMPTY;
        break;
    default:
        cout << "Zabroniony stan" << endl;
    }
}

//////////////////////////

bool handle_input(sc_port<sc_fifo_out_if<int>>* fifo_out, sc_event task_events[5])
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
        (*fifo_out)->write(6);
        return true; // Awaria -> wejście z klawiatury -> powrót do tego samego zadania
    }
    if (input == "U" || input == "u")
    {
        task_events[4].notify();
        return true; // Uprzywilejowany -> wejście z klawiatury -> powrót do tego samego zadania
    }

    return false; // Normalne wykonanie
}

//////////////////////////////////////////////////////////////////

// Submoduł dla Zadania 2
SC_MODULE(SubModule)
{
    sc_event* parent_task_events;           // Wydarzenie do powiadomienia w nadrzędnym module (CPU1)
    sc_port<sc_fifo_out_if<int>>* fifo_out; // FIFO do komunikacji awaryjnej
    sc_event task2_event;                   // Wydarzenie do wywołania zadania 2

    SC_CTOR(SubModule) : fifo_out(nullptr), parent_task_events(nullptr)
    {
        SC_THREAD(task2);
    }

    void task2()
    {
        while (true)
        {
            wait(task2_event); // Czekaj na zdarzenie z nadrzędnego modułu
            cout << "CPU1 Zadanie 2 w SubModule: Wykonanie w czasie " << sc_time_stamp() << endl;
            render_lights(2);
            if (handle_input(fifo_out, parent_task_events))
            {
                task2_event.notify();
                continue;
            }
            parent_task_events[2].notify();
        }
    }
};

//////////////////////////////////////////////////////////////////

// CPU1
SC_MODULE(CPU1)
{
    sc_port<sc_fifo_out_if<int>> fifo_out; // FIFO do komunikacji awaryjnej
    sc_event task_events[5];               // Wydarzenia dla każdego zadania
    SubModule submodule;                   // Submoduł dla Zadania 2

    SC_CTOR(CPU1) : submodule("SubModule")
    {
        submodule.parent_task_events = task_events; // Przekazanie tablicy zdarzeń do Submodułu
        submodule.fifo_out = &fifo_out;             // Przekazanie FIFO do Submodułu

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
            if (handle_input(&fifo_out, task_events))
            {
                task_events[0].notify(SC_ZERO_TIME);
                continue;
            }
            submodule.task2_event.notify(); // Powiadomienie submodułu
        }
    }

    void task3()
    {
        while (true)
        {
            wait(task_events[2]);

            cout << "CPU1 Zadanie 3: Wykonywanie w czasie " << sc_time_stamp() << endl;
            render_lights(3);
            if (handle_input(&fifo_out, task_events))
            {
                task_events[2].notify(SC_ZERO_TIME);
                continue;
            }
            task_events[3].notify();
        }
    }

    void task4()
    {
        while (true)
        {
            wait(task_events[3]);
            cout << "CPU1 Zadanie 4: Wykonywanie w czasie " << sc_time_stamp() << endl;
            render_lights(4);
            if (handle_input(&fifo_out, task_events))
            {
                task_events[3].notify(SC_ZERO_TIME);
                continue;
            }
            task_events[0].notify();
        }
    }

	void task5() // Pojazd uprzywilejowany
	{
		while (true)
		{
			wait(task_events[4]);
			cout << "CPU1 Zadanie 5: Wykonywanie w czasie " << sc_time_stamp() << endl;
			render_lights(5);
			if (handle_input(&fifo_out, task_events))
			{
				task_events[4].notify(SC_ZERO_TIME);
				continue;
			}
			task_events[0].notify();
		}
	}
};

///////////////////////////////////////////////////////////////////

// CPU2: Obsługuje stan awarii
SC_MODULE(CPU2)
{
    sc_port<sc_fifo_in_if<int>> fifo_in; // FIFO do odbioru sygnałów awaryjnych

    SC_CTOR(CPU2)
    {
        SC_THREAD(task6);
    }

	void task6() // Stan awaryjny (Pomarańczowe migające)
    {
        while (true)
        {
            int state = fifo_in->read();
            if (state == 6)
            {
                cout << "CPU2: Obsluguje stan awaryjny w czasie " << sc_time_stamp() << endl;
                render_lights(6);

                // czekaj na naciśnięcie klawisza, aby zresetować
                cout << "Nacisnij Enter, aby powrocic do normalnego wykonania";
                string input;
                getline(cin, input);
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

    SC_CTOR(Top) : cpu1("CPU1"), cpu2("CPU2")
    {
        cpu1.fifo_out(fifo);
        cpu2.fifo_in(fifo);

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
	setlocale(LC_ALL, "pl_PL.UTF-8"); // Ustawienie kodowania UTF-8 dla konsoli, ale nie działa z system C

    Top top("Top");
    sc_start();
    return 0;
}

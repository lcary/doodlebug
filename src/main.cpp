#include "controller.hpp"
#include "grid.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

// debugging only:
#include<mach/mach.h>
struct task_basic_info t_info;

void debug_info()
{
    // debugging for memory leaks only:
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    if (KERN_SUCCESS != task_info(mach_task_self(),
                                  TASK_BASIC_INFO, (task_info_t)&t_info, 
                                  &t_info_count)) {
        exit(-1);
    }
    std::cout << "resident memory size is: " << t_info.resident_size << std::endl;
    std::cout << "virtual memory size is: " << t_info.virtual_size << std::endl;
}

int main()
{
    std::ofstream out_stream;
    std::string temp;
    int count = 0;
    bool interactive_mode = true;

    out_stream.open("out/doodlebug.csv");
    if (out_stream.fail())
    {
        std::cout << "Output file in out/ failed to open.\n";
        exit(1);
    }
    out_stream << "iteration,doodlebugs,ants\n";  // csv header

    std::srand(std::time(NULL));  // required for random function usage

    Grid* grid = new Grid(20, 20);
    GridController controller = GridController(grid);

    // Initialize the world with 5 doodlebugs and 100 ants.
    std::cout << "Creating organisms..." << std::endl;
    controller.createAnts(100);
    controller.createDoodlebugs(5);
    controller.writeDataToCsv(out_stream, count);
    controller.display();

    // After each time step, prompt the user to press
    // Enter to move to the next time step.

    if (interactive_mode) {
        while (true) {
            std::cout << "Press enter to move to the next time step.\n";
            std::getline(std::cin, temp);

            controller.step();
            count++;

            std::cout << "iterations: " << count << std::endl;
            controller.writeDataToCsv(out_stream, count);
            controller.display();
            debug_info();
        }
    }
    else {
        while (count < 10000) {
            controller.step();
            count++;
            controller.writeDataToCsv(out_stream, count);
        }
        std::cout << "completed iterations: " << count << std::endl;
    }

    out_stream.close();
}

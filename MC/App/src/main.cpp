
#include <MainController.h>
#include "BJBPConfig.h"
#include "CommonTypes.h"
#include "Logger.h"
#include <csignal>
#include <iostream>

using namespace std;

MainController* mainController;

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    // Stop the controller
    if(mainController->stop() != BJBPCore::BJBP_SUCCESS)
    {
    	// Log error
    }

    exit(1);
}

int main(int argc, char** argv)
{
/*
	BJBP_LOG_INFO("Starting program\n");
    // register signal SIGINT and signal handler
    signal(SIGINT, signalHandler);

    // Get the main controller instance
    mainController = MainController::getInstance();

    // Start the Maincontroller
    if(mainController->start() != BJBPCore::BJBP_SUCCESS)
    {
    	// Log error
    	BJBP_LOG_ERR("Unable to start maincontroller. Terminating program\n");

    	std::this_thread::sleep_for(std::chrono::seconds(1));

    	return 0;
    }

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
*/
}

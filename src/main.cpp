#include <iostream>
#include <string>

#include <thread>
#include <chrono>
#include "terminal.hpp"

using namespace Terminal;

bool shutdown = false;
void outputThread()
{
	for(int i = 0;!shutdown;i++){
		std::this_thread::sleep_for(std::chrono::seconds(1));
		Terminal::terminal << Terminal::save << Terminal::Location(20,20) << std::string("Counting the time: ") << i << Terminal::restore;
	}
}



int main()
{
	terminal.onTextEntry([](std::string s)
	                     {
		                     if(s == "exit") shutdown = true; 
		                     else terminal << save << Location(0,4) << AnsiCode("0K") << "You entered: " << s << restore;
	                     }
		);

	terminal << clear;

	std::thread x(outputThread);
	
	terminal << "This is on the first line.\n";

	
	x.join();
	
	return 0;
}


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
	
	terminal << clear;

	auto inputFunc = [](std::string s){
		                     if(s == "exit") shutdown = true; 
		                     else terminal << save << Location(0,4) << AnsiCode("0K") << "You entered: " << s << restore;
		                     return !shutdown;
	};

	InputHandler<Prompt> inputHandler(terminal, Prompt("Enter Some text: ", Location(0,2)), inputFunc);

	std::thread x(outputThread);
	
	terminal << save << "This is on the first line.\n" << restore;

	
	x.join();
	shutdown = false;
	terminal << clear;
	std::thread y(outputThread);
	terminal << save << Location(0,0) << "Origin\n";
	
	auto inputFunc2 = [](std::string s){
		                     if(s == "exit") shutdown = true; 
		                     else terminal << save << Location(0,0) << AnsiCode("0K") << "You entered: " << s << restore;
		                     return !shutdown;
	};

	InputHandler<std::string> inputHandler2(terminal, "Scrolling Prompt: ", inputFunc2);
	

	y.join();

	
	return 0;
}


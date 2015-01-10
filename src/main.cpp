#include <iostream>
#include <string>
//#include <regex>
#include <algorithm>
#include <boost/regex.hpp>
#include <thread>
#include <functional>

#include "terminal.hpp"

std::string replString(boost::smatch s)
{
	//s = s + "found";
	return "IT is here";
}


class Fun
{
	public:
	std::string operator () (boost::smatch x)
	{
		std::cout << "I GOT CALLED";
		return "IGOTIMSERE";
	}
	//std::string fmt(boost::smatch x)
	//{
//		return "";
//	}
	//std::string value;
};


bool shutdown = false;
void outputThread()
{
	for(int i = 0;!shutdown;i++){
		sleep(1);
		Terminal::terminal << Terminal::save << Terminal::Location(20,20) << std::string("Hello") << i << Terminal::restore;
	}
}


template <typename T>
void testfun(T func)
{
	func(2);
}

void first(int a)
{
	std::cout << a;
}

class TestC
{
	public:
		void operator()(int a){
			std::cout << a + 10;
		}
};

template <typename T>
int ab(T i)
{
	std::cout << i;
	return i;
	
}
using namespace Terminal;

int main()
{
	std::thread x(outputThread);
	//testfun(first);
	//auto f = [](int a){std::cout <<a+4;};
	 
	//testfun(f);
	//TestC testC;
	
	//testfun(testC);
	
	//ab(1);
	
	terminal.clear();
	
	
	sleep(2);
	terminal << "First";
	//terminal.saveCursor();
	//terminal << save << Location(5,10) << "What" << restore;
	terminal << "What" << save << "where:";
	terminal << "Nextline" << "Test";
	//std::cout << "\nlockCount = " << lockCount << std::endl;
	//std::cout << "unlockCount = " << unlockCount << std::endl;
 
//terminal.cursorLocation();
	//terminal.restoreCursor();
	
//terminal.saveCursor();
	//terminal << Location(10,15) << "Where Dude";
	//terminal.restoreCursor();
	//terminal.restoreCursor();

	terminal.onTextEntry([](std::string s){if(s == "exit") shutdown = true; else terminal << save << Location(0,2) << AnsiCode("0K") << "You entered:" << s << restore;} );
	
	x.join();
	
	//sleep(10000);
	//std::string  inputString;
	//std::cin >> inputString;
	//std::cout << inputString;

	return 0;
}


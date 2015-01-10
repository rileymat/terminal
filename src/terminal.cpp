#include "terminal.hpp"

#include <iostream>
#include <chrono>

static const std::string termEscape = "\033[";

static void saveCursorPosition()
{
	std::cout << termEscape << "s" << std::flush;
}

static void restoreCursorPosition()
{
	std::cout << termEscape << "u" << std::flush;
}

static void moveCursor(int x, int y)
{
	std::cout << termEscape << y << ";" << x << "H" << std::flush;
}

static void clearLine()
{
	std::cout << termEscape << "0K" << std::flush;
}

static void clearScreen()
{
	std::cout << termEscape << "2J" << std::flush;
}


namespace Terminal
{
	Terminal terminal;

	LockedTerminal save(Terminal & term) { return LockedTerminal(term).saveCursor();}
	const LockedTerminal & save(const LockedTerminal & term){return term.saveCursor();}	
	const LockedTerminal& restore(const LockedTerminal & term){term.restoreCursor(); return term;}


	void Terminal::onTextEntry(std::function<void(std::string)> func)
	{
		textEntryFunc_ = func;
	}

	void Terminal::inputThreadFunc_()
	{
		while(true)
		{
			std::string line;
			std::getline (std::cin, line);
			terminal << termEscape << "1F" << termEscape <<"0K";
			textEntryFunc_(line);
		}
	}

	Terminal::Terminal():inputThread_(&Terminal::inputThreadFunc_,this)
	{
		inputThread_.detach();
	}

	void Terminal::clear() const
	{
		clearScreen();
		moveCursor(Location(0,0));
	}

	void Terminal::moveCursor(Location const & loc) const
	{
		::moveCursor(loc.getX(), loc.getY());
	}

	void Terminal::saveCursor() const
	{
		saveCursorPosition();
	}

	void Terminal::restoreCursor() const
	{
		restoreCursorPosition();
	}	


	LockedTerminal::LockedTerminal(Terminal & term):term_(term),lock_(new std::lock_guard<std::mutex>(term.outputMutex_)) {}

	const LockedTerminal &  LockedTerminal::moveCursor(Location const & loc) const {term_.moveCursor(loc); return *this;}
	
	const LockedTerminal & LockedTerminal::saveCursor() const {term_.saveCursor(); return *this;}
	
	const LockedTerminal & LockedTerminal::restoreCursor() const {term_.restoreCursor(); return *this;}
			
}








/*
  Location Terminal::cursorLocation()
  {
  std::cout << termEscape << "8m" << termEscape << "6n"<< std::flush;
  std::string result;
  std::cin >> result;
  std::cout << termEscape << "0m" << std::flush;
    
  //std::cout << "Result :" << result;
	
  return Location(0,0);
  }
*/


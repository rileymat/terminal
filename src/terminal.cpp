#include "terminal.hpp"

#include <iostream>
#include <chrono>

namespace Terminal
{
	Terminal terminal;

	Location::Location(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Terminal::Terminal()
	{

	}

	void Terminal::print(std::string const & outString) const
	{
		std::cout << outString << std::flush;
	}

	Terminal::LockedTerminal::LockedTerminal(Terminal & term):term_(term),lock_(new std::lock_guard<std::mutex>(term.outputMutex_)) {}


	//Stream modifier functions.
	Terminal::LockedTerminal save(Terminal & term) 
	{
		return Terminal::LockedTerminal(term) << AnsiCode("s");
	}
	const Terminal::LockedTerminal & save(const Terminal::LockedTerminal & term)
	{
		return term << AnsiCode("s");
	}
	
	Terminal::LockedTerminal Restore(Terminal & term)
	{
		return Terminal::LockedTerminal(term) <<  AnsiCode("u");
	}
	const Terminal::LockedTerminal& restore(const Terminal::LockedTerminal & term)
	{
		return term << AnsiCode("u");
	}

	Terminal::LockedTerminal clear(Terminal & term)
	{
		return Terminal::LockedTerminal(term) <<  AnsiCode("2J") << Location(0,0);
	}
	const Terminal::LockedTerminal& clear(const Terminal::LockedTerminal & term)
	{
		return term << AnsiCode("2J") << Location(0,0);
	}

}

#pragma once

#include <functional>
#include <string>
#include <sstream>
#include <initializer_list>
#include <iostream>
#include <mutex>
#include <thread>

namespace Terminal
{

	class Location
	{
		public:
			Location(int x, int y)
				{
					this->x = x;
					this->y = y;
				}
			int getX() const {return x;}
			int getY() const {return y;}
		private:
			int x;
			int y;
	};

	class AnsiCode
	{
		public:
			AnsiCode(std::string code):code_(code){};
			
			template<typename charT, typename traits>
			friend std::basic_ostream<charT, traits> &
			operator<< (std::basic_ostream<charT, traits> &lhs, AnsiCode const &rhs) {
				return lhs << "\033[" << rhs.code_;
			}
		private:
			std::string code_;
	};

	class Terminal;

	class LockedTerminal
	{
		public:
			LockedTerminal(Terminal & term);
			
		    const LockedTerminal & moveCursor(Location const & loc) const;
			const LockedTerminal & saveCursor() const;
			const LockedTerminal & restoreCursor() const;
			
			
			const LockedTerminal & operator << (Location const &  l)  const {moveCursor(l); return *this;}  
			const LockedTerminal & operator << (const LockedTerminal& (*func)(const LockedTerminal&)) const {(*func)(*this); return *this;}
			
			template<typename T>
			const LockedTerminal & operator <<(T t) const{std::stringstream ss; ss<<t; std::cout << ss.str() << std::flush; return *this;}	
		private:
			Terminal & term_;
			std::shared_ptr<std::lock_guard<std::mutex> > lock_;
	};

	class Terminal
	{
   
		public:
			Terminal();

			void onTextEntry(std::function<void(std::string)>);
			void clear() const;
			void moveCursor(Location const & loc) const;
			void saveCursor() const;
			void restoreCursor() const;

			LockedTerminal operator << (LockedTerminal (*func)(Terminal&)){ return (*func)(*this);}

			template<typename T>
			LockedTerminal operator << (T t){return LockedTerminal(*this) << t;}
		
		private:
			void inputThreadFunc_();

			
		private:
			std::function<void(std::string)> textEntryFunc_;
			std::thread inputThread_;
		protected:
			std::mutex outputMutex_;

			friend class LockedTerminal;
	};
	
	
	LockedTerminal save(Terminal & term);
	const LockedTerminal & save(const LockedTerminal & term);

	LockedTerminal restore(Terminal & term);
	const LockedTerminal& restore(const LockedTerminal & term);

		
	extern Terminal terminal;	
}

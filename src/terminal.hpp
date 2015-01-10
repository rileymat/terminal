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
	class AnsiCode
	{
		public:
			AnsiCode(std::string code):code_(code){};
			
			template<typename charT, typename traits>
			friend std::basic_ostream<charT, traits> &
			operator << (std::basic_ostream<charT, traits> &lhs, AnsiCode const &rhs) 
			{
				return lhs << "\033[" << rhs.code_;
			}
		private:
			std::string code_;
	};

	class Location
	{
		public:
			Location(int x, int y);
			template<typename charT, typename traits>
			friend std::basic_ostream<charT, traits> &
			operator << (std::basic_ostream<charT, traits> &lhs, Location const &rhs) 
			{
				std::stringstream ss;
				ss << rhs.y << ";" << rhs.x << "H";
				return lhs << AnsiCode(ss.str());
			}
		private:
			int x;
			int y;
	};

	class Terminal
	{
		public:
			Terminal();

			void onTextEntry(std::function<void(std::string)>);
			
			class LockedTerminal;
			Terminal::LockedTerminal operator << (Terminal::LockedTerminal (*func)(Terminal&)){ return (*func)(*this);}

			template<typename T>
			Terminal::LockedTerminal operator << (T t){return Terminal::LockedTerminal(*this) << t;}
		
		private:
			std::function<void(std::string)> textEntryFunc_;
			std::thread inputThread_;

			void inputThreadFunc_();
	
		protected:
			std::mutex outputMutex_;
			void print(std::string const &) const;
		
		public:
			
			class LockedTerminal
			{
				public:
					LockedTerminal(Terminal & term);
			
					const LockedTerminal & operator << (const LockedTerminal& (*func)(const LockedTerminal&)) const 
					{
							(*func)(*this); 
							return *this;
					 }
			
					template<typename T>
					const LockedTerminal & operator <<(T t) const
					{
						std::stringstream ss; 
						ss<<t; 
						term_.print(ss.str());
						return *this;
					}

				private:
					Terminal & term_;
					std::shared_ptr<std::lock_guard<std::mutex> > lock_;
			};
	};
	
	
	Terminal::LockedTerminal save(Terminal & term);
	const Terminal::LockedTerminal & save(const Terminal::LockedTerminal & term);

	Terminal::LockedTerminal restore(Terminal & term);
	const Terminal::LockedTerminal& restore(const Terminal::LockedTerminal & term);

	Terminal::LockedTerminal clear(Terminal & term);
	const Terminal::LockedTerminal& clear(const Terminal::LockedTerminal & term);


	extern Terminal terminal;	
}

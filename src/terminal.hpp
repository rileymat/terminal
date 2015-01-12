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

			class LockedTerminal;
			Terminal::LockedTerminal operator << (Terminal::LockedTerminal (*func)(Terminal&)){ return (*func)(*this);}

			template<typename T>
			Terminal::LockedTerminal operator << (T t){return Terminal::LockedTerminal(*this) << t;}
		
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

	
	class Prompt
	{
		public:
			Prompt(std::string text, Location location):text_(text),location_(location){}
			template<typename charT, typename traits>
			friend std::basic_ostream<charT, traits> &
			operator << (std::basic_ostream<charT, traits> &lhs, Prompt const &rhs) 
			{
				return lhs << rhs.location_ << rhs.text_ <<AnsiCode("0K");
			}
			
		private:
			Location location_;
			std::string text_;
	};

	template <typename P>
	class InputHandler
	{
		public:
			
			InputHandler(Terminal &term, P prompt, std::function<bool(std::string)> onTextEntry):
				inputThread_(&InputHandler::inputThreadFunc_, this),
				textEntryFunc_(onTextEntry),
				prompt_(prompt),
				term_(term)  
				{
					inputThread_.detach();
				}

		private:
			Terminal & term_;
			
			P prompt_;
			std::function<bool(std::string)> textEntryFunc_;
			std::thread inputThread_;

			void inputThreadFunc_()
				{
					std::string line = "";
					do
					{
						term_ << prompt_;
						std::getline (std::cin, line);
					} while(textEntryFunc_(line));
				}
			
	};
	
	
	Terminal::LockedTerminal save(Terminal & term);
	const Terminal::LockedTerminal & save(const Terminal::LockedTerminal & term);

	Terminal::LockedTerminal restore(Terminal & term);
	const Terminal::LockedTerminal& restore(const Terminal::LockedTerminal & term);

	Terminal::LockedTerminal clear(Terminal & term);
	const Terminal::LockedTerminal& clear(const Terminal::LockedTerminal & term);


	extern Terminal terminal;	
}

// <copyright file="SystemConsoleInputStream.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.IO
{
	/// <summary>
	/// The system console input stream implementation
	/// </summary>
	public class SystemConsoleInputStream : IConsoleInputStream
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='SystemConsoleInputStream'/> class.
		/// </summary>
		public SystemConsoleInputStream()
		{
#ifdef WIN32
			_inputHandle = GetStdHandle(STD_INPUT_HANDLE);
			if (_inputHandle == INVALID_HANDLE_VALUE) 
				throw std::runtime_error("GetStdHandle Failed");
#endif
		}

		~SystemConsoleInputStream()
		{
#ifdef WIN32
			_inputHandle = INVALID_HANDLE_VALUE;
#endif
		}

		/// <summary>
		/// Read a line from the stream
		/// </summary>
		std::string ReadLine() override final
		{
			std::string result;
			std::getline(std::cin, result);
			return result;
		}

		/// <summary>
		/// Read a password from the stream
		/// </summary>
		std::string ReadPassword() override final
		{
#ifdef WIN32
			const char Backspace = 8;
			const char Return = 13;

			// Save off the original state
			DWORD previousMode;
			if (!GetConsoleMode(_inputHandle, &previousMode))
				throw std::runtime_error("GetConsoleMode Failed");

			// Disable echo input and read individual characters
			DWORD currentMode = previousMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
			if (!SetConsoleMode(_inputHandle, currentMode))
				throw std::runtime_error("SetConsoleMode Failed");

			// Read until we get a return character
			std::string password;
			unsigned char character = 0;
			DWORD charactersRead;
			while (ReadConsoleA(_inputHandle, &character, 1, &charactersRead, nullptr))
			{
				if (character == Return)
				{
					std::cout << std::endl;
					break;
				}
				else if (character == Backspace)
				{
					// If not empty then remove a single character
					if (!password.empty())
					{
						std::cout << "\b \b";
						password.resize(password.length() - 1);
					}
				}
				else
				{
					password += character;
					std::cout << '*';
				}
			}

			// Restore the original state
			if (!SetConsoleMode(_inputHandle, previousMode))
				throw std::runtime_error("SetConsoleMode Failed");

			return password;
#else
			// TODO: Implement for realz
			// www.cplusplus.com/articles/E6vU7k9E/
			std::string result;
			std::getline(std::cin, result);
			return result;
#endif
		}

	private:
#ifdef WIN32
		HANDLE _inputHandle;
#endif
	};
}

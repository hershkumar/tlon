// File that contains functions for logging to a file, for debugging

#include <iostream>
#include <fstream>
#include <string>

class Logger {
	public:
		// file object to the log file
		std::ofstream file;
		bool initialized = false;	

		// function that gets the current timestamp, formatted nicely
		std::string ts()
		{
			std::string ret;
			//TODO: get timestamp and format it
			ret.append(": ");
		}
		
		Logger(std::string filename)
		{
			//TODO make sure that this doesn't error when the file doesn't exist
			file.open(filename);
			initialized = true;
			file << ts() << "Log file initialized" << std::endl;
		}		

		//TODO: function to log a matrix to a file

		//TODO function that logs a circuit diagram to a file

		//TODO: function to log any string to the log file
		int log_string(std::string str)
		{
			if (!initialized){
				//TODO throw an error here
				return 1;
			}
			file << ts() << str << std::endl;
			return 0;
		}
};


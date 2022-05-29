// Functions that read or write to the config file (tlon.cfg)

#include <iostream>
#include <fstream>

//TODO: function that checks for valid settings


//TODO function that generates a file pointer to the log file
std::ofstream open_log()
{
	//TODO: make this actually read from the config file to get the log path
	//for now, this just opens "tlon.log" in the same directory
	std::ofstream log_file;
	log_file.open("tlon.log");
	return log_file;
}

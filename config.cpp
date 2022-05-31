// Functions that read or write to the config file (tlon.cfg)

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

// config file
std::fstream config_file;

// dictionary storing all the config keys and their values
std::unordered_map<std::string, std::string> settings;

//TODO: function that checks for valid settings
void read_config(){
	config_file.open("tlon.cfg");
	//TODO: read through the file line by line and store each setting
	std::string curr_line;
	while(getline(config_file, curr_line)){
		// parse the line
		// add the line as a key value pair in the settings map
		
	}
}

// function that closes the config file reader
void config_close()
{
	config_file.close();
}

std::string get_log_filename()
{
	std::string ret;

	return ret;
}

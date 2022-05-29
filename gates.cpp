// gates are members of a class called Gate
// This file also defines some different gates that tlon comes with
#include <iostream>
#include <string>
#include <complex>
#include <vector>
#include <algorithm>
#include <fstream>

class Gate
{
private:
	// counter for the number of unique gates generated
	static int gate_count;
	// list of gate names
	static std::vector<std::string> gates;
	// name of the gate
	std::string name;    
	// id of the gate
	int gate_id;
	// matrix for the gate, given as a 2d vector of complex numbers 
	int size;
	std::vector<std::vector<std::complex<double>>> matrix;
	// description of what the gate does
	std::string description;

public:
	// constructor for the gate
	Gate(std::string gate_name, int gate_size, std::vector<std::vector<std::complex<double>>> gate_matrix, std::string desc = "N/A")
	{
		name = gate_name;
		size = gate_size;
		matrix = gate_matrix;
		description = desc;
		// Now check if the gate is a new gate
		// if its new, we have to assign it a new id
		auto it = std::find(gates.begin(), gates.end(), name);
		int index;
		if (it != gates.end()){
			index = it - gates.begin();
		}
		else {
			index = -1;
		}
		// if the gate is new
		if (index == -1){
			gates.push_back(name);
			gate_id = gate_count;
			gate_count++;
		}
		// if the gate isn't new, just make the id the index of the vector
		// this will match up with the id assigned to the original definition of the gate
		else {
			gate_id = index;
		}
	}

	// function to write matrix of gate to file
	void print(std::ofstream file)
	{
		file << "Matrix for " << name << std::endl;
		for (int i = 0; i < matrix.size(); i++){
			for (int j = 0; j < matrix.size(); j++){
				file << matrix[i][j] << " "; 
			}
			file << std::endl;
		}
	}
};

// function that reads a matrix from a csv file
std::vector<std::vector<std::complex<double>>> read_from_csv(std::ofstream input_file)
{
	std::vector<std::vector<std::complex<double>>> ret;
	//TODO: read in matrix from a csv file
	return ret;
}

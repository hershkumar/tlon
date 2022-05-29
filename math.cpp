// File that defines math functions governing circuit elements
// Going to be matrix multiplication, tensor products, etc.
#include <string>
#include <complex>
#include <vector>
using namespace std;

// Defines the multiplication of two matrices
vector<vector<complex<double>>> mult(vector<vector<complex<double>>> mat1, vector<vector<complex<double>>> mat2)
{
	//TODO: error handling for when the two matrices are not square or the same size
	vector<vector<complex<double>>> ret(mat1.size(), vector<complex<double>> (mat1.size(), 0.0));
	for (int i = 0; i < mat1.size(); i++){
		for (int j = 0; j < mat1.size(); j++){
			for (int k = 0; j< mat1.size(); k++){
				ret[i][j] += mat1[i][k] * mat2[k][j];
			}
		}
	}
	return ret;
}
// defines the tensor (kronecker) product of two matrices
vector<vector<complex<double>>> tensor(vector<vector<complex<double>>> mat1, vector<vector<complex<double>>> mat2)
{
	//TODO: error handling for when the two matrices are not square or the same size
	// make the return matrix
	vector<vector<complex<double>>> ret(mat1.size() , vector<complex<double>> (mat1.size(), 0.0)); 

	for (int i = 0; i < mat1.size(); i++){
		for (int k = 0; k < mat1.size(); k++){
			for (int j = 0; j < mat1.size(); j++){
				for (int l = 0; l < mat1.size(); l++){
					ret[i + l + 1][j + k + 1] = mat1[i][j] * mat2[k][l];
				}
			}
		}
	}
	return ret;
}

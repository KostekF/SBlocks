#include<iostream>
#include<fstream>
#include<vector>
#include <algorithm>
using namespace std;

const int numOfFunctions = 8;




//Create numOfFunctions function vectors with 256 bits each
void createFunctionsFromSblock(const std::vector<uint8_t> & buffer, vector<vector<bool>> & sblockFunctions)
{
	
	for (int i = 0; i < numOfFunctions; ++i)
	{
		vector<bool> v1 = {};
		//Iterate over input buffer and store one bit of each byte(omit every second byte with 0x00 data) in function vector v1
		for (auto itr = buffer.begin(); itr != buffer.end(); ++itr)
		{
			v1.push_back((*itr & (0b00000001 << i)) >> i);
			++itr; //Omit every second zeros byte
		}
		sblockFunctions.push_back(v1);
	}
}


//(zbalansowanie): Check balance in each function. Number of zeros has to be equal to number of ones in each function vector
void balance(const vector<vector<bool>> & sblockFunctions)
{
	
	int numOfOnes = 0;
	int numOfZeros = 0;

	//Iterate over every function vector (8 vectors)
	for (int i = 0; i<numOfFunctions; ++i)
	{
		numOfOnes = 0;
		numOfZeros = 0;
		//iterate over single function vector(256bits)
		for (int j = 0; j<sblockFunctions[i].size(); ++j)
		{
			if (sblockFunctions[i][j])
			{
				cout << "1";
				numOfOnes++;
			}
			else
			{
				cout << "0";
				numOfZeros++;
			}
		}
		cout << endl;
		cout << "Num Of Ones:" << numOfOnes << endl;
		cout << "Num Of Zeros:" << numOfZeros << endl;
		cout << endl;
	}
}


//Create vector of 256 linear functions (8-bit each)
void createLinearFunctions(vector<uint8_t> & linearFunction)
{
	for (int i = 0; i < 256; ++i)
	{
		linearFunction.push_back(0b0 + i);
	}
}

void generateAffineFunctions(vector<vector<bool>> & affineFunc)
{
	vector<uint8_t> linearFunction = {};
	createLinearFunctions(linearFunction);

	//Create 8 vectors of affine Functions using previously generated 256 linear functions

	for (int i = 0; i < numOfFunctions; ++i)
	{
		vector<bool> v1 = {};
		//Iterate over 256 linear Func and store one bit of each byte in function vector v1
		for (auto itr = linearFunction.begin(); itr != linearFunction.end(); ++itr)
		{
			v1.push_back((*itr & (0b00000001 << i)) >> i);
		}
		affineFunc.push_back(v1);
	}
	//cout << "Affine size: " << affineFunc.size() << endl;
}


void generateCombinedAffineFunctions(vector<vector<bool>> & combinedAffineFunc, vector<vector<bool>> &affineFunc)
{
	vector<bool> vTmpFalse(256, false);
	int counter = 0;
	int n = numOfFunctions;
	for (int r = 1; r <= n; ++r)
	{


		std::vector<bool> v(n);
		std::fill(v.begin(), v.begin() + r, true);
		do
		{

			fill(vTmpFalse.begin(), vTmpFalse.end(), false);
			//std::cout << counter << ") ";
			for (int i = 0; i < n; ++i)
			{
				if (v[i])
				{
					//std::cout << (i) << " ";
					//vTmpFalse = affineFunc[i] XOR vTmpFalse;

					vector<bool> vTmpNew = {};
					for (int g = 0; g<vTmpFalse.size(); ++g)
					{
						//XOR on bools

						if (vTmpFalse[g] != affineFunc[i][g])
						{

							vTmpNew.push_back(true);
						}
						else { vTmpNew.push_back(false); }
					}
					vTmpFalse = vTmpNew;

				}



			}
			combinedAffineFunc.push_back(vTmpFalse);
			counter++;
			//std::cout << "\n";
		} while (std::prev_permutation(v.begin(), v.end()));
	}




	//cout << "Counter: " << combinedAffineFunc.size() << "\n";
}

void calculateNonlinearity(vector<vector<bool>> & sblockFunctions, vector<vector<bool>> & combinedAffineFunc)
{
	//cout << "Sblock functions " << sblockFunctions[0].size() << "\n";
	//cout << "Combined affine functions: " << combinedAffineFunc[0].size() << "\n";
	int nonLinearity = -1;
	int minNonLinearity = 1000;
	//XOR 8 basic functions with affinic functions so 8x255=2040 XORs.
	for (int i = 0; i < sblockFunctions.size(); ++i)
	{

		minNonLinearity = 1000;
		for (int v = 0; v < combinedAffineFunc.size(); ++v)
		{
			nonLinearity = 0;
			for (int j = 0; j < sblockFunctions[i].size(); ++j)
			{
				//XOR on bools
				if (sblockFunctions[i][j] != combinedAffineFunc[v][j])
				{

				}
				else { nonLinearity++; }
			}
			//cout << "Non-linearity= " << nonLinearity << endl;

			minNonLinearity = min(nonLinearity, minNonLinearity);

		}
		cout << endl << endl;
		cout << "Non-linearity of SBLOCK is equal to: " << minNonLinearity << endl;
	}
}

double calculateSAC(const vector<bool> & sblockFunction)
{
	int wynik = 0;
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < sblockFunction.size(); ++j)
		{
			if (sblockFunction[j] != sblockFunction[j ^ (1 << i)])
				wynik++;
		}

	}
	//cout << 100.0*wynik / (256.0 * 8.0) << "\n";
	return 100.0*wynik / (256.0 * 8.0);
}
int main()
{

	
	//Take input SBLOCK binary file and store it in buffer
	ifstream input("C:\\Users\\Filip\\Documents\\..StudiaMAG_1sem\\Kryptografia\\sbox_08x08_20130109_221329_01.SBX", ios::binary);
	std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});

	//Bits will be stored as bools. False=0, true=1;
	//Those 8 functions(vectors of bool) will be stored in vector named sblockFunctions
	vector<vector<bool>> sblockFunctions = {};
	
	createFunctionsFromSblock(buffer, sblockFunctions);

	balance(sblockFunctions);

	cout << endl << endl;




	///////(nieliniowoœæ):  checking non-linearity of Sblock
	vector<vector<bool>> affineFunc;
	generateAffineFunctions(affineFunc);


	//Combinatorics - generate 255 functions from 8
	vector<vector<bool>> combinedAffineFunc = {};
	generateCombinedAffineFunctions(combinedAffineFunc,affineFunc);
	
	
	
	calculateNonlinearity(sblockFunctions, combinedAffineFunc);

	cout <<"\nObliczanie SAC dla kazdej z funkcji:\n";
	//calculate SAC for 8 functions and it's mean
	double SAC = 0.0;
	double mean = 0.0;
	for (int i = 0; i < 8; ++i)
	{
		SAC = calculateSAC(sblockFunctions[i]);
		cout << SAC << "\n";
		mean += SAC;
	}
	mean /= 8;
	cout << "Sredni SAC: " << mean << "\n";
	system("pause");

	return 0;
}
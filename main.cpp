#include<iostream>
#include<fstream>
#include<vector>
#include <algorithm>
using namespace std;

int main()
{
	//Take input SBLOCK binary file and store it in buffer
	ifstream input("C:\\Users\\Filip\\Documents\\..StudiaMAG_1sem\\Kryptografia\\sbox_08x08_20130109_221329_01.SBX", ios::binary);
	std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});

	//Bits will be stored as bools. False=0, true=1;
	//Those 8 functions(vectors of bool) will be stored in vector named functions
	vector<vector<bool>> functions = {};
	
	//Create 8 function vectors with 256 bits each
	for (int i = 0; i < 8; ++i)
	{
		vector<bool> v1 = {};
		//Iterate over input buffer and store one bit of each byte(omit every second byte with 0x00 data) in function vector v1
		for (auto itr = buffer.begin(); itr != buffer.end(); ++itr)
		{
			v1.push_back((*itr & (0b00000001 <<i )) >> i);
			++itr;
		}
		functions.push_back(v1);
	}


	//(zbalansowanie): Check balance in each function. Number of zeros has to be equal to number of ones in each function vector
	int numOfOnes = 0;
	int numOfZeros = 0;

	//Iterate over every function vector
	for (int i=0;i<8;++i)
	{
		numOfOnes = 0;
		numOfZeros = 0;
		//iterate over single function vector(256bits)
		for (int j=0; j<functions[i].size(); ++j)
		{
			if (functions[i][j])
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
		cout<< endl;
	}
	cout << endl << endl;

	///////(nieliniowoœæ):  checking non-linearity of Sblock

	//Create vector of 256 linear funcitons
	vector<uint8_t> linFun = {};
	for (int i = 0; i < 256; ++i)
	{
			linFun.push_back(0b0+i);
	}

	//Create 8 vectors of affine Functions using previously generated 256 linear functions
	vector<vector<bool>> affineFunc;
	for (int i = 0; i < 8; ++i)
	{
		vector<bool> v1 = {};
		//Iterate over 256 linear Func and store one bit of each byte in function vector v1
		for (auto itr = linFun.begin(); itr != linFun.end(); ++itr)
		{
			v1.push_back((*itr & (0b00000001 << i)) >> i);
		}
		affineFunc.push_back(v1);
	}
	//cout << "Affine size: " << affineFunc.size() << endl;


	int nonLinearity = -1;
	int minNonLinearity = 1000;
	//XOR 8 basic functions with affinic functions so 8x8=64 XORs.
	for (int i = 0; i < functions.size(); ++i)
	{
		for (int v = 0; v < affineFunc.size(); ++v)
		{
			nonLinearity = 0;
			for (int j = 0; j < functions[i].size(); ++j)
			{
				//XOR on bools
				if (functions[i][j] != affineFunc[v][j])
				{
					nonLinearity++;
				}
				else {}
			}
			cout << "Non-linearity= " << nonLinearity << endl;
			
			minNonLinearity = min(nonLinearity, minNonLinearity);

		}
	}
	cout << endl << endl;
	cout << "Non-linearity of SBLOCK is equal to: " << minNonLinearity << endl;


	//CZY TRZEBA TUTAJ DODAC PERMUTACJE FUNKCJI AFFINICZNYCH? TJ. np. XOR funAfini1 z funAfini2, z funAfini3 itd, chyba 8!=40320,
	//wtedy zamiast 64 XOR wyjdzie 8! x 8 XOR = 40320 x 8 = 322560 XORow


	system("pause");

	return 0;
}
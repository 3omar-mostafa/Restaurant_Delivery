#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inFile;
	inFile.open("input.txt");
	int x[3];
	for (int i = 0; i < 3; i++)
		inFile >> x[i];
	cout << x[0] + x[1] + x[2] << endl;
	system("pause");	
	
	
	
	return 0;
}
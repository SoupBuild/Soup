#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	std::ofstream outfile("test.txt");
	outfile << "my text here!" << std::endl;
	outfile.close();

	return 0;
}
#include "ShaderPermute.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	std::ifstream inputfile("../basicTest.json");
	if (!inputfile.good())
		throw std::runtime_error("File not found!");
	nlohmann::json json;
	inputfile >> json;
	auto perm = permute::fromJson<permute::PermuteText>(json);
	perm.generate();
	std::cout << perm.getContent();
	return 0;
}

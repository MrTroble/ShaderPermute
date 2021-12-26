#include "../ShaderPermute.hpp"
#include <iostream>

using namespace std;

int main() {
  auto perm = permute::fromFile<permute::PermuteGLSL>("basicTest.json");
  perm.generate();
  perm.generate({"NORMAL"});
  perm.generate({"TEXCOORD_0"});
  std::cout << perm.getContent();
  return 0;
}

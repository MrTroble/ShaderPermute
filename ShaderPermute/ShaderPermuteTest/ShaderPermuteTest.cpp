/*
   Copyright 2021 MrTroble

   Licensed under the Apache License,
   Version 2.0(the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "../ShaderPermute.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

int main() {
  glslang::InitializeProcess();
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}

TEST(glsl, compile) {
	permute::Permute permute;
	
	auto glslPermutation = permute.fromFile("basicTest.vert");
	const auto firstResult = glslPermutation->generate();
	ASSERT_TRUE(firstResult) << firstResult.error;

	auto glslPermutation2 = permute.fromFile("lightPassVert.vert");
	const auto result2 = glslPermutation2->generate();
	ASSERT_TRUE(result2) << result2.error;
	
	auto glslPermutation3 = permute.fromFile("lightPassFrag.frag");
	const auto result3 = glslPermutation3->generate();
	ASSERT_TRUE(result3) << result3.error;

}

class TestTraverser : public permute::ShaderTraverser {
public:
	std::unordered_set<std::string> symbols{};

	void visitSymbol(glslang::TIntermSymbol* node) override {
		symbols.insert(node->getName().c_str());
	}
};

TEST(glsl, compileWithDeps) {
	permute::Permute permute;

	TestTraverser traverser;

	auto glslPermutation = permute.fromFile("basicTest.vert");
	glslPermutation->traverser.push_back(&traverser);
	const auto firstResult = glslPermutation->generate();
	ASSERT_TRUE(firstResult) << firstResult.error;
	ASSERT_FALSE(traverser.symbols.contains("COLOR"));
	traverser.symbols.clear();

	const auto secondResult = glslPermutation->generate({ {"REQ_COLOR"} });
	ASSERT_TRUE(secondResult) << secondResult.error;
	ASSERT_GT(secondResult.output.size(), firstResult.output.size());
	ASSERT_TRUE(traverser.symbols.contains("COLOR"));
}

TEST(glsl, compileWithDepsAndAllCache) {
	permute::Permute<permute::AllCache> permute;

	auto glslPermutation = permute.fromFile("basicTest.vert");
	const auto firstResult = glslPermutation->generate();
	ASSERT_TRUE(firstResult) << firstResult.error;

	const auto secondResult = glslPermutation->generate({ {"REQ_COLOR"} });
	ASSERT_TRUE(secondResult) << secondResult.error;
	ASSERT_GT(secondResult.output.size(), firstResult.output.size());

	for (size_t i = 0; i < 100; i++)
	{
		const auto thirdResult = glslPermutation->generate({ {"REQ_COLOR"} });
		ASSERT_TRUE(thirdResult) << thirdResult.error;
	}
}

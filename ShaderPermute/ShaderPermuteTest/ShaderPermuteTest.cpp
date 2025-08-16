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
	const auto firstResult = glslPermutation.generate();
	ASSERT_TRUE(firstResult) << firstResult.error;
}

TEST(glsl, compileWithDeps) {
	permute::Permute permute;

	auto glslPermutation = permute.fromFile("basicTest.vert");
	const auto firstResult = glslPermutation.generate();
	ASSERT_TRUE(firstResult) << firstResult.error;

	const auto secondResult = glslPermutation.generate({ {"REQ_COLOR"} });
	ASSERT_TRUE(secondResult) << secondResult.error;
	ASSERT_GT(secondResult.output.size(), firstResult.output.size());
	// TODO Test more
}


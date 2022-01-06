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

class TestTraverser : public permute::ShaderTraverser {
public:
  bool visited = false;

  TestTraverser() : permute::ShaderTraverser() {}

  void visitSymbol(glslang::TIntermSymbol *) override { visited = true; }

  bool isValid(const permute::GlslSettings &settings) override { return true; }
};

TEST(glsl, compile) {
  auto perm = permute::fromFile<permute::PermuteGLSL>("basicTest.json");
  ASSERT_TRUE(perm.generate());
  ASSERT_TRUE(perm.generate({"NORMAL"}));
  ASSERT_TRUE(perm.generate({"TEXCOORD_0"}));
  ASSERT_TRUE(perm.generate({"TEXCOORD_0", "NORMAL", "COLOR"}));
  ASSERT_TRUE(perm.generate({"COLOR"}));
  ASSERT_TRUE(perm.generate());
}

TEST(glsl, traverser) {
  auto perm = permute::fromFile<permute::PermuteGLSL>("basicTest.json");
  TestTraverser tt;
  ASSERT_TRUE(perm.generate());
  ASSERT_TRUE(tt.visited);
}

TEST(glsl, writeBin) {
  auto perm = permute::fromFile<permute::PermuteGLSL>("basicTest.json");
  ASSERT_TRUE(perm.generate());
  ASSERT_NO_THROW(perm.toBinaryFile("testOutput.spv"));
}

TEST(text, equality) {
  nlohmann::json js = {{"codes", //
                        {
                            //
                            {
                                {"code", {"test"}}, {"flags", "required"} //
                            },                                            //
                            {
                                {"code", {"not"}}, {"dependsOn", {"not"}} //
                            }                                             //
                        }}};
  auto perm = permute::fromJson<permute::PermuteText>(js);
  ASSERT_TRUE(perm.generate());
  ASSERT_EQ(perm.getContent(), "test\n");
  ASSERT_TRUE(perm.generate({"not"}));
  ASSERT_EQ(perm.getContent(), "test\nnot\n");
}

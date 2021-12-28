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
  auto perm = permute::fromFile<permute::PermuteGLSL>("basicTest.json");
  ASSERT_TRUE(perm.generate());
  ASSERT_TRUE(perm.generate({"NORMAL"}));
  ASSERT_TRUE(perm.generate({"TEXCOORD_0"}));
  ASSERT_TRUE(perm.generate({"TEXCOORD_0", "NORMAL", "COLOR"}));
  ASSERT_TRUE(perm.generate({"COLOR"}));
  ASSERT_TRUE(perm.generate());
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

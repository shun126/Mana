#include <gtest/gtest.h>

#include "Path.h"

#include <array>

namespace mana::tests {

TEST(PathTest, PathSeparatorMatchesPlatform) {
#if defined(MANA_TARGET_WINDOWS)
  EXPECT_EQ(PathSeparator(), '\\');
#else
  EXPECT_EQ(PathSeparator(), '/');
#endif
}

TEST(PathTest, SplitAndMakePathRoundTrip) {
#if defined(MANA_TARGET_WINDOWS)
  const char* input = "C:\\tmp\\mana_test.txt";
#else
  const char* input = "/tmp/mana_test.txt";
#endif

  std::array<char, _MAX_DRIVE> drive{};
  std::array<char, _MAX_DIR> dir{};
  std::array<char, _MAX_FNAME> file{};
  std::array<char, _MAX_EXT> ext{};

  splitpath(input,
            drive.data(), drive.size(),
            dir.data(), dir.size(),
            file.data(), file.size(),
            ext.data(), ext.size());

  std::array<char, _MAX_PATH> output{};
  makepath(output.data(), output.size(),
           drive.data(), dir.data(), file.data(), ext.data());

  EXPECT_STREQ(output.data(), input);
}

TEST(PathTest, FullpathReturnsValue) {
  std::array<char, _MAX_PATH> output{};
  EXPECT_NE(fullpath(output.data(), ".", output.size()), nullptr);
}

}  // namespace mana::tests

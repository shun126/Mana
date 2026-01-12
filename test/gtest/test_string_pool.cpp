#include <gtest/gtest.h>

#include "StringPool.h"

namespace mana::tests {

TEST(StringPoolTest, SetReturnsPooledView) {
  StringPool pool;
  const auto pooled = pool.Set("alpha");

  EXPECT_EQ(pooled, "alpha");
}

TEST(StringPoolTest, SetDeduplicatesStrings) {
  StringPool pool;

  const auto first = pool.Set("dup");
  const auto second = pool.Set("dup");

  EXPECT_EQ(first, second);
  EXPECT_EQ(first.data(), second.data());
}

TEST(StringPoolTest, GetReturnsEmptyForMissing) {
  StringPool pool;

  const auto result = pool.Get("missing");

  EXPECT_TRUE(result.empty());
}

TEST(StringPoolTest, GetReturnsExistingString) {
  StringPool pool;
  pool.Set("beta");

  const auto result = pool.Get("beta");

  EXPECT_EQ(result, "beta");
}

}  // namespace mana::tests

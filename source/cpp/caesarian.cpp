#include <string>

std::string caesarian(std::string m, int n) {
    for ( auto& i : m)
        i = (i + 7 + n % 26) % 26 + 97;
    return m;
}

#include <gtest/gtest.h>
//-----------------------------------------------------------------------------
TEST (caesarian, abc) {
    EXPECT_EQ ( "def", caesarian (
      "abc",3
));
};
//-----------------------------------------------------------------------------
TEST (caesarian, egg) {
    EXPECT_EQ ( "dff", caesarian (
      "egg",-1
));
};
//-----------------------------------------------------------------------------
TEST (caesarian, super) {
    EXPECT_EQ ( "tvqfs", caesarian (
      "super",27
));
};
//-----------------------------------------------------------------------------
TEST (caesarian, internationally) {
    EXPECT_EQ ( "joufsobujpobmmz", caesarian (
      "internationally",12039
));
};
//-----------------------------------------------------------------------------
TEST (caesarian, stargazing) {
    EXPECT_EQ ( "lmtkztsbgz", caesarian (
      "stargazing",-3023
));
};
//-----------------------------------------------------------------------------
TEST (caesarian, polarequation) {
    EXPECT_EQ ( "febqhugkqjyed", caesarian (
      "polarequation",16
));
};
//-----------------------------------------------------------------------------
TEST (caesarian, codefights) {
    EXPECT_EQ ( "pbqrsvtugf", caesarian (
      "codefights",-1001
));
};
//-----------------------------------------------------------------------------
TEST (caesarian, caesarian) {
    EXPECT_EQ ( "aycqypgyl", caesarian (
      "caesarian",2000000000
));
};
#include <vector>
#include <string>
#include <regex>
#include <iostream>

#include <cstdlib>

// int P(int X,int Y) { return X % Y + (X % Y < 0 ? Y : 0);}
// int i,j,z,a = 0,b[9] = {0},v[9] = {0},
// ConwaysVariationsOfLife( auto d, auto c, int t)
// {
//   std::regex r("B(\\d+)/S(\\d+)");
//   std::smatch m;
//   std::regex_match(d, m, r);
//
//   for (  c: m[1].str())
//     b[c-48] = 1;
//
//   for (  c: m[2].str())
//     v[c-48] = 1;
//
//   auto w = c[0].size(),l = c.size();
//
//   auto n = c;
//   while (t--)
//   {
//     for(i=0; i<l; ++i)
//       for(j=0; j<w; ++j)
//       {
//           z =
//              c[i]      [P(j+1,w)] %2
//            + c[i]      [P(j-1,w)] %2
//            + c[P(i+1,l)][j]       %2
//            + c[P(i+1,l)][P(j+1,w)]%2
//            + c[P(i+1,l)][P(j-1,w)]%2
//            + c[P(i-1,l)][j]       %2
//            + c[P(i-1,l)][P(j+1,w)]%2
//            + c[P(i-1,l)][P(j-1,w)]%2;
//         n[i][j] =  c[i][j] == '#' ?
//           (v[z])?'#':' '
//          :
//           (b[z])?'#':' ';
//       }
//       c = n;
//   }
//   for ( r:n)
//     for( c:r)
//       a +=  c=='#' ? 1 : 0;
//   return a;
// }
int ConwaysVariationsOfLife( std::string def,
                             std::vector<std::string> current,
                             int iterations)
{
  int ans = 0;
  int born[9] = {false};
  int live[9] = {false};

  std::regex regex("B([0-9]+)/S([0-9]+)");
  std::smatch match;
  std::regex_match(def, match, regex);

#if _DEBUG
  std::cout << "B = " << match[1].str() << std::endl;
#endif

  for ( auto m: match[1].str())
    born[m-48] = true;

#if _DEBUG
  std::cout << "S = " << match[2].str() << std::endl;
#endif

  for ( auto m: match[2].str())
    live[m-48] = true;

  size_t w = current[0].size();
  size_t l = current.size();

  auto nextboard = current;

#if _DEBUG > 1
  std::cout << "====Begin====" << std::endl;
  for ( auto s: current)
    std::cout << s << std::endl;
  std::cout << "==== End ====" << std::endl;
#endif

  while (iterations--)
  {
    //nextboard = current;
    for(auto i=0; i<l; ++i)
    {

      for( auto j=0; j<w; ++j)
      {
        int neighbors = 0;
        neighbors += (w-1 == j)? current[i][0  ]  == '#' : current[i][j+1] == '#';
        neighbors += (0   == j)? current[i][w-1]  == '#' : current[i][j-1] == '#';
        if (l-1 == i) {
          neighbors += current[0  ][j] == '#';
          neighbors += (w-1 == j)? current[0][0  ]  == '#' : current[0][j+1] == '#';
          neighbors += (0   == j)? current[0][w-1]  == '#' : current[0][j-1] == '#';

        } else {
          neighbors += current[i+1][j]  == '#';
          neighbors += (w-1 == j)? current[i+1][0  ]  == '#' : current[i+1][j+1] == '#';
          neighbors += (0   == j)? current[i+1][w-1]  == '#' : current[i+1][j-1] == '#';
        }
        if (0  == i) {
          neighbors += current[l-1][j] == '#';
          neighbors += (w-1 == j)? current[l-1][0  ]  == '#' : current[l-1][j+1] == '#';
          neighbors += (0   == j)? current[l-1][w-1]  == '#' : current[l-1][j-1] == '#';
        } else {
          neighbors += current[i-1][j]  == '#';
          neighbors += (w-1 == j)? current[i-1][0  ]  == '#' : current[i-1][j+1] == '#';
          neighbors += (0   == j)? current[i-1][w-1]  == '#' : current[i-1][j-1] == '#';
        }
        if ( current[i][j] == '#' ){
          if (live[neighbors])
            nextboard[i][j] = '#';
          else
            nextboard[i][j] = ' ';
        } else {
            if (born[neighbors])
              nextboard[i][j] = '#';
            else
              nextboard[i][j] = ' ';
        }
      }
    }
    current = nextboard;
#if _DEBUG > 1
  std::cout << "====Begin====" << std::endl;
  for ( auto s: current)
    std::cout << s << std::endl;
  std::cout << "==== End ====" << std::endl;
#endif

  }

  for ( auto r:nextboard)
    for(auto c:r)
      ans +=  c=='#' ? 1 : 0;

  return ans;
}

#include <gtest/gtest.h>
//-----------------------------------------------------------------------------
TEST (Conways, Test_1) {
    EXPECT_EQ ( 12, ConwaysVariationsOfLife ( "B3/S23",
                                  {"       ", 
                                   "       ", 
                                   "   #   ", 
                                   "  ###  ", 
                                   "   #   ", 
                                   "       ", 
                                   "       "}
                                   ,4));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_2) {
    EXPECT_EQ ( 3, ConwaysVariationsOfLife ( "B3/S23",
                               {"     ",
                                "     ",
                                " ### ",
                                "     ",
                                "     "}
                                ,10));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_3) {
    EXPECT_EQ ( 8, ConwaysVariationsOfLife ( "B3/S23",
                              {"      ",
                               " ##   ", 
                               " #    ", 
                               "    # ", 
                               "   ## ", 
                               "      "}
                                ,3));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_4) {
    EXPECT_EQ ( 100, ConwaysVariationsOfLife ( "B0/S012345678",
                               {"          ", 
                                "          ", 
                                "          ", 
                                "          ", 
                                "          ", 
                                "          ", 
                                "          ", 
                                "          ", 
                                "          ", 
                                "          "}
                                ,5));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_5) {
    EXPECT_EQ ( 0, ConwaysVariationsOfLife ( "B0/S01234567",
                               {"     ", 
                                "     ", 
                                "     ", 
                                "     ", 
                                "     "}
                                ,10));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_6) {
    EXPECT_EQ ( 9, ConwaysVariationsOfLife ( "B1357/S02468",
                               {"     ", 
                                "     ", 
                                "  #  ", 
                                "     ", 
                                "     "}
                                ,7));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_7) {
    EXPECT_EQ ( 10, ConwaysVariationsOfLife ( "B3/S23",
                               {"      #   ", 
                                "       #  ", 
                                "     ###  ", 
                                "          ", 
                                "          ", 
                                "  #       ", 
                                "   #      ", 
                                " ###      ", 
                                "          ", 
                                "          "}
                                ,10));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_8) {
    EXPECT_EQ ( 67, ConwaysVariationsOfLife ( "B34/S234",
                              {"      #   ", 
                               "       #  ", 
                               "     ###  ", 
                               "          ", 
                               "          ", 
                               "  #       ", 
                               "   #      ", 
                               " ###      ", 
                               "          ", 
                               "          "}
                                ,10));
};
//-------------------------------------------------------------------------------
TEST (Conways, Test_9) {
    EXPECT_EQ ( 9, ConwaysVariationsOfLife ( 
      "B4/S03",
      {"          ", 
       "      ##  ", 
       "      ##  ", 
       "          ", 
       "          ", 
       "          ", 
       "  ##    # ", 
       "  ##   # #", 
       "        # ", 
       "          "}
      ,9));
};
//-------------------------------------------------------------------------------

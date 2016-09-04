#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include <algorithm>
#include <numeric>
#include <cmath>

//-----------------------------------------------------------------------------
inline int32_t find_gcd(int32_t a, int32_t b)
{ 
  int32_t c;
  while ( a != 0 ) {
     c = a; a = b%a;  b = c;
  }
  return b;
}
//-----------------------------------------------------------------------------
inline int32_t find_gcd(std::vector<int32_t> set)
{ 
  int a = set.back();
  set.pop_back();
  for ( int32_t b : set )
  {
    a = find_gcd(a,b);
  }
  return a;
}
//-----------------------------------------------------------------------------
int threeGlasses(std::vector<int32_t> cap) {
    int32_t ans = 1;
    std::sort(cap.begin(),cap.end());
    int32_t gcd_ab = find_gcd(cap[2],cap[1]);
    int32_t gcd_bc = find_gcd(cap[1],cap[0]);
    int32_t sum = std::accumulate(cap.begin(), cap.end(),0);
    // if( cap.front() == 1 ){
    //   ans = std::accumulate(cap.begin(), cap.end(),0);
    // } else

    // if ( cap[0] == cap[1]  ) {
    //   //gcd = find_gcd(cap[0],cap[2]);
    //   ans =  sum - (2 * cap[1]);
    //   ans =  ans/gcd + ((ans%gcd)?1:0) + 2 + ((ans%cap[0])?2:0);
    // }else
    //  {
    //    if( gcd == cap[0] ){
    //      ans = sum / gcd + ((cap[2]%cap[0])?1:0);
    //    } else {
    //     ans =  sum - cap[1];
    //     ans =  ans/gcd + cap[1]/cap[0] + ((cap[2]%cap[0])?1:0);
    //    }
    // }
    int32_t a = cap[2];
    int32_t b = cap[1];
    int32_t c = cap[0];

    ans  = (b%c != 0 )? (a-b+c) / std::min(gcd_ab,gcd_bc) 
            + ((b) / gcd_bc)
            + ((b%gcd_bc)?1:0)
            + (((a-b+c)%std::min(gcd_ab,gcd_bc))?1:0)
            + (b/c)
            + 1
    : (a+b+c)/ gcd_bc
            + (((a+b+c) % gcd_bc)?1:0)
            + (( a != c && a % c)?2:0);
    return ans;
}


#include "gtest/gtest.h"
//----------------------------------------------------------------------------- 
TEST (Glasses, 16_5_3) { 
    EXPECT_EQ ( 21, threeGlasses ({16,  5,  3}));
  }
//-----------------------------------------------------------------------------
TEST (Glasses, 1_1_1) { 
    EXPECT_EQ (  3, threeGlasses ({ 1,  1,  1}));
  }
//-----------------------------------------------------------------------------
TEST (Glasses, 1_50_100) { 
    EXPECT_EQ (151, threeGlasses ({ 1, 50,100}));
  }
//-----------------------------------------------------------------------------
TEST (Glasses, 8_5_3) { 
    EXPECT_EQ ( 13, threeGlasses ({ 8,  5,  3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 9_6_3) { 
    EXPECT_EQ ( 6, threeGlasses ({ 9,  6,  3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 3_3_3) { 
    EXPECT_EQ ( 3, threeGlasses ({  3, 3, 3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 7_7_7) { 
    EXPECT_EQ ( 3, threeGlasses ({  7, 7, 7}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 7_3_3) { 
    EXPECT_EQ ( 7, threeGlasses ({  7, 3, 3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 19_3_3) { 
    EXPECT_EQ ( 11, threeGlasses ({  19, 3, 3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 21_3_3) { 
    EXPECT_EQ ( 9, threeGlasses ({  21, 3, 3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 21_21_3) { 
    EXPECT_EQ ( 15, threeGlasses ({  21, 21, 3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 12_1_1) { 
    EXPECT_EQ ( 7, threeGlasses ({  8, 4, 2}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 7_7_3) { 
    EXPECT_EQ ( 13, threeGlasses ({  7, 7, 3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 6_2_3) { 
    EXPECT_EQ (7, threeGlasses ({  99,66,99}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 47_6_3) { 
    EXPECT_EQ (22, threeGlasses ({  47,6,3}));
}
//-----------------------------------------------------------------------------
TEST (Glasses, 41_61_79) { 
    EXPECT_EQ (104, threeGlasses ({ 41,61,79}));
}
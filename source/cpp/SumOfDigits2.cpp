#include<cmath>

constexpr int SumOfDigits(int n){

  return n%10 + ((n==0)?0 : SumOfDigits(n/10));
}

int SumOfDigits2(int n){
    int ans = 1;
    const int sum = SumOfDigits(n);
    if( sum == 1)
    {
        n/=10;
        while ( n != 0 )
        {
            ++ans;
            n/=10;
        }
    }
    else if ( n < 99){
      return std::fabs(n/10-n%10)+1;
    }
    else if ( sum < 9){
        //
    }
    else 
        for ( int i = 1; i< n; ++i){
           if ( SumOfDigits(i) == sum )
           ++ans;
         }
    return ans;
}

#include<gtest/gtest.h>
//-----------------------------------------------------------------------------
TEST (SumOfDigits2, codefights) {
    EXPECT_EQ ( 10, SumOfDigits2(90));
};

TEST (SumOfDigits2, BigAttempt) {
    EXPECT_EQ ( 7, SumOfDigits2(1000000));
};

TEST (SumOfDigits2, BigAttempt2) {
    EXPECT_EQ ( 3606591, SumOfDigits2(92104842));
};

TEST (SumOfDigits2, BigAttempt3) {
    EXPECT_EQ ( 6, SumOfDigits2(100000));
};
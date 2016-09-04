#include <iostream>
#include <chrono>
#include <ctime>

bool isLeapYear(int year) {
  if (year % 4 != 0) {
    return false;
  } else if (year % 400 == 0) {
    return true;
  } else if (year % 100 == 0) {
    return false;
  } else {
    return true;
  }
}
//----------------------------------------------------------------------------- 
long year_to_day(long year, long month, long day) { 
  long ans;
  long day_of_year;
  if (year < 0){
    ans = (year+1) * 365 + (year / 4) - (year / 100) + (year / 400);
    day_of_year = ( month == 12) ? day = 32 - day,0 :
           ( month == 11) ? day = 31 - day,31 :
           ( month == 10) ? day = 32 - day,61 :
           ( month ==  9) ? day = 31 - day,92 :
           ( month ==  8) ? day = 32 - day,122 :
           ( month ==  7) ? day = 32 - day,153 :
           ( month ==  6) ? day = 31 - day,184 :
           ( month ==  5) ? day = 32 - day,214 :
           ( month ==  4) ? day = 31 - day,245 :
           ( month ==  3) ? day = 32 - day,275 :
           ( month ==  2) ? day = 29 - day,306 :
           ( month ==  1) ? day = 32 - day,334 : 0;
   day_of_year += day;
   
  
   if ( isLeapYear( std::abs(year)) && month > 2)
   {
     ++ans;
   }
  
   ans = ans-day_of_year ;
   return ans;   
  } else {
    ans = year * 365 + (year / 4) - (year / 100) + (year / 400);
    day_of_year = ( month == 12) ? 334 :
           ( month == 11) ? 304 :
           ( month == 10) ? 273 :
           ( month ==  9) ? 243 :
           ( month ==  8) ? 212 :
           ( month ==  7) ? 181 :
           ( month ==  6) ? 151 :
           ( month ==  5) ? 120 :
           ( month ==  4) ? 90 :
           ( month ==  3) ? 59 :
           ( month ==  2) ? 31 :
           ( month ==  1) ? 0 : 0;
   day_of_year += day;
      
   if ( isLeapYear(year) && month < 3 )
   {
     --ans;
   }
    std::cout << ans << " " << day_of_year << std::endl;
   return ans+day_of_year;
  } 
  return 0;
}
//----------------------------------------------------------------------------- 
long uShouldDate(long day1, long month1, long year1, long day2, long month2, long year2)
{
  long ans = std::abs(year_to_day(year2, month2, day2) 
                    - year_to_day(year1, month1, day1));

  return ans;
}
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

//-----------------------------------------------------------------------------
TEST (uShouldDate, 1_1_1__12_2_1) {
    EXPECT_EQ ( 42, uShouldDate (1,1,1, 12,2,1));
  }
//-----------------------------------------------------------------------------
TEST (uShouldDate, 0_0_0__0_0_0) {
    EXPECT_EQ ( 0, uShouldDate (0,0,0, 0,0,0));
  }
// //-----------------------------------------------------------------------------
TEST (uShouldDate, MIN_TO_MAX) {
    EXPECT_EQ ( 368894925, uShouldDate (1,1,-10000, 1,1,1000000));
}
//-----------------------------------------------------------------------------
TEST (uShouldDate, year) {
    EXPECT_EQ ( 364, uShouldDate (1,1,1, 31,12,1));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, Positive_Year) {
    EXPECT_EQ ( 364, uShouldDate (1,1,1,31,12,1));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, Neitive_Year) {
    EXPECT_EQ ( 364, uShouldDate (31,12,-1,1,1,-1));
}
// -----------------------------------------------------------------------------
  TEST (uShouldDate, Negitive_Leap_Year) {
    EXPECT_EQ ( 365, uShouldDate (31,12,-8,1,1,-8));
}
// -----------------------------------------------------------------------------
  TEST (uShouldDate, Leap_Year) {
    EXPECT_EQ ( 365, uShouldDate (1,1,8,31,12,8));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, BC_TO_AD) {
    EXPECT_EQ ( 1, uShouldDate (31,12,-1,1,1,0));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, YEAR_ONE) {
    EXPECT_EQ ( 364, uShouldDate (1,1,1,31,12,1));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, EDGE_CASE) {
    EXPECT_EQ ( 104459355, uShouldDate (1,1,-10000,1,1,276000));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, N10K_YEARS) {          
    EXPECT_EQ ( 1095, uShouldDate (31,12,-4,31,12,-1));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, 10K_YEARS) {
    EXPECT_EQ (   1095, uShouldDate (1,1,1,1,1,4));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, 2_YEARS) {
    EXPECT_EQ ( 1096, uShouldDate (1,1,0,1,1,3));
}
//-----------------------------------------------------------------------------
  TEST (uShouldDate, N2_YEARS) {
    EXPECT_EQ ( 365, uShouldDate (1,1,-1,1,1,0));
}
//-----------------------------------------------------------------------------
TEST (uShouldDate, First_day) {
  EXPECT_EQ ( 1, uShouldDate (1,1,0,2,1,0));
}
//-----------------------------------------------------------------------------
TEST (uShouldDate, First_month) {
  EXPECT_EQ ( 31, uShouldDate (1,1,0,1,2,0));
}
//-----------------------------------------------------------------------------
TEST (uShouldDate, First_year) {
  EXPECT_EQ ( 365, uShouldDate (1,1,0,31,12,0));
}
//-----------------------------------------------------------------------------
TEST (uShouldDate, First_leap_year) {
  EXPECT_EQ ( 365, uShouldDate (1,1,400,31,12,400));
}
//-----------------------------------------------------------------------------
TEST (uShouldDate, FAILED) {
  EXPECT_EQ ( 14244 , uShouldDate (1,1,1,1,1,40));
}

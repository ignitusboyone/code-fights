// a=b     =a+b    = 0+1  0k+1
// aa=bb   =aa+bb  = 0+1  0k+1
// aaa=bbb =aaa+bbb= 0+1

// b=c     = b+b    =1+1  1k+1
// bb=dd   = bb+cc  =1+2  1k+1
// bbb=eee = bbb+ddd=1+3  1k+1
    
// c=d     = c+b    =2+1 c^0  2k+1   1+(2*(count-1)
// cc=ff   = cc+dd  =2+3 c^1  2k+1
// ccc=hhh = ccc+eee=2+5 c^2  2k+1 

// d =e    3k+1
// dd=hh   3k+1
// dd=kkk  3k+1
// f=g     = f+b = 5+1  5k+1
// ff==ll  =ff+gg= 5+6  5k+1
// fff=mmm =fff+ll=5+ll 5k+1
// 
#include <string>
 
std::string o;
std::string tranformLetters(std::string s) {
    o = s;  
    auto& f = tolower;
    for(auto& c:o)
    {          
        int a=0;
        for(auto p:s)a+=f(p)==f(c)?1:0; 
        c=(c%32-1+(a-1)*(f(c)-97)%26+1)%26 + c/32*32+1;
    }
    return o;
}

// LONG FORM
// auto tranformLetters(auto s) {
//     auto copy = s;  
//     auto& func = tolower;
//     for(auto& c:copy)
//     {          
//         int adj=0;
//         for(char p:s) 
//             adj+=(func(p)==func(c))?1:0;
        
//         adj =  (adj-1)*(func(c)-97)%26+1;   
//         if( c<='Z')
//             c= (c - 65 + adj) % 26 + 65;
//         else {
//             c= (c - 97 + aadj) % 26 + 97;
//         } 
//     }
//     return copy;
// }
#include "gtest/gtest.h"
//-----------------------------------------------------------------------------
TEST (TransformLetters, ahsk) {
    EXPECT_EQ ( "bitl", tranformLetters ("ahsk"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, cfebnb) {
    EXPECT_EQ ( "dgfdod", tranformLetters ("cfebnb"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, aDwar) {
    EXPECT_EQ ( "bExbs", tranformLetters ("aDwar"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, faKzaPaf) {
    EXPECT_EQ ( "lbLabQbl", tranformLetters ("faKzaPaf"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, ccc) {
    EXPECT_EQ ( "hhh", tranformLetters ("ccc"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, SrSrSrSt) {
    EXPECT_EQ ( "VaVaVaVu", tranformLetters ("SrSrSrSt"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, bcCdddeeee) {
    EXPECT_EQ ( "cfFkkkrrrr", tranformLetters ("bcCdddeeee"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, a) {
    EXPECT_EQ ( "b", tranformLetters ("a"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, aA) {
    EXPECT_EQ ( "bB", tranformLetters ("aA"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, c) {
    EXPECT_EQ ( "d", tranformLetters ("c"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, Cc) {
    EXPECT_EQ ( "Ff", tranformLetters ("Cc"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, Ccc) {
    EXPECT_EQ ( "Hhh", tranformLetters ("Ccc"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, d) {
    EXPECT_EQ ( "e", tranformLetters ("d"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, dd) {
    EXPECT_EQ ( "hh", tranformLetters ("dd"));
}
//-----------------------------------------------------------------------------
TEST (tranformLetters, ddd) {
    EXPECT_EQ ( "kkk", tranformLetters ("ddd"));
}
//-----------------------------------------------------------------------------
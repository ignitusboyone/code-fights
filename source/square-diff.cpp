#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

int MoreTimesLess(std::string in) {
   int ans = 0;
   uint64_t n = std::atoi(in.c_str());
   uint64_t n_half = std::ceil(n/2.0);
   uint64_t i_sq, j_sq;
   uint64_t i_lower= std::max(1.0,std::sqrt(n));
   uint64_t j_lower;
#if _DEBUG > 2
    printf("for ( int64_t i=%llu i<=%llu; i++)\n",i_lower,n_half);
#endif
   for ( uint64_t i=i_lower; i<=n_half; i++)
   {
       i_sq = std::pow(i,2) ;
       j_lower = std::max(1.0,std::sqrt(i_sq - n));
#if _DEBUG > 2
       printf("\tfor ( int64_t j=%llu; j<%llu; j++)\n",j_lower,i);
#endif
           j_sq = std::pow(j_lower,2);
           if ( n == i_sq - j_sq)
           {
               ++ans;
#if _DEBUG > 1
                 printf("%llu^2 - %llu^2\n",i,j_lower);
#endif
           }
   }
    return ans;
}
int main( int argc , char* argv[]){
  if (argc > 0)
  {
    std::cout << MoreTimesLess(std::string(argv[1])) << std::endl;
    return 0;
  } else {
    return -1;
  }
}

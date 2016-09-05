#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include <algorithm>
#include <numeric>


//-----------------------------------------------------------------------------
struct Glass{ 
    int64_t volume;
    int64_t capacity;
};
using Glasses = std::vector<Glass>;

std::deque<Glasses> work_queue;
std::map<decltype(Glass::volume),size_t> volumes;
std::map<decltype(Glass::volume),size_t> states;
//-----------------------------------------------------------------------------
Glass CreateGlass(int cap)
{ return {cap,cap}; }

//-----------------------------------------------------------------------------
inline Glasses empty_glass(const Glasses& state, size_t index)
{ 
  if ( state.size() < 3){
    return {};
  }
  if(state[index].volume != 0){
    Glasses new_state = state;
    new_state[index].volume = 0;
    if (new_state[0].volume | new_state[1].volume | new_state[2].volume) {
      return new_state;  
    } else {
      return {};
    }
  } 
  return {};
}

//-----------------------------------------------------------------------------
inline Glasses glass_transfer(const Glasses& state, size_t from, size_t to){
    if ( state.size() < 3){
      return {};
    }
    if ( state[from].volume != 0 && state[to].capacity != state[to].volume ){
        Glasses new_state = state;

        int64_t vaccent = state[to].capacity - state[to].volume;
        int64_t avaliable = state[from].volume;
        if ( vaccent >= avaliable){
          new_state[from].volume = 0;
          new_state[to].volume+= avaliable;
        } else {
          avaliable -= vaccent;
          new_state[from].volume -= vaccent;
          new_state[to].volume   += vaccent;
        }
        return new_state;
    }
    return {};
}
//-----------------------------------------------------------------------------
inline size_t glass_key(const Glasses& state){
  if(state.size() == 3)
  {
    return state[0].volume << 32 | state[1].volume << 16 | state[2].volume;
  } else { 
    return 0;
  }
}
//-----------------------------------------------------------------------------
int threeGlasses(std::vector<int64_t> cap) {
    volumes.clear();
    work_queue.clear();
    states.clear();
    int64_t ans = 1;
    std::sort(cap.begin(),cap.end());
    if( cap.front() == 1 ){
      ans = std::accumulate(cap.begin(), cap.end(),0);
    } else {
      volumes[std::accumulate(cap.begin(), cap.end(),0)]++;

      work_queue.push_back({ {    0 ,cap[0]}, {cap[1],cap[1]}, {cap[2],cap[2]} });
      work_queue.push_back({ {cap[0],cap[0]}, {    0 ,cap[1]}, {cap[2],cap[2]} });
      work_queue.push_back({ {cap[0],cap[0]}, {cap[1],cap[1]}, {    0 ,cap[2]} });
      
      states[glass_key({ {cap[0],cap[0]}, {cap[1],cap[1]}, {cap[2],cap[2]} })]++;

      Glasses new_state;
      while ( work_queue.begin() != work_queue.end())
      {
        Glasses& current = work_queue.front();
        if ( current.empty() )
        { 
           work_queue.pop_front();
           continue;  
        }
        
        volumes[std::accumulate(current.begin(), current.end(),0, 
          [](int64_t sum,const Glass& g){return sum + g.volume;})  ]++;
        
        Glasses new_state     = glass_transfer(current,0,1);
        if( !states[glass_key(new_state)]++ )
        {
          work_queue.push_back( new_state );
          work_queue.push_back( empty_glass(new_state,0) );
          work_queue.push_back( empty_glass(new_state,1) );
          work_queue.push_back( empty_glass(new_state,2) );
        }
        new_state   = glass_transfer(current,0,2);
        if( !states[glass_key(new_state)]++ )
        {
          work_queue.push_back( new_state );
          work_queue.push_back( empty_glass(new_state,0) );
          work_queue.push_back( empty_glass(new_state,1) );
          work_queue.push_back( empty_glass(new_state,2) );
        }
        new_state     = glass_transfer(current,1,0);
        if( !states[glass_key(new_state)]++ )
        {
          work_queue.push_back( new_state );
          work_queue.push_back( empty_glass(new_state,0) );
          work_queue.push_back( empty_glass(new_state,1) );
          work_queue.push_back( empty_glass(new_state,2) );
        }
        new_state   = glass_transfer(current,1,2);
        if( !states[glass_key(new_state)]++ )
        {
          work_queue.push_back( new_state );
          work_queue.push_back( empty_glass(new_state,0) );
          work_queue.push_back( empty_glass(new_state,1) );
          work_queue.push_back( empty_glass(new_state,2) );
        }
        new_state   = glass_transfer(current,2,0);
        if( !states[glass_key(new_state)]++ )
        {
          work_queue.push_back( new_state );
          work_queue.push_back( empty_glass(new_state,0) );
          work_queue.push_back( empty_glass(new_state,1) );
          work_queue.push_back( empty_glass(new_state,2) );
        }
        new_state   = glass_transfer(current,2,1);
        if( !states[glass_key(new_state)]++ )
        {
          work_queue.push_back( new_state );
          work_queue.push_back( empty_glass(new_state,0) );
          work_queue.push_back( empty_glass(new_state,1) );
          work_queue.push_back( empty_glass(new_state,2) );
        }
        
        work_queue.pop_front();
      }
      ans = volumes.size();
      // for (auto& ans: volumes){
      //   std::cout << "( "<< ans.first << ", " << ans.second << " )" << "\n";
      // }
    }
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
// -----------------------------------------------------------------------------
TEST (Glasses, 300_1123_389) {
    EXPECT_EQ (200, threeGlasses ({ 98,99,100}));
}
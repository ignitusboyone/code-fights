#include <vector>
#include <deque>
#include <string>
#include <iostream>

#include <cstdlib>
#include <cstdint>
#ifdef _DEBUG
  #undef _DEBUG
  #define _DEBUG 0
#endif
// enum Direction : int8_t {UP,DOWN,LEFT,RIGHT, NEUTRAL};
#define UP 'U'
#define DOWN 'D'
#define LEFT 'L'
#define RIGHT 'R'
//-----------------------------------------------------------------------------
struct Position 
{
  int _X = -1;
  int _Y = -1;
  bool operator==(const Position& rhs) const{
    return _X == rhs._X && _Y == rhs._Y;
  }
  bool operator!=(const Position& rhs){
    return !(*this == rhs);
  }
  Position operator-(const Position& rhs) const{
    Position ans = {_X-rhs._X, _Y-rhs._Y};
    return ans;
  }
};
//-----------------------------------------------------------------------------
// #if _DEBUG
std::ostream& operator<<(std::ostream& os, const Position& pose){
  os << "(" << pose._X << " , " << pose._Y<< ")";
  return os;
}
// #endif
//-----------------------------------------------------------------------------
class Solution : public std::string {
public:
  Solution(std::string str)
    :std::string(str)
  {}
  
  Solution(char str)
    :std::string({str})
  {} 
  Solution(const Solution& obj)
  :std::string(obj)
    ,tile(obj.tile)
  {}  
  
  Position tile = {-1,-1};
};

//-----------------------------------------------------------------------------
class Laybrinth 
{
public:
  Laybrinth(std::vector<std::string> map, 
             Position initial, 
             Position finish)
  :_labyrinth(map)
  ,_start(initial)
  ,_finish(finish)
  {}
  
  inline Position Start() const { return _start;};
  inline Position Finish() const { return _finish;};
  //---------------------------------------------
  bool simulate(Solution& work_queue,
                  size_t& cycles)
  {
    cycles = 0;
    bool done = false;
    Position current = _start;
    std::vector<Position> history{_start};
    
    Position& tile = work_queue.tile;
    if(tile != Position() )
    {  
      switch (work_queue.back()){
          case UP:    tile._Y-=1;  break;
          case DOWN:  tile._Y+=1;  break;
          case LEFT:  tile._X-=1;  break;
          case RIGHT: tile._X+=1;  break;
          default:          break;
      }

      if(tile._X == _finish._X && tile._Y == _finish._Y){ 
        return true;
      } else if ( _labyrinth[tile._Y][tile._X] != '.' ) { 
        return false;
      } else {
        current = tile;
        ++cycles;
        work_queue.tile = current;

        //Reverse Cycle Check
        history.clear();
        // history.push_back(Position());
        Position temp;
        for(auto itr = (work_queue.end()-1);
            work_queue.size() > 16 && itr != work_queue.end()-(15);
            --itr)
        {
          switch (*itr){
              case UP:    temp._Y-=1;  break;
              case DOWN:  temp._Y+=1;  break;
              case LEFT:  temp._X-=1;  break;
              case RIGHT: temp._X+=1;  break;
              default:          break;
          }
          if ( find (history.begin(), history.end(), temp) != history.end()
                              &&  itr != work_queue.begin()
                              &&  itr != work_queue.end()-1
                          )
          {cycles = 0; return false;}
          history.push_back(temp);
        }
        history.clear();
        history.push_back(current);

      }
    }
#if _DEBUG > 5
    std::cout << work_queue << "\n";
#endif
    auto itr = work_queue.begin();
    while(!done) {
      
      for ( ; 
            itr != work_queue.end(); 
            ++itr)
      {
          
          
        switch (*itr){
            case UP:    current._Y-=1;  break;
            case DOWN:  current._Y+=1;  break;
            case LEFT:  current._X-=1;  break;
            case RIGHT: current._X+=1;  break;
            default:          break;
        }

        if(current._X == _finish._X && current._Y == _finish._Y){ 
          return true;
        } else if ( _labyrinth[current._Y][current._X] != '.' ) { 
          return false;
        } 
        else if ( work_queue.size()>3&&find (history.begin(), history.end(), current) != history.end()
                    &&  itr != work_queue.begin()
                    &&  itr != work_queue.end()-1
                )
        {

          cycles = 0; return false;
        }
        history.push_back(current);
      }
      if(!cycles++){
        work_queue.tile = current;
      }
      history.resize(0);
      history.push_back(current);
      if( current._X == _start._X && current._Y == _start._Y){
        break;
      }
      itr = work_queue.begin();
    }
    return false;
  }
private:
    const std::vector<std::string> _labyrinth;
    const Position _start;
    const Position _finish;
};
//-----------------------------------------------------------------------------
int labyrinthNavigation(std::vector<std::string> map, 
                        std::vector<int> start, 
                        std::vector<int> dest) 
{
  decltype(map)  padded_map(map.size()+2,
                            std::string(map[0].size()+2,'#'));
  for (auto i=0; i<map.size(); ++i)
    padded_map[i+1].replace(1,map[i].size(),map[i]);
  Position adj_start = {start[1]+1,start[0]+1};
  Position adj_dest  = {dest[1]+1,dest[0]+1};
  Laybrinth laybrinth(padded_map,adj_start, adj_dest); 

  #if _DEBUG > 1
    std::cout << "Start( "<< start[1]+1<< " , " << start[0]+1 <<" )\n";
    std::cout << "Finish( "<< dest[1]+1<< " , " <<  dest[0]+1 <<" )\n";

    padded_map[(start[0]+1)][(start[1]+1)] = 'S';
    padded_map[ (dest[0]+1)][ (dest[1]+1)] = 'F';
  
    for(auto s:padded_map)
      std::cout << s << "\n";
    std::cout << "\n";
  #endif
    
  std::deque<Solution> work_queue;
  work_queue.push_back(Solution(UP));
  work_queue.push_back(Solution(DOWN));
  work_queue.push_back(Solution(LEFT));
  work_queue.push_back(Solution(RIGHT));
  
  while (work_queue.size()){
    auto cmd_queue = work_queue.front();
    size_t cycles = 0;
    if( laybrinth.simulate(cmd_queue,cycles )){
#ifdef _DEBUG 
      for( auto c: cmd_queue)
        std::cout << c;
      std::cout << "\n";
#endif
       return cmd_queue.size();
    } else {
      if (cycles ){
        if( cmd_queue.size()<6 )
        {
           cmd_queue+="U";
           work_queue.push_back(cmd_queue);
           cmd_queue.pop_back();
           
           cmd_queue+='D';
           work_queue.push_back(cmd_queue);
           cmd_queue.pop_back();
           
           cmd_queue+='L';
           work_queue.push_back(cmd_queue);
           cmd_queue.pop_back();
           
           cmd_queue+='R';
           work_queue.push_back(cmd_queue);
           cmd_queue.pop_back();
        } else  {
          if (cmd_queue.back() != 'D'){
            cmd_queue+="U";
            work_queue.push_back(cmd_queue);
            cmd_queue.pop_back();
          }
          
          if (cmd_queue.back() != 'U'){
            cmd_queue+="D";
            work_queue.push_back(cmd_queue);
            cmd_queue.pop_back();
          }
          
          if (cmd_queue.back() != 'R'){
            cmd_queue+="L";
            work_queue.push_back(cmd_queue);
            cmd_queue.pop_back();
          }
          
          if (cmd_queue.back() != 'L'){
            cmd_queue+="R";
            work_queue.push_back(cmd_queue);
            cmd_queue.pop_back();
          }
        }
      }
    }
    work_queue.pop_front();
  }
  return -1;
}

#include <gtest/gtest.h>
//-----------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_1) {
    EXPECT_EQ ( 3, labyrinthNavigation (
      {".......",
       "...*.*.",
       "*....*.",
       ".......",
       ".**...."}
       ,{4,6}
       ,{0,0}
));
};

//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_2) {
    EXPECT_EQ ( 4, labyrinthNavigation (
      {"..",
       ".*",
       "..",
       ".*",
       "..",
       ".*",
       "..",
       ".*",
       ".."}
     ,{8,1}
     ,{0,1}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_3) {
    EXPECT_EQ ( 5, labyrinthNavigation (
      {"...**...*",
       "....*....",
       ".......*.",
       ".*.*...*.",
       ".***.*...",
       ".....*...",
       ".***.*.*.",
       ".*.*.***.",
       "........."}
     ,{6,8}
     ,{2,0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_4) {
    EXPECT_EQ ( 6, labyrinthNavigation (
      {".*.",
       ".*.",
       "..."}
     ,{0,2}
     ,{0,0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_5) {
    EXPECT_EQ ( 1, labyrinthNavigation (
      {".................................................."}
      ,{0,0}
      ,{0,49}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_6) {
    EXPECT_EQ ( 7, labyrinthNavigation (
      {"...................",
       "***.*..*.*..*.*..*."}
     ,{1,18}
     ,{0,0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_7) {
    EXPECT_EQ ( 8, labyrinthNavigation (
      {"...",
       ".*.",
       ".**",
       "..."}
       ,{1,2}
       ,{3,2}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_8) {
    EXPECT_EQ ( 1, labyrinthNavigation (
      {"*",
       ".",
       ".",
       ".",
       ".",
       "*",
       ".",
       ".",
       "*",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       ".",
       "*",
       ".",
       ".",
       "*",
       "*",
       "*",
       "*"}
     ,{42,0}
     ,{16,0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_9) {
    EXPECT_EQ ( 5, labyrinthNavigation (
      {"......*",
       ".*.....",
       ".**...*",
       "......."}
     ,{0,0}
     ,{3,6}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_10) {
    EXPECT_EQ ( 6, labyrinthNavigation (
      {"...........................",
       "...........................",
       "...........................",
       "...........................",
       "...........................",
       "...........................",
       "..........................."}
     ,{5,24}
     ,{1, 0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_11) {
    EXPECT_EQ ( 8, labyrinthNavigation (
      {".........**....",
       ".*...*...*.**..",
       ".**............",
       "......*...*...."}
     ,{1,3}
     ,{3,14}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_12) {
    EXPECT_EQ ( 4, labyrinthNavigation (
      {"......*",
       ".*.....",
       ".**...*",
       "......."}
     ,{0,5}
     ,{3,6}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_13) {
    EXPECT_EQ ( 4, labyrinthNavigation (
      {"...........*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       ".***.*.*.*.*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       "..............."}
     ,{7,0}
     ,{6,14}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_14) {
    EXPECT_EQ ( 6, labyrinthNavigation (
      {"...........*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       ".*.*.*.*.*.*...",
       "...............",
       ".*.*.*.*.*.*...",
       ".*............."}
     ,{9,0}
     ,{9,14}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_15) {
    EXPECT_EQ ( 3, labyrinthNavigation (
      {"...............",
       ".*************.",
       ".*...........*.",
       ".*...........*.",
       ".*...........*.",
       ".*...........*.",
       ".*...........*.",
       ".*...........*.",
       ".*************.",
       "..............."}
     ,{2, 2}
     ,{6,11}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_16) {
    EXPECT_EQ ( 32, labyrinthNavigation (
      {"*************..",
       "...............",
       "..*************",
       "...............",
       "*************..",
       "..............",
       "..*************",
       "...............",
       "*************..",
       "..............."}
     ,{0,14}
     ,{9,0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_17) {
    EXPECT_EQ ( 3, labyrinthNavigation (
      {"...........................",
       "...........................",
       "...........................",
       "...........................",
       "...........................",
       "...........................",
       "..........................."}
     ,{0,0}
     ,{6,13}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_18) {
    EXPECT_EQ ( 52, labyrinthNavigation (
      {"************************..",
       "..........................",
       "..************************",
       "..........................",
       "************************..",
       ".........................",
       "..************************",
       "..........................",
       "************************..",
       ".........................."}
     ,{0,24}
     ,{9,0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_19) {
    EXPECT_EQ ( 192, labyrinthNavigation (
      {"**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................"}
     ,{0,47}
     ,{13,0}
));
};
//-------------------------------------------------------------------------------
TEST (labyrinthNavigation, Test_20_50_50) {
    EXPECT_EQ ( 98, labyrinthNavigation (
      {"**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************",
       "................................................",
       "**********************************************..",
       "................................................",
       "..**********************************************"}
     ,{0,47}
     ,{49,0}
));
};
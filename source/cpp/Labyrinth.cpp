#include <vector>
#include <deque>
#include <string>
#include <iostream>

#include <cstdlib>
#define _DEBUG 0

enum Direction : int8_t {UP,DOWN,LEFT,RIGHT, NEUTRAL};
//-----------------------------------------------------------------------------
#ifdef _DEBUG
std::ostream& operator<< (std::ostream& ostr, Direction obj)
{
  switch(obj){
    case UP:    ostr<<'U';  break;
    case DOWN:  ostr<<'D';  break;
    case LEFT:  ostr<<'L';  break;
    case RIGHT: ostr<<'R';  break;
    default:          break;
  }
  return ostr;
}
#endif
//-----------------------------------------------------------------------------
class Position 
{
public:
  Position(size_t x=0, size_t y=0)
    :_X(x)
    ,_Y(y)
  {}
  inline void setX(size_t x){ _X = x;}
  inline void setY(size_t y){ _Y = y;}
  inline size_t getX() const { return _X;}
  inline size_t getY() const { return _Y;}
  //---------------------------------------------
  bool operator==(const Position& rhs)
    { return _X == rhs._X && _Y == rhs._Y;}
  //---------------------------------------------
  Position operator+(Position& rhs)
    { 
      Position rvalue(_X+rhs._X, _Y+rhs._Y);
      return rvalue;
    }
  //---------------------------------------------
  Position operator-(Position& rhs)
    { 
      Position rvalue(_X-rhs._X, _Y-rhs._Y);
      return rvalue;
    }
  //---------------------------------------------
  Position& operator+=(Position& rhs)
    { 
      _X+=rhs._X;
      _Y+=rhs._Y;
      return *this;
    }
  //---------------------------------------------
  Position& operator-=(Position& rhs)
    { 
      _X-=rhs._X;
      _Y-=rhs._Y;
      return *this;
    }
  //---------------------------------------------
  void move(const Direction& rhs)
    { 
      switch (rhs){
        case UP:    _Y-=1;  break;
        case DOWN:  _Y+=1;  break;
        case LEFT:  _X-=1;  break;
        case RIGHT: _X+=1;  break;
        default:          break;
      }
    }
  //---------------------------------------------
  void unmove(const Direction& rhs)
    { 
      switch (rhs){
        case UP:    _Y+=1;  break;
        case DOWN:  _Y-=1;  break;
        case LEFT:  _X+=1;  break;
        case RIGHT: _X-=1;  break;
        default:          break;
      }
    }
private:
  size_t _X;
  size_t _Y;
};
//-----------------------------------------------------------------------------
#ifdef _DEBUG
std::ostream& operator<< (std::ostream& ostr, Position obj){
  ostr << "(" << obj.getX() <<" , "<< obj.getY() <<")";
  return ostr;
}
#endif
//-----------------------------------------------------------------------------
class Solution: public std::vector<Direction> 
{
public:
  Solution(std::vector<Direction> cmds, Position start)
    :std::vector<Direction>(cmds)
    ,_qsp(start)
  {}
  
    Position getQuickStart() const { 
      return _qsp ; 
    }
    void     setQuickStart(const Position& pose) { _qsp = pose; }
protected:
  Position _qsp;
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
  bool simulate(Solution work_queue,
                  size_t& cycles)
  {
    cycles = -1;
    bool done = false;
    Position current = work_queue.getQuickStart();;
    std::vector<Position> history{_start};

    auto itr = work_queue.begin();


    while(!done) {
      
      for ( ; 
            itr != work_queue.end(); 
            ++itr)
      {
        current.move(*itr);
        if(current == _finish){ 
          return true;
        } else if ( _labyrinth[current.getY()][current.getX()] != '.' ) { 
          done = true; break;
        } 
        else if ( work_queue.size()>3&&find (history.begin(), history.end(), current) != history.end()
                    &&  itr != work_queue.begin()
                    &&  itr != work_queue.end()-1
                )
        {
#if _DEBUG > 4
          std::cout << work_queue.size() << "##";
          for ( auto cmd : work_queue )
            std::cout << cmd << ",";
          std::cout << "\n";
#endif
          cycles = 0; return false;
        }
        history.push_back(current);
      }
      ++cycles;
      history.clear();
      history.push_back(current);
      if( current == _start){
        break;
      }
      itr = work_queue.begin();
    }
    work_queue.setQuickStart(current);
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
                            std::string(map[1].size()+2,'#'));
  for (auto i=0; i<map.size(); ++i)
    padded_map[i+1].replace(1,map[i].size(),map[i]);
 
  Laybrinth laybrinth(padded_map,Position(start[1]+1,start[0]+1),Position(dest[1]+1,dest[0]+1)); 

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
  work_queue.push_back(Solution({UP},   laybrinth.Start()));
  work_queue.push_back(Solution({DOWN}, laybrinth.Start()));
  work_queue.push_back(Solution({LEFT}, laybrinth.Start()));
  work_queue.push_back(Solution({RIGHT},laybrinth.Start()));
  
  size_t max_queue_size = 0;
  while (work_queue.size()){
    auto cmd_queue = work_queue.front();
    size_t cycles = 0;
    if( laybrinth.simulate(cmd_queue,cycles )){
#if _DEBUG
      for ( auto cmd : cmd_queue )
        std::cout << cmd << ",";
      std::cout << "\n";
#endif
       return cmd_queue.size();
    } else {
      if (cycles ){
#if _DEBUG > 2
        std::cout << work_queue.size() << ": ";
        for ( auto cmd : cmd_queue )
          std::cout << cmd << ",";
        std::cout << "\n";
#endif        
        if(max_queue_size < work_queue.size())
        {
          max_queue_size = work_queue.size();
        }
        cmd_queue.push_back(UP);
        work_queue.push_back(cmd_queue);
        cmd_queue.pop_back();
        
        cmd_queue.push_back(DOWN);
        work_queue.push_back(cmd_queue);
        cmd_queue.pop_back();
          
        cmd_queue.push_back(LEFT);
        work_queue.push_back(cmd_queue);
        cmd_queue.pop_back();
          
        cmd_queue.push_back(RIGHT);
        work_queue.push_back(cmd_queue);
        cmd_queue.pop_back();
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
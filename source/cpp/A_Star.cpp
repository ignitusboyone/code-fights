/*
*  \author Steven A White
*  \date   Sep 11th 2016
*/

#include <iostream>
#include <iomanip>

#include<cstdint>
#include<cmath>

#include<algorithm>
#include<deque>
#include<vector>
#include<list>
#include<limits>
#include<tuple>
//-----------------------------------------------------------------------------
class Position 
{
  using Coordinate = std::pair<int, int>;
public:
  enum Style {COORDS,ASCII};
  enum Tile:char { WALL='#', OBSTACLE='*', PATH='.', VERT='-', HORIZ='|' };
  Position()
    :_from(0,0)
    ,_coord(0,0)
  {}
  Position(int x, int y)
    :_from(x,y)
    ,_coord(x,y)
  {}
  //-----------------------------------------------------------
  Position(      Position&& obj)
    :_g_score(obj._g_score)
    ,_f_score(obj._f_score)
    ,_h_score(obj._h_score)
    ,_from (std::move(obj._from))
    ,_coord(std::move(obj._coord))
    ,_value(std::move(obj._value))
  {}
  Position(const Position& obj)
    :_g_score(obj._g_score)
    ,_f_score(obj._f_score)
    ,_h_score(obj._h_score)
    ,_from (obj._from)
    ,_coord(obj._coord)
    ,_value(std::move(obj._value))
  {}
  //-----------------------------------------------------------
  inline int& X() { return _coord.first;}
  inline int& Y() { return _coord.second;}
  inline char& Value() { return _value;}
  //-----------------------------------------------------------
  inline const int& X() const { return _coord.first;}
  inline const int& Y() const { return _coord.second;}
  inline const char& Value() const { return _value;}
  //-----------------------------------------------------------
  inline Position& X(int x) { _coord.first=x; return *this;}
  inline Position& Y(int y) { _coord.second=y; return *this;}
  inline Position& Value(char c) { _value=c; return *this;}
  //-----------------------------------------------------------
  bool operator==(const Position& rhs) const{
    return this->X() == rhs.X() && Y() == rhs.Y();
  }
  bool operator!=(const Position& rhs) const{
    return !(*this == rhs);
  }
  bool operator<(const Position& rhs) const{
    return _h_score < rhs._h_score;
  }
  bool operator>(const Position& rhs) const{
    return rhs < *this;
  }
  bool operator<=(const Position& rhs) const{
    return _h_score <= rhs._h_score;
  }
  bool operator>=(const Position& rhs) const{
    return rhs  <= *this;
  }
  Position operator-(const Position& rhs) const{
    Position ans = { X()-rhs.X(), Y()-rhs.X()};
    return ans;
  }
  Position operator+(const Position& rhs) const{
    Position ans = { X()+rhs.X(), Y()+rhs.Y()};
    return ans;
  }
  //-----------------------------------------------------------
  Position& operator=(const Position& obj)
  {
    if( this != &obj)
    {
      _g_score = (obj._g_score);
      _f_score = (obj._f_score);
      _h_score = (obj._h_score);
      _from  = (obj._from);
      _coord = (obj._coord);
      _value = (obj._value);
    }
    return *this;
  }
  //-----------------------------------------------------------
  Position& operator=(   Position&& obj)
  {
    if( this != &obj)
    {
      _g_score = (obj._g_score);
      _f_score = (obj._f_score);
      _h_score = (obj._h_score);
      _from  = (std::move(obj._from));
      _coord = (std::move(obj._coord));
      _value = (std::move(obj._value));
    }
  return *this;
  }
  //-----------------------------------------------------------
  inline int& GScore() { return _g_score;}
  inline int& FScore() { return _f_score;}
  inline int& HScore() { return _h_score;}
  //-----------------------------------------------------------
  inline const int& GScore() const { return _g_score;}
  inline const int& FScore() const { return _f_score;}
  inline const int& HScore() const { return _h_score;}
  //-----------------------------------------------------------
  inline void GScore(int score) { _g_score = score;}
  inline void FScore(int score) { _f_score = score;}
  inline void HScore(int score) { _h_score = score;}
  //-----------------------------------------------------------
  inline Coordinate& From() { return _from;}
  inline const Coordinate& From() const { return _from;}
  inline void From(int x, int y)   { _from = {x,y};}
  //-----------------------------------------------------------
  // inline Coordinate& Coord() { return _from;}
  inline const Coordinate& Coord() const { return _from;}
  // inline void Coord(int x, int y)   { _from = {x,y};}
  //-----------------------------------------------------------
  void move(int x, int y)
  {
    _coord.first+=x;
    _coord.second+=y;
  }
  //-----------------------------------------------------------
  void unmove(int x, int y)
  {
    _coord.first-=x;
    _coord.second-=y;
  }
  //-----------------------------------------------------------
  inline int cityDistance(Position p2) const {
    return p2.X()-X() + p2.Y()-X();
  }
  //-----------------------------------------------------------
  inline float euclidianDistance(Position p2) const {
    return std::sqrt( p2.X()-std::pow(X(),2) + p2.Y()-std::pow(Y(),2));
  }
  //-----------------------------------------------------------
  float dist(Position p2) const {
    return cityDistance(p2);
  }
  //-----------------------------------------------------------
  float absDist(Position p2) const {
    return std::abs(dist(p2));
  }
  //-----------------------------------------------------------
  int heuristicCostEstimate(const Position& p2) const {
    int cost = 0;
    cost = absDist(p2);
    return cost;
  }
private:
  int _g_score = 0;
  int _f_score = 0;
  int _h_score = 0;
  
  char _value = '#';
  
  Coordinate _from;
  Coordinate _coord;
};
//-----------------------------------------------------------------------------
#ifndef CODE_FIGHTS 
std::ostream& operator<<(std::ostream& os, const Position& pos){
  os << "(" << pos.X() << ", " << pos.Y()<< ")";
  return os;
}
#endif
//-----------------------------------------------------------------------------
class Laybrinth
{
using Coordinate = std::pair<int, int>;  
using NodeMap = std::vector<std::vector<Position>>;
using NodeRow = std::vector<Position>;
public:
  Laybrinth() = default;
  //-----------------------------------------------------------
  Laybrinth(size_t w, size_t l)
    :_laybrinth(w,std::vector<Position>(l))
  { 
    for( size_t i = 0; i < _laybrinth.size(); ++i)
    {
      for( size_t j = 0; j < _laybrinth[0].size(); ++j)
      {
        _laybrinth[i][j] = Position(i,j);
      }
    } 
  }
  //-----------------------------------------------------------
  Laybrinth(std::vector<std::string> map)
    :_laybrinth(map[0].size(),std::vector<Position>(map.size()))
  {
    for( size_t i = 0; i < _laybrinth.size(); ++i)
    {
      for( size_t j = 0; j < _laybrinth[0].size(); ++j)
      {
        _laybrinth[i][j] = Position(i,j).Value(map[j][i]);
      }
    } 
  }
  //-----------------------------------------------------------
  Laybrinth(const Laybrinth& obj)
    :_laybrinth(obj._laybrinth)
    ,_start(obj._start)
    ,_finish(obj._finish)
    ,_style(obj._style)
  {}
  Laybrinth(Laybrinth&& obj)
    { *this = std::move(obj);}
  //-----------------------------------------------------------
  inline int Width() const  { return _laybrinth.size();}
  inline int Height() const { return _laybrinth[0].size();}
  //-----------------------------------------------------------
  inline Position& Start()  { return _start;}
  inline Position& Finish() { return _finish;}
  inline Position::Style OutputStyle() { return _style;}
  //-----------------------------------------------------------
  inline const Position& Start()  const { return _start; }
  inline const Position& Finish() const { return _finish;}
  inline const Position::Style OutputStyle() const { return _style;}
  //-----------------------------------------------------------
  inline  Laybrinth& Start (int x,int y)  { _start = {x,y}; return *this;}
  inline  Laybrinth& Finish(int x,int y)  { _finish = {x,y};return *this;}
  inline  Laybrinth& Start (Position p)   { _start = p;     return *this;}
  inline  Laybrinth& Finish(Position p)   { _finish = p;    return *this;}
  inline  Laybrinth& OutputStyle(Position::Style s) {  _style = s;return *this;}
  //-----------------------------------------------------------
  Position& operator()(int x, int y){ return _laybrinth[x][y];}
  Position& operator()(const Coordinate p){ return _laybrinth[p.first][p.second];}
  Position& operator()(const Position& pos){ return _laybrinth[pos.X()][pos.Y()];}
  //--------------------------------------------------¬230---------
  const Position& operator() (int x, int y)        const { return _laybrinth[x][y];}
  const Position& operator() (const Coordinate p)  const { return _laybrinth[p.first][p.second];}
  const Position& operator() (const Position& pos) const { return _laybrinth[pos.X()][pos.Y()];}
  //-----------------------------------------------------------
  Laybrinth& operator=(const Laybrinth& obj)
  {
    if( this != &obj)
    {
      _laybrinth = (obj._laybrinth);
      _start = (obj._start);
      _finish = (obj._finish);
      _style  = (obj._style);
    }
    return *this;
  }
  Laybrinth& operator=(Laybrinth&& obj)
  {
    if( this != &obj)
    {
      _laybrinth = std::move(obj._laybrinth);
      _start = (obj._start);
      _finish = (obj._finish);
      _style  = (obj._style);
    }
    return *this;
  }
  //-----------------------------------------------------------
  std::deque<Position> reconstructPath(const Position& goal) const
  {
      std::deque<Position> path = {};
      Position current = goal;
      while (current != _start)
      {
          current = operator()(current.From());
          path.push_front(current);
        }
      return path;
  }
  //-----------------------------------------------------------
  std::string reconstructPathAscii(const Position& goal) const
  {
      std::string path = "";
      Position current = goal;
      Position prev;
      while (current != _start)
      {
          prev = current;
          current = operator()(current.From());
          if( prev.X() - current.X() == 1)
            path+='R';
          else if( prev.X() - current.X() == -1)
            path+='L';
          else if( prev.Y() - current.Y() ==  1)
            path+='D';
          else if( prev.Y() - current.Y() == -1)
            path+='U';
        }
      std::reverse(path.begin(),path.end());
      return path;
  }
  //-----------------------------------------------------------
  bool isValid(Position node) const {
    bool ans = 0 <= node.X() && node.X() < _laybrinth.size();
    ans&= 0 <= node.Y() && node.Y() < _laybrinth[0].size();
    
    return (ans)? _laybrinth[node.X()][node.Y()].Value() == Position::Tile::PATH
                : false;
  }
private:
   NodeMap _laybrinth = {};
   Position _start  = {0,0};
   Position _finish = {0,0};
   
   Position::Style _style = Position::ASCII;

};
//-----------------------------------------------------------------------------
#ifndef CODE_FIGHTS 
std::ostream& operator<<(std::ostream& os, Laybrinth layb)
{
  if(layb.OutputStyle() == Position::COORDS)
    for ( int i = 0; i < layb.Width(); ++i)
    {
      for ( int j = 0; j < layb.Height(); ++j)
      {
         os << layb(i,j) <<", "; 
      } os << "\n";
    }
  else
    os << "     ";
    for ( int j = 0; j < layb.Width(); ++j)
    {
      os << std::setw(2) << std::left;
      os << j;
    }
    os << "\n";
    for ( int i = 0; i < layb.Height(); ++i)
    { 
      os << std::setw(4);
      os << i << ": ";
      for ( int j = 0; j < layb.Width(); ++j)
      {   
        os << std::setw(2);
        if( layb(j,i) != layb.Start() && layb(j,i) != layb.Finish())
          os << layb(j,i).Value(); 
        else if(layb(j,i) == layb.Start())
          os << 'S'; 
        else
          os << 'G'; 
      } os << "\n";
    }
  return os;
}
#endif
//-----------------------------------------------------------------------------
class Agent {
  const Position Down = { 0, 1};
  const Position Up   = { 0,-1};
  const Position Left = {-1, 0};
  const Position Right= { 1, 0};
public:
  Agent(Laybrinth layb)
    :_laybrinth(layb)
    ,_start(0,0)
    ,_goal(_laybrinth.Width()-1,_laybrinth.Height()-1)
  {}
  //-----------------------------------------------------------
  Agent(Laybrinth layb,Position s, Position g)
    :_laybrinth(layb)
    ,_start(s)
    ,_goal(g)
  {}
  //-----------------------------------------------------------  
  Agent(const Agent& obj)
    :_laybrinth(obj._laybrinth)
    ,_start(obj._start)
    ,_goal(obj._goal)
  {}
  Agent( Agent&& obj)
    { *this = std::move(obj); }
  //-----------------------------------------------------------  
  Agent& operator=(const Agent& obj){
    if(this != &obj)
    {
      _laybrinth=obj._laybrinth;
      _start    =obj._start;
      _goal     =obj._goal;
    }
    return *this;
  }
  Agent& operator=( Agent&& obj){
    if(this != &obj)
    {
      _laybrinth=std::move(obj._laybrinth);
      _start    =std::move(obj._start);
      _goal     =std::move(obj._goal);
    }
    return *this;
  }
  //-----------------------------------------------------------  
  std::deque<Position> A_Star() 
  {
    std::list<Position> closedSet;
    std::list<Position> openedSet = {_start};

    
    _laybrinth(_start).heuristicCostEstimate(_goal);
    while(openedSet.size()){
      Position current = *std::min_element(std::begin(openedSet), std::end(openedSet));;
      if (current == _goal)
          return _laybrinth.reconstructPath(_goal);
      openedSet.remove(current);
      closedSet.push_back(current);
      for( auto n : {Down,Left,Right,Up} ){
        Position neighbor = current+n;
        if(!_laybrinth.isValid(neighbor))
          continue;
        if(std::find(std::begin(closedSet),std::end(closedSet),neighbor) != std::end(closedSet))
          continue;
        int tentative_gScore = _laybrinth(current).GScore() + current.absDist(neighbor);
        if(std::find(std::begin(openedSet),std::end(openedSet),neighbor) == std::end(openedSet))
          openedSet.push_back(neighbor);
        else if ( tentative_gScore >= _laybrinth(current).GScore())
            continue;
        _laybrinth(neighbor).From()   = current.Coord();
        _laybrinth(neighbor).GScore() = tentative_gScore;
        _laybrinth(neighbor).FScore() = neighbor.GScore() + _laybrinth(neighbor).heuristicCostEstimate(_goal);
        }
    }
    return {};
  }
  std::string A_StarAscii() 
  {
    std::list<Position> closedSet;
    std::list<Position> openedSet = {_start};

    
    _laybrinth(_start).heuristicCostEstimate(_goal);
    while(openedSet.size()){
      Position current = *std::min_element(std::begin(openedSet), std::end(openedSet));;
      if (current == _goal)
          return _laybrinth.reconstructPathAscii(_goal);
      openedSet.remove(current);
      closedSet.push_back(current);
      for( auto n : {Down,Left,Right,Up} ){
        Position neighbor = current+n;
        if(!_laybrinth.isValid(neighbor))
          continue;
        if(std::find(std::begin(closedSet),std::end(closedSet),neighbor) != std::end(closedSet))
          continue;
        int tentative_gScore = _laybrinth(current).GScore() + current.absDist(neighbor);
        if(std::find(std::begin(openedSet),std::end(openedSet),neighbor) == std::end(openedSet))
          openedSet.push_back(neighbor);
        else if ( tentative_gScore >= _laybrinth(current).GScore())
            continue;
        _laybrinth(neighbor).From()   = current.Coord();
        _laybrinth(neighbor).GScore() = tentative_gScore;
        _laybrinth(neighbor).FScore() = neighbor.GScore() + _laybrinth(neighbor).heuristicCostEstimate(_goal);
        }
    }
    return {};
  }
  //-----------------------------------------------------------
  std::deque<Position> A_Star( Position s, Position g)  
  {
    _start = s;
    _goal = g;
    return A_Star();
  } 
  std::string A_StarAscii( Position s, Position g)  
  {
    _start = s;
    _goal = g;
    return A_StarAscii();
  }      
  //-----------------------------------------------------------
  std::deque<Position> reconstructPath(){ return _laybrinth.reconstructPath(_goal); }
  std::string reconstructPathAscii(){ return _laybrinth.reconstructPathAscii(_goal); }
private:
  Laybrinth _laybrinth;
  Position  _start;
  Position  _goal;
};
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]){
  
  
  
  
  
  Laybrinth maze(
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
       "..**********************************************"});
  maze.OutputStyle(Position::ASCII);
  maze.Start(47,0);
  maze.Finish(0,55);;
  std::cout << "Search for S" << maze.Start() << " F" << maze.Finish() <<"\n";
  std::cout << "In:\n";

  
  Agent bob = Agent(maze,{47,0},{0,55});
  auto path = bob.A_StarAscii({47,0},{0,55});
  std::cout << "Found:\n";
  for ( auto n : bob.reconstructPath())
    maze(n).Value('^');
  std::cout << maze;
  std::cout << path << "\n";
  
  //---------------------
  maze = Laybrinth(
  {"...............",
   ".*************.",
   ".*...........*.",
   ".*...........*.",
   ".*...........*.",
   ".*...........*.",
   ".*...........*.",
   ".*...........*.",
   ".*************.",
   "..............."});
   maze.Start(2,2);
   maze.Finish(11,6);
 
   bob = Agent(maze,maze.Start(),maze.Finish());
   path = bob.A_StarAscii({2,2},{11,6});
  
   std::cout << "Found:\n";
   for ( auto n : bob.reconstructPath())
     maze(n).Value('^');
   std::cout << maze;
   std::cout << path << "\n";
   
   //---------------------
   maze = Laybrinth(
     {"...........*...",
      ".*.*.*.*.*.*...",
      ".*.*.*.*.*.*...",
      ".*.*.*.*.*.*...",
      ".***.*.*.*.*...",
      ".*.*.*.*.*.*...",
      ".*.*.*.*.*.*...",
      "..............."}
   );
   
    maze.Start(0,7);
    maze.Finish(14,6);
 
    bob = Agent(maze,maze.Start(),maze.Finish());
    path =  bob.A_StarAscii({0,7},{14,6});

    std::cout << "Found:\n";
    for ( auto n : bob.reconstructPath())
    {  
      maze(n).Value('^');
      std::cout << n;
    }std::cout << "\n";
    std::cout << path << "\n";
    std::cout << maze;
}

/*
*  \author Steven A White
*  \date   Sep 11th 2016
*/

#include <iostream>
#include <iomanip>

#include<cstdint>
#include<cmath>

#include<algorithm>
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
  enum Tile:char { WALL='|', OBSTACLE='#', PATH='#' };
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
  {}
  Position(const Position& obj)
    :_g_score(obj._g_score)
    ,_f_score(obj._f_score)
    ,_h_score(obj._h_score)
    ,_from (obj._from)
    ,_coord(obj._coord)
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
  inline void X(int x) { _coord.first=x;}
  inline void Y(int y) { _coord.second=y;}
  inline void Value(char c) { _value=c;}
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
  Laybrinth(size_t w, size_t l)
    :_laybrinth(w,std::vector<Position>(l))
  { 
    for( size_t i = 0; i < _laybrinth.size(); ++i)
    {
      for( size_t j = 0; j < _laybrinth.size(); ++j)
      {
        _laybrinth[i][j] = Position(i,j);
      }
    } 
  }
  
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
  inline  void Start (int x,int y)   { _start = {x,y}; }
  inline  void Finish(int x,int y)  { _finish = {x,y};}
  inline  void OutputStyle(Position::Style s) {  _style = s;}
  //-----------------------------------------------------------
  Position& operator()(int x, int y){ return _laybrinth[x][y];}
  Position& operator()(const Coordinate p){ return _laybrinth[p.first][p.second];}
  Position& operator()(const Position& pos){ return _laybrinth[pos.X()][pos.Y()];}
  //--------------------------------------------------¬230---------
  const Position& operator() (int x, int y)        const { return _laybrinth[x][y];}
  const Position& operator() (const Coordinate p)  const { return _laybrinth[p.first][p.second];}
  const Position& operator() (const Position& pos) const { return _laybrinth[pos.X()][pos.Y()];}
  //-----------------------------------------------------------
  std::vector<Position> reconstructPath(const Position& goal) const
  {
      std::vector<Position> path = {goal};
      Position current = goal;
      while (current != _start)
          current = operator()(current.From());
          path.push_back(current);
      return path;
  }
  bool isValid(Position node) const {
    bool ans = 0 <= node.X() && node.X() < _laybrinth.size();
    ans&= 0 <= node.Y() && node.Y() < _laybrinth[0].size();
    ans&= node.Value() == Position::Tile::PATH;
    return ans;
  }
private:
   NodeMap _laybrinth;
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
      os << std::setw(2);
      os << j;
    }
    os << "\n";
    for ( int i = 0; i < layb.Height(); ++i)
    { 
      os << std::setw(4);
      os << i << ": ";
      for ( int j = 0; j < layb.Width(); ++j)
      {
         os << std::setw(2) << layb(j,i).Value(); 
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
  std::vector<Position> A_Star() 
  {
    std::vector<Position> path;
    std::list<Position> closedSet;
    std::list<Position> openedSet = {_start};

    
    _laybrinth(_start).heuristicCostEstimate(_goal);
    while(openedSet.size()){
      Position current = *std::min_element(std::begin(openedSet), std::end(openedSet));;
      if (current == _goal)
          return _laybrinth.reconstructPath(_goal);
      openedSet.remove(current);
      closedSet.push_back(current);
      for( auto n : {Up,Down,Left,Right} ){
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
    return path;
  }
  //-----------------------------------------------------------
  std::vector<Position> A_Star( Position s, Position g)  
  {
    _start = s;
    _goal = g;
    return A_Star();
  }      

private:
  Laybrinth _laybrinth;
  Position  _start;
  Position  _goal;
};
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]){
  Laybrinth maze(10,10);
  maze.OutputStyle(Position::ASCII);
  
  Position  start(0,0);
  Position finish(9,9);
  std::cout << "Search for S" << start << " F" << finish <<"\n";
  std::cout << "In:\n";
  std::cout << maze;
  
  Agent bob = Agent(maze, start,finish);
  auto path = bob.A_Star(start,finish);
  
  std::cout << "Found:\n";
  for ( auto n : path)
    std::cout << n << ", ";
  std::cout << "\b\b.\n";
}

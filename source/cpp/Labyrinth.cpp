#include <vector>
#include <list>
#include <deque>
#include <string>
#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <cstdint>
#include <cmath>

// #define CODE_FIGHTS
// #ifdef _DEBUG
  // #undef _DEBUG
  #define _DEBUG 2
// #endif
//----------------------------------------------------------------------------
//--
//--  A* Search Approach
//--
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class AStar_Node 
{
  using Coordinate = std::pair<int, int>;
public:
  enum Style {COORDS,ASCII};
  enum Tile:char { WALL='#', OBSTACLE='*', PATH='.', VERT='-', HORIZ='|' };
  AStar_Node()
    :_from(0,0)
    ,_coord(0,0)
  {}
  AStar_Node(int x, int y)
    :_from(x,y)
    ,_coord(x,y)
  {}
  //-----------------------------------------------------------
  AStar_Node(      AStar_Node&& obj)
    :_g_score(obj._g_score)
    ,_f_score(obj._f_score)
    ,_h_score(obj._h_score)
    ,_from (std::move(obj._from))
    ,_coord(std::move(obj._coord))
    ,_value(std::move(obj._value))
  {}
  AStar_Node(const AStar_Node& obj)
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
  inline AStar_Node& X(int x) { _coord.first=x; return *this;}
  inline AStar_Node& Y(int y) { _coord.second=y; return *this;}
  inline AStar_Node& Value(char c) { _value=c; return *this;}
  //-----------------------------------------------------------
  bool operator==(const AStar_Node& rhs) const{
    return this->X() == rhs.X() && Y() == rhs.Y();
  }
  bool operator!=(const AStar_Node& rhs) const{
    return !(*this == rhs);
  }
  bool operator<(const AStar_Node& rhs) const{
    return _h_score < rhs._h_score;
  }
  bool operator>(const AStar_Node& rhs) const{
    return rhs < *this;
  }
  bool operator<=(const AStar_Node& rhs) const{
    return _h_score <= rhs._h_score;
  }
  bool operator>=(const AStar_Node& rhs) const{
    return rhs  <= *this;
  }
  AStar_Node operator-(const AStar_Node& rhs) const{
    AStar_Node ans = { X()-rhs.X(), Y()-rhs.X()};
    return ans;
  }
  AStar_Node operator+(const AStar_Node& rhs) const{
    AStar_Node ans = { X()+rhs.X(), Y()+rhs.Y()};
    return ans;
  }
  //-----------------------------------------------------------
  AStar_Node& operator=(const AStar_Node& obj)
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
  AStar_Node& operator=(   AStar_Node&& obj)
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
  inline int cityDistance(AStar_Node p2) const {
    return p2.X()-X() + p2.Y()-X();
  }
  //-----------------------------------------------------------
  inline float euclidianDistance(AStar_Node p2) const {
    return std::sqrt( p2.X()-std::pow(X(),2) + p2.Y()-std::pow(Y(),2));
  }
  //-----------------------------------------------------------
  float dist(AStar_Node p2) const {
    return cityDistance(p2);
  }
  //-----------------------------------------------------------
  float absDist(AStar_Node p2) const {
    return std::abs(dist(p2));
  }
  //-----------------------------------------------------------
  int heuristicCostEstimate(const AStar_Node& p2) const {
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
std::ostream& operator<<(std::ostream& os, const AStar_Node& pos){
  os << "(" << pos.X() << ", " << pos.Y()<< ")";
  return os;
}
#endif
//-----------------------------------------------------------------------------
class AStar_Laybrinth
{
using Coordinate = std::pair<int, int>;  
using NodeMap = std::vector<std::vector<AStar_Node>>;
using NodeRow = std::vector<AStar_Node>;
public:
  AStar_Laybrinth() = default;
  //-----------------------------------------------------------
  AStar_Laybrinth(size_t w, size_t l)
    :_laybrinth(w,std::vector<AStar_Node>(l))
  { 
    for( size_t i = 0; i < _laybrinth.size(); ++i)
    {
      for( size_t j = 0; j < _laybrinth[0].size(); ++j)
      {
        _laybrinth[i][j] = AStar_Node(i,j);
      }
    } 
  }
  //-----------------------------------------------------------
  AStar_Laybrinth(std::vector<std::string> map)
    :_laybrinth(map[0].size(),std::vector<AStar_Node>(map.size()))
  {
    for( size_t i = 0; i < _laybrinth.size(); ++i)
    {
      for( size_t j = 0; j < _laybrinth[0].size(); ++j)
      {
        _laybrinth[i][j] = AStar_Node(i,j).Value(map[j][i]);
      }
    } 
  }
  //-----------------------------------------------------------
  AStar_Laybrinth(const AStar_Laybrinth& obj)
    :_laybrinth(obj._laybrinth)
    ,_start(obj._start)
    ,_finish(obj._finish)
    ,_style(obj._style)
  {}
  AStar_Laybrinth(AStar_Laybrinth&& obj)
    { *this = std::move(obj);}
  //-----------------------------------------------------------
  inline int Width() const  { return _laybrinth.size();}
  inline int Height() const { return _laybrinth[0].size();}
  //-----------------------------------------------------------
  inline AStar_Node& Start()  { return _start;}
  inline AStar_Node& Finish() { return _finish;}
  inline AStar_Node::Style OutputStyle() { return _style;}
  //-----------------------------------------------------------
  inline const AStar_Node& Start()  const { return _start; }
  inline const AStar_Node& Finish() const { return _finish;}
  inline const AStar_Node::Style OutputStyle() const { return _style;}
  //-----------------------------------------------------------
  inline  AStar_Laybrinth& Start (int x,int y)  { _start = {x,y}; return *this;}
  inline  AStar_Laybrinth& Finish(int x,int y)  { _finish = {x,y};return *this;}
  inline  AStar_Laybrinth& Start (AStar_Node p)   { _start = p;     return *this;}
  inline  AStar_Laybrinth& Finish(AStar_Node p)   { _finish = p;    return *this;}
  inline  AStar_Laybrinth& OutputStyle(AStar_Node::Style s) {  _style = s;return *this;}
  //-----------------------------------------------------------
  AStar_Node& operator()(int x, int y){ return _laybrinth[x][y];}
  AStar_Node& operator()(const Coordinate p){ return _laybrinth[p.first][p.second];}
  AStar_Node& operator()(const AStar_Node& pos){ return _laybrinth[pos.X()][pos.Y()];}
  //--------------------------------------------------¬230---------
  const AStar_Node& operator() (int x, int y)        const { return _laybrinth[x][y];}
  const AStar_Node& operator() (const Coordinate p)  const { return _laybrinth[p.first][p.second];}
  const AStar_Node& operator() (const AStar_Node& pos) const { return _laybrinth[pos.X()][pos.Y()];}
  //-----------------------------------------------------------
  AStar_Laybrinth& operator=(const AStar_Laybrinth& obj)
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
  AStar_Laybrinth& operator=(AStar_Laybrinth&& obj)
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
  std::deque<AStar_Node> reconstructPath(const AStar_Node& goal) const
  {
      std::deque<AStar_Node> path = {};
      AStar_Node current = goal;
      while (current != _start)
      {
          current = operator()(current.From());
          path.push_front(current);
        }
      return path;
  }
  //-----------------------------------------------------------
  std::string reconstructPathAscii(const AStar_Node& goal) const
  {
      std::string path = "";
      AStar_Node current = goal;
      AStar_Node prev;
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
  bool isValid(AStar_Node node) const {
    bool ans = 0 <= node.X() && node.X() < _laybrinth.size();
    ans&= 0 <= node.Y() && node.Y() < _laybrinth[0].size();
    
    return (ans)? _laybrinth[node.X()][node.Y()].Value() == AStar_Node::Tile::PATH
                : false;
  }
private:
   NodeMap _laybrinth = {};
   AStar_Node _start  = {0,0};
   AStar_Node _finish = {0,0};
   
   AStar_Node::Style _style = AStar_Node::ASCII;

};
//-----------------------------------------------------------------------------
#ifndef CODE_FIGHTS 
std::ostream& operator<<(std::ostream& os, AStar_Laybrinth layb)
{
  if(layb.OutputStyle() == AStar_Node::COORDS)
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
  const AStar_Node Down = { 0, 1};
  const AStar_Node Up   = { 0,-1};
  const AStar_Node Left = {-1, 0};
  const AStar_Node Right= { 1, 0};
public:
  Agent(AStar_Laybrinth layb)
    :_laybrinth(layb)
    ,_start(_laybrinth.Start())
    ,_goal(_laybrinth.Finish())
  {}
  //-----------------------------------------------------------
  Agent(AStar_Laybrinth layb,AStar_Node s, AStar_Node g)
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
  std::deque<AStar_Node> A_Star() 
  {
    std::list<AStar_Node> closedSet;
    std::list<AStar_Node> openedSet = {_start};

    
    _laybrinth(_start).heuristicCostEstimate(_goal);
    while(openedSet.size()){
      AStar_Node current = *std::min_element(std::begin(openedSet), std::end(openedSet));;
      if (current == _goal)
          return _laybrinth.reconstructPath(_goal);
      openedSet.remove(current);
      closedSet.push_back(current);
      for( auto n : {Down,Left,Right,Up} ){
        AStar_Node neighbor = current+n;
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
    std::list<AStar_Node> closedSet;
    std::list<AStar_Node> openedSet = {_start};

    
    _laybrinth(_start).heuristicCostEstimate(_goal);
    while(openedSet.size()){
      AStar_Node current = *std::min_element(std::begin(openedSet), std::end(openedSet));;
      if (current == _goal)
          return _laybrinth.reconstructPathAscii(_goal);
      openedSet.remove(current);
      closedSet.push_back(current);
      for( auto n : {Down,Left,Right,Up} ){
        AStar_Node neighbor = current+n;
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
  std::deque<AStar_Node> A_Star( AStar_Node s, AStar_Node g)  
  {
    _start = s;
    _goal = g;
    return A_Star();
  } 
  std::string A_StarAscii( AStar_Node s, AStar_Node g)  
  {
    _start = s;
    _goal = g;
    return A_StarAscii();
  }      
  //-----------------------------------------------------------
  std::deque<AStar_Node> reconstructPath(){ return _laybrinth.reconstructPath(_goal); }
  std::string reconstructPathAscii(){ return _laybrinth.reconstructPathAscii(_goal); }
private:
  AStar_Laybrinth _laybrinth;
  AStar_Node  _start;
  AStar_Node  _goal;
};
//----------------------------------------------------------------------------
//--
//--  Breadth Based Search Approach
//--  Prunes Solutions based on previous iteration
//--  Does some cycle detection
//----------------------------------------------------------------------------
#define UP 'U'
#define DOWN 'D'
#define LEFT 'L'
#define RIGHT 'R'

struct Brute_Position
{
  int _X = -1;
  int _Y = -1;
  bool operator==(const Brute_Position& rhs) const{
    return _X == rhs._X && _Y == rhs._Y;
  }
  bool operator!=(const Brute_Position& rhs){
    return !(*this == rhs);
  }
  Brute_Position operator-(const Brute_Position& rhs) const{
    Brute_Position ans = {_X-rhs._X, _Y-rhs._Y};
    return ans;
  }
};
//-----------------------------------------------------------------------------
#ifndef CODE_FIGHTS
std::ostream& operator<<(std::ostream& os, const Brute_Position& pose){
  os << "(" << pose._X << " , " << pose._Y<< ")";
  return os;
}
#endif
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
  
  Brute_Position tile = {-1,-1};
};

//-----------------------------------------------------------------------------
class Brute_Laybrinth 
{
public:
  Brute_Laybrinth(std::vector<std::string> map, 
             Brute_Position initial, 
             Brute_Position finish)
  :_labyrinth(map)
  ,_start(initial)
  ,_finish(finish)
  {}
  
  inline Brute_Position Start() const { return _start;};
  inline Brute_Position Finish() const { return _finish;};
  //---------------------------------------------
  bool simulate(Solution& work_queue,
                  size_t& cycles)
  {
    cycles = 0;
    bool done = false;
    Brute_Position current = _start;
    std::vector<Brute_Position> history{_start};
    
    Brute_Position& tile = work_queue.tile;
    if(tile != Brute_Position() )
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
        // history.push_back(Brute_Position());
        Brute_Position temp;
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
    const Brute_Position _start;
    const Brute_Position _finish;
};
//-----------------------------------------------------------------------------
int labyrinthNavigation(std::vector<std::string> map, 
                        std::vector<int> start, 
                        std::vector<int> dest) 
{
  AStar_Laybrinth AStar_maze(map);
  AStar_maze.Start (start[1],start[0]);
  AStar_maze.Finish( dest[1], dest[0]);;
  Agent bob = Agent(AStar_maze);
  auto path = bob.A_StarAscii();
  
  for ( auto n : bob.reconstructPath())
  {  
    AStar_maze(n).Value('^');
  }std::cout << "\n";
  std::cout << path << "\n";
  std::cout << AStar_maze;
  
  decltype(map)  padded_map(map.size()+2,
                            std::string(map[0].size()+2,'#'));
  for (auto i=0; i<map.size(); ++i)
    padded_map[i+1].replace(1,map[i].size(),map[i]);
  Brute_Position adj_start = {start[1]+1,start[0]+1};
  Brute_Position adj_dest  = {dest[1]+1,dest[0]+1};
  Brute_Laybrinth laybrinth(padded_map,adj_start, adj_dest); 

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
  auto search_space = (path.size()<50)?path.size():path.size()/2;
  for ( auto length : {size_t(0),4*path.size()/8
                               ,3*path.size()/8
                               ,5*path.size()/8
                               ,2*path.size()/8
                               ,6*path.size()/8
                               ,7*path.size()/8
                               ,1*path.size()/8})
  {
    auto search_space = length + 15;
    work_queue.clear();
    if(length==0){
            work_queue.push_back(LEFT);
            work_queue.push_back(RIGHT);
            work_queue.push_back(UP);
            work_queue.push_back(DOWN);
    }else {
      work_queue.push_back(path.substr(0,length));
    }
    while (!work_queue.empty()){
      auto cmd_queue = work_queue.front();
      size_t cycles = 0;
      if(  
        cmd_queue.size() < search_space
                 && laybrinth.simulate(cmd_queue,cycles ))
      { 

        if(cmd_queue.size() < path.size())
          { 
            std::cout <<"SHORT PATH\n";
            #if _DEBUG > 2
                  for( auto c: cmd_queue)
                    std::cout << c;
                  std::cout << "\n";
            #endif
            path = cmd_queue; } 
        break;
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
  }  
  return path.size();
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
    EXPECT_EQ ( 1224, labyrinthNavigation (
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
 ,{55,0}
));
};
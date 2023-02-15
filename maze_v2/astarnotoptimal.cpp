//
// astar.cpp - Version 1.0
//
// Please write your name, your student ID, and your email address here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.
//

#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
#include <set> 
#include <unordered_map>

// ---------------------------------------------------------------------
// Include the optional header with backward compatibility
// see http://codereview.stackexchange.com/questions/136350/seamlessly-migrating-experimental-optional-to-optional
#if __has_include(<optional>)
#   include <optional>
namespace stdx {
  using namespace ::std;
}
#elif __has_include(<experimental/optional>)
#   include <experimental/optional>
namespace stdx {
  using namespace ::std;
  using namespace ::std::experimental;
}
#else
#   error <experimental/optional> and <optional> not found
#endif


using namespace stdx;
// ---------------------------------------------------------------------


typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;
#define m_p make_pair 
#define MAX_SIZE 300
#define INF 1e9


class Coordinate {

  int x, y;

public:
  Coordinate(int x, int y) : x(x), y(y) {}

  int getX() const {
    return x;
  }

  int getY() const {
    return y;
  }

  bool operator==(const Coordinate &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  bool operator!=(const Coordinate &rhs) const {
    return !(rhs == *this);
  }
};

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const
  {
      auto hash1 = hash<T1>{}(p.first);
      auto hash2 = hash<T2>{}(p.second);

      if (hash1 != hash2) {
          return hash1 ^ hash2;             
      }
        
      // If hash1 == hash2, their XOR is zero.
        return hash1;
  }
};

class AStarFirstSearchAgent { 
  Pair end;
  unordered_map <Pair, Pair, hash_pair> parent; 
  unordered_map <Pair, int, hash_pair> vis;
 
  list <Pair> q; 
  set <pPair> ls;
  set <pPair> cld;

  int x = 0, y = 0;

public:

  AStarFirstSearchAgent(int size_x, int size_y) {

    // enter your code here   
    q.push_back(m_p(0, 0));
    parent[m_p(0, 0)] = m_p(0, 0);
  }

  optional<Coordinate> move(bool isExit, bool hasWallSouth, bool hasWallNorth, bool hasWallEast, bool hasWallWest, double distance) {
    
    // enter your code here   
    vis[m_p(x, y)] = 1;
    if(q.size())
      q.pop_front();
    if(!cld.count(m_p(distance, m_p(x, y)))){
      ls.insert(m_p(distance, m_p(x, y)));
      cld.insert(m_p(distance, m_p(x, y)));
    }

    if(!q.empty()){
      x = q.front().first;
      y = q.front().second;
      return Coordinate(x, y);
    }
    else{
      int i = (*ls.begin()).second.first;
      int j = (*ls.begin()).second.second;
      if(!(x == i && j == y)){
        x = i;
        y = j;
        return Coordinate(x, y);
      }
      else
        ls.erase(ls.begin());
    }

    if(isExit){
      end = m_p(x, y);
      return {};
    }
    if(!hasWallSouth && !vis.count(m_p(x, y + 1))){
      parent[m_p(x, y + 1)] = m_p(x, y);
      q.push_back(m_p(x, y + 1));
    }
    if(!hasWallNorth && !vis.count(m_p(x, y - 1))){
      parent[m_p(x, y - 1)] = m_p(x, y);
      q.push_back(m_p(x, y - 1));
    }
    if(!hasWallEast && !vis.count(m_p(x + 1, y))){
      parent[m_p(x + 1, y)] = m_p(x, y);
      q.push_back(m_p(x + 1, y));
    }
    if(!hasWallWest && !vis.count(m_p(x - 1, y))){
      parent[m_p(x - 1, y)] = m_p(x, y);
      q.push_back(m_p(x - 1, y));
    }
    x = q.front().first;
    y = q.front().second;
    return Coordinate(x, y);
  }

  list<Coordinate> getShortestPath() {
    
    // enter your code here
    list<Coordinate> ans;
    while(!(end.first == 0 && end.second == 0)){
      ans.push_front(Coordinate(end.first, end.second));
      end = parent[end];
    }
    ans.push_front(Coordinate(0, 0)); 
    return ans;
  }

};

int main(int argc, char *argv[]) {

  int size_x, size_y;

  if (argc == 3) {
    size_x = atoi(argv[1]);
    size_y = atoi(argv[2]);
  } else {
    cerr << "Error: wrong arguments." << endl;
    return -1;  // do nothing
  }

  AStarFirstSearchAgent agent(size_x, size_y);

  while(true) {
    string s1, s2, s3, s4, s5, s6;
    cin >> s1 >> s2 >> s3 >> s4 >> s5 >> s6;

    bool isExit = (s1 != "0");
    bool hasWallSouth = (s2 != "0");
    bool hasWallNorth = (s3 != "0");
    bool hasWallEast = (s4 != "0");
    bool hasWallWest = (s5 != "0");
    double distance = stof(s6);

    auto coord = agent.move(isExit, hasWallSouth, hasWallNorth, hasWallEast, hasWallWest, distance);

    if (coord) {
      cout << coord->getX() << " " << coord->getY() << endl;
    } else {
      break;
    }
  }

  list<Coordinate> path = agent.getShortestPath();

  cout << "PATH" << endl;
  for(auto&& coord : path) {
    cout << coord.getX() << " " << coord.getY() << endl;
  }
  cout << "END" << endl;

  return 0;
}

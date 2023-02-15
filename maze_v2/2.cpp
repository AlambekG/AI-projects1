//
// dfs.cpp - Version 1.0
//
// Please write your name, your student ID, and your email address here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.
//
/*
   Alambek Gulamidinov 20192012
   
*/

#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
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


#define MAX_SIZE 300


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

class DepthFirstSearchAgent {
  list <pair <int, int > > q;
  pair <int, int> end;
  unordered_map <pair <int, int>, pair<int, int>, hash_pair> parent;
  unordered_map <pair <int, int>, int, hash_pair> vis;

public:

  DepthFirstSearchAgent(int size_x, int size_y) {
    // enter your code here
    q.push_back(make_pair(0, 0));
    parent[make_pair(0, 0)] = make_pair(-1, -1);
    vis[make_pair(0, 0)] = 1;
  }

  int dist(pair <int, int> v){
      if(v.first == 0 && v.second == 0) return 1;
      else return 1 + dist(parent[v]);
  }

  optional<Coordinate> move(bool isExit, bool hasWallSouth, bool hasWallNorth, bool hasWallEast, bool hasWallWest) {
     
    // enter your code here
    pair <int, int > cur = q.front(); 
    int x = cur.first, y = cur.second;
    int d = dist(make_pair(x, y));
    //vis[make_pair(x, y)] = 1;

    q.pop_front();
    if(isExit){
      end = make_pair(x, y);
      if(q.empty()) return {};
      else return Coordinate(q.front().first, q.front().second);
    }
    if(!hasWallSouth){
      bool visited = vis[make_pair(x, y + 1)];
      if(!visited || (dist(make_pair(x, y + 1)) > d + 1)){
        vis[make_pair(x, y + 1)] = 1;
        parent[make_pair(x, y + 1)] = make_pair(x, y);
        q.push_front(make_pair(x, y + 1));
      }
    }
    if(!hasWallNorth){
      bool visited = vis[make_pair(x, y - 1)]; 
      if(!visited || (dist(make_pair(x, y - 1)) > d + 1)){
        vis[make_pair(x, y - 1)] = 1;
        parent[make_pair(x, y - 1)] = make_pair(x, y);
        q.push_front(make_pair(x, y - 1));
      }
    }
    if(!hasWallEast){
      bool visited = vis[make_pair(x + 1, y)];
      if(!visited || (dist(make_pair(x + 1, y)) > d + 1)){
        vis[make_pair(x + 1, y)] = 1;
        parent[make_pair(x + 1, y)] = make_pair(x, y);
        q.push_front(make_pair(x + 1, y));
      }
    }
    if(!hasWallWest){
      bool visited = vis[make_pair(x - 1, y)];
      if(!visited || (dist(make_pair(x - 1, y)) > d + 1)){
        vis[make_pair(x - 1, y)] = 1;
        parent[make_pair(x - 1, y)] = make_pair(x, y);
        q.push_front(make_pair(x - 1, y));
      }
    }
    if(q.empty()) return {};
    else return Coordinate(q.front().first, q.front().second);
  }

  list<Coordinate> getShortestPath() {

    // enter your code here
    list<Coordinate> ls;
    while(!(end.first == 0 && end.second == 0)){
      ls.push_front(Coordinate(end.first, end.second));
      end = parent[end];
    }
    ls.push_front(Coordinate(0, 0));
    return ls;
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

  DepthFirstSearchAgent agent(size_x, size_y);

  while(true) {
    string s1, s2, s3, s4, s5;
    cin >> s1 >> s2 >> s3 >> s4 >> s5;

    bool isExit = (s1 != "0");
    bool hasWallSouth = (s2 != "0");
    bool hasWallNorth = (s3 != "0");
    bool hasWallEast = (s4 != "0");
    bool hasWallWest = (s5 != "0");

    auto coord = agent.move(isExit, hasWallSouth, hasWallNorth, hasWallEast, hasWallWest);

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





//////
//
// dfs.cpp - Version 1.0
//
// Please write your name, your student ID, and your email address here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.
//
/*
   Alambek Gulamidinov 20192012
   
*/

#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
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


#define MAX_SIZE 300


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

class DepthFirstSearchAgent {
  list <pair <int, int > > q;
  pair <int, int> end;
  unordered_map <pair <int, int>, pair<int, int>, hash_pair> parent;
  unordered_map <pair <int, int>, int, hash_pair> vis;

public:

  DepthFirstSearchAgent(int size_x, int size_y) {
    // enter your code here
    q.push_back(make_pair(0, 0));
    parent[make_pair(0, 0)] = make_pair(-1, -1);
    //vis[make_pair(0, 0)] = 1;
  }

  int dist(pair <int, int> v){
      if(v.first == 0 && v.second == 0) return 1;
      else return 1 + dist(parent[v]);
  }

  optional<Coordinate> move(bool isExit, bool hasWallSouth, bool hasWallNorth, bool hasWallEast, bool hasWallWest) {
     
    // enter your code here
    pair <int, int > cur = q.front(); 
    int x = cur.first, y = cur.second;
    int d = dist(make_pair(x, y));
    vis[make_pair(x, y)] = 1;

    q.pop_front();
    if(isExit){
      end = make_pair(x, y);
      if(q.empty()) return {};
      else return Coordinate(q.front().first, q.front().second);
    }
    if(!hasWallSouth){
      bool visited = vis[make_pair(x, y + 1)];
      if(visited && (dist(make_pair(x, y + 1)) > d + 1))
        parent[make_pair(x, y + 1)] = make_pair(x, y);
      else if(!visited){
        //vis[make_pair(x, y + 1)] = 1;
        parent[make_pair(x, y + 1)] = make_pair(x, y);
        q.push_front(make_pair(x, y + 1));
      }
    }
    if(!hasWallNorth){
      bool visited = vis[make_pair(x, y - 1)];
      if(visited && (dist(make_pair(x, y - 1)) > d + 1))
        parent[make_pair(x, y - 1)] = make_pair(x, y);
      else if(!visited){
        //vis[make_pair(x, y - 1)] = 1;
        parent[make_pair(x, y - 1)] = make_pair(x, y);
        q.push_front(make_pair(x, y - 1));
      }
    }
    if(!hasWallEast){
      bool visited = vis[make_pair(x + 1, y)];
      if(visited && (dist(make_pair(x + 1, y)) > d + 1))
        parent[make_pair(x + 1, y)] = make_pair(x, y);
      else if(!visited){
        //vis[make_pair(x + 1, y)] = 1;
        parent[make_pair(x + 1, y)] = make_pair(x, y);
        q.push_front(make_pair(x + 1, y));
      }
    }
    if(!hasWallWest){
      bool visited = vis[make_pair(x - 1, y)];
      if(visited && (dist(make_pair(x - 1, y)) > d + 1))
        parent[make_pair(x - 1, y)] = make_pair(x, y);
      else if(!visited){
        //vis[make_pair(x - 1, y)] = 1;
        parent[make_pair(x - 1, y)] = make_pair(x, y);
        q.push_front(make_pair(x - 1, y));
      }
    }
    if(q.empty()) return {};
    else return Coordinate(q.front().first, q.front().second);
  }

  list<Coordinate> getShortestPath() {

    // enter your code here
    list<Coordinate> ls;
    while(!(end.first == 0 && end.second == 0)){
      ls.push_front(Coordinate(end.first, end.second));
      end = parent[end];
    }
    ls.push_front(Coordinate(0, 0));
    return ls;
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

  DepthFirstSearchAgent agent(size_x, size_y);

  while(true) {
    string s1, s2, s3, s4, s5;
    cin >> s1 >> s2 >> s3 >> s4 >> s5;

    bool isExit = (s1 != "0");
    bool hasWallSouth = (s2 != "0");
    bool hasWallNorth = (s3 != "0");
    bool hasWallEast = (s4 != "0");
    bool hasWallWest = (s5 != "0");

    auto coord = agent.move(isExit, hasWallSouth, hasWallNorth, hasWallEast, hasWallWest);

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


/*
 * Author: Alambek Gulamidinov
 * Student_ID: 20192012
 * Email: alambek@unist.ac.kr
 */

#ifndef GAME_TREE_GAME_TREE_SEARCH_H
#define GAME_TREE_GAME_TREE_SEARCH_H

#include <iostream>
#include <vector>

#include "game_spec.h"

using namespace std;

class GameTree {
  int size;
  int n, m;
  vector<vector<char>> board;
  vector <int> win_seq;
  vector <vector <int> > win_board; 

public:

  int computeGameTree(const GameSpec& spec);

  void printGameTree(const GameSpec& spec);

  void playAsFirstPlayer(const GameSpec& spec);

  void playAsSecondPlayer(const GameSpec& spec);
};


#endif //GAME_TREE_GAME_TREE_SEARCH_H

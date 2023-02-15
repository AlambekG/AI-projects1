/*
 * Author: Alambek Gulamidinov
 * Student_ID: 20192012
 * Email: alambek@unist.ac.kr
 */

#include <iostream>
#include <unordered_map> 

#include "game_spec.h"
#include "game_tree_search.h"

const int inf = 1e9;
const int Prime = 31;
unordered_map <int, int> numID;


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
unordered_map <pair <int, int> , int, hash_pair> stored_minmx;

void countID(int nn){
    int p = 1;
    for(int i = 0;i < nn;i ++){
        numID[i] = p;
        p = p * Prime % inf;
    }
}

int checkwin(int xP, int oP, vector < int > win_seq){
    for(auto k : win_seq){
        if(k == xP) return 1;
        if(k == oP) return -1;
    }
    return 0;
}

int minimax(int xP, int oP, bool isMax, int alpha, int beta, vector <int> win_seq, vector <int> visited, int nn){
    int res = checkwin(xP, oP, win_seq);
    if(res != 0) return res;

    unordered_map <int, int> vis;
    for(auto i : visited) vis[i] = 1;
    bool flag = false;
    int bestScore;

    if(isMax){
        bestScore = -inf;
        for(int i = 0;i < nn;i ++){
            if(!vis[i]){
                xP += numID[i];
                visited.push_back(i);
                if(!stored_minmx.count(make_pair(xP, oP))) 
                    stored_minmx[make_pair(xP, oP)] =  minimax(xP, oP, false, alpha, beta, win_seq, visited, nn);
                bestScore = max(bestScore, stored_minmx[make_pair(xP, oP)]);
                xP -= numID[i];
                visited.pop_back();
                flag = true;
                if(bestScore >= beta) return bestScore;
                if(bestScore > alpha) alpha = bestScore;
            }
        }
    }
    else{
        int bestScore = inf; 
        for(int i = 0;i < nn;i ++){
            if(!vis[i]){
                oP += numID[i];
                visited.push_back(i);
                if(!stored_minmx.count(make_pair(xP, oP))) 
                    stored_minmx[make_pair(xP, oP)] =  minimax(xP, oP, true, alpha, beta, win_seq, visited, nn);
                bestScore = min(bestScore, stored_minmx[make_pair(xP, oP)]);
                oP -= numID[i];
                visited.pop_back();
                flag = true;
                if(bestScore <= alpha) return bestScore;
                if(bestScore < beta) beta = bestScore;
            }
        }
    }  
    if(!flag) return 0;
    return bestScore;  
}

int GameTree::computeGameTree(const GameSpec& spec) {

    size = spec.getSize();
    n = spec.getSizeX();
    m = spec.getSizeY();
    countID(size);

    vector <int> vis;
    
    for(int i = 0;i < size;i ++){
        if(spec.isBlocked(i))
            vis.push_back(i);
    }
    
    for(auto i : spec.getWinSeq()){
        int val = 0;
        for(auto j : i){
            val += numID[j];
        }
        win_seq.push_back(val);
    }
    int res = minimax(0, 0, true, -inf, inf, win_seq, vis, size);
    return res;
}


int checkwinboard(vector <vector <char> > board, vector <vector <int> > win_seq){
    unordered_map<int, int> mp, mm;
    vector <int> a, b;

    int c = 0;
    for(int i = 0;i < board.size();i ++){
        for(int j = 0;j < board[i].size();j ++, c ++){
            if(board[i][j] == 'X') mp[c] = 1;
            if(board[i][j] == 'O') mm[c] = 1;
        }
    }

    for(auto k : win_seq){
        bool flag = true;
        for(auto i : k){
            if(!mp[i]){
                flag = false;
                break;
            }
        }
        if(flag) return 1;
        flag = true;
        for(auto i : k){
            if(!mm[i]){
                flag = false;
                break;
            }
        }
        if(flag) return -1;
    }
    return 0;
}

int state = 0;
int dfs_print(vector <vector <char> > bb, int depth, bool isMax, int alpha, 
int beta, vector <vector <int>> win_seq, int parent){
    if(depth > 1000) return 0;
    vector <int> children;
    int n = bb.size();
    int m = bb[0].size();
    int res = checkwinboard(bb, win_seq);

    int bestScore, c = 1;
    bool flag = false, ispruned = false;
    int id = state;
    if(res != 0) bestScore = res;
    else if(isMax){
        bestScore = -inf;
        for(int i = 0;i < n;i ++){
            for(int j = 0;j < m;j ++){
                if(bb[i][j] == '*'){
                    state ++;
                    children.push_back(state);
                    bb[i][j] = 'X';
                    int score = dfs_print(bb, depth + 1, false, alpha, beta, win_seq, id);
                    if(score > bestScore){
                        bestScore = score;
                    }
                    if(bestScore >= beta){
                        ispruned = true;
                        break;
                    }
                    if(bestScore > alpha){
                        alpha = bestScore;
                    }
                    bb[i][j] = '*';  
                    flag = true;
                }
            }
        } 
    }
    else{
        bestScore = inf; 
        for(int i = 0;i < n;i ++){
            for(int j = 0;j < m;j ++){
                if(bb[i][j] == '*'){
                    bb[i][j] = 'O';
                    state ++;
                    children.push_back(state);
                    int score = dfs_print(bb, depth + 1, true, alpha, beta, win_seq, id);
                    if(score < bestScore){
                        bestScore = score;
                    }
                    if(bestScore <= alpha){
                        ispruned = true;
                        break;
                    }
                    if(bestScore < beta)
                        beta = bestScore;
                    bb[i][j] = '*';
                    flag = true;
                }
            }
        }    
    }
    std::cout << "--- s" << id << " parent=";
    if(id != 0) std::cout << parent << ' ';
    else std::cout << "nill ";

    if(ispruned){
        std::cout << "pruned\n";
    }
    else{
        if(res != 0){
            if(res == 1) std::cout << "WIN payoff=1\n";
            if(res == -1) std::cout << "LOST payoff=-1\n";
            bestScore = res;
        }
        else if(!flag) std::cout << "DRAW payoff=0\n"; 
        else{   
            std::cout << "children= {";
            for(auto i : children){
                cout << " s" << i << ' ';
            }
            std::cout << "} payoff=" << bestScore << endl;
        }
    }
    for(int i = 0;i < n;i ++){
        for(int j = 0;j < m;j ++){
            cout << bb[i][j];
        }
        cout << endl;
     }
    if(res == 0 && !flag) return 0;
    return bestScore;  
}

void GameTree::printGameTree(const GameSpec& spec) {
    size = spec.getSize();
    n = spec.getSizeX();
    m = spec.getSizeY();
    
    int c = 0;
    for(int i = 0;i < n;i ++){
        vector <char> v;
        for(int j = 0;j < m;j ++, c ++){
            if(spec.isBlocked(c))
                v.push_back('#');
            else
                v.push_back('*');
        }
        board.push_back(v);
    }

    win_board = spec.getWinSeq();

    int res = dfs_print(board, 0, true, -inf, inf, win_board, 0);
}

void GameTree::playAsFirstPlayer(const GameSpec& spec){
    int depth = 0, xP = 0, oP = 0, player2move = 0;
    countID(spec.getSize() + 1);
    unordered_map <int, int> vis;
    vector <int> visited;

    for(int i = 0;i < spec.getSize();i ++){
        if(spec.isBlocked(i)){
            vis[i] = 1;
            visited.push_back(i);
        }
    }
    int res = minimax(0, 0, true, -inf, inf, win_seq, visited, size);

    while(true){
        bool flag = false;
        int bestScore = -inf;
        int move;
        for(int i = 0;i < spec.getSize();i ++){
            if(!vis[i]){
                xP += numID[i];
                if(stored_minmx[make_pair(xP, oP)] > bestScore){
                    bestScore = stored_minmx[make_pair(xP, oP)];
                    move = i;
                }
                xP -= numID[i];
                flag = true;
            }
        }
        cout << move << endl;
        vis[move] = 1;
        xP += numID[move];
        depth ++;
        if(depth > 10000000) break;
        cin >> player2move;
        oP += numID[player2move];
    }
}

void GameTree::playAsSecondPlayer(const GameSpec& spec) {
    int depth = 0, xP = 0, oP = 0, player1move = 0;
    countID(spec.getSize() + 1);
    unordered_map <int, int> vis;
    vector <int> visited;

    for(int i = 0;i < spec.getSize();i ++){
        if(spec.isBlocked(i)){
            vis[i] = 1;
            visited.push_back(i);
        }
    }
    int res = minimax(0, 0, true, -inf, inf, win_seq, visited, size);

    while(true){
        cin >> player1move;
        xP += numID[player1move];
        int bestScore = inf;
        int move;
        for(int i = 0;i < spec.getSize();i ++){
            if(!vis[i]){
                oP += numID[i];
                if(stored_minmx[make_pair(xP, oP)] < bestScore){
                    bestScore = stored_minmx[make_pair(xP, oP)];
                    move = i;
                }
                oP -= numID[i];
            }
        }
        cout << move << endl;
        vis[move] = 1;
        oP += numID[move];
        depth ++;
        if(depth > 10000000) break;
    }
}


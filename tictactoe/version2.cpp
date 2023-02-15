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


int checkwin(vector <vector <char> > board, vector <vector <int> > win_seq){
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

int minimax(vector <vector <char> > bb, int depth, bool isMax, int alpha, int beta, vector <vector <int>> win_seq){
    if(depth > 1000) return 0;
    cout << depth << ' ' << isMax << endl;
    int res = checkwin(bb, win_seq);
    if(res != 0) return res;
    int n = bb.size();
    int m = bb[0].size();

    if(isMax){
        int bestScore = -inf;
        bool flag = false;
        for(int i = 0;i < n;i ++){
            for(int j = 0;j < m;j ++){
                if(bb[i][j] == '*'){
                    bb[i][j] = 'X';
                    bestScore = max(bestScore, minimax(bb, depth + 1, false, alpha, beta, win_seq));
                    bb[i][j] = '*';  
                    flag = true;
                    if(bestScore >= beta)
                        return bestScore;
                    if(bestScore > alpha){
                        alpha = bestScore;
                    }
                }
            }
        } 
        if(!flag) return 0;
        return bestScore;
    }
    else{
        int bestScore = inf; 
        bool flag = false;
        for(int i = 0;i < n;i ++){
            for(int j = 0;j < m;j ++){
                if(bb[i][j] == '*'){
                    bb[i][j] = 'O';
                    bestScore = min(bestScore, minimax(bb, depth + 1, true, alpha, beta, win_seq));
                    bb[i][j] = '*';
                    flag = true;
                    if(bestScore <= alpha)
                        return bestScore;
                    if(bestScore < beta)
                        beta = bestScore;
                }
            }
        }    
        if(!flag) return 0;
        return bestScore;  
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
    int res = checkwin(bb, win_seq);

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

int GameTree::computeGameTree(const GameSpec& spec) {

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

    win_seq = spec.getWinSeq();

    int res = minimax(board, 0, true, -inf, inf, win_seq);
    return res;
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

    win_seq = spec.getWinSeq();

    int res = dfs_print(board, 0, true, -inf, inf, win_seq, 0);
}

void GameTree::playAsFirstPlayer(const GameSpec& spec) {
    int depth = 0;
    while(true){
        int bestScore = -inf;
        pair <int, int> move;
        int bestMove, c = 0;

        for(int i = 0;i < n;i ++){
            for(int j = 0;j < m;j ++, c++){
                if(board[i][j] == '*'){
                    board[i][j] = 'X';
                    int score = minimax(board, depth, true, -inf, inf, win_seq);
                    board[i][j] = '*'; 
                    if(score > bestScore){
                        bestScore = score;
                        move = make_pair(i, j);  
                        bestMove = c;
                    }
                }
            }
        } 
        board[move.first][move.second] = 'X';
        depth ++;
        break;
    }
}

void GameTree::playAsSecondPlayer(const GameSpec& spec) {
    int depth = 0;
    while(true){
        int bestScore = inf;
        pair <int, int> move;
        int bestMove, c = 0;
        for(int i = 0;i < n;i ++){
            for(int j = 0;j < m;j ++, c ++){
                if(board[i][j] == '*'){
                    board[i][j] = 'O';
                    int score = minimax(board, depth, false, -inf, inf, win_seq);
                    board[i][j] = '*'; 
                    if(score < bestScore){
                        bestScore = score;
                        move = make_pair(i, j);  
                        bestMove = c;
                    }
                }
            }
        }
        board[move.first][move.second] = 'O';
        depth ++;
        break;
    }
}


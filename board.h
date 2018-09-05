// Jack Hill
// CSE 20311
// Fund Comp
// Lab 9
// Due 11/21/17
// board.h
// Creates the Board class that will be used in crossword.cpp to represent the
// crossword board. Descriptions of the different methods can be found above
// their implementations


#include<vector>
#include<string>

using namespace std;

class Board
{
    public:
        Board();
        ~Board();
        void setWord(string);
        void sortWord();
        void loadBoard();
        void playHere(string, int, int, vector<int>&);
        char direction(string, int, int);
        bool objectAdjacent(int, int);
        void playHorizontal(string, int, int);
        void playVertical(string, int, int);
        void scrambleWords();
        void displayBoard();
        void displaySolution();
        void outputToFile(char[]);
        bool nextToClear(char, int, int);

    private:
        vector<vector<char> > grid;
        vector<string> words;
        vector<string> clues;

};

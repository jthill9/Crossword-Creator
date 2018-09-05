// Jack Hill
// CSE 20311
// Fund Comp
// Lab 9
// Due 11/21/17
// board.h
// Implements the Board class and defines all of its methods

#include "board.h"
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<cstdlib>
#include<ctime>

using namespace std;

const int boardSize = 15;

// Default constructor that sets all the spaces in the grid to '#'
Board::Board()
{
    vector<char> cols (boardSize);
    fill(cols.begin(), cols.end(), '#');
    for(int i = 0; i < boardSize; i++)
    {
        grid.push_back(cols);
    }
}

// Destructor for Board class
Board::~Board()
{  }

// Lets the user add a word to the front of the vector holding all the words that are going
// to be used in the crossword
void Board::setWord(string str)
{
    words.insert(words.begin(), str);
    sortWord();
}

// Moves the word at the beginning however far back in the vector it needs to go
// based on its size, with the larger words staying in the front. Only needs to
// act on one word since it is called after every new word is added to the front
// of the vector
void Board::sortWord()
{
    if(words.size() > 1)
    {
        for(int i = 0; i < words.size() - 1; i++)
        {
            string temp;
            if(words[i].length() < words[i+1].length())
            {
                temp = words[i];
                words[i] = words[i+1];
                words[i+1] = temp;
            }
        }
    }
}

// Loads the words in the vector into the grid used by the crossword
void Board::loadBoard()
{
    vector<string> cpy = words;
    if(cpy.size())
    {
        int lenFirst = cpy[0].length();
        int middleBoard = (grid[0].size())/2;
        int startingPt = middleBoard - lenFirst/2;

        string clue = "(" + to_string(startingPt + 1) + ", " + to_string(middleBoard + 1) + ")  " + "across: ";
        clues.push_back(clue);
        
        //Puts the first word horizontally in the middle of the crossword
        for(int i = 0; i < lenFirst; i++)
        {
            grid[middleBoard][startingPt + i] = (cpy[0])[i];
        }
        cpy.erase(cpy.begin());
    }
    
    // Plays until all the words have been used and deleted or until a word
    // cannot be played and the loop is broken
    while(cpy.size() != 0)
    {
        int startNumWords = cpy.size();
        int wordLength = cpy[0].length();
        // Nested for loop that goes through every space on the grid
        for(int i = 0; i < boardSize; i++)
        {
            bool played = false;
            for(int j = 0; j < boardSize; j++)
            {
                // Keeps track of how many available plays there are at a space
                // and where they are position-wise in the word that is being
                // played
                vector<int> plays;
                // Sees if the whole board has been checked and deletes the
                // current word if it has
                if(i==boardSize && j==boardSize) cpy.clear();
                playHere(cpy[0], i, j, plays);
                
                // Gets called if there is at least one play
                if(plays.size())
                {
                    char dir = direction(cpy[0], i, j);
                    while(plays.size())
                    {
                        bool canPlay = true;
                        if(!nextToClear(dir, i, j)) canPlay = false;
                        // Switched on the direction the word can be played and
                        // play it that way
                        switch(dir)
                        {
                            case 'h':
                            {
                                int spacesLeft = plays[0];
                                int spacesRight = wordLength - plays[0];
                                
                                for(int a = 2; a <= spacesLeft; a++)
                                {
                                    if(objectAdjacent(i, j - a)) canPlay = false;
                                }
                                for(int a = 2; a <= spacesRight; a++)
                                {
                                    if(objectAdjacent(i, j + a)) canPlay = false;
                                }

                                if(canPlay)
                                {
                                    playHorizontal(cpy[0], i, j -spacesLeft);
                                    // 1 added to the clue so it starts at 1
                                    // instead of 0
                                    string clue = "(" + to_string(j - spacesLeft + 1) + ", " + to_string(i + 1) + ")  " + "across: ";
                                    clues.push_back(clue);
                                    plays.clear();
                                    cpy.erase(cpy.begin());
                                    played = true;
                                }
                                else
                                    plays.erase(plays.begin());
                                break;
                            }
                            case 'v':
                            {
                                int spacesUp = plays[0];
                                int spacesDown = wordLength - plays[0];
                                for(int a = 2; a <= spacesUp; a++)
                                {
                                    if(objectAdjacent(i - a, j)) canPlay = false;
                                }
                                for(int a = 2; a <= spacesDown; a++)
                                {
                                    if(objectAdjacent(i + a, j)) canPlay = false;
                                }
                                if(canPlay)
                                {
                                   playVertical(cpy[0], i - spacesUp, j);
                                   // 1 added to clue so it starts at 1 instead
                                   // of 0
                                   string clue = "(" + to_string(j + 1) + ", " + to_string(i - spacesUp + 1) + ")  " + "down: ";
                                   clues.push_back(clue);
                                   plays.clear();
                                   cpy.erase(cpy.begin());
                                   played = true;
                                }
                                else
                                    plays.erase(plays.begin());
                                break;
                            }
                        }
                    }
                }
                if(played) break;
            }
            if(played) break;
        }
        if(startNumWords == cpy.size())
        {
            cout << "Sorry, not all the words could be played." << endl;
            break;
        }
    }
}

// Checks to see if the space next to a word is clear of other words next to it
bool Board::nextToClear(char dir, int i, int j)
{
    switch(dir)
    {
        case 'h':
            if(grid[i][j-2] != '#' || grid[i+1][j-1] != '#' || grid[i-1][j-1] != '#' || grid[i][j+2] != '#' || grid[i+1][j+1] != '#' || grid[i-1][j+1] != '#') return false;
            break;
        
        case 'v':
            if(grid[i+2][j] != '#' || grid[i+1][j-1] != '#' || grid[i+1][j+1] != '#' || grid[i-2][j] != '#' || grid[i-1][j-1] != '#' || grid[i-1][j+1] != '#') return false;
            break;
    }
    return true;
}

// Check to see if a string can be played through a certain point and returns
// all of the positions in the string that it could fit there
void Board::playHere(string str, int xco, int yco, vector<int>& positions)
{
    for(int i = 0; i < str.length(); i++)
    {
        if(str[i] == (grid[xco][yco]))
        {
            positions.push_back(i);
        }
    }
}

// Returns 'v' if the word can be played vertically or 'h' if the word can be
// played horizonatally

char Board::direction(string str, int xco, int yco)
{
    if( ((grid[xco + 1][yco] != '#') || (grid[xco - 1][yco] != '#')) && (grid[xco][yco - 1] == '#' && grid[xco][yco + 1] == '#') )
        return 'h';
    else
        return 'v';
}

// Checks to see if there are letters next to a given point in the grid or if a
// spot is off the grid
bool Board::objectAdjacent(int xco, int yco)
{
    if(xco < 0 || xco >= boardSize || yco < 0 || yco >= boardSize)
        return true;
    
    if(xco != 0)
        if(grid[xco - 1][yco] != '#') return true;
    if(xco != (boardSize - 1))
        if(grid[xco + 1][yco] != '#') return true;
    if(yco != 0)
        if(grid[xco][yco - 1] != '#') return true;
    if(yco != (boardSize - 1))
        if(grid[xco][yco + 1] != '#') return true;

    return false;
}

// Plays a word vertically on the board
void Board::playVertical(string str, int xco, int yco)
{
    int length = str.length();

    for(int i = 0; i < length; i++)
    {
        grid[xco + i][yco] = str[i];
    }
}

// Plays a word horizontally on the board
void Board::playHorizontal(string str, int xco, int yco)
{
    int length = str.length();

    for(int i = 0; i < length; i++)
    {
        grid[xco][yco + i] = str[i];
    }
}

// Displays the crossword with pound signs making up the board everywhere where
// there is no letter and blank spaces where the letters are
void Board::displayBoard()
{
    for(int i = 0; i < boardSize; i++)
    {
        for(int j = 0; j < boardSize; j++)
        {
            if(j==0) cout << endl;
            if(grid[i][j] != '#') 
                cout << "  ";
            else
                cout << grid[i][j] << " ";
        }
    }

    cout << endl;
}

// Displays the solution for the crossword with periods where there are no
// letters
void Board::displaySolution()
{
    for(int i = 0; i < boardSize; i++)
    {
        for(int j = 0; j < boardSize; j++)
        {
            if(j==0) cout << endl;
            if(grid[i][j] == '#') 
                cout << ". ";
            else
                cout << grid[i][j] << " ";
        }
    }

    cout << endl;
}

// Outputs the clues for the words, which consists of a starting point,
// direction, and scrambled up word
void Board::scrambleWords()
{
    srand(unsigned(time(0)));
    cout << "Clues:\n\n";
    for(int i = 0; i < clues.size(); i++)
    {
        cout << clues[i];
        random_shuffle(words[i].begin(), words[i].end());
        cout << words[i] << endl;
    }
}

// Outputs everything that is usually displayed to a text file
void Board::outputToFile(char filename[])
{
    // Section to output the solution

    ofstream ofs;
    ofs.open(filename);

    ofs << "Solution:\n";

    for(int i = 0; i < boardSize; i++)
    {
        for(int j = 0; j < boardSize; j++)
        {
            if(j==0) ofs << endl;
            if(grid[i][j] == '#') 
                ofs << ". ";
            else
                ofs << grid[i][j] << " ";
        }
    }

    ofs << endl;

    // Section to output the actual puzzle

    ofs << "\n\nCrossword Puzzle:\n\n";

    for(int i = 0; i < boardSize; i++)
    {
        for(int j = 0; j < boardSize; j++)
        {
            if(j==0) ofs << endl;
            if(grid[i][j] != '#') 
                ofs << "  ";
            else
                ofs << grid[i][j] << " ";
        }
    }

    ofs << endl;

    // Section to output the clues for each word, including where it starts, its direction, and the scrambled up word 

    srand(unsigned(time(0)));
    ofs << "Clues:\n\n";
    for(int i = 0; i < clues.size(); i++)
    {
        ofs << clues[i];
        random_shuffle(words[i].begin(), words[i].end());
        ofs << words[i] << endl;
    }
}


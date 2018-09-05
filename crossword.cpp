// Jack Hill
// CSE 20311 Fund Comp
// Lab 9
// Due 11/21/17
// crossword.cpp
// This is the main function that takes in input and makes a crossword puzzle
// out of a list of words

#include "board.h"
#include<fstream>
#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    Board crossword = Board();
    
    // Interactive mode, used when there is only the executable called in the
    // command line
    if(argc == 1)
    {
        string word;
        cout << "Enter a list of words:\n";
        // Reads in words until the user enters a period
        cin >> word;
        while(word != ".")
        {
            for(int i = 0; i < word.length(); i++)
            {
                word[i] = toupper(word[i]);
            }
            crossword.setWord(word);
            cin >> word;
        }

        crossword.loadBoard();
        cout << "\nSolution:\n\n";
        crossword.displaySolution();
        cout << "\n\nCrossword Puzzle:\n\n";
        crossword.displayBoard();
        cout << endl;
        crossword.scrambleWords();
        
        return 1;    
    }

    // Reads from a file but has a standard output to the user
    if(argc == 2)
    {
        ifstream ifs;
        ifs.open(argv[1]);
        
        string word;
        
        // Reads in all the words from a file
        ifs >> word;
        while(word != ".")
        {
            for(int i = 0; i < word.length(); i++)
            {
                word[i] = toupper(word[i]);
            }
            crossword.setWord(word);
            ifs >> word;
        }

        crossword.loadBoard();
        cout << "\nSolution:\n\n";
        crossword.displaySolution();
        cout << "\n\nCrossword Puzzle:\n\n";
        crossword.displayBoard();
        cout << endl;
        crossword.scrambleWords();
        cout << endl;
    }

    // Reads from a file and sends the output to a different file
    if(argc == 3)
    {
        ifstream ifs;
        ifs.open(argv[1]);
        
        string word;
        
        // Reads in all the words from a file
        while(ifs >> word)
        {
            for(int i = 0; i < word.length(); i++)
            {
                word[i] = toupper(word[i]);
            }
            crossword.setWord(word);
        }

        crossword.loadBoard();
        crossword.outputToFile(argv[2]);
        
        return 1;
    }

    // Handles the case when more than three arguments passed from the command
    // line
    if(argc > 3)
    {
        cout << "Too many command line arguments.\n";
    }


    return 1;
}

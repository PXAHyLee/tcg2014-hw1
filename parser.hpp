#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "board.hpp"

using std::string;
using std::fstream;
using std::cout;
using std::endl;
using std::ios;

class Parser {
    public:
        Parser(const string& f) : filename(f)
        {
            input_file.open(filename, ios::in);
            if(!input_file.is_open())
            {
                cout << __PRETTY_FUNCTION__ << ": not opened!" << endl;
                exit(1);
            }
        }
        ~Parser()
        {
            input_file.close();
        }
        Board getBoardFromFile();
    private:
        string filename;
        fstream input_file;
};

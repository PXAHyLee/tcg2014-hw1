#include "common.h"
#include "searchalgorithm.hpp"
#include "parser.hpp"

using std::string;
string getOutputFilename(const string& inputFilename)
{
    string ending(".out");
    int i = 0;

    while(inputFilename[i] != '/')
        i++;
    i++;
    return string("sol/") + string(inputFilename, i) + ending;
}

int writeSolution(const char* filename)
{
    return 1;
}

int main()
{
    vector<string> filenames;
    filenames.reserve(8);

    /* manually add the filename */
    filenames.emplace_back("input/1");
    filenames.emplace_back("input/3");
    filenames.emplace_back("input/4");
    filenames.emplace_back("input/a179");
    filenames.emplace_back("input/b2");
    filenames.emplace_back("input/e314");

    auto it = filenames.begin();

    while(it != filenames.end())
    {
        Parser parser(*it);
        Board board(parser.getBoardFromFile());
        fstream output_file;

        output_file.open(getOutputFilename(*it), ios::out | ios::trunc);
        if(!output_file.is_open())
        {
            cout << "Output file " << getOutputFilename(*it) <<
                " fails to open." << endl;
            exit(1);
        }

        output_file << "Initial Board:" << endl;
        output_file << board;

        SearchAlgorithm* algo = new BFSWithVistedStatesCheck(board,
                output_file);
        Board::setAlgorithm("BFS - Checking Visited states");
        output_file << Board::algorithm << endl;
        algo->solveProblem();

        SearchAlgorithm* algo2 = new AStar(board, output_file);
        Board::setAlgorithm("AStar - Checking Visited states");
        output_file << Board::algorithm << endl;
        algo2->solveProblem();

        ++it;
        output_file.close();
        delete algo;
        delete algo2;
    }
    return 0;
}


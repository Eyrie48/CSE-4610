#include "table.h"
#include "filesys.h"
#include "block.cpp"
#include <sstream>
#include <cstdlib>
using namespace std;

Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile):Filesys(diskname, numberofblocks, blocksize)
{
    this->flatfile = flatfile;
    this->indexfile = indexfile;
}

int Table::build_table(string input_file)
{
    int code = newfile(indexfile);
    code = newfile(flatfile);
    ostringstream ostream; //create indexfile
    ifstream infile;
    infile.open(input_file.c_str());

    string inputline; 
    setline(infile, inputline);

    while(infile.good())
    {
        string key = inputline.substr(0,5); 
        string rest = inputline.substr(5);

        vector<string> blocks = block(inputline, getblocksize());

        //could be int or string 
        int blockid = addblock(flatfile, blocks[0]);

        ostream << key << " " << blockid << " ";
        getline(infile, inputline);
    }

    ostream << "XXXXX" << " " << 0 << " ";

    string buffer = ostream.str();
    vector<string> iblocks = block(buffer, getblocksize());

    for(int i = 0; i < iblocks.size(); i++)
    {
        //could be blocks or iblocks
        addblock(indexfile, iblocks[i]);
    }

    return 1;
}
#include "table.h"
#include "filesys.h"
#include "block.h"
#include "shell.h"
#include "sdisk.h"
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
    getline(infile, inputline);
    
    while(infile.good())
    {
        string key = inputline.substr(0,5); 
        string rest = inputline.substr(5);
        //string rest = inputline.substr(key.length());

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

    return 0;
}

int Table::indexSearch(string value)
{
    string buffer; 
    int blockid= getfirstblock(indexfile);
    //change
    string bigBuffer;
    ///
    while(blockid != 0)
    {
        /*
        string b;
        readblock(indexfile, blockid, b);
        buffer += b;
        blockid = nextblock(indexfile, blockid);
        */
       string buffer; 
       int error = readblock(indexfile, blockid, buffer);
       bigBuffer += buffer;
       blockid = nextblock(indexfile, blockid);
    }


    istringstream istream;
    //istream.str(buffer);
    //change
    istream.str(bigBuffer);
    //
    string k;
    int b;
    istream >> k >> b;
    while(k != "XXXXX")
    {
        if(k == value)
        {
            //return b;
            return b;
            //return 0;
            //return -1; 
        }

        istream >> k >> b; 
    }

    return 0;
}

int Table::search(string value)
{
    int ecode = indexSearch(value);
    if(ecode == 0)
    {
        cout << "record not found" << endl;
        return 0;
    }
    else
    {
        string buffer;
        ecode = readblock(flatfile, ecode, buffer);
        cout << buffer << endl;
        return 1;
    }
}
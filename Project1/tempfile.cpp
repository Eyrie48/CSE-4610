
#include <sstream>
#include "sdisk.h"

/*
string buffer = "1 2 3 4 5 6";

istringstream istream;

istream.str(buffer); //loads buufer to istream

int x, y;
istream>>x>>y; // x=1 y=2

ostringstream ostream;

ostream<< "Hello World";

string buffer = ostream.str();
*/

////////////////////////////////////////////////////////////////////////////
vector<string> block(string s, int b)//   s-buffer , b-blocksize
{
    vector<string> blocks;
    int numberofblocks;

    if(s.length()%b==0){
        numberofblocks=s.length()/b;
    }
    else{
        numberofblocks=s.length()/b+1;
    }

    string tempblock;
    for(int i=0; i < numberofblocks; i++)
    {
        tempblock =s.substr(b*i, b);
        blocks.push_back(tempblock);
    }

    //add #'s to 
    int lastblock = blocks.size()-1;
    for(int i = blocks[lastblock].length(); i < b; i++)
    {
        blocks[lastblock] += "#"; 
    }

    return blocks;
}







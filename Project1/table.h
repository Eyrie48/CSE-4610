#include "sdisk.h"
#include "filesys.h"
#ifndef TABLE_H
#define TABLE_H

#include <sstream>
#include <cstdlib>
using namespace std;

class Table : public Filesys
{
    public :
    Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile);
    int build_table(string input_file);
    int search(string value);

    private :
    string flatfile;
    string indexfile;

    //int numberofrecords;
    //Filesys filesystem;
    int indexSearch(string value);
};

#endif
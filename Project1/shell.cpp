#include "shell.h"

Shell::Shell(string filename, int numberofblocks, int blocksize)
{

}

int Shell::dir()
{

}// lists all files
int Shell::add(string file)
{
    int code =getfirstblock(file);
    if(code >= 0)
    {
        cout << "File exists";
        return 0;
    }
    code = newfile(filename);
    if(code ==0)
    {
        cout << "failed";
        return 0;
    }
    vector<string> blocks = block(buffer, getblocksize());
    for(int i = 0; i < blocks.size(); i++)
    {
        code = addblock(filename, blocks[i]);
        if(code == 0)
        {
            cout << "error occured";
            return 0;
        }
    }
    return 1;
}
int del(string file)
{
    int block = getfirstblock(file);
    while(block != 0)
    {
       int code = delblock(file, block);
       block = getfirstblock(file);
    }    
    rmfile(file);

}// deletes the file
int Shell::type(string file)
{

}//lists the contents of file
int Shell::copy(string file1, string file2)
{
    int block = getfirstblock(file1);
    int code = newfile(file2);
    while(block != 0)
    {
        string buffer;
        readblock(file1, block, buffer);
        addblock(file2, buffer);
        block = nextblock(file1, block);
    }

    return 1;
}//copies file1 to file2
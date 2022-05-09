#include "shell.h"

Shell::Shell(string filename, int numberofblocks, int blocksize):Filesys(filename, numberofblocks, blocksize)
{
    //
}

int Shell::dir()
{
    vector<string> flist = ls();
    for (int i=0; i< flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
    
    return 1;
}// lists all files

int Shell::add(string file, string buffer)
{

    
    int code =getfirstblock(file);
    if(code >= 0)
    {
        cout << "File exists";
        return 0;
    }
    code = newfile(file);
    if(code == 0)
    {
        cout << "failed";
        return 0;
    }

    //not sure added buffer for error
    ///////////////////////
    
    vector<string> blocks = block(buffer, getblocksize());
    for(int i = 0; i < blocks.size(); i++)
    {
        code = addblock(file, blocks[i]);
        if(code == 0)
        {
            cout << "error occured";
            return 0;
        }
    }
    
    return 1;
}

int Shell::del(string file)
{
    int block = getfirstblock(file);
    while(block > 0)
    {
       //int code = delblock(file, block);
       int code = delblock(file, block);
       block = getfirstblock(file);
    }    
    rmfile(file);
    return 1;

}// deletes the file

int Shell::type(string file)
{
    //edit for type 
    //int block = getfirstblock(file1);
    //int code = newfile(file2);
    int block = getfirstblock(file);
    string buffer;
    while(block != 0)
    {
        
        string temp; 
        int error = readblock(file, block, buffer);
        //addblock(file2, buffer);
        buffer += temp;
        block = nextblock(file, block);
    }

    cout << buffer << endl;
    cout << buffer.length() << endl;
    return 1;
}//lists the contents of file

int Shell::copy(string file1, string file2)
{
    int block = getfirstblock(file1);
    int code = newfile(file2);
    string buffer;
    while(block != 0)
    {
        string temp;
        code = readblock(file1, block, temp);
        buffer += temp;
        addblock(file2, buffer);
        block = nextblock(file1, block);
    }

    return 1;
}//copies file1 to file2

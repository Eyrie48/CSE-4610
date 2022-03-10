#include "filesys.h"
//#include "sdisk.h"

Filesys::Filesys(string filename, int numberofblocks, int blocksize):Sdisk(filename, numberofblocks, blocksize)
{
    rootsize = getblocksize() / 12;
    fatsize = (getnumberofblocks() * 6) / getblocksize() + 1;

    string buffer;
    int code = getblock(1, buffer);
    if(code == 0)
    {
        cout << "error occures" << endl;
        return;
    }

    if(buffer[0] == '#')
    {
        buildfs();
        cout << "Building file system" << endl;
        return;
    }
    else
    {
        readfs();
        return;
    }
}

int Filesys::buildfs()
{
    //build the root
    for(int i = 0; i < rootsize; i++)
    {
        filename.push_back("XXXXX");
        firstblock.push_back(0);
    }

    //build FAT
    fat.push_back(fatsize+2);
    fat.push_back(-1);

    for(int i = 0; i < fatsize; i++)
    {
        fat.push_back(-1);
    }

    for(int i = fatsize+2; i < getnumberofblocks(); i++)
    {
        fat.push_back(i + 1);
    }

    fat[fat.size()-1] = 0;

    return fssynch();
}

int Filesys::fssynch()
{
    ostringstream ostream;
    for(int i = 0; i < rootsize; i++)
    {
        ostream << filename[i] << " " << firstblock[i] << " ";
        
    }
    
    string buffer1 = ostream.str();
    
    //unable to define getblock?
    vector<string> blocks1 = block(buffer1, getblocksize());
    putblock(1, blocks1[0]);

    ostringstream ostream2; 
    for(int i = 0; i < fat.size(); i++)
    {
        ostream2 << fat[i] << " "; 
    }

    string buffer2 = ostream2.str();
    vector<string> blocks2 = block(buffer2, getblocksize());
    for(int i = 0; i < blocks2.size(); i++)
    {
        putblock(2+1, blocks2[i]);
    }

}

int Filesys::readfs(){
    string buffer1, buffer2; 
    getblock(1, buffer1);
    string tempbuffer;
    for(int i = 0; i < fatsize; i++)
    {
        getblock(2+i, tempbuffer);
        buffer2 += tempbuffer;
        tempbuffer.clear();
    }

    istringstream istream1, istream2;
    istream1.str(buffer1);
    istream2.str(buffer2);

    for(int i = 0; i < rootsize; i++)
    {
        string f;
        int n;
        //vs code error? 
        istream1 << f << n;
        filename.push_back(f);
        firstblock.push_back(n);
    }

    int k;
    for(int i = 0; i < getnumberofblocks(); i++)
    {
        istream2 << k;
        fat.push_back(k);
    }
    return 1;
    
}

int Filesys::fsclose()
{

}

int Filesys::newfile(string file)
{

}

int Filesys::rmfile(string file)
{

}

int Filesys::getfirstblock(string file)
{

}

int Filesys::addblock(string file, string block)
{

}

int Filesys::delblock(string file, int blocknumber)
{

}

int Filesys::readblock(string file, int blocknumber, string& buffer)
{

}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{

}

int Filesys::nextblock(string file, int blocknumber)
{

}
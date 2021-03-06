#include "filesys.h"
#include "sdisk.h"
#include <sstream>
#include <cstdlib>
using namespace std;

#include "block.h"

//#include "block.cpp"

Filesys::Filesys(string filename, int numberofblocks, int blocksize):Sdisk(filename, numberofblocks, blocksize)
{
    //change to 10
    rootsize = getblocksize() / 12;
    //change to 5
    fatsize = (getnumberofblocks() * 6) / getblocksize() + 1;

    string buffer;
    int code = getblock(0, buffer);
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
    fssynch();
}

int Filesys::buildfs()
{
	//change from files 
    ostringstream ostream;
	
    //build the root
    for(int i = 0; i < rootsize; i++)
    {
        filename.push_back("XXXXX");
        firstblock.push_back(0);
	//change from file
	    ostream << "XXXXX" << " " << 0 << " ";
    }


    string buffer = ostream.str();
    vector<string>blocks = block(buffer, getblocksize());
    putblock(1, blocks[0]);
    ostringstream ostream2;
    
    //build FAT
    fat.push_back(fatsize+2);
    fat.push_back(1);

    for(int i = 0; i < fatsize; i++)
    {
        fat.push_back(-1);
    }

    for(int i = fatsize + 2; i < getnumberofblocks(); i++)
    {
        fat.push_back(i + 1);
    }
	
    fat[fat.size()-1] = 0;
    
    //change from file
    //ostringstream ostream2;
    for(int i = 0; i < fat.size(); i++)
    {
	ostream2 << fat[i] << " ";
    }
     
    
    string buffer2 = ostream2.str();
    vector<string>blocks2 = block(buffer2, getblocksize());
    
    for(int i =0; i < blocks2.size(); i++)
    {
        putblock(2+i, blocks2[i]);
    }
	/////////////////////////
    return 0;
}

int Filesys::fssynch()
{
    ostringstream ostream;
    //ostream << endl;
    for(int i = 0; i < rootsize; i++)
    {
        ostream << filename[i] << " " << firstblock[i] << " ";
        
    }
    string buffer1 = ostream.str();   
    vector<string> blocks1 = block(buffer1, getblocksize());
    putblock(1, blocks1[0]);
    ostringstream ostream2; 
    //fat.size() or getnumberofblocks()
    for(int i = 0; i < fat.size(); i++)
    {
        ostream2 << fat[i] << " "; 
    }

    
    string buffer2 = ostream2.str();
    vector<string> blocks2 = block(buffer2, getblocksize());

    for(int i = 0; i < blocks2.size(); i++)
    {
        putblock(2+i, blocks2[i]);
    }
    //change from other
    return 0;
}

int Filesys::readfs(){

    string buffer1, buffer2; 
    getblock(1, buffer1);
    

    /**/

    istringstream istream1, istream2;
    istream1.str(buffer1);
    istream2.str(buffer2);

    for(int i = 0; i < rootsize; i++)
    {
        string f;
        int n;
        //vs code error? 
        //before error << f << 
        istream1 >> f >> n;
        filename.push_back(f);
        firstblock.push_back(n);
    }

    for(int i = 0; i < fatsize; i++)
    {
        string tempbuffer;
        getblock(2+i, tempbuffer);
        buffer2 += tempbuffer;
        tempbuffer.clear();
    }

    
    for(int i = 0; i < getnumberofblocks(); i++)
    {   
        int k;
        //before error << f <<
        istream2 >> k;
        fat.push_back(k);
    }
    return 0;
}

int Filesys::fsclose()
{
    //not sure
    fssynch();
    //changed 
    return 0;
    
}

int Filesys::newfile(string newname)
{
    //                 rootsize or filename.size()
    for(int i = 0; i < filename.size(); i++)
    {
        if(filename[i] == newname)
        {
            cout << "file exist" << endl;
            //change from other 
            return 0;
        }
    }

    for(int i =0; i < filename.size(); i++)
    {
        //filename
        if(filename[i] <= "XXXXX")
        {
            filename[i] = newname; 
            //change from other
            firstblock[i] = 0;
            fssynch();
            //change from other 
            return 1;
        }
    }

    cout << "No space in Root" << endl;
    return 0;
}

int Filesys::rmfile(string file)
{
    for(int i = 0; i < filename.size(); i++)
    {
        if( filename[i] == file && firstblock[i] == 0)
        {
            filename[i] = "XXXXX";
            firstblock[i] = 0;
            fssynch();
            return 1;
        }
    }

    return 1;
}

//My code for getfirstblock 
int Filesys::getfirstblock(string file)
{
    //rootsize or filename.size()
    for(int i = 0; i < filename.size(); i++)
    {
        if(file == filename[i])
        {
            //otherwise firstblock 
            //firstblock[i]; ??
            //firstblock;
            return firstblock[i];
        }     
        //return -1 if does not exist 
        //cout << file << " does not exist" << endl;   
    }
    //cout << file << " does not exist" << endl;
    return -1;
}

//addblock("file1, buffer")
int Filesys::addblock(string file, string block)
{
    int blockid = getfirstblock(file);
    if(blockid == -1)
    {
        //cout << file << " does not exist" << endl;
        return 0;
    }
    
    int allocate = fat[0];
    if(allocate == 0)
    {
        cout << "no space on disk" << endl;
	//change from file
        return 0;
    }
    //change to else statement from file 
    //else
    //{
    fat[0] = fat[fat[0]];
    fat[allocate] = 0;
    //} 
    /////
    if(blockid == 0)
    {
        for(int i = 0; i < rootsize; i++)
        {
            if(filename[i] == file)
            {
                firstblock[i] = allocate;
                //change 
            }
        }
    }
    else
    {
        int b = blockid;
        while(fat[b] != 0)
        {
            b = fat[b];
        }

        fat[b] = allocate;
    }
    putblock(allocate, block);
    fssynch();
    //cout << "Finish adding..." << endl;
    //putblock(allocate, block);
    //return 1;
    return allocate;
}

bool Filesys::checkblock(string file, int block)
{
    //check if block number block is in file 
    int blockid = getfirstblock(file);
    if(blockid == -1)
    {
        cout << "No such file" << endl;
        return false;
    }
    while(blockid != 0)
    {
        if(blockid == block)
        {
            return true;
        }
        blockid = fat[blockid];
    }
    //added for void
    return false;
}

int Filesys::delblock(string file, int blocknumber)
{
    if(!checkblock(file, blocknumber))
    {
        cout << "Can't do it" << endl;
        return 0;
    }

    int block = getfirstblock(file);
    if(block == blocknumber)
    {
        for(int  i = 0; i < filename.size(); i++)
        {
            if(filename[i] == file)
            {
                firstblock[i] = fat[blocknumber];
                break;
            }
        }
    }
    else
    {
        while(fat[block] != blocknumber)
        {
            block = fat[block];
        }
        fat[block] = fat[blocknumber];
    }

    fat[blocknumber] = fat[0];
    fat[0] = blocknumber;
    fssynch();
    //cout << "Deleted the block" << endl;

    return 1;
}

int Filesys::readblock(string file, int blocknumber, string& buffer)
{
    if(checkblock(file, blocknumber))
    {
        getblock(blocknumber, buffer);
        return 1;
    }
    else
    {
        cout << "Could not perform..." << endl;
        return 0;
    }
}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
    if(checkblock(file, blocknumber))
    {
        putblock(blocknumber, buffer);
        return 1;
    }
    else
    {
        cout << "Could not perform..." << endl;
        return 0;
    }
}

int Filesys::nextblock(string file, int blocknumber)
{
    if(checkblock(file, blocknumber))
    {
        return fat[blocknumber];
    }
    else 
    {
        cout << "Could not perform..." << endl;
        return 0;
    }
}

/*
vector<string> Filesys::block(string s, int b)//   s-buffer , b-blocksize
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
*/

vector<string> Filesys::ls()
{ 
    vector<string> flist;
    for (int i=0; i<filename.size(); i++)
    {
        if (filename[i] != "XXXXX")
        {
           flist.push_back(filename[i]);
        }
    }
    
    return flist;
}

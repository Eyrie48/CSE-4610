#include "filesys.h"
//#include "sdisk.h"
#include "block.cpp"

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
        //before error << f << 
        istream1 >> f >> n;
        filename.push_back(f);
        firstblock.push_back(n);
    }

    int k;
    for(int i = 0; i < getnumberofblocks(); i++)
    {
        //before error << f <<
        istream2 >> k;
        fat.push_back(k);
    }
    return 1;
    
}

int Filesys::fsclose()
{

}

int Filesys::newfile(string newname)
{
    //                 rootsize
    for(int i = 0; i < filename.size(); i++)
    {
        if(filename[i] == newname)
        {
            cout << "file exist" << endl;
        }
    }

    for(int i =0; i < rootsize; i++)
    {
        //filename
        if(filename[i] <= "XXXXX")
        {
            filename[i] = newname; 
            return 1;
        }
    }

    cout << "No space in Root" << endl;
    return 0;
}

int Filesys::rmfile(string file)
{

}

//My code for getfirstblock 
int Filesys::getfirstblock(string file)
{
    for(int i = 0; i < rootsize; i++)
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
    return -1;
}

//addblock("file1, buffer")
int Filesys::addblock(string file, string block)
{
    int blockid = getfirstblock(file);
    if(blockid == -1)
    {
        cout << file << " does not exist" << endl;
        return 0;
    }
    
    int allocate = fat[0];
    if(allocate == 0)
    {
        cout << "no space on disk" << endl;
        return 0;
    }

    fat[0] = fat[fat[0]];
    fat[allocate] = 0;

    if(blockid == 0)
    {
        for(int i = 0; i < rootsize; i++)
        {
            if(filename[i] == file)
            {
                firstblock[i] = allocate;
            }
        }
    }
    else
    {
        while(fat[blockid] != 0)
        {
            blockid = fat[blockid];
        }

        fat[blockid] = allocate;
    }

    fssynch();
    putblock(allocate, block);
    return 1;
}

bool Filesys::checkblock(string file, int block)
{
    //check if block number block is in file 
    int blockid = getfirstblock(file);
    if(blockid == -1)
    {
        return false;
    }
    while(blockid != 0)
    {
        if(blockid == block)
        {
            return true;
        }
        blockid == fat[blockid];
    }
}

int Filesys::delblock(string file, int blocknumber)
{

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
        return -1;
    }
}

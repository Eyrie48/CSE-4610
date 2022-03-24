#pragma once
#ifndef SDISK_H
#define SDISK_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;

class Sdisk
{
public:
Sdisk(string diskname, int numberofblocks, int blocksize);
int getblock(int blocknumber, string& buffer);
int putblock(int blocknumber, string buffer);
int getnumberofblocks(); // accessor function
int getblocksize(); // accessor function
private:
string diskname;        // file name of software-disk
int numberofblocks;     // number of blocks on disk
int blocksize;          // block size in bytes
};

#endif

/*
to compile on linux
cd proj1
$c++ -c sdisk.cpp source.cpp
$c++ -o proj1 sdisk.o source.o
./proj1
cat disk0.txt
*/

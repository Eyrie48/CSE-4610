#include "sdisk.h"
using namespace std;


Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)
{
	/*
	fstream f;
	ifstream file1;

	this->diskname = diskname;
	this->numberofblocks = numberofblocks;
	this->blocksize = blocksize;

	//might break the project later on for part 3, 4

	/*
	f.open("diskname", ios::in | ios::out);
	if (!f)
	{
		cout << "Error opening file" << endl;
		exit(0);
	}
	//

	//looking for a c_string
	file1.open(diskname.c_str());
	bool diskStatus = file1.good(); file1.close();
	if(diskStatus == true){
		return;
	}
	else{
		ofstream file2;
		file2.open(diskname.c_str());
		for(int i = 0; i < numberofblocks * blocksize; i++)
		{
			file2.put('#');
		}

		file2.close();
	}
		//create diskname;
	*/
	this->diskname = diskname;
	this->numberofblocks = numberofblocks;
	this->blocksize = blocksize;

	fstream input;
	input.open(diskname.c_str(), ios::in);

	if(input.good())
	{
		input.close();
		return;
	}
	else
	{
		input.close();
		ofstream outfile;
		outfile.open(diskname.c_str());
		for(int i = 0; i < numberofblocks * blocksize; i++)
		{
			outfile.put('#');
		}
		outfile.close();
		return;
	}
	
}

int Sdisk::getnumberofblocks()
{
	return this->numberofblocks;
}

int Sdisk::getblocksize()
{
	return this->blocksize;
}

int Sdisk::getblock(int blocknumber, string& buffer)
{
	/*fstream iofile;
	iofile.open(diskname.c_str(), ios::in | ios::out);
	buffer.clear();

	if(blocknumber >= 0 && blocknumber < blocksize * numberofblocks)
	{
		
		/*if (!iofile.is_open())
		{
			cout << "Could not getBlock" << endl;
			return 0;
		}
		//
		
		char c;
		iofile.seekg(blocknumber * blocksize);

		for (int i = 0; i < blocksize; i++)
		{
			iofile.get(c);
			buffer.push_back(c); 
		}

		cout << "Successfully accessed the file" << endl;
		return 1;
	}
	else
	{
		cout << "Blocknumber is 0 or blocknumber is less than blocksize * number of blocks" << endl;
		return 0;
	}*/
	//buffer.clear();
	if (blocknumber > (numberofblocks -1))
	{
		return 0;
	}
	
	ifstream input; 
	char temp;

	input.open(diskname.c_str());
		
	if(!input.is_open())
	{
		return 0;
	}

	input.seekg(blocknumber * blocksize);
	for(int i = 0; i < blocksize; ++i)
	{
		input.get(temp);
		buffer.push_back(temp);
	}

	input.close();
	return 1;
}

int Sdisk::putblock(int blocknumber, string buffer)
{
	

	//blocksize * numberofblocks
	if(blocknumber > (numberofblocks-1))
	{
		return 0;
	}
	//else
	//{
//		cout << "Blocknumber is 0 or blocknumber is less than blocksize * number of blocks" << endl;
//		return 0;
//	}
	fstream iofile;
	iofile.open(diskname.c_str(), ios::in | ios::out);//ios::binary final part of project
	if(!iofile.is_open())
	{ 
		return 0;
	}
	iofile.seekp(blocknumber * blocksize);

	for (int i = 0; i < buffer.length(); ++i)
	{
		iofile.put(buffer[i]);
	}

	iofile.close();
	//cout << "Successfully accessed the file" << endl;
	return 1;


	/*
	if (blocknumber > (numberofblocks -1))
	{
		return 0;
	}
	fstream output; 
	output.open(diskname.c_str(), ios::in | ios::out);
		
	if(!output.is_open())
	{
		return 0;
	}

	output.seekp(blocknumber * blocksize);

	for(int i = 0; i < buffer.length(); ++i)
	{
		output.put(buffer[i]);
	}

	output.close();
	return 1;
	*/
}

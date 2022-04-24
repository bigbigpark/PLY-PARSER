/**
 * @author Seongchang Park (scsc1125@gmail.com)
 * @date 2022-04-24 11:13
 */
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>


using namespace std;

enum openMode{ fileOpenMode_OUT = 0, fileOpenMode_IN = 1 };

class File
{
public:
	File(string path, openMode flag);
	~File();


protected:
	const openMode _mode;
	string     _path;
	fstream    _file;
};
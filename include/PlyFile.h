/**
 * @author Seongchang Park (scsc1125@gmail.com)
 * @date 2022-04-24 11:07
 */
#pragma once

#include <sstream>
#include <list>

#include <File.h>

#include <cmath>

using namespace std;


enum plyFormat{ binary_little_endian = 0, binary_big_endian = 1, ascii = 2 };
enum plyTypes{ float32 = 0, float64 = 1, uchar = 2, int32 = 3, otherxx = -1 };


class PlyFile : public File
{
public:
	PlyFile(string path, openMode flag);
	~PlyFile();
	
	void readFile(char*& points, int& pointSize, int& numPoints);
	void writeFile(char* points, int sizeOfPoints, int numPoints, list<string> properties, list<plyTypes> types);

	void makePoints(char* points, int& pointSize, int& numPoints);
	void generatePoints(char*& points, const int sizeOfPoints, const int numPoints, const int numProperties, const char* pointsIn);

	void displayInfos();
	void displayHeader();
	void copyHeader(string header);
	

	static const int READ_SIZE = 16000;
	static const int WRITE_SIZE = 16000;

private:
	void readHeader();
	void writeHeader();

public:
	string    _header;
	plyFormat _format;

	int       _propertyNum;
	string*   _propertyName;
	plyTypes* _propertyType;
	int*	  _propertySize;

	int   _numPoints;
	int   _pointSize;

  // This code will be deprecated
	float timestamp_;
};
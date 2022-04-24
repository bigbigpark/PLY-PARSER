/**
 * @author Seongchang Park (scsc1125@gmail.com)
 * @date 2022-04-24 14:34
 */
#include <PlyFile.h>

PlyFile::PlyFile(string path, openMode flag) : File(path, flag), _header(""), _format(binary_little_endian), 
_propertyNum(0), _propertyType(NULL), _propertySize(NULL), _propertyName(NULL), 
_numPoints(0), _pointSize(0)
{
	if (_mode == fileOpenMode_IN)
	{
		readHeader();
	}
}

PlyFile::~PlyFile()
{
	delete[] _propertyType;
	delete[] _propertySize;
	delete[] _propertyName;
}

void PlyFile::readHeader()
{
	// GET HEADER
	// ------------------------------------------------------------------------------------------
	string tmpStr = "";
	
	do
	{
		getline(_file, tmpStr);
		_header += tmpStr + "\n";
	} while (tmpStr.find("end_header") != 0);

	// PARSE HEADER
	// ------------------------------------------------------------------------------------------
	stringstream streamHeader(_header);
	string strTmp = "";
	list<plyTypes> typePptTmp;
	list<int>      sizePptTmp;
	list<string>   namePptTmp;

	while (!streamHeader.eof())
	{
		streamHeader >> strTmp;
		
		if (strTmp.compare("format") == 0)
		{
			streamHeader >> strTmp;
			if (strTmp.compare("binary_little_endian") == 0)      _format = binary_little_endian;
			else if (strTmp.compare("binary_big_endian") == 0)    _format = binary_big_endian;
			else if (strTmp.compare("ascii") == 0)                _format = ascii;	
		}

		if (strTmp.compare("element") == 0)
		{
			streamHeader >> strTmp;
			if (strTmp.compare("vertex") == 0) streamHeader >> _numPoints;
		}

		if (strTmp.compare("property") == 0)
		{
			_propertyNum++;
			streamHeader >> strTmp;
			if ((strTmp.compare("float32") == 0) | (strTmp.compare("float") == 0))
			{
				typePptTmp.push_back(float32);
				sizePptTmp.push_back(4);
			}
			else if ((strTmp.compare("float64") == 0) | (strTmp.compare("double") == 0))
			{
				typePptTmp.push_back(float64);
				sizePptTmp.push_back(8);
			}
			else if ((strTmp.compare("int") == 0))
			{
				typePptTmp.push_back(int32);
				sizePptTmp.push_back(4);
			}
			else if ((strTmp.compare("uchar") == 0))
			{
				typePptTmp.push_back(uchar);
				sizePptTmp.push_back(1);
			}
			else
			{
				typePptTmp.push_back(otherxx);
				sizePptTmp.push_back(4); // Default
			}

			streamHeader >> strTmp;
			namePptTmp.push_back(strTmp);
		}
	}

	// FILL PROPERTIES ARRAYS
	// ------------------------------------------------------------------------------------------
	_propertyType = new plyTypes[_propertyNum];
	_propertySize = new int[_propertyNum];
	_propertyName = new string[_propertyNum];

	for (int i(0); i < _propertyNum; i++)
	{
		_propertyType[i] = typePptTmp.front();
		_propertySize[i] = sizePptTmp.front();
		_propertyName[i] = namePptTmp.front();
		typePptTmp.pop_front();
		sizePptTmp.pop_front();
		namePptTmp.pop_front();

		_pointSize += _propertySize[i];
	}
}

void PlyFile::displayHeader()
{
}

void PlyFile::writeHeader()
{
	_header = "";

	_header += "ply";
	_header += "\n";


		_header += "format ";
		switch (_format)
		{
		case binary_little_endian:
		{
			_header += "binary_little_endian 1.0";
			break;
		}
		case binary_big_endian:
		{
			_header += "binary_big_endian 1.0";
			break;
		}
		case ascii:
		{
			_header += "binary_ascii 1.0";
			break;
		}
		}
		_header += "\n";


		_header += "element vertex ";
		_header += to_string(_numPoints);
		_header += "\n";


		for (int i(0); i < _propertyNum; i++)
		{
			_header += "property ";
			switch (_propertyType[i])
			{
			case float32:
			{
				_header += "float32 ";
				break;
			}
			case float64:
			{
				_header += "float64 ";
				break;
			}
			case uchar:
			{
				_header += "uchar ";
				break;
			}
			case int32:
			{
				_header += "int ";
				break;
			}
			}
			_header += _propertyName[i];
			_header += "\n";
		}


	_header += "end_header";
	_header += "\n";

	_file << _header;
}

void PlyFile::copyHeader(string header)
{
  _header = header;
}

void PlyFile::readFile(char*& points, int& pointSize, int& numPoints)
{
	switch (_format)
	{
	case binary_little_endian:
	{
		// ----- Allocate memory ------------------------------------------------
		if (points != 0)
		{
			delete[] points;
		}

		points = new char[(unsigned long long int)_pointSize*(unsigned long long int)_numPoints];
		unsigned long long int bufferSize = (unsigned long long int)_pointSize*(unsigned long long int)_numPoints;

		cout << "_pointSize: " << _pointSize << endl;
		cout << "_numPoints: " << _numPoints << endl;
		cout << "bufferSize: " << bufferSize << endl;

		// ----- Read raw data --------------------------------------------------
		unsigned long long int n = bufferSize / (unsigned long long int)READ_SIZE;
		unsigned long long int r = bufferSize % (unsigned long long int)READ_SIZE;
		cout << "n : " << n << endl;
		cout << "r : " << r << endl;


		for (unsigned long long int i(0); i < n; i++)
		{
			_file.read(points + i*(unsigned long long int)READ_SIZE, READ_SIZE);
		}

		_file.read(points + n*(unsigned long long int)READ_SIZE, r);

		numPoints = _numPoints;
		pointSize = _pointSize;

		break;
	}
	case binary_big_endian:
	{
		cout << "WARNING: function not implemented for binary big endian file" << endl;
		break;
	}
	case ascii:
	{
		cout << "WARNING: function not implemented for ascii file" << endl;
		break;
	}
	}	
}

void PlyFile::makePoints(char* points, int& pointSize, int& numPoints)
{
	// --------------------------------------------
  //              Allocate memory
  // --------------------------------------------
  if (points != 0) delete[] points;

  // For OS1-64ch
	_pointSize = 16; // x, y, z and timestamp per 4 bytes
	_numPoints = 65536; // e.g. OS1-64ch. If not, Check your LiDAR specification

  points = new char[(unsigned long long int)_pointSize*(unsigned long long int)_numPoints];
		unsigned long long int bufferSize = (unsigned long long int)_pointSize*(unsigned long long int)_numPoints;

	cout << "_pointSize: " << _pointSize << endl;
	cout << "_numPoints: " << _numPoints << endl;
	cout << "bufferSize: " << bufferSize << endl;

	// --------------------------------------------
  //              Write data
  // --------------------------------------------
	unsigned long long int n = bufferSize / (unsigned long long int)WRITE_SIZE;	//WRITE_SIZE = 16000
	unsigned long long int r = bufferSize % (unsigned long long int)WRITE_SIZE;
	cout << "n : " << n << endl;
	cout << "r : " << r << endl;

  for (unsigned long long int i(0); i < n; i++)
	{
		_file.write(points + i*(unsigned long long int)WRITE_SIZE, WRITE_SIZE);
	}

	_file.write(points + n*(unsigned long long int)WRITE_SIZE, r);

	pointSize = _pointSize;
	numPoints = _numPoints;
}

void PlyFile::generatePoints(char*& points, const int sizeOfPoints, const int numPoints, const int numProperties, const char* pointsIn)
{
	cout << "In generatePoints, numPoints: " << numPoints << "/ " << sizeof(float32) << endl;

	int loop_count = 0;
	// static float timestamp = 0.0; // [sec]

	// #############################################
	//									Hard Coded
	// #############################################
	// Generate Points , numPoints: 65536, buffer: 65536 * 16(sizeOfPoints)
	for (int i(0); i < numPoints; i++)
	{
		const int offset = sizeof(float32);
		const int offsetIn = 29; // sangam data -> 29 off set same as seadronix

		// x, y, z and timesatmp
		*((float *) (points + 4*i*offset +  0)) = *((float *) (pointsIn + i*offsetIn + 0));
		*((float *) (points + 4*i*offset +  4)) = *((float *) (pointsIn + i*offsetIn + 4));
		*((float *) (points + 4*i*offset +  8)) = *((float *) (pointsIn + i*offsetIn + 8));
		*((float *) (points + 4*i*offset + 12)) = timestamp_;

		// cout << " -- x, y, z and timestamp: \n";
		// cout << "  " << *((float *) (points + 4*i*offset +   0)) << "\n";
		// cout << "  " << *((float *) (points + 4*i*offset +   4)) << "\n";
		// cout << "  " << *((float *) (points + 4*i*offset +   8)) << "\n";
		// cout << "  " << *((float *) (points + 4*i*offset +  12)) << "\n";

		loop_count++;
	}

	// timestamp += 0.1;
	cout << endl;
	cout << "loop_count: " << loop_count << endl;
}

void PlyFile::writeFile(char* points, int sizeOfPoints, int numPoints, list<string> properties, list<plyTypes> types)
{
	// ----- Set properties -------------------------------------------------
	_format = binary_little_endian;
	_numPoints = numPoints;

	if (properties.size() != types.size())
	{
		cout << "Warning : mismatch between properties and types" << endl;
		return;
	}

	cout << "properties.size(): " << properties.size() << endl;
	cout << "types.size(): " << types.size() << endl;

	_propertyNum = (int) properties.size();
	_pointSize = 0;

	_propertyType = new plyTypes[_propertyNum];
	_propertySize = new int[_propertyNum];
	_propertyName = new string[_propertyNum];

	auto propIt = properties.begin();
	auto typesIt = types.begin();

	cout << "_propertyNum(): " << _propertyNum << endl;
	for (int i(0); i < _propertyNum; i++)
	{
		_propertyName[i] = *propIt;
		_propertyType[i] = *typesIt;


		if (_propertyType[i] == float32)
			_propertySize[i] = 4;
		if (_propertyType[i] == float64)
			_propertySize[i] = 8;
		if (_propertyType[i] == int32)
			_propertySize[i] = 4;
		if (_propertyType[i] == uchar)
			_propertySize[i] = 1;
		if (_propertyType[i] == otherxx)
			_propertySize[i] = 4; //default

		_pointSize += _propertySize[i];

		++propIt;
		++typesIt;
	}

	// ----- Write header ---------------------------------------------------
	writeHeader();

	// ----- Write points ---------------------------------------------------
	unsigned long long int bufferSize = (unsigned long long int)_pointSize*(unsigned long long int)_numPoints;
	cout << "_pointSize: " << _pointSize << endl;
	cout << "_numPoints: " << _numPoints << endl;
	cout << "bufferSize: " << bufferSize << endl;
	
	unsigned long long int n = bufferSize / (unsigned long long int)WRITE_SIZE;
	unsigned long long int r = bufferSize % (unsigned long long int)WRITE_SIZE;
	cout << "n : " << n << endl;
	cout << "r : " << r << endl;

	for (unsigned long long int i(0); i < n; i++)
	{
		_file.write(points + i*(unsigned long long int)WRITE_SIZE, WRITE_SIZE);
	}

	_file.write(points + n*(unsigned long long int)WRITE_SIZE, r);
}

void PlyFile::displayInfos()
{
	cout << "------------------------------------------------------" << endl;
	cout << " PLY File : " << _path << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "  - format     : " << _format << endl;
	cout << "  - num points : " << _numPoints << endl;
	cout << "  - properties : " << endl;
	for (int i(0); i < _propertyNum; i++)
	{
		cout << "     - " << _propertyName[i] << " :	" << _propertyType[i] << " |	" << _propertySize[i] << " bytes " << endl;
	}
	cout << "------------------------------------------------------" << endl << endl;
}
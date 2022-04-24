/**
 * @author Seongchang Park (scsc1125@gmail.com)
 * @date 2022-04-24 11:15
 */
#include <File.h>

File::File(string path, openMode flag) : _path(path), _mode(flag)
{
	switch (_mode)
	{
	case fileOpenMode_IN:
	{
		_file = fstream(_path, ios::in | ios::binary);
		break;
	}
	case fileOpenMode_OUT:
	{
		_file = fstream(_path, ios::out | ios::binary);
		break;
	}
	}

	if (!_file.good())
	{
		cout << "ERROR: can't open " << _path << endl;
	}
}

File::~File()
{
	_file.close();
}
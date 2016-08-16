#ifndef TEXTFILE_H
#define	TEXTFILE_H
#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::ofstream;
using namespace::std;

class TextFile
{
public:

	enum FileType
	{
		ANIMAL,
		CAR,
		MAP,
	};
	void ReadFile(const string FileName);
	void SaveMapObj(const string FileName);
	bool RemoveMapObj(const string FileName, float x , float y);

	void SetData(string type,float x, float y, float scale);

	TextFile(FileType ftype = ANIMAL);
	~TextFile();
private:
	float x;
	float y;
	float scale;
	string type;
};
#endif
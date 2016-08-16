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

	};
	void ReadFile(const string FileName);
	void WriteFile(const string FileName);


	void SetData(string type,float x, float y, float scale);

	void SetTest(float a, float b);

	string GetType();

	TextFile(FileType ftype = ANIMAL);
	~TextFile();
private:
	float x;
	float y;
	float scale;
	string type;
};
#endif
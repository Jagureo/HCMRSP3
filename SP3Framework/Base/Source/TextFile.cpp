#include "TextFile.h"

TextFile::TextFile(FileType ftype)
{
	x = 0;
	y = 0;
	scale = 0;
	type = "";
}
TextFile::~TextFile()
{

}
void TextFile::ReadFile(const string FileName)
{
	ifstream File;
	File.open(FileName);

	if (File.is_open())
	{
		string text;
		while (getline(File, text))
		{
			cout << text << "\n";
		}
	}
}
void TextFile::WriteFile(const string FileName)
{
	ofstream File;
	File.open(FileName, ios_base::app);

	if (File.is_open())
	{
		File << type << "," << x << "," << y << "," << scale << "\n";
	}
}
void TextFile::SetData(string type , float x , float y , float scale )
{
	this->type = type;
	this->x = x;
	this->y =  y;
	this->scale =  scale;
}
//string TextFile::GetType()
//{
//	return type;
//}
//void TextFile::SetTest(float a, float b)
//{
//	x = a;
//	y = b;
//}
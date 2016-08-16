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
void TextFile::SaveMapObj(const string FileName)
{
	ofstream File;
	File.open(FileName, ios_base::app);

	if (File.is_open())
	{
		File << type << "," << x << "," << y << "," << scale << "\n";
	}
}
bool TextFile::RemoveMapObj(const string FileName, float x , float y)
{
	fstream File;
	string line;
	File.open(FileName);
	int lineno = 0;
	if (File.is_open())
	{
		while (getline(File, line))
		{
			size_t pos = line.find(",");
			size_t temp = 0;
			size_t length = pos;
			for (int i = 0; i < 4; i++)
			{
				string hold = line.substr(temp, length);
				temp = pos + 1;
				pos = line.find(",", temp);

				length = pos -  temp;

				if (i == 0)
				{
					type = hold;
				}
				else if (i == 1)
				{
					this->x = stoi(hold);
				}
				else if (i == 2)
				{
					this->y = stoi(hold);
				}
				else if (i == 3)
				{
					scale = stoi(hold);
				}
			}
			if ((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y) < 1)
			{
				return true;
			}
			lineno++;
		}
	}
	else
	{
		return false;
	}
	return false;
}
//For map editor
void TextFile::SetData(string type , float x , float y , float scale )
{
	this->type = type;
	this->x = x;
	this->y =  y;
	this->scale =  scale;
}


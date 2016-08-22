#include "TextFile.h"

TextFile::TextFile(FileType ftype)
{
	x = 0;
	y = 0;
	type = "";
}
TextFile::~TextFile()
{

}
int TextFile::get_x()
{
	return x;
}
int TextFile::get_y()
{
	return y;
}
string TextFile::get_type()
{
	return type;
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
		File << type << "," << x << "," << y << ",\n";
	}
}
void TextFile::SetData(string type, float x, float y)
{
	this->type = type;
	this->x = x;
	this->y = y;

}
bool TextFile::SaveFile(const string File1, const string File2)
{
	fstream OutFile;
	OutFile.open(File1);

	ofstream InFile;
	InFile.open(File2);

	if (OutFile.is_open() && InFile.is_open())
	{
		string moveline;
		while (getline(OutFile, moveline))
		{
			InFile << moveline << "\n";
		}
		OutFile.close();
		/*ofstream ClearFile;
		ClearFile.open(File1, ios::trunc);
		ClearFile.close();*/
		return true;
	}
}
string TextFile::CreateMapFile(const string MapName, bool Loading)
{
	string FileName = MapName + ".txt";
	string linetext;
	fstream MapFile(FileName);
	if (MapFile.good() == true)  // if file exists
	{
		if (!Loading)
		{
			return "fail";
		}
	}
	MapFile.open(FileName); // create text file
	return FileName;

}
bool TextFile::LoadMap(const string MapName, int row)
{
	if (MapName == "")
	{
		return false;
	}
	fstream Map;
	string info;
	Map.open(MapName);

	cout << info << endl;
	if (Map.is_open())
	{
		for (int i = 0; i < row; ++i)
		{
			getline(Map, info);
		}
		size_t pos = info.find(",");
		size_t temp = 0;
		size_t length = pos;
		for (int i = 0; i < 3; i++)
		{
			string hold = info.substr(temp, length);
			temp = pos + 1;
			pos = info.find(",", temp);

			length = pos - temp;

			if (i == 0)
			{
				type = hold;
			}
			else if (i == 1)
			{
				x = stof(hold);
			}
			else if (i == 2)
			{
				y = stof(hold);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}
int TextFile::getRows(const string MapName)
{
	fstream file;
	string a;
	int rows = 0;
	file.open(MapName);

	if (file.is_open())
	{
		cout << "file opened: " << MapName << endl;
		while (getline(file, a))
		{
			++rows;
		}
	}
	return rows;
}
bool TextFile::RemoveMapObj(const string FileName, float x, float y)
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
			for (int i = 0; i < 3; i++)
			{
				string hold = line.substr(temp, length);
				temp = pos + 1;
				pos = line.find(",", temp);

				length = pos - temp;

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
			}
		}
	}
	return true;
}
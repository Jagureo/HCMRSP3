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
	InFile.open(File2, ios_base::out);

	if (OutFile.is_open() && InFile.is_open())
	{
		string moveline;
		while (getline(OutFile, moveline))
		{
			InFile << moveline << "\n";
		}
		OutFile.close();
		return true;
	}
}
string TextFile::CreateMapFile(const string MapName, bool Loading)
{
	string FileName = MapName + ".txt";
	string linetext;
	fstream MapFile;

	if (!Loading) //overwrite save file
	{
		MapFile.open(FileName, ios_base::out); // create text file
	}
	else
	{
		MapFile.open(FileName);
	}
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
		//cout << "file opened: " << MapName << endl;
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
int TextFile::get_stamina()
{
	return stamina;
}
int TextFile::get_speed()
{
	return speed;
}
int TextFile::get_strength()
{
	return strength;
}
bool TextFile::GetAnimalStat(string name)
{
	fstream File;
	File.open("AnimalStats.txt");
	
	bool correctLine = false;
	if (File.is_open())
	{
		string line;
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
				if (pos == 0)
				{
					pos = line.find(";", temp);
				}

				length = pos - temp;

				if (i == 0)
				{
					if (hold == name)
					{
						correctLine = true;
					}
				}
				if (i == 1 && correctLine)
				{
					stamina = stoi(hold);
				}
				if (i == 2 && correctLine)
				{
					speed = stoi(hold);
				}
				if (i == 3 && correctLine)
				{
					strength = stoi(hold);
				}
			}
			if (correctLine)
			{
				cout << name << "," << stamina << "," << speed << "," << strength << endl;
				return true;
			}
		}
	}
	return false;
}

string TextFile::get_carname()
{
	return name;
}
int TextFile::get_engine()
{
	return engine;
}
int TextFile::get_hp()
{
	return hp;
}
int TextFile::get_handling()
{
	return handling;
}
int TextFile::get_lassolength()
{
	return lassolength;
}
int TextFile::get_lassospeed()
{
	return lassospeed;
}
int TextFile::get_tranqcount()
{
	return tranqcount;
}
int TextFile::get_tranqduration()
{
	return tranqduration;
}
bool TextFile::GetCarStat(string name, string filename)
{
	fstream File;
	File.open(filename);
	if (File.is_open())
	{
		string line;
		while (getline(File, line))
		{
			size_t pos = line.find(",");
			size_t temp = 0;
			size_t length = pos;
			bool correctLine = false;
			for (int i = 0; i < 8; i++)
			{
				string hold = line.substr(temp, pos);
				temp = pos + 1;
				pos = line.find(",", temp);
				if (pos == 0)
				{
					pos = line.find(";", temp);
				}

				length = pos - temp;
				if (i == 0)
				{
					if (hold == name)
					{
						this->name = hold;
						correctLine = true;
					}
				}
				else if (i == 1 && correctLine)
				{
					engine = stoi(hold);
				}
				else if (i == 2 && correctLine)
				{
					hp = stoi(hold);
				}
				else if (i == 3 && correctLine)
				{
					handling = stoi(hold);
				}
				else if (i == 4 && correctLine)
				{
					lassolength = stoi(hold);
				}
				else if (i == 5 && correctLine)
				{
					lassospeed = stoi(hold);
				}
				else if (i == 6 && correctLine)
				{
					tranqcount = stoi(hold);
				}
				else if (i == 7 && correctLine)
				{
					tranqduration = stoi(hold);
				}
			}
			if (correctLine)
			{
				cout << this->name << " stats- " << endl;
				cout << "Engine: " << engine << " HP: " << hp << " Handling: " << handling << endl;
				cout << "Lasso Length: " << lassolength << " Lasso Speed: " << lassospeed << endl;
				cout << "Tranq Count: " << tranqcount << " Tranq Duration: " << tranqduration << endl;
				break;
			}
		}
		while (getline(File, line))
		{
			size_t pos = line.find(",");
			size_t temp = 0;
			size_t length = pos;
			bool correctLine = false;
			for (int i = 0; i < 2; i++)
			{
				string hold = line.substr(temp, pos);
				temp = pos + 1;
				pos = line.find(",", temp);
				if (pos == 0)
				{
					pos = line.find(";", temp);
				}

				length = pos - temp;
				if (i == 0)
				{
					if (hold == "money")
					{
						correctLine = true;
					}
				}
				if (i == 1 && correctLine)
				{
					money = stoi(hold);
				}
			}
			if (correctLine)
			{
				cout << "Money: " << money << endl;
				break;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool TextFile::SetCarStat(string replacename, string stattochange, string valueofstat)
{
	fstream File;
	File.open("tempsave.txt");

	fstream TempFile;
	TempFile.open("SavedCarStats.txt", ios_base::out);

	if (File.is_open() && TempFile.is_open())
	{
		string line;
		int lineno = 0;
		while (getline(File, line))
		{
			size_t pos = line.find(",");
			size_t temp = 0;
			size_t length = pos;
			string hold = line.substr(temp, pos);

			length = pos - temp;
			if (lineno == 0 && replacename == "Car1")
			{
				GetCarStat(replacename, "tempsave.txt");
				if (stattochange == "engine")
				{
					TempFile << replacename << "," << valueofstat << "," << hp << "," << handling << "," << lassolength << "," << lassospeed << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "hp")
				{
					TempFile << replacename << "," << engine << "," << valueofstat << "," << handling << "," << lassolength << "," << lassospeed << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "handling")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << valueofstat << "," << lassolength << "," << lassospeed << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "lassolength")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << valueofstat << "," << lassospeed << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "lassolength")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << valueofstat << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "tranqcount")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << lassospeed << "," << valueofstat << "," << tranqduration << ";\n";
				}
				else if (stattochange == "tranqduration")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << lassospeed << "," << tranqcount << "," << valueofstat << ";\n";
				}
			}
			else if (lineno == 1 && replacename == "Car2")
			{
				GetCarStat(replacename, "tempsave.txt");
				if (stattochange == "engine")
				{
					TempFile << replacename << "," << valueofstat << "," << hp << "," << handling << "," << lassolength << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "hp")
				{
					TempFile << replacename << "," << engine << "," << valueofstat << "," << handling << "," << lassolength << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "handling")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << valueofstat << "," << lassolength << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "lassolength")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << valueofstat << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "lassolength")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << valueofstat << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "tranqcount")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << lassolength << "," << valueofstat << "," << tranqduration << ";\n";
				}
				else if (stattochange == "tranqduration")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << lassolength << "," << tranqcount << "," << valueofstat << ";\n";
				}
			}
			else if (lineno == 2 && replacename == "Car3")
			{
				GetCarStat(replacename, "tempsave.txt");
				if (stattochange == "engine")
				{
					TempFile << replacename << "," << valueofstat << "," << hp << "," << handling << "," << lassolength << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "hp")
				{
					TempFile << replacename << "," << engine << "," << valueofstat << "," << handling << "," << lassolength << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "handling")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << valueofstat << "," << lassolength << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "lassolength")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << valueofstat << "," << lassolength << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "lassolength")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << valueofstat << "," << tranqcount << "," << tranqduration << ";\n";
				}
				else if (stattochange == "tranqcount")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << lassolength << "," << valueofstat << "," << tranqduration << ";\n";
				}
				else if (stattochange == "tranqduration")
				{
					TempFile << replacename << "," << engine << "," << hp << "," << handling << "," << lassolength << "," << lassolength << "," << tranqcount << "," << valueofstat << ";\n";
				}
			}
			else
			{
				TempFile << line << "\n";
			}
			lineno++;
		}
	}
	return true;
}
int TextFile::get_money()
{
	return money;
}
void TextFile::ListHighScore(string Level)
{
	ifstream File;
	File.open("HighScore.txt");
	int ScoreArray[100];
	int rows = getRows("HighScore.txt");
	int arraycount = 0;
	if (File.is_open())
	{
		string line;
		for (int i = 0; i < rows; i++)
		{
			getline(File, line);
			size_t pos = line.find(",");
			size_t temp = 0;
			size_t length = pos;
			bool correctLine = false;
			for (int a = 0; a < 2; a++)
			{
				string hold = line.substr(temp, pos);
				temp = pos + 1;
				pos = line.find(",", temp);
				if (pos == 0)
				{
					pos = line.find(";", temp);
				}

				length = pos - temp;
				if (a == 0)
				{
					if (hold == Level)
					{
						correctLine = true;
					}
				}
				else if (a == 1 && correctLine)
				{
					ScoreArray[arraycount] = stoi(hold);
					arraycount++;
				}
			}
		}
	}
	bool sorted = false;
	int optimize = 1;
	while (true)
	{
		sorted = true;
		for (int i = 0; i < arraycount - optimize; i++)
		{
			if (ScoreArray[i] < ScoreArray[i + 1])
			{
				int temp = ScoreArray[i];
				ScoreArray[i] = ScoreArray[i + 1];
				ScoreArray[i + 1] = temp;
				sorted = false;
			}
		}
		if (sorted == true)
		{
			break;
		}
		optimize++;
	}
	cout << "Displaying scores for: " << Level << endl;
	for (int i = 0; i < arraycount; i++)
	{
		if (ScoreArray[i] > 0)
		{
			scorearray[i] = ScoreArray[i];
		}
	}
}
void TextFile::SetScore(int score, string level)
{
	ofstream File;
	string sscore = to_string(score);
	File.open("HighScore.txt", ios_base::app);

	if (File.is_open())
	{
		File << level << "," << sscore << ";\n";
	}
}
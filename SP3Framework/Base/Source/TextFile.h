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
	void WriteFile(const string FileName);
	string CreateMapFile(const string MapName, bool Loading);

	int getRows(const string FileName);
	void SetData(string type, float x, float y);

	int get_x();
	int get_y();
	string get_type();

	bool LoadMap(const string FileName, int row);
	void SaveMapObj(const string FileName);
	bool RemoveMapObj(const string FileName, float x, float y);

	//void SetData(string type,float x, float y, float scale);
	bool SaveFile(const string File1, const string File2);

	int get_stamina();
	int get_speed();
	int get_strength();

	bool GetAnimalStat(string name);

	string get_carname();
	int get_engine();
	int get_hp();
	int get_handling();

	int get_lassolength();
	int get_lassostrength();

	int get_tranqcount();
	int get_tranqduration();
	void get_money();

	bool GetCarStat(string name, string filename);
	bool SetCarStat(string name, string stat, string data);

	void ListHighScore();
	void SetScore(int score);
	TextFile(FileType ftype = ANIMAL);
	~TextFile();
private:
	float x;
	float y;
	string type;

	int stamina;
	int speed;
	int strength;

	string name;
	int engine, hp, handling;
	int lassolength, lassostrength;
	int tranqcount, tranqduration;
	int money;
};
#endif
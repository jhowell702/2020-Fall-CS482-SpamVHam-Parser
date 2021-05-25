//Training.cc
#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>

using namespace std;


//this adds words to the map
void addMap(unordered_map<string,int>  &map, string message, string word)
{
	
	//Makes into stream
	istringstream theStream(message);
	//goes word by word
	while (getline(theStream, word, ' '))
	{
		//remove puncutation from word
		word.erase(remove(word.begin(), word.end(), '.'), word.end());
		word.erase(remove(word.begin(), word.end(), '"'), word.end());
		word.erase(remove(word.begin(), word.end(), '!'), word.end());
		word.erase(remove(word.begin(), word.end(), '?'), word.end());
		word.erase(remove(word.begin(), word.end(), ':'), word.end());
		word.erase(remove(word.begin(), word.end(), ','), word.end());
		
		//put into map
		if(map.find(word) == map.end())
		{
			//if not in map, put it in map and give it a value of 1
			map[word] = 1;
		}
		else
		{
			//if in map, add one to the value
			map[word]++;
		}
	}
}
void printto(unordered_map<string,int> map, string fileName)
{
	int count= 0;
	ofstream file;
	file.open(fileName);
	for (auto const& x : map)
	{
		count += x.second;
	}
	file << count << endl;
	for (auto const& x : map)
	{
		file << x.first << " " << x.second << endl;
	}
	file.close();
}
int main(int argc, char *argv[])
{
	string theMessage;
	string theWord;
	string type;
	string header;
	string inputFile, spamName, hamName;
	unordered_map<string,int> hamMap;
	unordered_map<string,int> spamMap;

	for(int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-i"))
		{
			inputFile = argv[i+1];
		}
		else if(!strcmp(argv[i],"-os"))
		{
			spamName = argv[i+1];
		}
		else if (!strcmp(argv[i],"-oh"))
		{
			hamName = argv[i+1];
		}
	}

	ifstream testing;
	testing.open (inputFile);
	getline(testing, header);

	//runs code until end of file
	while(!testing.eof())
	{
		//read type: spam or ham
		getline(testing, type, ',');
		

		//put into correct map
		if(type == "spam")
		{
			getline(testing, theMessage);
			addMap(spamMap, theMessage, theWord);
		}
		else if (type == "ham")
		{
			getline(testing, theMessage);
			addMap(hamMap, theMessage, theWord);
		}

	}

	printto(hamMap, hamName);
	printto(spamMap, spamName);

	testing.close();
	return 0;
}
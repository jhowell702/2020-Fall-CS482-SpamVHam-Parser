//Classify.cc

#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>


using namespace std;

int main(int argc, char* argv[])
{
	string testName, spamName, hamName, classOut, header;
	string totalNumSpamS,totalNumHamS;
	string theLine, theWord, theVal, theLineH, theWordH, theValH ;
	string type, theMessage, word;
	double wordVal =  0;
	double probMessIsSpam = 1;
	double probSgivenM =0;
	double wordValH =  0;
	double probMessIsHam = 1;
	double probHgivenM =0;
	int theOccurance, theOccuranceH; 
	double correct = 0;
	double numEmails = 0;
	double percent;
	unordered_map<string,int> hamMap;
	unordered_map<string,int> spamMap;

	for(int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-i"))
		{
			testName = argv[i+1];
		}
		else if(!strcmp(argv[i],"-is"))
		{
			spamName = argv[i+1];
		}
		else if (!strcmp(argv[i],"-ih"))
		{
			hamName = argv[i+1];
		}
		else if (!strcmp(argv[i],"-o"))
		{
			classOut = argv[i+1];
		}
	}

	//open testing.csv
	ifstream testFile;
	testFile.open (testName);

	//open spamOutput
	ifstream spamFile;
	spamFile.open (spamName);

	//open hamOutput
	ifstream hamFile;
	hamFile.open (hamName);

	//open classOutput
	ofstream classFile;
	classFile.open(classOut);

	//Get the total number of words in spam
	getline(spamFile, totalNumSpamS);
	//convert totalNumSpamS from string to int
	int spamCount = stoi(totalNumSpamS);
	

	//Get the total number of words in ham
	getline(hamFile, totalNumHamS);
	//convert totalNumHamS from string to int
	int hamCount = stoi(totalNumHamS);
	

	//populate Spam map
	while(!spamFile.eof())
	{
		//each individual line
		while(getline(spamFile, theLine))
		{
			//get theLine into a stream
			istringstream lineStream(theLine);
			//get individual word
			getline(lineStream, theWord, ' ');
			//get the value after the word
			getline(lineStream, theVal, ' ');

			//change val from string to int
			theOccurance = stoi(theVal);

			//insert into map (key, occurances)
			spamMap.insert({theWord,theOccurance});


		}
	}

		//populate ham map
	while(!hamFile.eof())
	{
		//each individual line
		while(getline(hamFile, theLineH))
		{
			//get theLine into a stream
			istringstream lineStreamH(theLineH);
			//get individual word
			getline(lineStreamH, theWordH, ' ');
			//get the value after the word
			getline(lineStreamH, theValH, ' ');

			//change val from string to int
			theOccuranceH = stoi(theValH);

			//insert into map (key, occurances)
			hamMap.insert({theWordH,theOccuranceH});


		}
	}

	//math
	//total occurance of all words
	int totalOcc = hamCount + spamCount;
	//probabilty of spam ...p(s)
	//words that are spam divided by the total amount of words
	double spamProb = double(spamCount) / double(totalOcc);
	
	//probability of ham...P(h) or P(!s)
	//words that are ham divided by the total amount of words
	double hamProb = double(hamCount) / double(totalOcc);

	//P(m) which we know is a message so it's 1
	double probMessage = 1;
	//p(w|s) = number of word occurance / total number of spam words
	//p(w|!s) = number of word occurance / total number of ham words
	/*I want to get the whole message from csv then compare each indivual word 
	with the map to get the amount its used*/

	//remove the first line v1,v2
	getline(testFile, header);
	
	while(!testFile.eof())
	{
		//read type: spam or ham -- ignore
		getline(testFile, type, ',');
		//get the get each message line by line  
		getline(testFile, theMessage);
		//Makes into stream
		istringstream theStream(theMessage);
		//goes word by word

		numEmails++;

		probMessIsSpam = 1;
		probMessIsHam = 1;
		while (getline(theStream, word, ' '))
		{
			//remove puncutation from word
			word.erase(remove(word.begin(), word.end(), '.'), word.end());
			word.erase(remove(word.begin(), word.end(), '"'), word.end());
			word.erase(remove(word.begin(), word.end(), '!'), word.end());
			word.erase(remove(word.begin(), word.end(), '?'), word.end());
			word.erase(remove(word.begin(), word.end(), ':'), word.end());
			word.erase(remove(word.begin(), word.end(), ','), word.end());

			//gives p(w1|s) * p(w2|s) * ... * p(wn|s) = p(m|s)
			unordered_map<string, int>::iterator it;
			if((it = spamMap.find(word)) !=spamMap.end())
			{
				
				//find value of word
				wordVal = it -> second;
				
				wordVal /= spamCount;
				 
				probMessIsSpam =  probMessIsSpam *wordVal;
				 

			}
			else if((it = spamMap.find(word)) ==spamMap.end())
			{
				probMessIsSpam = probMessIsSpam * 0.00001;
			}
			//gives p(w1|!s) * p(w2|!s) * ... * p(wn|!s) = p(m|!s)
			unordered_map<string, int>::iterator iter;
			if((iter = hamMap.find(word)) !=hamMap.end())
			{
				
				//find value of word
				wordValH = iter -> second;
				
				wordValH /= hamCount;
				 
				probMessIsHam =  probMessIsHam *wordValH;
				

			}
			else if((it = hamMap.find(word)) ==hamMap.end())
			{
				probMessIsHam = probMessIsHam * 0.00001;
			}

			
		}
		
		//p(s|m) = p(m|s) * p(s) /p(m)
		probSgivenM = (probMessIsSpam * spamProb) / probMessage;
		
		//p(!s|m) = p(m|!s) * p(!s) /p(m)
		probHgivenM = (probMessIsHam * hamProb) / probMessage;
	
		//if(p(s|m) > p(!s|m)) email is spam
		if(probMessIsSpam > probMessIsHam)
		{
			//is spam
			if(type == "spam")
			{
				classFile << "spam," << theMessage << endl;
				correct++;
			}

		}
		else
		{
			//is ham
			if(type == "ham")
			{
				classFile << "ham," << theMessage << endl;
				correct++;
			}
		}
	}
	

	
	//cout << "numEmails:" << numEmails << endl; 
	//cout << correct << endl;
	//percent = correct/numEmails;
	//cout << "percent " << percent << endl;

	//close csv
	testFile.close();

	//close spam
	spamFile.close();

	//close ham
	hamFile.close();

	//close classOutput
	classFile.close();

	return 0;
} 
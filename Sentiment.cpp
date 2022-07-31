#include <iostream>
#include <fstream>
using namespace std;
int get_LENGTH(char const* word);
void converte_SENTENCE_WORDS_TO_LOWER_CASE(char* sentence);
bool filter(char const* extractedWord);
bool stopWordsFilter(char const* extractedWord);
bool resetWord(char* extractedWord, double& positiveCount, double& negativeCount, double& totalCount, double& positiveNorm, double& negativeNor);
int main()
{
	cout << "Enter a sentence to analyze : ";
	char* sentence = new char[200];
	cin.getline(sentence, 200);
	// Convert all those characters that a aphabetical form to lower case
	converte_SENTENCE_WORDS_TO_LOWER_CASE(sentence);
	// Now find the total length of a sentance
	int length = get_LENGTH(sentence);

	// Now create a 2d dynamic array to store a word on each index, extracted from sentence

	double positiveCount = 0, negativeCount = 0, totalCount = 0;
	double normalizePositiveCount = 0, normalizeNegativeCount = 0;

	// Now calculation start here
	for (int i = 0; i < length; i++)
	{
		char* extractedWord = new char[100];
		int counter = 0;

		// Now extract a word from sentence
		for (int j = i; sentence[j] != ' ' && sentence[j] != '\0'; j++, i++)
		{
			extractedWord[counter] = sentence[j];
			counter++;
		}
		extractedWord[counter] = '\0';

		// Now check if there is any non-Alphabetical character in extracted word
		bool nonAlpha = filter(extractedWord);

		if (nonAlpha)
		{
			// if word is completely Alphabeticall, then pass it in function resetWord and check if it is
			// not a stop word, when it is verified that it is not a stop word, the we'll verify that the extracted
			// word is also available in dictionar.txt. If that word is avaible in dicionary, then further porcess will
			// start.
			resetWord(extractedWord, positiveCount, negativeCount, totalCount, normalizePositiveCount, normalizeNegativeCount);
		}
		delete[] extractedWord;
	}
	std::cout << "Positive count :" << positiveCount << "\n";
	std::cout << "Positive count :" << negativeCount << "\n";
	if (positiveCount > negativeCount)
		std::cout << "\nText is positive.\n";
	else if (negativeCount > negativeCount)
		std::cout << "\nText is negative.\n";
	std::cout << "Text with Normalization:\n";
	std::cout << "Positive count : " << normalizePositiveCount << "\n";
	std::cout << "Positive count : " << normalizeNegativeCount << "\n";
	if (normalizePositiveCount > normalizeNegativeCount)
		std::cout << "Text is positive.\n";
	else if (normalizePositiveCount > normalizeNegativeCount)
		std::cout << "Text is negative.\n";
	return 0;
}
void converte_SENTENCE_WORDS_TO_LOWER_CASE(char* sentence)
{
	int length = get_LENGTH(sentence);

	for (int i = 0; i < length; i++)
	{
		if (sentence[i] >= 'A' && sentence[i] <= 'Z')
			sentence[i] += 32;
	}
}
int get_LENGTH(char const* word)
{
	int length = 0;
	for (int i = 0; word[i] != '\0'; i++, length++);
	return length;
}
bool filter(char const* extractedWord)
{
	int length = get_LENGTH(extractedWord);
	int check = 0;
	for (int i = 0; i < length; i++)
	{
		if ((extractedWord[i] >= 'A' && extractedWord[i] <= 'Z') || (extractedWord[i] >= 'a' && extractedWord[i] <= 'z'))
			check++;
	}
	if (check == length)
		return true;
	return false;
}
bool stopWordsFilter(char const* extractedWord)
{
	int length = get_LENGTH(extractedWord);
	char stopWord[50];
	ifstream read;
	read.open("stopwords.txt");
	while (read >> stopWord)
	{
		int count = 0;
		int stopWordLength = get_LENGTH(stopWord);

		if (length == stopWordLength)
		{
			for (int i = 0; i < length; i++)
			{
				if (stopWord[i] == extractedWord[i])
				{
					count++;
				}
			}
		}
		if (count == length)
			return true;
	}
	read.close();
	return false;
}
bool resetWord(char* extractedWord, double& positiveCount, double& negativeCount, double& totalCount, double& positiveNorm, double& negativeNor)
{
	bool stopWord = stopWordsFilter(extractedWord);
	if (!stopWord)
	{
		int length = get_LENGTH(extractedWord);
		char* dictionaryWord = new char[500];
		ifstream read;
		read.open("dictionary.txt");
		while (read >> dictionaryWord)
		{
			bool ifEqualLength = true;
			int dictionaryWordLength = get_LENGTH(dictionaryWord), check = 0;
			int positive, negative, total;
			if (length == dictionaryWordLength)
			{
				for (int i = 0; i < length; i++)
				{
					if (extractedWord[i] == dictionaryWord[i])
					{
						check++;
					}
				}
			}
			else
			{
				read >> positive >> negative >> total;
				ifEqualLength = false;
			}
			if (ifEqualLength)
			{
				if (length == check)
				{
					read >> total;
					totalCount = total;
					read >> positive;
					positiveNorm += (positive / totalCount);
					positiveCount += positive;
					read >> negative;
					negativeNor += (negative / totalCount);
					negativeCount += negative;
					read.close();
				}
				else
					read >> positive >> negative >> total;
			}
		}
		read.close();
		return false;
	}
	return false;
}
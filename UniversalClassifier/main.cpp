#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "Header.h"
#include <time.h>

using namespace std;


int main(int argc, char *argv[])
{
	vector<string> rawInput; //Raw input lines
	rawInput.reserve(5000);
	vector<vector<string>> splitedInput; // 2D data
	splitedInput.reserve(5000);
	vector<vector<double>> convertedInput; // 'doublized
	convertedInput.reserve(5000);
	vector<vector<double>> trainDataset;
	trainDataset.reserve(5000);
	vector<vector<double>> validationDataset;
	validationDataset.reserve(5000);
	vector<vector<double>> result;
	result.reserve(5000);
	string buffer;
	ifstream input;
	input.open(argv[1]);

	//Store the raw input line by line
	cout << "Input File: " << argv[1] << endl;
	for ( int i = 0; getline(input, buffer, '\n'); i++)
	{
		rawInput.push_back(buffer);
	}

	//Start the timer
	clock_t t1 = clock();

	//Split a ine into strings
	cout << "Splitting input." << endl;
	StringSplitter stringSplitter;
	for (int i = 0, j = rawInput.size(); i < j; i++)
	{
		splitedInput.push_back(stringSplitter.parseString(rawInput[i]));
	}

	//Convert all values into double, including strings, ints, and double
	cout << "Convert all features into double." << endl;
	DataTypeConverter dataTypeConverter;
	convertedInput = dataTypeConverter.convertToDouble(splitedInput);

	//Shuffle the dataset
	cout << "Shuffling dataset." << endl;
	DatasetShuffler datasetShuffler;
	convertedInput = datasetShuffler.execute(convertedInput);

	//Split dataset into training dataset and validating dataset
	trainDataset.assign(convertedInput.begin(), convertedInput.begin() + (convertedInput.size() / 2));
	validationDataset.assign(convertedInput.begin() + (convertedInput.size() / 2), convertedInput.end());

	//Construct tree
	cout << "Constructing tree." << endl;
	struct Node*  tree = constructEntropyTree(trainDataset, 0);
	cout << endl;
	cout << "Finish Construction." << endl;
	
	//Validate tree
	cout << "Validating." << endl;
	Validator validator;
	result = validator.getResult(tree, validationDataset);
	cout << "Finish validation." << endl;

	//Stop the timer
	clock_t t2 = clock();

	//Show result
	cout << "Prediction result:" << endl;
	
	double rate = 0.0;
	for (int k = 0; k < validationDataset.size(); k++)
	{
		if (result[k][0] - result[k][1] == 0)
		{
			rate += 1.0;
		}
	}
	rate /= (double)validationDataset.size();
	cout << "Classfication +-0: " << rate * 100.0 << " %" << endl;

	rate = 0.0;
	for (int k = 0; k < validationDataset.size(); k++)
	{
		if (abs(result[k][0] - result[k][1]) <= 1.0)
		{
			rate += 1.0;
		}
	}
	rate /= (double)validationDataset.size();
	cout << "Classfication +-1: " << rate * 100.0 << " %" << endl;

	rate = 0.0;
	for (int k = 0; k < validationDataset.size(); k++)
	{
		if (abs(result[k][0] - result[k][1]) <= 2.0)
		{
			rate += 1.0;
		}
	}
	rate /= (double)validationDataset.size();
	cout << "Classfication +-2: " << rate * 100.0 << " %" << endl;

	//Show the execution time
	cout << "Execution time: " << (t2 - t1) / (double)(CLOCKS_PER_SEC) << endl;
	system("pause");

	return 0;
}


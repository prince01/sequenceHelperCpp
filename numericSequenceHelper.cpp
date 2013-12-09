// numericSequenceHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

void generatePrimes(int upperLimit, ofstream& out)
{
	vector<int> primes;
	for (int i = 2; i <= upperLimit; i++)
		primes.push_back(i);
	for (size_t i = 0; i < primes.size(); i++)
	for (size_t j = 0; j < i; j++)
	if (primes[i] % primes[j] == 0)
	{
		primes.erase(primes.begin() + i--);
		break;
	}
	for (int i : primes)
		out << i << endl;
}

struct Factor {
	int fact; int frequency;
};

class Sequence {
	vector<int> sequence;
	static bool compare(int a, int b) { return a < b; }
	static bool compareReversely(int a, int b) { return a > b; }
	int factorial(int n) { return n == 1 ? 1 : n * factorial(n - 1); }
	int newtonFormula(int n, int k) { return n >= k && k >= 0 ? factorial(n) / (factorial(n - k)*factorial(k)) : 0; }
public:
	Sequence() { }
	Sequence(vector<int>& vi) : sequence(vi) { }
	Sequence(const Sequence& sequencee) { sequence = sequencee.sequence; }
	~Sequence() { sequence.clear(); }
	void addNumber(int number) {
		sequence.push_back(number);
	}
	void deleteNumber(int index) {
		sequence.erase(sequence.begin() + index);
	}
	vector<int> sortedSequence() {
		vector<int> sorted = sequence;
		
		sort(sorted.begin(), sorted.end(), compare);

		return sorted;
	}
	vector<int> sortedReversely() {
		vector<int> sorted = sequence;

		sort(sorted.begin(), sorted.end(), compareReversely);

		return sorted;
	}
	vector<Factor> factors(int index) {
		vector<Factor> vFactors;
		int currNumber = sequence[index];

		for (int i = 2; i <= currNumber; i++)
		{
			if (currNumber % i == 0)
			{
				if (vFactors.size() > 0 && vFactors[vFactors.size() - 1].fact == i)
					vFactors[vFactors.size() - 1].frequency++;
				else
				{
					vFactors.push_back(Factor());
					vFactors[vFactors.size() - 1].fact = i;
					vFactors[vFactors.size() - 1].frequency = 1;
				}
				currNumber /= i;
				i = 1;
			}
		}
		return vFactors;
	}
	int count() const { return sequence.size(); }
	int operator[](int index) const { return sequence[index]; }
	friend ostream& operator << (ostream& out, const Sequence& s) { for (int i = 0; i < s.count(); i++) out << i + 1 << ": " << s[i] << "    "; return out; }
};

int _tmain(int argc, _TCHAR* argv[])
{
	ofstream output("output.txt");
	bool change = false;
	cout << "Do you want to change range of necessary primes?: ";
	cin >> change;
	if (change)
	{
		int upperLimit;
		ofstream out("primes.txt");
		cout << "What is upper limit of prime to generate?: ";
		try{
			if (cin >> upperLimit)
				generatePrimes(upperLimit, out);
			else
				throw exception();
		}
		catch (exception& exc)
		{
			cerr << exc.what() << endl;
			getchar();
			getchar();
			terminate();
		}
	}
	cout << "Loading primes..." << endl;
	vector<int> primes;
	{
		ifstream in("primes.txt");
		string str;
		while (getline(in, str))
		{
			istringstream is(str);
			int number;
			is >> number;
			primes.push_back(number);
		}
	}
	cout << "Type your number in sequence. After, type whichever character(in spite of numbers) you want...)." << endl;
	Sequence s;
	int c;
	while (cin >> c)
		s.addNumber(c);
	output << "The sequence contains: " << s << endl;
	output << "Sorted: ";
	{
		vector<int> sorted = s.sortedSequence();
		for (int i = 0; i < sorted.size(); i++)
			output << i + 1 << ": " << sorted[i] << "    ";
		output << endl << "Sorted reversely: ";
		sorted = s.sortedReversely();
		for (int i = 0; i < sorted.size(); i++)
			output << i + 1 << ": " << sorted[i] << "    ";
		output << endl;
	}
	for (int i = 0; i < s.count(); i++)
	{
		vector<Factor> factors;
		factors = s.factors(i);
		output << "Factors of " << s[i] << endl;
		for (int j = 0; j < factors.size(); j++)
		{
			int whichPrime;
			for (int i = 0; i < primes.size(); i++)
			if (primes[i] == factors[j].fact)
				whichPrime = i;
			if (j > 0) output << ", ";
			output << factors[j].fact << "(" << whichPrime + 1 << " prime " << factors[j].frequency << " times) ";
		}
		output << endl;
	}
	return 0;
}


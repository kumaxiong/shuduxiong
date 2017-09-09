#include<iostream>
#include<set>
#include<bitset>
#include<cstring>
#include<time.h>
#include<cstdio>
#include<fstream>

using namespace std;

int g_currentIndex = -1;
bitset<81> g_affectedFlags[9][9];
bitset<10> g_candidate[9][9];
int g_candidateNum[9][9];
int resultNum;
const int maxNum = 100;

int g_map[9][9] = {
	{ 7,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
};

void AddElement(int row, int column, int num)
{
	++g_currentIndex;
	g_map[row][column] = num;

	int old;
	for (int i = 0; i<9; ++i)
	{
		if (g_map[row][i] == 0 && g_candidate[row][i].test(num))
		{
			g_candidate[row][i].reset(num);
			--g_candidateNum[row][i];
			g_affectedFlags[row][i].set(g_currentIndex);
		}

		if (g_map[i][column] == 0 && g_candidate[i][column].test(num))
		{
			g_candidate[i][column].reset(num);
			--g_candidateNum[i][column];
			g_affectedFlags[i][column].set(g_currentIndex);
		}
	}

	int palaceRow = row>2 ? (row>5 ? 6 : 3) : 0;
	int palaceColumn = column>2 ? (column>5 ? 6 : 3) : 0;

	for (int i = 0; i<3; ++i)
	{
		for (int j = 0; j<3; ++j)
		{
			row = palaceRow + i;
			column = palaceColumn + j;
			if (g_map[row][column] == 0 && g_candidate[row][column].test(num))
			{
				g_candidate[row][column].reset(num);
				--g_candidateNum[row][column];
				g_affectedFlags[row][column].set(g_currentIndex);
			}
		}
	}
}

void RecoverElement(int row, int column, int num)
{
	g_map[row][column] = 0;
	for (int i = 0; i<9; ++i)
	{
		if (g_map[row][i] == 0 && g_affectedFlags[row][i].test(g_currentIndex))
		{
			g_candidate[row][i].set(num);
			++g_candidateNum[row][i];
			g_affectedFlags[row][i].reset(g_currentIndex);
		}

		if (g_map[i][column] == 0 && g_affectedFlags[i][column].test(g_currentIndex))
		{
			g_candidate[i][column].set(num);
			++g_candidateNum[i][column];
			g_affectedFlags[i][column].reset(g_currentIndex);
		}
	}

	int palaceRow = row>2 ? (row>5 ? 6 : 3) : 0;
	int palaceColumn = column>2 ? (column>5 ? 6 : 3) : 0;

	for (int i = 0; i<3; ++i)
	{
		for (int j = 0; j<3; ++j)
		{
			row = palaceRow + i;
			column = palaceColumn + j;
			if (g_map[row][column] == 0 && g_affectedFlags[row][column].test(g_currentIndex))
			{
				g_candidate[row][column].set(num);
				++g_candidateNum[row][column];
				g_affectedFlags[row][column].reset(g_currentIndex);
			}
		}
	}

	--g_currentIndex;
}

void Init()
{
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			g_candidate[i][j].set();
			g_candidateNum[i][j] = 10;
		}
	}

	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			if (g_map[i][j] != 0)
				AddElement(i, j, g_map[i][j]);
		}
	}
}

bool FindBest(int &row, int &column)
{
	int min = 999;
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			if (g_map[i][j] == 0 && g_candidateNum[i][j]>1 && g_candidateNum[i][j]<min)
			{
				row = i;
				column = j;
				min = g_candidateNum[i][j];
			}
		}
	}

	if (min == 999)
		return false;
	return true;
}

bool CheckResult()
{
	set<int> elements;
	set<int> elements2;

	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			elements.insert(g_map[i][j]);
			elements2.insert(g_map[j][i]);
		}
		if (elements.size() != 9)
			return false;
		if (elements2.size() != 9)
			return false;
		elements.clear();
		elements2.clear();
	}

	elements.clear();
	int row, column;
	for (int i = 0; i<3; ++i)
	{
		for (int j = 0; j<3; ++j)
		{
			row = i * 3;
			column = j * 3;
			for (int k = 0; k<9; ++k)
			{
				elements.insert(g_map[row + k / 3][column + k % 3]);
			}
			if (elements.size() != 9)
				return false;

			elements.clear();
		}
	}
	return true;
}

void OutputResult()
{
	cout << endl;
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			cout << g_map[i][j] << " ";
		}
		cout << endl;
	}
}

bool Try()
{
	int row, column;
	if (!FindBest(row, column))
		return true;
	for (int i = 1; i<10; ++i)
	{
		if (!g_candidate[row][column].test(i))
			continue;

		AddElement(row, column, i);

		if (Try())
		{
			if (g_currentIndex == 80 && CheckResult())
			{
				cout << endl << "Result:" << ++resultNum << endl;
				ofstream outfile;
				outfile.open("output.txt");
				OutputResult();
				outfile.close();
				if (resultNum >= maxNum)
					return false;
			}
		}
		else
			return false;
		RecoverElement(row, column, i);
	}
	return true;
}

int main()
{
	double start, end, cost;
	start = clock();
	Init();
	Try();
	if (resultNum)
		cout << endl << "OK!" << endl;
	else
		cout << endl << "Wrong Input!" << endl;

	end = clock();
	cost = end - start;
	cout << "Costed time:" << cost << "ms" << endl;
	char c;
	cin >> c;
	return 0;
}
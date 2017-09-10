#include<iostream>
#include<set>
#include<bitset>
#include<cstring>
#include<time.h>
#include<cstdio>
#include<stdio.h>
#include<fstream>

using namespace std;
int currentIndex = -1;
bitset<81> affectedFlags[9][9];
bitset<10> candidate[9][9];
int candidateNum[9][9];
int resultNum;
int maxNum = 5;
ofstream out("suduku.txt");

int shudu_map[9][9] = {
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
	++currentIndex;
	shudu_map[row][column] = num;
	for (int i = 0; i<9; ++i)
	{
		if (shudu_map[row][i] == 0 && candidate[row][i].test(num))
		{
			candidate[row][i].reset(num);
			--candidateNum[row][i];
			affectedFlags[row][i].set(currentIndex);
		}

		if (shudu_map[i][column] == 0 && candidate[i][column].test(num))
		{
			candidate[i][column].reset(num);
			--candidateNum[i][column];
			affectedFlags[i][column].set(currentIndex);
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
			if (shudu_map[row][column] == 0 && candidate[row][column].test(num))
			{
				candidate[row][column].reset(num);
				--candidateNum[row][column];
				affectedFlags[row][column].set(currentIndex);
			}
		}
	}
}

void RecoverElement(int row, int column, int num)
{
	shudu_map[row][column] = 0;
	for (int i = 0; i<9; ++i)
	{
		if (shudu_map[row][i] == 0 && affectedFlags[row][i].test(currentIndex))
		{
			candidate[row][i].set(num);
			++candidateNum[row][i];
			affectedFlags[row][i].reset(currentIndex);
		}

		if (shudu_map[i][column] == 0 && affectedFlags[i][column].test(currentIndex))
		{
			candidate[i][column].set(num);
			++candidateNum[i][column];
			affectedFlags[i][column].reset(currentIndex);
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
			if (shudu_map[row][column] == 0 && affectedFlags[row][column].test(currentIndex))
			{
				candidate[row][column].set(num);
				++candidateNum[row][column];
				affectedFlags[row][column].reset(currentIndex);
			}
		}
	}

	--currentIndex;
}

void Init()
{
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			candidate[i][j].set();
			candidateNum[i][j] = 10;
		}
	}

	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			if (shudu_map[i][j] != 0)
				AddElement(i, j, shudu_map[i][j]);
		}
	}
}

bool F_Best(int &row, int &column)
{
	int min = 999;
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			if (shudu_map[i][j] == 0 && candidateNum[i][j]>1 && candidateNum[i][j]<min)
			{
				row = i;
				column = j;
				min = candidateNum[i][j];
			}
		}
	}

	if (min == 999)
		return false;
	return true;
}

bool check_resultShudu()
{
	set<int> elements;
	set<int> elements2;

	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			elements.insert(shudu_map[i][j]);
			elements2.insert(shudu_map[j][i]);
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
				elements.insert(shudu_map[row + k / 3][column + k % 3]);
			}
			if (elements.size() != 9)
				return false;

			elements.clear();
		}
	}
	return true;
}

void printResult()
{
	out << endl;
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			out << shudu_map[i][j] << " ";
		}
		out << endl;
	}
}

bool SolveShudu()
{
	int row, column;
	if (!F_Best(row, column))
		return true;
	for (int i = 1; i<10; ++i)
	{
		if (!candidate[row][column].test(i))
			continue;
		AddElement(row, column, i);
		if (SolveShudu())
		{
			if (currentIndex == 80 && check_resultShudu())
			{
				out << endl << "Result:" << ++resultNum << endl;
				printResult();
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

int main(int argc, char *argv[])
{
	cin >> maxNum;
	if (out.is_open())
	{
		double start, end, cost;
		start = clock();
		Init();
		SolveShudu();
		if (resultNum)
			out << endl << "OK!" << endl;
		else
			out << endl << "Wrong Input!" << endl;

		end = clock();
		cost = end - start;
		out << "Costed time:" << cost << "ms" << endl;
		out.close();
	}

	system("pause");
	return 0;
}
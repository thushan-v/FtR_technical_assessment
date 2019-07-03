// FTR.cpp : Defines the entry point for the console application.
//
/**
 * @file
 * @author  Thushan Viswakula <thushan-v@hotmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is written to address FTR Gold Software Engineer Coding Test assessment.
 * The program is written in Visual Studio 2008 version with C++ prior to C++11 because
 * at the moment of writing this program this was the only IDE available for me. If this 
 * was written in a much recent IDE, I would write some of the code parts in a different
 * way adhering to the latest capabilities of the language.
 *
 * There are several methods we can code this. One way of doing is using several threads
 * to read keyboard input and data table publish as events and achieve the interactiveness.
 *
 * I decided to write on a simple blocking and non-blocking events because using thread
 * model will be an overkill for a program like this. My idea is keep it simple and less
 * complex and minimum LOCs.
 *
 * @section FTR Gold Software Engineer Coding Test
 *
 */
#include "stdafx.h"
#include <iostream>
#include <string>
#include <cctype>
#include <conio.h>
#include <map>
#include <ctime>
#include <math.h> 
#include <vector>
#include <algorithm>

#ifdef _WIN32
    #define WINPAUSE system("pause")
#endif

using namespace std;

/**
 * <Check if string is a number>
 *
 * @_sPass  String to check
 * @return bool if passed string is a number
 */
bool isNumber(const string& _sPass)
{
	string::const_iterator iter = _sPass.begin();

    while (iter != _sPass.end() && isdigit(*iter)) 
		++iter;
	
	return !_sPass.empty() && iter == _sPass.end();
}

/**
 * <Update the data table>
 *
 * @_iEntry  Input to increase the count
 * @OUT param _mapDataMap to updated data table 
 * @return void
 */
void updateDataTable(int _iEntry, std::map<int, int>& _mapDataMap)
{
	if (_mapDataMap.find(_iEntry) == _mapDataMap.end())
		_mapDataMap[_iEntry] = 1;
	else
		_mapDataMap[_iEntry] = _mapDataMap[_iEntry] + 1;
}

/**
 * <Print the data table map according to the expected output>
 *
 * @_mapDataMap  data table map
 * @return void
 */
void printDataTable(map<int, int> _mapDataMap)
{
	map<int, vector<int>> mapTemp;
	map<int,int>::iterator iter;
	
	for ( iter =_mapDataMap.begin(); iter != _mapDataMap.end(); ++iter)
	{
		map<int, vector<int>>::iterator iter2;
		iter2 = mapTemp.find(iter->second);
		if ( iter2 == mapTemp.end())
		{
			vector<int> vFreq;
			vFreq.push_back(iter->first);
			mapTemp[iter->second] = vFreq;
		}
		else
		{
			iter2->second.push_back(iter->first);
		}
	}

	int iCnt = 0;
	map<int, vector<int>>::reverse_iterator iter2;

	for ( iter2 = mapTemp.rbegin(); iter2 != mapTemp.rend(); ++iter2)
	{
		sort(iter2->second.begin(), iter2->second.end()); 

		vector<int>::iterator iter3 = iter2->second.begin();
		for ( ; iter3 != iter2->second.end(); ++iter3)
		{
			if (iCnt > 0)
				cout << ", ";
			cout << (*iter3) << ":" << iter2->first;
			iCnt++;
		}
	}

	cout << "\n";
}

/**
 * <Prompt text expecting integer value>
 *
 * @_sRequest  Request messgage
 * @_bAboveZero  option to request above zero values
 * @return valid integer value
 */
int promptIntRequest(string _sRequest, bool _bAboveZero)
{
	int iRet = 0;
	bool iInvalid = true;
	
	cout << _sRequest;

	while (iInvalid)
	{
		string sInput;
		cin >> sInput;

		if (isNumber(sInput))
		{
			iRet = atoi(sInput.c_str());

			if (_bAboveZero && iRet < 1)
				cout << "(Non-negative positive numeric)" << _sRequest;
			else
				iInvalid = false;
		}
		else
			cout << "(Valid numeric)" << _sRequest;
	}

	return iRet;
}

/**
 * <Find if number is Fibonacci>
 *
 * @_iNumber  Find number
 * @_iPrevPrev  Previous previous number
 * @_iPrev Previous number
 * @return ture if number is in Fibonacci sequence
 */
bool isFibonacci(int _iNumber, int _iPrevPrev, int _iPrev)
{
	if (_iNumber == 0 || _iNumber == 1)
		return true;

	int iNextFib = _iPrevPrev + _iPrev;

	if( iNextFib == _iNumber )
        return true;
	else if( iNextFib > _iNumber )
        return false;
    
	return isFibonacci( _iNumber, _iPrev, iNextFib );
}

/**
 * <Main method>
 *
 * @return 0
 */
int _tmain(int argc, _TCHAR* argv[])
{
	int iSecOutputPeriod = 0;
	int iFirstNum = 0;
	map<int, int>  mapDataMap;

	iSecOutputPeriod = promptIntRequest("Please enter the output period:", true);
	iFirstNum = promptIntRequest("Please enter the first number:", false);

	updateDataTable(iFirstNum, mapDataMap);
	printDataTable(mapDataMap);

	clock_t tStart;
    double dDuration = 0.0;

    tStart = clock();
	bool bStatePause = false;

	while (true)
	{
		if(_kbhit())
        {
			char cKey = 0;
            cKey = _getch();
			if (cKey == 'N')
			{
				int iNum = promptIntRequest("Please enter the new number:", false); //Blocking call
				if (isFibonacci(iNum, 0, 1))
					cout << "FIB!\n";
				updateDataTable(iNum, mapDataMap);
			}
			else if (cKey == 'Q')
			{
				printDataTable(mapDataMap);
				cout << "See you next time!\n";
				break;
			}
			else if (cKey == 'P')
			{
				bStatePause = true;
			}
			else if (cKey == 'R')
			{
				bStatePause = false;
			}
        }
		else
		{
			if (!bStatePause)
			{
				dDuration = ( clock() - tStart ) / (double) CLOCKS_PER_SEC;
			
				if ( iSecOutputPeriod <= floor(dDuration) )
				{
					printDataTable(mapDataMap);
					tStart = clock();
				}
			}
		}
	}

	WINPAUSE; //keeping the program open to see the last lines

	return 0;
}


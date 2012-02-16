/*
 * FtpPreprocessor.hpp
 *
 *  Created on: Feb 7, 2012
 *      Author: issle
 */

#ifndef FTPPREPROCESSOR_HPP_
#define FTPPREPROCESSOR_HPP_

#include "AbstractPreprocessor.hpp"
#include <iostream>
#include <map>
#include "commons/tools/LexicalCast.hpp"
using namespace std;

class FtpPreprocessor: public AbstractPreprocessor
{
private:
	map<int,int>responces;
public:
	void preprocess(Packet& packet, string lastResponce)
	{
		if(lastResponce.length() < 3)
			return;

		string s = lastResponce.substr(0,3);
		int responce = LexicalCast::get<int>(s);

		int counter = responces[responce];
		counter ++;

		responces[responce] = counter;
	}

	void print()
	{
		cout << "10" << endl;
		cout << "[Preprocessor: FTP] Statistics:" << endl;

		map<int,int>::iterator it;
		cout <<"Code";
		cout << setw(5) << "Freq" << endl;
		cout << "11" << endl;
		for(it = responces.begin(); it != responces.end(); it++)
		{

			cout << it->first;
			cout << setw(5) << it->second << endl;
		}
	}
};


#endif /* FTPPREPROCESSOR_HPP_ */

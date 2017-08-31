/**
    Weighted Index
    Copyright (C) 2016 Carl Barton, Tomasz Kociumak, Chang Liu, Solon P. Pissis and Jakub Radoszewski.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <string>
#include <list>

using namespace std;

#ifndef TSWITCH_H
#define TSWITCH_H

struct TSwitch
{
	double z;
	int mod;
	string text_file_name;
	string pattern_file_name;
	string output_file_name;
};

#endif


int decode_switches ( int argc, char * argv[], TSwitch * sw );
void usage ( void );
int read ( string filename, double *** text, int * n );
int read_pattern ( string filename, string * pattern, int * m );
void maximalSF ( double ** text, string & sq, int N, int n, double z,int * ME);
int multi_match ( string * pattern, int num_pattern, string & text, int n, int * SA, int * LCP, int * ME, list<int> & Occ );
void LCParray ( string & x, int n, int * SA, int * iSA, int *ME, int *LCP );
void Rank_index ( string & sq, int n, int * ME, int *SA );

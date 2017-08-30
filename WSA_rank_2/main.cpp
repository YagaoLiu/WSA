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

#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "node.h"
#include "defs.h"

#include <divsufsort.h>
#include <sdsl/rmq_support.hpp>

using namespace std;
using get_time = chrono::steady_clock;

double ** text;

int main (int argc, char ** argv )
{
	TSwitch sw;
	int mod;
	string text_file;
	string pattern_file;
	string output_file;
	double z;
	
	int n;

	ofstream result;

	unsigned int k;

	/* Decodes the arguments */

	k = decode_switches ( argc, argv, &sw );

	/* Check the arguments */
	if ( k < 5 )
	{
		usage();
		return 1;
	}
	else
	{
		if ( sw.text_file_name.empty() )
		{
			cout << "Error: No Text input!\n";
			return 0;
		}
		else
		{
			text_file = sw.text_file_name;
		}
		if ( sw.output_file_name.empty() )
		{
			cout << "Error: No Output file\n";
			return 0;
		}
		else
		{
			output_file = sw.output_file_name;
		}

		if ( sw.z > 0 )
		{
			z = sw.z;
		}
		else
		{
			cout << "Error: z must be a positive number!\n";
			return 0;
		}
		if ( sw.mod < 0 || sw.mod > 2 )
		{
			cout << "Error: Wrong mod select!\n";
			return 0;
		}
		else
		{
			mod = sw.mod;
		}
		
		if ( mod == 2 )
		{
			if ( sw.pattern_file_name.empty() )
			{
				cout << "Error: No pattern input for mod 2! \n";
				return 0;
			}
			else
			{
				pattern_file = sw.pattern_file_name;
			}
		}
	}

	/* read input */
	if ( !read ( text_file, &text, &n ) )
	{
		cout << "Error: Failed to read input!\n";
		return 0;
	}
	cout << "text length:" << n << endl;
	cout << "Finish reading input" << endl;

	auto begin = get_time::now();
	cout << "index begin" << endl;

	string sq;
	weighted_index_building ( text, n, z, &sq );

	auto end = get_time::now();
	auto diff1 = end-begin;
	cout << "Time for special weighted sequence: " << chrono::duration_cast<chrono::milliseconds>(diff1).count() << " ms" << endl;
//	cout << "After building, sq length is " << sq.size() << endl;
	//Index using Suffix array
	cout << "build suffix array index\n";
	begin = get_time::now();

	int N = sq.size();

	int * SA	= new int [N];
	int * iSA	= new int [N];
	int * LCP	= new int [N];
	int * ME	= new int [N];

	maximalSF ( text, sq, N, n, z, ME );

	int level = floor ( log ( N ) / log ( 2 ) ) + 1;

	Rank_index ( sq, N, ME, SA, iSA );
//	SuffixArray ( N, ME, rank, SA, iSA);
//	LCParray ( sq, N, SA, iSA, ME, LCP );

#if 0 //print & check
	cout << sq << endl;
	for ( int i = 0; i < N; i++ )
	{
		cout << sq.substr(SA[i], ME[SA[i]]) << endl;
	}
#endif
	end = get_time::now();
	auto diff2 = end - begin;
	cout<<"Time:  "<< chrono::duration_cast<chrono::milliseconds>(diff2).count()<<" ms "<<endl;

#if 0
	result.open ( output_file );
	for ( int i = 0; i < N; i++ )
		if ( ME[SA[i]] != 0 )
			result << sq.substr(SA[i], ME[SA[i]]) << '\n';
	result.close();
#endif
	delete[] SA;
	delete[] iSA;
	delete[] LCP;
	delete[] ME;

	for ( int i = 0; i < n; i++ )
		delete[] text[i];
	delete[] text;

	return 0;
}


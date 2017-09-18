/**
    Weighted Suffix Array
    Copyright (C) 2017 Panagiotis.Charalampopoulos, Costas.Iliopoulos, Chang.Liu, Solon.Pissis
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
	if ( k < 1)
	{
		usage();
		return 1;
	}
	else
	{
			text_file = sw.text_file_name;
			output_file = sw.output_file_name;
			z = sw.z;
		if ( sw.mod < 0 )
		{
			cout << "Error: Wrong mod select!\n";
			return 0;
		}
		else
		{
			mod = sw.mod;
		}
		
	}

	/* read input */
	if ( mod != 3 )
	{
		if ( !read ( text_file, &text, &n ) )
		{
			cout << "Error: Failed to read input!\n";
			return 0;
		}
	}

#if 1 
//	cout << "text length:" << n << endl;
//	cout << "Finish reading input" << endl;

	result.open ( output_file, ios_base::app );
	auto begin = get_time::now();
//	cout << "index begin" << endl;

	string sq;
	if ( mod != 3 )
		weighted_index_building ( text, n, z, &sq );

	auto end = get_time::now();
	auto diff1 = end - begin;
//	cout<<"Time for special weighted sequence:  "<< chrono::duration_cast<chrono::milliseconds>(diff1).count()<<" ms "<<endl;
//	cout << "build suffix array index\n";


	int N = sq.size();
	result << "zstrings length:" << N/1000000 << " MB\n";
	if ( mod == 3 )
	{
		cout << "Input string:" << endl;
		cin >> sq;
		N = sq.size();
	}

#if 1 
	int * SA	= new int [N];
	int * LCP	= new int [N];
	int * ME	= new int [N];

	if ( mod == 3 )
	{
		for ( int i = 0; i < N; i++ )
		{
			cout << "ME[" << i << "]:";
			cin >> ME[i];
		}
	}
	else
	{
		maximalSF ( text, sq, N, n, z, ME );	
	}
	
	if ( mod != 3 )
	{
		for ( int i = 0; i < n; i++ )
			delete[] text[i];
		delete[] text;
	}
#endif

	begin = get_time::now();
	SA_LCP_index ( text, sq.c_str(), N, n, z, SA, LCP );

	if ( mod == 3 )
	{
		for ( int i = 0; i < N; i++ )
		{
			cout << setw(2) << i << ": " << SA[i] << ' ' << LCP[i] << '\n';
		}
	}
#if 1
	union_find_resort ( SA, LCP, ME, N );
	LCParray ( sq, N, SA, ME, LCP );
	vector<int> WSA;
	WSA.reserve ( N );
	begin =get_time:: now();
	WeightedIndex ( n, N, SA, LCP, ME, WSA );

	if ( mod == 2 )
	{//print & check
		cout << "n=" << n << " N=" << N << endl;
		for ( int i = 0, j=-1; i < N; i++ )
		{
			if ( ME[SA[i]] != 0 )
			{
				cout << setw(3) << ++j << ": " <<  setw(3)<< SA[i] << ", " << setw(3) << LCP[i] << ", " << sq.substr(SA[i], ME[SA[i]]) << '\n';
			}
		}
		for ( int i = 0; i < WSA.size(); i++ )
		{
			cout << setw(3) << i << ":" << setw(3) << WSA[i] << ", " << sq.substr(WSA[i], ME[SA[i]]) << '\n';
		}
	}
	if ( mod == 1 )
	{//print & check
//		result << sq << '\n';
		for ( int i = 0, j = -1; i < N; i++ )
		{
		if ( ME[SA[i]] != 0 )
			result << ++j << ":\t" << SA[i] << ",\t" << sq.substr(SA[i], ME[SA[i]]) << '\n';
		}
	}
	if ( mod == 3 )
	{
		 for ( int i = 0, j = -1; i < N; i++ )
		 {
			 if ( ME[SA[i]] != 0 )
				 cout << setw(2) << ++j << ": " << setw(2) << SA[i] << ", " << sq.substr( SA[i], ME[SA[i]] ) << '\n';
		 }
	}
	end = get_time::now();
	auto diff2 = end - begin;
	result<<"Time:  "<< chrono::duration_cast<chrono::milliseconds>(diff2).count()<<" ms "<<endl;
#if 0
	for ( int i = 0; i < N; i++ )
		if ( ME[SA[i]] != 0 )
			result << sq.substr(SA[i], ME[SA[i]]) << '\n';
	result.close();	
#endif
	delete[] SA;
	delete[] LCP;
	delete[] ME;
#endif
#endif
	return 0;
}


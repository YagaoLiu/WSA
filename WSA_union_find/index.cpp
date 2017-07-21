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

#include <map>
#include <iostream>
#include <string>

#include "defs.h"

#include <divsufsort.h>
#include <sdsl/rmq_support.hpp>

using namespace std;
using namespace sdsl;

int i = 0;
int j = 0;

double lp ( double ** text, int position, char letter )
{
	unsigned int c;
	switch(letter)
	{
		case 'A':
			c = 0;
			break;
		case 'C':
			c = 1;
			break;
		case 'G':
			c = 2;
			break;
		case 'T':
			c = 3;
			break;
		default:
			c = 5;
			break;
	}
	if ( c == 5 )
		return 0;
	else
	{
		return text[position][c];
	}
}

int minimal ( int a, int b, int c )
{
	if ( a<=b && a<= c )
		return a;
	if ( b<=a && b<= c )
		return b;
	else
		return c;
}

void pa ( double * a, int n )
{
	for ( int i = 0; i < n; i++ )
		cout << a[i] << ' ';
	cout << endl;
}

void SA_LCP_index ( double ** text, unsigned char * sq, int N, int n, double z, int * SA, int * LCP, int * ME )
{
	// compute valid SA using slide window:
	double p = 1.0;
	int valid_length = 0;
	int k = 0;

	for ( j = 0; j < z; j++ )
	{
		for ( i = 0; i < n; i++ )
		{
			if ( i == 0 || ME[i+j*(n)-1] <= 1 )
			{
				k = 0;
				p = 1.0;
				for ( k = 0; k < n-i; k++ )
				{
					if ( p * lp( text, i+k, sq[i+j*n+k] ) >= 1/z )
						p *= lp( text, i+k, sq[i+k+j*n]);
					else
						break;
				}
				ME[i+j*(n)] = k;
			}
			else
			{
				k = ME[i+j*(n)-1];
				p /= lp ( text, i-1, sq[i-1+j*(n)] );
				if ( p < 1/z )
					ME[i+j*(n)] = k -1;
				else
				{
					for ( k = k-1; k < n-i; k++ )
					{
						if ( p * lp( text, i+k, sq[i+k+j*(n)] ) >= 1/z )
							p *= lp( text, i+k, sq[i+k+j*(n)] );
						else
							break;
					}
				}
				ME[i+j*(n)] = k;
			}
		}
	}

	// compute SA:
	if ( divsufsort ( sq, SA, N ) != 0 )
	{
		fprintf ( stderr, " Error: Cannot allocate memory.\n" );
		exit ( EXIT_FAILURE );
	}

	int * iSA = new int [N];
	// compute inverseSA:
	for ( i = 0; i < N; i++ )
	{
		iSA[SA[i]] = i;
	}
	
	// compute LCP:
	LCP[0] = 0;
	int l = 0;
	for ( i = 0; i < N; i++ )
	{
		int k = iSA[i];
		int j = SA[k-1];
		while ( sq[i+l] == sq[j+l] )
			l = l + 1;
		LCP[k] = l;
		if ( l > 0 )
			l = l - 1;
	}

	delete[] iSA;
}


				




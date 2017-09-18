#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

void LCParray ( string  &text, int n, int * SA, int * ME, int * LCP )
{
	int * ISA = new int [n];
	int i=0, j=0;

	for ( i = 0; i < n; i++ )
	{
		ISA[SA[i]] = i;
		LCP[i] = 0;
	}

	for ( i = 0; i < n; i++ ) // compute LCP[ISA[i]]
	{
		if ( ISA[i] != 0 )
		{
			if ( i == 0 ) j = 0;
			else j = ( LCP[ISA[i-1]] >=2 ) ? LCP[ISA[i-1]]-1 : 0;
			while ( text[i+j] == text[SA[ISA[i]-1]+j] && j < min(ME[i], ME[SA[ISA[i]-1]]) )
				j++;
			LCP[ISA[i]] = j;
		}
	}
}


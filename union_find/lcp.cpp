#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;
/*
void LCParray ( string  &text, int n, int * SA, int * ME, int * LCP )
{
	int * ISA = new int [n];
	int i=0, j=0;
	int l = 0;

	for ( i = 0; i < n; i++ )
	{
		ISA[SA[i]] = i;
	}
	LCP[0] = 0;
	for ( i = 0; i < n; i++ )
	{
		if ( ISA[i] != 0 )
		{
			if ( i == 0 ) l = 0;
			else l = ( LCP[ISA[i-1]] > 1) ? LCP[ISA[i-1]]-1 : 0;
			cout << "i=" << i << "\tl=" << l << endl;
			while ( text[i+l] == text[SA[ISA[i]-1]+l] && l < min ( ME[i], ME[SA[ISA[i]-1]] ) )
				l++;
			l = min( l, min( ME[i], ME[SA[ISA[i]-1]] ) );
			LCP[ISA[i]] = l;
			cout << "i=" << i << "\tl=" << l << endl;
		}
	}
	delete[] ISA;
}
*/

void LCParray( string& text, int n, int * SA, int * ME, int * LCP )
{
	int i = 0, j = 0, k = 0, l = 0;
	int * iSA = new int [n];
	for ( i = 0; i < n; i++ )
	{
		iSA[SA[i]] = i;
		LCP[i] = 0;
	}
	for ( i = 0; i < n; i++ )
	{
		if ( iSA[i] == 0)
		{
			k = 0;
			continue;
		}

		j = SA[iSA[i]-1];
		while ( i+k < n && j+l < n && text[i+k] == text[j+k] && k < ME[i] && k < ME[j] )
			k++;
		LCP[iSA[i]] = k;
		if ( k > 0 )
			k--;
	}
}




	

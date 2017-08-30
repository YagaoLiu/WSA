#include <string>
#include <cmath>

using namespace std;

void LCParray ( string & x, int n, int * SA, int * ISA, int * ME, int * LCP )
{										
	int i=0, j=0;

	LCP[0] = 0;
	for ( i = 0; i < n; i++ ) // compute LCP[ISA[i]]
		if ( ISA[i] != 0 ) 
		{
			if ( i == 0) j = 0;
			else j = (LCP[ISA[i-1]] >= 2) ? LCP[ISA[i-1]]-1 : 0;
			while ( x[i+j] == x[SA[ISA[i]-1]+j] )
				j++;
			LCP[ISA[i]] = j;
		}

	for ( i = 1; i < n; i++ )
	{
		LCP[i] = min ( LCP[i], min( ME[SA[i]], ME[SA[i-1]] ) );
	}
}


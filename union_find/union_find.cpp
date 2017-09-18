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
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>

using namespace std;

class UF    {
	int *id, cnt, *sz;
	public:
	// Create an empty union find data structure with N isolated sets.
	UF(int N)   {
		cnt = N;
		id = new int[N];
		sz = new int[N];
		for(int i=0; i<N; i++)	{
			id[i] = i;
			sz[i] = 1;
		}
	}
	~UF()	{
		delete [] id;
		delete [] sz;
	}
	// Return the id of component corresponding to object p.
	int find(int p)	{
		int root = p;
		while (root != id[root])
			root = id[root];
		while (p != root) {
			int newp = id[p];
			id[p] = root;
			p = newp;
		}
		return root;
	}
	// Replace sets containing x and y with their union.
	void merge(int x, int y)	{
		int i = find(x);
		int j = find(y);
		if (i == j) return;

		if ( i < j ){
			id[j] = i;
		}	
		else	{
			id[i] = j;
		}
		cnt--;
	}
	// Are objects x and y in the same set?
	bool connected(int x, int y)    {
		return find(x) == find(y);
	}
	// Return the number of disjoint sets.
	int count() {
		return cnt;
	}
};

void union_find_resort( int * SA, int * LCP, int * ME, int n )
{
	UF uf(n);
	vector< forward_list <int> > SI (n);
	map<int, vector<int> > lcp_map;
	map<int, vector<int> > me_map;
	int max_lcp = 0;
	int max_me = 0;
	for ( int i = 0; i < n; i++ )
	{
		lcp_map[LCP[i]].push_back(i);
		if ( max_lcp < LCP[i] )
			max_lcp = LCP[i];
		me_map[ME[SA[i]]].push_back(i);
		if ( max_me < ME[SA[i]] )
			max_me = ME[SA[i]];
	}

	for ( int l = max( max_lcp, max_me ); l >= 0; l-- )
	{
		if ( !lcp_map[l].empty() )
		{
			for ( auto it = lcp_map[l].begin(); it != lcp_map[l].end(); it ++ )
			{
				if( *it > 0 )
				{
					uf.merge( *it-1, *it );
				}
			}
		}
		if ( !me_map[l].empty() )
		{
			for ( auto it = me_map[l].begin(); it != me_map[l].end(); it++ )
			{
				int f = uf.find( *it );
				SI[f].push_front ( SA[*it] );
			}
		}
	}
	int counter = 0;
	for ( int i = 0; i < n; i++ )
		for ( auto it = SI[i].begin(); it != SI[i].end(); it++ )
		{
			SA[counter++] = *it;
		}
}
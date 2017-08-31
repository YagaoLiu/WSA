#include <iostream>
#include <map>
#include <vector>
#include <list>
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
	vector < int > me_sa_order (n);
	vector < list <int> > SI ( n );
	for ( int i = 0; i < n; i++ )
		me_sa_order[i] = ME[SA[i]];
	map<int, vector<int> > lcp_map;
	map<int, vector<int> > me_map;
	int max_lcp = 0;
	int max_me = 0;
	for ( int i = 0; i < n; i++ )
	{
		lcp_map[LCP[i]].push_back(i);
		if ( max_lcp < LCP[i] )
			max_lcp = LCP[i];
		me_map[me_sa_order[i]].push_back(i);
		if ( max_me < me_sa_order[i] )
			max_me = me_sa_order[i];
	}
	for ( int l = max( max_lcp, max_me ); l > 0; l-- )
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

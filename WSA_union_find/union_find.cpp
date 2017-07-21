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
//		make smaller root point to larger one
//		if   (sz[i] < sz[j])	{ 
//			id[i] = j; 
//			sz[j] += sz[i]; 
//		} else	{ 
//			id[j] = i; 
//			sz[i] += sz[j]; 
//		}
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

struct rightshift {
};

struct two_compare
{
	int * i;
	int * l;
	two_compare ( int * index, int * length ):
		i ( index ), l ( length ) {}
	bool operator () ( int a, int b )
	{
		if ( i[a] != i[b] )
			return i[a] < i[b];
		else
			return l[a] < l[b];
	}
};


void union_find_resort( int * SA, int * LCP, int * ME, int n )
{
	UF uf(n);
	vector < int > me_sa_order (n);
	vector < int > SA_support (n);
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
		SA_support[i] = SA[i];
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
#if 0	//print and check
		cout << "check length=" << l << endl;
		for ( int i = 0; i < n; i++ )
		{
			if ( i == 14 )
				cout << endl;
			cout << uf.find(i) << '\t';
		}
		cout << endl;
#endif
	}
#if 0 //print and check
	cout << "SA:\n";
	for ( int i = 0; i < n; i++ )
		cout << SA[i] << '\t';
	cout << endl;
	cout << "index:\n";
	for ( int i = 0; i < n; i++ )
		cout << index[SA[i]] << '\t';
	cout << endl;
	cout << "length:\n";
	for ( int i = 0; i < n; i++ )
		cout << length[SA[i]] << '\t';
	cout << endl;
	cout << "finish\n";
#endif
#if 0
	int alloc_size = n / uf.count();
	//bucket sort
	typedef map<int, map<int, vector<int> > > Bucket;
	Bucket b;
	for ( int i = 0; i < n; i++ )
	{
		b[index[i]][length[i]].push_back( i );
	}
	int counter = 0;
	for ( auto out = b.begin(); out != b.end(); out++ )
		for ( auto mid = out->second.begin(); mid != out->second.end(); mid++ )
			for ( auto iner = mid->second.begin(); iner != mid->second.end(); iner++ )
				SA[counter++] = *iner;
	
	delete [] index;
	delete [] length;
#endif
//	sort ( SA, SA+n, two_compare( index, length ) );
	int counter = 0;
	for ( int i = 0; i < n; i++ )
		for ( auto it = SI[i].begin(); it != SI[i].end(); it++ )
		{
			SA[counter++] = *it;
		}
}

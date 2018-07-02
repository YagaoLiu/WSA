#include <vector>
#include <string>
#include "property_string.h"

using namespace std;

class PropertySuffixArray{
	vector<int> PSA;
	vector<int> LCP;
	struct pair_compare{
		vector<int>& r;
		vector<int>& s;
		vector<int>& p;
		int t;
		pair_compare ( vector<int>& Rank, vector<int>& Signature, vector<int> Property, int T ):
			r(Rank), s(Signature), p(Property), t(T) {}
		bool operator() ( int i1, int i2 );
//		{
//			if ( r[i1] != r[i2] ) return r[i1] < r[i2];
//			else if ( return sign[i1+t] < sign[i2+t];
//		}
	};
	public:
	int& operator[] (size_t i) { return PSA[i]; }
	PropertySuffixArray ( PropertyString& S, string& alphabet );
	int size() { return PSA.size(); }
	vector<int> signature ( vector<int>& text, vector<int> const& property, int n, int t, int sigma );
};

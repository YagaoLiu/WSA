#include <vector>
#include <string>
#include "property_string.h"

using namespace std;

class PropertySuffixArray{
	vector<int> PSA;
	vector<int> LCP;
	
	struct pair_compare{
		vector<int>& rank;
		vector<int>& sign;
		int t;
		pair_compar ( vector<int>& R, vector<int>& S, T ):
			rank(R), sign(S), t(T) {}
		bool operator() ( int i1, int i2 ){
			if ( rank[i1] != rank[i2] ) return rank[i1] < rank[i2];
			else return sign[i1+t] < sign[i2+t]
		}
	}	
	PropertySuffixArray ( PropertyString const& S, string& alphabet );
	void signature ( vector<int>& text, int n, int t, int sigma );
};

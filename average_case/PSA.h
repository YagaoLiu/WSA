#include <vector>
#include <string>
#include "property_string.h"

using namespace std;

class PropertySuffixArray{
	vector<int> PSA;
	vector<int> LCP;
	public:
	int& operator[] (size_t i) { return PSA[i]; }
	PropertySuffixArray ( PropertyString& S, string& alphabet );
	int size() { return PSA.size(); }
	vector<int> signature ( vector<int>& text, vector<int> const& property, int n, int t, int sigma );
};

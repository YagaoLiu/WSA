#include <vector>
#include <string>
#include "property_string.h"

using namespace std;

class PropertySuffixArray{
	vector<int> PSA;
	vector<int> LCP;

	void signature ( PropertyString const& S, string& alphabet );
};

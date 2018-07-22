#include <vector>
#include <string>
#include "property_string.h"

using namespace std;
typedef unsigned long int word;

class PropertySuffixArray{
	vector<int> PSA;
	vector<int> LCP;
	int length;
	int sigma;
	int t;
	vector<int> const& property;
	string const& text;
	public:
	int& operator[] (size_t i) { return PSA[i]; }
	PropertySuffixArray ( PropertyString& S, string& alphabet );
	int size() { return length; }
	vector<word> signature ( vector<int>& integer_text);
	void sign_double ( vector<word>& sign, int tt );
	void sign_update ( vector<word>& sign, vector<word>& sign_base, int tt );
};

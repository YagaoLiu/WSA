#include <iostream>
#include <vecter>
#include <cmath>
#include <bitset>

using namespace std;

vector<int>& signature( PropertyString const& S, string& alphabet ){
	sigma = alphabet.size() + 1;
	map<char, int> char_to_int;
	for ( int i = 0; i < alphabet.size(); i++ ){
		char_to_int[alphabet[i]] = i+1;
	}
	double t = floor( log(text.size()) / log(sigma) );
	vector<int> integer_text ( text.size() + t );
	vector<int> sign ( text.size() );
	for ( int i = 0; i < text.size(); i++ ){
		integer_text[i] = char_to_int[text[i]];
	}
	for ( int i = text.size(); i < integer_text; i++ ){
		integer_text[i] = 0;
	}

	sign[0] = 0;
	for ( int j = 1; j < t; j++ ){
		sign[0] += integer_text[j-1] * pow(sigma, t-j);
	}
	for ( int i = 1; i < text.size(); i++ ){
		sign[i] = ( sign[i-1] - integer_text[i-1] * pow(sigma, t-1) ) * sigma + integer_text[i+t-1];
	}
	for ( int i = 0; i < text.size(); i++ ){
		if ( property[i] < t ){
			sign[i] = sign[i]&( pow(sigma, t) - pow(sigma, t-property[i]) );
		}
	}
	return sign;
}


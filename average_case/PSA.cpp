#include <iostream>
#include <iomanip> 
#include <vector>
#include <cmath>
#include <map>
#include <numeric>
#include <bitset>
#include <algorithm>
#include <chrono>

#include "radixsort.h"
#include "PSA.h"

using get_time = chrono::steady_clock;

using namespace std;

PropertySuffixArray::PropertySuffixArray( PropertyString & S, string& alphabet): property(S.property()), text(S.string()) {
	length = S.length();
	sigma = ceil( log(alphabet.size()+1) / log(2) );
	t = floor( log(length)/log( 1<<sigma ) );
	PSA.resize ( length );
	iota( PSA.begin(), PSA.end(), 0 );
	int max_pi = 0;
	for ( int i : property ){
		if ( i > max_pi ) max_pi = i;
	}

	map<char,int> char_to_int;
	for( int i = 0; i < (int) alphabet.size(); i++ ){
		char_to_int[alphabet[i]] = i+1;
	}
	vector<int> integer_text ( length+t );
	for ( int i = 0; i < length; i++ ){
		integer_text[i] = char_to_int[S[i]];
	}
	vector<word> sign_base = signature( integer_text );
	vector<word> sign ( sign_base );
	vector<int> rank(length);
	vector<int> next_rank(length);
	radixsort ( sign.begin(), PSA.begin(), length );
	rank[0] = 0;
	int r = 0;
	for ( int i = 1; i < length; i++ ){
		if ( sign[i] != sign[i-1] ){
			 rank[i] = ++r;
		}
		else{
			rank[i] = r;
		}
	}
	rank.push_back ( length );
//	for ( int level = 0; level < log(length)/log(2); level++ ){
//	for ( int level = 0; level < length; level++ ){
	for ( int tt = t; tt < max_pi; tt = tt << 1 ){
		cout << tt << endl;
		sign_update ( sign, sign_base, tt );
		int l = 1;
		r = 0;
		cout << "sign check:" << sign[6430] << '\t' << sign[6431] << endl;
		cout << "origin sign check:" << sign_base[PSA[6430]+tt] << '\t' << sign_base[PSA[6431]+tt] << endl;
		for ( int i = 1; i < length+1; i++ ){
			if ( PSA[i] == 860364 ) cout << i << endl;
			if ( (i == length) || (rank[i] != rank[i-1]) ){
					radixsort ( sign.begin()+i-l, PSA.begin()+i-l, l );
					next_rank[i-l] = ++r;
					for ( int j = i-l+1; j < i; j++ ){
						if ( sign[j] != sign[j-1] )
							next_rank[j] = ++r;
						else
							next_rank[j] = r;
					}
					l = 1;
				}
			else{
				l++;
			}
		}
		if ( tt > max_pi ) break;
		for ( int i = 0; i < length; i++ )
			rank[i] = next_rank[i];
		next_rank.clear();
		sign_double ( sign_base, tt );
	}
}

vector<word> PropertySuffixArray::signature( vector<int>& integer_text ){
	vector<word> sign (length);
	sign[0] = 0;
	for ( int j = 0; j < t; j++ ){
		sign[0] += integer_text[j] * (1 << (sigma*( t-j-1) ) );
	}
	for ( int i = 1; i < length; i++ ){
		sign[i] = ( sign[i-1] - integer_text[i-1] * ( 1<<(sigma*(t-1)) ) ) * (1<<sigma) + integer_text[i+t-1];
	}
	for ( int i = 0; i < length; i++ ){
		if ( property[i] < t ){
			int remove = (1<<(sigma*t)) - (1<<(sigma*(t-property[i])));
			sign[i] = sign[i]&remove;
		}
	}
	return sign;
}

void PropertySuffixArray::sign_double( vector<word>& sign, int tt ){
	for ( int i = 0; i < length-tt; i++ ){
		sign[i] = (sign[i]<<(sigma*tt)) + sign[i+tt];
	}
	for ( int i = length-tt; i < length; i++ ){
		sign[i] = (sign[i]<<(sigma*tt));
	}
}

void PropertySuffixArray::sign_update( vector<word>& sign, vector<word>& sign_base, int tt ){
	for ( int i = 0; i < length; i++ ){
		if ( property[PSA[i]] <= tt ){
			sign[i] = 0;
		}
		else if ( property[PSA[i]] < tt*2 ){
			int remover = ( 1<<sigma*tt ) - ( 1<<(sigma*( tt*2-property[PSA[i]] ) ) ) ;
			sign[i] = sign_base[PSA[i]+tt];
			sign[i] = sign[i] & remover;
		}
		else{
			sign[i] = sign_base[PSA[i]+tt];
		}
	}
}

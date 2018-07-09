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

PropertySuffixArray::PropertySuffixArray( PropertyString & S, string& alphabet){
	cout << sizeof(int) << endl;
	const string& text = S.string();
	int length = S.length();
	vector<int> const& property = S.property();
	int sigma = ceil( log(alphabet.size()+1) / log(2) );
	int t = floor( log(length)/log( 1<<sigma ) );
	cout << "t=" << t << endl;
	PSA.resize ( length );
	iota( PSA.begin(), PSA.end(), 0 );
	int max_pi = 0;
	for ( int i : property ){
		if ( i > max_pi ) max_pi = i;
	}
	cout << "max property:" << max_pi << endl;

	map<char,int> char_to_int;
	for( int i = 0; i < (int) alphabet.size(); i++ ){
		char_to_int[alphabet[i]] = i+1;
	}
	vector<int> integer_text ( length+t );
	for ( int i = 0; i < length; i++ ){
		integer_text[i] = char_to_int[S[i]];
	}
	vector<int> sign = signature( integer_text, property, length, t, sigma );
	vector<int> sign_original ( sign );
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
	for ( int level = 0; level < length; level++ ){
		int tt = t * (level+1);
//		cout << "tt:" << tt << endl;
		for ( int i = 0; i < length; i++ ){
			if ( property[PSA[i]] <= tt ){
				sign[i] = 0;
			}
			else if ( property[PSA[i]] < tt + t ){
				int remover = ( 1<<sigma*t) - ( 1<<(sigma*max(t-(property[PSA[i]]-tt), 0)));
				sign[i] = sign_original[PSA[i]+tt];
				sign[i] = sign[i] & remover;
			}
			else{
				sign[i] = sign_original[PSA[i]+tt];
			}
		}
		int l = 1;
		r = 0;
		for ( int i = 1; i < length+1; i++ ){
			if ( (i == length) || (rank[i] != rank[i-1]) ){
				radixsort ( sign.begin()+i-l, PSA.begin()+i-l, l );
				next_rank[i-l] = ++r;
				for ( int j = i-l+1; j < i; j++ ){
					if ( level == 6 && j==6448 ) cout << "Here" << (sign[j]!=sign[j-1]) << endl;
					if ( sign[j] != sign[j-1] ){
						next_rank[j] = ++r;
					if ( level == 6 && j==6448 ) cout << "Here" << next_rank[j-1] << ' ' << next_rank[j] << endl;
					}
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
#if 0
		if ( level == 6 ){
			cout << "after sort" << endl;
			for ( int i = 6445; i < 6450; i++ ){
//				if ( PSA[i] == 963945 ) cout << i << ' ' << (property[PSA[i]] < tt+t) << endl;
//				if ( PSA[i] == 713945 ) cout << i << ' ' << (property[PSA[i]] < tt+t) << endl;
				cout << i << ":" << PSA[i] << ":"  << rank[i] << ":" << sign[i] << ":" << text.substr(PSA[i]+tt, max(property[PSA[i]]-tt,0) ) << endl;
			}
			cout << endl;
		}
#endif
	}
}

vector<int> PropertySuffixArray::signature( vector<int>& text, vector<int> const& property, int n, int t, int sigma ){
	vector<int> sign (n);
	sign[0] = 0;
	for ( int j = 0; j < t; j++ ){
		sign[0] += text[j] * (1 << (sigma*( t-j-1) ) );
	}
	for ( int i = 1; i < n; i++ ){
		sign[i] = ( sign[i-1] - text[i-1] * ( 1<<(sigma*(t-1)) ) ) * (1<<sigma) + text[i+t-1];
	}
	for ( int i = 0; i < n; i++ ){
		if ( property[i] < t ){
			int remove = (1<<(sigma*t)) - (1<<(sigma*(t-property[i])));
			sign[i] = sign[i]&remove;
		}
	}
	return sign;
}


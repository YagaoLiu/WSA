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
	vector<int> rank;
	rank.reserve(length);
	rank.push_back(0);
	for ( int i = 0; i < 10; i ++ )
		cout << sign[i] << endl;
	radixsort ( sign.begin(), PSA.begin(), length );
	for ( int i = 0; i < 10; i++ )
		cout << sign[i] << endl;
	for ( int i = 1; i < length; i++ ){
		if ( sign[i] != sign[i-1] ){
			rank.push_back(i);
		}
	}
	rank.push_back ( length );
//	for ( int level = 0; level < log(length)/log(2); level++ ){
	for ( int level = 0; level < length; level++ ){
		int tt = t * (level+1);
		cout << "tt:" << tt << endl;
		for ( int i = 0; i < length; i++ ){
			if ( (tt== 12) && (i == 745355) ) cout << sign[745355] << endl;
			if ( property[PSA[i]] <= tt ){
				sign[PSA[i]] = 0;
			}
			else if ( property[PSA[i]] < tt + t ){
				int remover = ( 1<<sigma*t) - ( 1<<(sigma*max(t-(property[PSA[i]]-tt), 0)));
				sign[PSA[i]] = sign_original[PSA[i]+tt];
				sign[PSA[i]] = sign[PSA[i]] & remover;
			}
			else{
				sign[PSA[i]] = sign_original[PSA[i]+tt];
				if ( (tt== 12) && (PSA[i] == 745355) ) cout << "else" << sign[PSA[i]] << ' ' << sign_original[PSA[i]+tt] << endl;
			}
		}
				if ( tt == 12 ) cout << "here" << sign[745355] <<  endl;
		bool break_point = true;
		int n = 0;
		for ( int r = 0; r < (int) rank.size()-1; r++ ){
			bool sort_check = false;
			for ( int i = rank[r]; i < rank[r+1]; i++ ){
				if ( sign[i] != 0 ){
					sort_check = true;
				}
			}
			if ( (rank[r+1]-rank[r] > 1) ){
						radixsort ( sign.begin()+rank[r], PSA.begin()+rank[r], rank[r+1]-rank[r] );
						break_point = false;
						n += rank[r+1]-rank[r];
			}
		}
		if ( break_point ) break;
		if ( tt > max_pi ) break;
		rank.clear();
		rank.push_back(0);
		for ( int i = 1; i < length; i++ ){
			if ( sign[i] != sign[i-1] ){
				rank.push_back(i);
			}
		}
		rank.clear();
		rank.push_back(0);
		for ( int i = 1; i < length; i++ ){
				if ( sign[i] != sign[i-1] )
					rank.push_back(i);
		}
		rank.push_back ( length );
	}
}

vector<int> PropertySuffixArray::signature( vector<int>& text, vector<int> const& property, int n, int t, int sigma ){
	auto begin = get_time::now();
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
	auto end = get_time::now();
	auto diff = end - begin;
	cout << "Time for signature:" << chrono::duration_cast<chrono::seconds>(diff).count() << "s" << endl;
	return sign;
}


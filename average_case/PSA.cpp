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

void PropertySuffixArray::NextRank(vector<int>& sign){

}

PropertySuffixArray::PropertySuffixArray( PropertyString & S, string& alphabet){
	int length = S.length();
	vector<int> const& property = S.property();
	int sigma = ceil( log(alphabet.size()+1) / log(2) );
	int t = floor( log(length)/log( 1<<sigma ) );
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

//	for ( int i = 0; i < length; i++ ){
//		cout << setw(3) << property[i]; 
//	}
//	cout << endl;	

	vector<int> sign = signature( integer_text, property, length, t, sigma );
//	for ( int i = 0; i < length; i++ ){
//		cout << setw(3) << sign[i];
//	}
//	cout << endl;
	vector<int> sign_original ( sign );
	vector<int> index ( length );
	vector<int> rank;
	rank.reserve(length);
	rank.push_back(0);
	iota ( index.begin(), index.end(), 0 );
	radixsort ( sign.begin(), PSA.begin(), length );
	for ( int i = 1; i < length; i++ ){
		if ( sign[i] != sign[i-1] && sign[i] != 0 ){
			rank.push_back(i);
		}
	}
	rank.push_back ( length-1 );
//	for ( int i = 0; i < length; i++ ){
//		cout << setw(3) << PSA[i];
//	}
//	cout << endl;
//	cout << "into loop" << endl;
	cout << "max level:" << log(length) / log(2) << endl;
	for ( int level = 0; level < log(length)/log(2); level++ ){
		vector<int> next_rank;
		next_rank.reserve( rank.size() );
		next_rank.push_back[0];
		int tt = t * (level+1);
		for ( int i = 0; i < length; i++ ){
			if ( PSA[i] > length-1-tt ){
				sign[i] = 0;
			}
			if ( property[PSA[i]] <= tt ){
				sign[i] = 0;
			}
			if ( property[PSA[i]] < tt + t ){
				int remover = ( 1<<sigma*t) - ( 1<<(sigma*(t-(property[PSA[i]]-tt))));
				sign[i] = sign_original[PSA[i]+tt];
				sign[i] = sign[i] & remover;
			}
			else{
				sign[i] = sign_original[PSA[i]+tt];
			}
		}
		for ( int r = 0; r < (int) rank.size()-1; r++ ){
			if ( rank[r+1]-rank[r] > 1 ){
				radixsort ( sign.begin()+rank[r], PSA.begin()+rank[r], rank[r+1]-rank[r] );
				for ( int i = rank[r]; i < rank[r+1]; r++ ){
					if ( sign[i] != sign[i-1] ){
						rank.push_back(i);
					}
				}
			}
		}
		rank.clear();
		rank.push_back(0);
		for ( int i = 1; i < length; i++ ){
			if ( sign[i] != sign[i-1] ){
				rank.push_back(i);
			}
		}
		cout << tt << endl;
		if ( tt > max_pi ) break;
		if ( rank.size() >= length ) break;
		rank.push_back ( length-1 );
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


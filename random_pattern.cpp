#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int n = stoi(argv[1]);
	int m = stoi(argv[2]);
	string name[m];
	for ( int i = 0; i < m; i++ )
	{
		name[i] = "text" + to_string(i);
		ofstream output( name[i] );
		output << n << endl;
		output << "ACGT" << endl;
		for ( int j = 0; j < n; j++ )
		{
			int k = rand()%101;
			int r = 100 - k;
			cout << r << endl;
			double l = (double) k /100;
			output << l << '\t';
			if ( r != 0 )
				k = rand()%r;
			else
				k = 0;
			r = r - k;
			l = (double) k / 100;
			output << l << '\t';
			if ( r != 0 )
				k = rand()%r;
			else
				k = 0;
			r = r - k;
			l = (double) k / 100;
			output << l << '\t';
			l = (double) r / 100;
			output << l << '\t';
			output << '\n';
		}
		output.close();
	}
}

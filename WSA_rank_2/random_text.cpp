#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main ( int argc, char ** argv )
{
	char * pEnd;
	int n = strtol ( argv[1], &pEnd, 10 );

	string alphabet = "ACGTRYSWKM";
	srand ( time(0));
	string text(n,'A');
	for ( int i = 0; i < n; i++ )
	{
		int k = rand()%10;
		text[i] = alphabet[k];
	}
	ofstream output ( "random_text.fa" );
	output << ">random_text\n" << text << endl;
	return 0;
}

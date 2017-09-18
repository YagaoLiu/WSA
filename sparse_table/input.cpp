#include <iostream>
#include <string>
#include <cstring>
#include <getopt.h>

#include "defs.h"

static struct option long_options[] =
{
	{ "text",				required_argument,	NULL,	't' },
	{ "output",				required_argument,	NULL,	'o' },
	{ "threshold",			required_argument,	NULL,	'z'	},
	{ "mod",				required_argument,  NULL,   'm' },
	{ "help",				0,					NULL,	'h' },
	{ "pattern",			required_argument,  NULL,	'p' },
};

int decode_switches ( int argc, char * argv[], struct TSwitch * sw )
{
	int opt;
	char *ep;
	double val;
	int args;

	/* initialisation */
	sw -> z						=	1;

	args = 0;

	while ( ( opt = getopt_long ( argc, argv, "t:p:m:o:z:h", long_options, NULL ) ) != -1 )
	{
		switch ( opt )
		{
			case 't':
				sw -> text_file_name = optarg;
				args ++;
				break;	
			case 'p':
				sw -> pattern_file_name = optarg;
				args ++;
				break;
			case 'o':
				sw -> output_file_name = optarg;
				args ++;
				break;
			case 'z':
				val = strtod ( optarg, &ep );
				if ( optarg == ep )
				{
					return 0;
				}
				sw -> z = val;
				args ++;
				break;
			case 'm':
				val = strtol ( optarg, &ep, 10 );
				if ( optarg == ep )
				{
					return 0;
				}
				sw -> mod = val;
				args++;
				break;
			case 'h':
				return 0;
		}
	}

	if ( args < 2 )
	{
		usage();
		exit ( 1 );
	}
	else
		return ( optind );
}

void usage ( void )
{
	cout << "Usage: Weighted Index <options>"  << endl;
	cout << "Standard (Mandatory):" << endl;
	cout << "	-t, --text\t<str>\tFilename for Text." << endl;
	cout << "	-o, --output\t<str>\tFilename for Output. Default \"Result.txt\"" << endl;
	cout << "	-z, --threshold\t <dbl>\tcumulative weight threshold."<<endl;
}

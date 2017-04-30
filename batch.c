#include "batch.h"

int main(int argc, char* argv[])                    
{
	
	char filename[20];
	FILE *file;
	
	if(argv[1] != NULL)
	{
		if( ( file = fopen(argv[1],"r") ) == NULL)
	    {
	    	printf("file does n't exist , please try again later \n");
	    	exit(0);
	    }
	}
	else
	{
		printf("\n please provide valid command line argument");
		exit(0);
	}

    main_module(file);
   	printf("Shell Run Succesfully ....Check files for output \n");
   	return 0;
}


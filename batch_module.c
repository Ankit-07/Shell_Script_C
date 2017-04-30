#include "batch.h"

const char *input = "\n Your input is => ";
const char *output = "\n Your Output is => \n\n";

int interflag = 0;
int line_number = 0;

int top = -1;
int stack[1];


void push(int data)
{
	top++;
	stack[top] = data;
}

int pop()
{
	int temp = stack[top];
	top--;
	return temp;
}

void main_module(FILE *file)
{
	removeComment(file);
    removeBlankSpace();

    FILE *fp = fopen("NewBatchFile.txt","r");

    if (checkBeginEnd(fp))
    {
    	printf("No End of progarmme ....progarmme Terminated abruptly..");
    	exit(0);
    }

	if(fp == NULL)
	{
		printf("File Doesn't exist in the directory");
		exit(EXIT_FAILURE);
	}
	fseek(fp,0,SEEK_SET);
	readLiner(fp);
}

int checkBeginEnd(FILE *fp)
{
		size_t len = 0,read;
		char *line = NULL;
		int flag = 0;

		while ((read = getline(&line, &len, fp)) != -1) 
		{	
			line[strlen(line)] = '\0';
			if (checkRead(line,6,BEGIN))
	        	flag = 1;
	        else if(checkRead(line,4,END))
	        	flag = 0;
		}
		return flag;
}

void readLiner(FILE *fp)
{
		size_t len = 0,read;
    	int i;
		char *line = NULL;

		while ((read = getline(&line, &len, fp)) != -1) 
		{
	        line_number++;
			line[strlen(line)] = '\0';

	        if (checkRead(line,6,BEGIN))
	        {
	        	while ((read = getline(&line, &len, fp)) != -1)
	        	{
	        		line_number++;
	        		line[strlen(line)-1] = '\0';
	        	
	        		if(checkRead(line,6,BEGIN))
	        			continue;
	        	
	        		if(checkRead(line,4,END))
	        			break;
	        	
	        		if(checkRead(line,11,INTERSTART))
	        			{	interflag = 1;  continue; }
	        	
	        		if(checkRead(line,10,INTERSTOP))
	        			{   interflag = 0;  continue;  }
	       
	        		
	        		if(interflag)
	        		{
	        			execute_instruction(line,line_number);
	        		}
	        		else
	        		{
	        			execute_instruction(line,0);
	        		} 
	        	}
	   		}
		}
}



void execute_instruction(char *line,int line_number)
{
	int pid,index=0;
	ssize_t numRead;
	char buf[BUF_SIZE];
	pid_t wpid;
	int status;
	int i=0,j,prev=0,k=0;

	int outputfile;
	int inputfile;
	int wredirect=0,prewredirect=0;
	int rredirect=0,prerredirect=0;

	char **word = split(line);
 	char **fcommand;
 	char *command;
 	
	while(word[i] != NULL)
	{
		command = (char *)malloc(strlen(word[i])*sizeof(char));
	 	strcpy(command,word[i]);

		while(word[i] != NULL && ( strcmp(word[i],"|") && strcmp(word[i],">") && strcmp(word[i],">>") ) )
	    {
	    	i++;
	    }
	    
	    if(wredirect)
	    	prewredirect = 1;
	    else if(rredirect)
	    	prerredirect = 1;
	    else
	    	prerredirect = 0 , prewredirect = 0;


	    if(word[i] != NULL &&  !strcmp(word[i],">"))
	    	wredirect = 1;
	    else if(word[i] != NULL &&  !strcmp(word[i],">>"))
	    	rredirect = 1;
	    else
	    	rredirect = 0,wredirect = 0;

	    fcommand = (char **) calloc((i-prev)+1,sizeof(char *));
    	
	    for(j=0;j<(i-prev);j++)
	    {
	    	fcommand[j] = (char *) calloc(strlen(word[j+prev]),sizeof(char));
	    	strcpy(fcommand[j],word[j+prev]);
	    }
    	fcommand[j] = NULL;

		prev = i + 1;

		switch (pid = fork()) 
		{ 
		case -1:
		 	perror("fork"); 
		 	exit(1); 
		case 0: /* child */

		 	childProcessExecution(index,prewredirect,prerredirect,fcommand,word[i-1],outputfile,inputfile);

			if(index==0)
			{   
				clearFile("temp1.txt");
				outputfile = open("temp1.txt", O_WRONLY | O_CREAT , 0666);
				dup2(outputfile,1);
				close(outputfile);
			}
			else
			{	
				
				if(prewredirect || prerredirect)
				{
					execute_redirect(prerredirect,prewredirect,word[i-1],index);
					exit(0);
				}

				execute_normalinstruction(index,outputfile,inputfile);
				
				close(inputfile);
				close(outputfile);
			}

				execvp(fcommand[0], fcommand);  
				perror("manual error"); 
		default: 
			// parent part
			do {
		     wpid = waitpid(pid, &status, WUNTRACED);
		    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
			
			index++;
			if(word[i] != NULL)
    		{	i++;   }

    		loggedtoFile(line_number,command,index);
				
			for (k = 0; k <= j; k++)
			{
				free(fcommand[k]);
			}
			free(fcommand);
			break;
		}
		
	}
}

void childProcessExecution(int index,int prewredirect,int prerredirect,char **fcommand,char *word,int outputfile,int inputfile)
{
			if(index==0)
			{   
				clearFile("temp1.txt");
				outputfile = open("temp1.txt", O_WRONLY | O_CREAT , 0666);
				dup2(outputfile,1);
				close(outputfile);
			}
			else
			{	
				
				if(prewredirect || prerredirect)
				{
					execute_redirect(prerredirect,prewredirect,word,index);
					exit(0);
				}

				execute_normalinstruction(index,outputfile,inputfile);
				
				close(inputfile);
				close(outputfile);
			}
				execvp(fcommand[0], fcommand);  
				perror("manual error"); 
}

void loggedtoFile(int line_number,char *command,int index)
{
			if(line_number)
				{
					char *filename;
					FILE *fp1,*fp2;
					char buffer[10];
				 	int a;
					snprintf(buffer, 10, "%d", line_number);
					filename = (char *) malloc(sizeof(char)*(strlen(buffer)+strlen(command)+1));
					buffer[strlen(buffer)] = '\0';
					strcpy(filename,buffer);
					strcat(filename,"_");
					strcat(filename,command);
					clearFile(filename);

					if( (index%2) != 0)
						fp1 = fopen("temp1.txt","r");
					else
						fp1 = fopen("temp2.txt","r");

					fp2 = fopen(filename,"w");
					a = getc(fp1);
					while(a != EOF)
					{
						putc(a,fp2);
						a = getc(fp1);
					}
					fclose(fp1);
					fclose(fp2);
				}
				else
				{
					int a;
					FILE *fd1,*fd2;
					if( (index%2) != 0)
						fd1 = fopen("temp1.txt","r");
					else
						fd1 = fopen("temp2.txt","r");
					fd2 = fopen("OUTPUT.txt", "a");
				    fseek(fd2, 0,SEEK_END);
				    fwrite(input ,1 , strlen(input),fd2);
				    fwrite(command ,1 , strlen(command),fd2);
				    fwrite("\n",1,strlen("\n") , fd2);
				    a = getc(fd1);
					while(a != EOF)
					{
						putc(a,fd2);
						a = getc(fd1);
					}

					fclose(fd1);
					fclose(fd2);
				}
}


void execute_normalinstruction(int index,int outputfile,int inputfile)
{
				if( (index%2 )!= 0)
				{
					clearFile("temp2.txt");
					outputfile = open("temp1.txt", O_RDONLY | O_CREAT , 0666);
					inputfile = open("temp2.txt",O_WRONLY | O_CREAT , 0666);
					dup2(outputfile,0);
					dup2(inputfile,1);
					dup2(inputfile,2);
				}
				else
				{
					clearFile("temp1.txt");
					outputfile = open("temp1.txt", O_WRONLY | O_CREAT , 0666);
					inputfile = open("temp2.txt",O_RDONLY | O_CREAT , 0666);
					dup2(inputfile,0);
					dup2(outputfile,1);
					dup2(outputfile,2);
				}
}


void execute_redirect(int prerredirect,int prewredirect,char *word,int index)
{
					FILE *fd1,*fd2,*fd3;
					int data;

					if((index%2 )!= 0)
					{
						fd1 = fopen("temp1.txt","r");
						fd2 = fopen("temp2.txt","w"); 
					}
					else
					{
						fd1 = fopen("temp2.txt","r");
						fd2 = fopen("temp1.txt","w");
					}

					if(prewredirect)
						fd3 = fopen(word,"w");
					else
						fd3 = fopen(word,"a");

					data = getc(fd1);
					while(data != EOF)
					{
						putc(data,fd2);
						putc(data,fd3);
						data = getc(fd1);
					}
					
					fclose(fd1);
					fclose(fd2);
					fclose(fd3);
}


void clearFile(char *filename)
{
	FILE *fd = fopen(filename,"w");
	fclose(fd);
}


char** split(char *fline)
{
	const char *s = " ";
    char *token;
    int count = 0,i;
    char **word = 0;
   
   /* get the first token */
   token = strtok(fline, s);
   
   /* walk through other tokens */
   while( token != NULL ) 
   {
   	  	word = (char **) realloc(word,sizeof(char *)*(count+1));
    	word[count] = (char *) malloc (strlen(token)*sizeof(char));
    	strcpy(word[count],token);
    	int len = strlen(token);
      token = strtok(NULL, s);
      count++;
   }
   word = (char **) realloc(word,sizeof(char *)*(count+1));
   word[count] = NULL;
   return word;
}



int checkRead(char *line,int charlength,char *string)
{
	int i,flag = 1;
	for(i=0; i<charlength;i++)
	{
		if(*(line+i) == '\0' || *(string+i) == '\0')
		{	
			flag = 0;
			break;
		}
		if( line[i] != string[i] )
		{
			flag = 0;
			break;	
		}
	}
	return flag;
}

int removeComment(FILE *fp1)
{
		int ch, i = 0, flag = 0, prev = '\0';
        FILE *fp2;
        char temp[] = "temporary.txt";
        char *line = NULL;
		size_t len = 0;
    	ssize_t read;

        /* open the temporary file in write mode */
        fp2 = fopen(temp, "w");

        /* error handling */
        if (!fp2) {
                printf("Unable to open temporary file!!\n");
                return 0;
        }
        /* removes comments from the given input file */
        prev = fgetc(fp1);
        while ((ch = fgetc(fp1)) != EOF) {
 
                /* flag is 1 - double slash comment */
                if (flag == 1) {
                        /* skip the contents until you detect \n */
                        if (ch == '\n') {
                                flag = 0;
                                fputc(ch, fp2);
                                prev = fgetc(fp1);
                        }
                        continue;
                }
 
                /* flag is 2 - slash arsterix comment */
                if (flag == 2) {
                        /* skip the contents until you detect asterix slash */
                        if (ch == '/' && prev == '*') {
                                flag = 0;
                                prev = fgetc(fp1);
                        }
                        continue;
                }
 
                /* checking for double slash comment */
                if (ch == '#'/*&& prev == '/'*/) {
                		long int where = ftell(fp1);
                		read = getline(&line, &len, fp1);
                		line[strlen(line)-1] = '\0';
                		if(!(checkRead(line,10,InterStart) || checkRead(line,10,InterStop)  ) )
                		{	
                        	flag = 1;
                        }
                        else
                        {
                        	fputc(prev, fp2);
                        }
                        fseek(fp1, where, SEEK_SET);
                } else if (prev == '/' && ch == '*') {
                        /* slash asterix comment */
                        flag = 2;
                } else {
                        /* contents outside of comments */
                        fputc(prev, fp2);
                }
                prev = ch;
        }

        /* closing the input file */
        fclose(fp1);
        fclose(fp2);
}

int removeBlankSpace()
{
	FILE *fp, *fp2;
    int p,p1;
    char new = '\n';
    fp = fopen("temporary.txt","r");
    if (fp == NULL)
    {
        printf("File does n't exist");
        return 0;
    }
 
    fp2 = fopen("NewBatchFile.txt","w");
    
    while(EOF != (p = getc(fp) ) )
    {
        if (32==p) 
        {
            if(new != '\n')
            {   
                fputc(p,fp2);
            }
            while(32 == (p1=getc(fp)))
            {
 
            }
            fputc(p1,fp2);
            new = (char) p1;
        }
        else
        {
            fputc(p,fp2);
            new = (char) p; 
        }
        
    }
    fclose(fp);
    fclose(fp2);
}




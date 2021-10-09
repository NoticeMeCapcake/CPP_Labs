#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
	// printf("%s\n", "nfjkbadjhscb");
	if (argc != 3)
		exit(-1);
	FILE *fin, *fout;
	fin = fopen(argv[1], "r");
	char input[50];
	char *in;
	char file1[] = "task";
	char *argvs[4];
	int ret = 0;
	while(1)
	{
		fout = fopen(argv[2], "a");
		in = fgets(input, sizeof(input), fin);
		if (in == NULL)
			break;

		argvs[0] = malloc(strlen(file1) + 1);
		strncpy(argvs[0], file1, strlen(file1));
		argvs[0][strlen(file1)] = '\0';
		printf("%s\n", "000");
		argvs[1] = malloc(strlen(in));
		strncpy(argvs[1], in, strlen(in) - 1);
		argvs[1][strlen(in) - 1] = '\0';
		printf("%s\n", "111");
		argvs[2] = malloc(strlen(argv[2]) + 1);
		strncpy(argvs[2], argv[2], strlen(argv[2]));
		argvs[2][strlen(argv[2])] = '\0';
		printf("%s\n", "222");

		argvs[3] = NULL;

		fprintf(fout, "%s = ", argvs[1]);
		fclose(fout);
		switch(fork())
		{
		  case -1:
		  // fprintf("1%s\n", argvs[0]);
		  // free(argvs[0]);
		  // free(argvs[1]);
		  // free(argvs[2]);
		  break;
		  case 0 :
		  // printf("2%s\n", argvs[0]);
			ret = execv(file1, argvs);
		  // printf("ret = %d\n", ret);
		  break;
		  default:
		  // printf("%s\n", "def");
		  wait(0);
		}
		// printf("%s\n", argvs[0]);
		// printf("%s\n", "broke");
		free(argvs[0]);
		free(argvs[1]);
		free(argvs[2]);
	}
	return 0;

}

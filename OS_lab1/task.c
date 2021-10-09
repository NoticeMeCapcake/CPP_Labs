#include<stdio.h>
#include<stdlib.h>
// #include<sys/types.h>
// #include<unistd.h>
int main(int argc, char const *argv[])
{
	double answer = 0;
	printf("here %f\n", answer);
	if (argc != 3)
		return -1;
	char *ptr;
	double num1 = strtod(argv[1], &ptr);
	char operation = *ptr;
	ptr++;
	double num2 = strtod(ptr, NULL);
	FILE *fout;
	fout = fopen(argv[2], "a");
	if (fout == NULL) return -2;
	switch(operation)
	{
		case '+':

			answer = num1 + num2;
			break;
		case '-':
			answer = num1 - num2;
			break;
		case '*':
			answer = num1 * num2;
			break;
		case '/':
			answer = num1 / num2;
			break;
	}
	fprintf(fout, "%f\n", answer);
	fclose(fout);
	printf("here %f\n", answer);
	return 0;

}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct question{
	char q[120],answer[40];
};
int main(void){
	FILE *infile;
	struct question input;
	infile = fopen("ques","r");
	int num = 0;			
	ssize_t read;
	size_t len =0;
	char *line;
	line = (char *)malloc(sizeof(char) * 120);
	while((read = getline(&line,&len,infile)) != -1){
		printf("%s\n",line );
		num++;
	}
	printf("%d\n",num);
	struct question ques[num];

	fseek(infile,0,SEEK_SET);
	for(int i =0 ; i < num;++i){
		read = getline(&line,&len,infile);
		printf("%s\n",line );
		if(i%2 == 0){
			strcpy(ques[i/2].q,line);
		}else{
			strcpy(ques[(i/2)-1].answer,line);
		}
	}
	for (int i = 0; i < num; ++i)
	{
		printf("%s\n", );
	}
}

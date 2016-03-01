#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct question{
	char q[120],answer[30];
};
int main(void){
	struct question q_1;
	int num,i;
	FILE *outfile;
	outfile = fopen("ques.data","w");
	printf("Enter the number of questions\n");
	num = 2;
	for(i = 0;i < num; ++i){
		printf("Enter the question\n");
		fgets(q_1.q,120,stdin);
		if((strlen(q_1.q) > 0) && (q_1.q[strlen(q_1.q)-1]=='\n'))
			q_1.q[strlen(q_1.q)-1] = '\0';
		printf("Enter the answer\n");
		fgets(q_1.answer,30,stdin);
		if((strlen(q_1.answer) > 0) && (q_1.answer[strlen(q_1.answer)-1]=='\n'))
			q_1.answer[strlen(q_1.answer)-1] = '\0';
		fwrite(&q_1,sizeof(struct question),1,outfile);
	}
}


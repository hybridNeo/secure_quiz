#include <stdio.h>
#include <stdlib.h>
struct question{
	char q[120],answer[30];
};
int main(void){
	FILE *infile;
	struct question input;
	infile = fopen("ques.data","r");
	int num = 0;			
	struct question temp;
	while(fread(&temp,sizeof(struct question),1,infile) != 0){
		//printf("%s \n %s \n",temp.q,temp.answer);
		num++;
	}
	fseek(infile,0,SEEK_SET);	
	printf("%d\n",num);
	struct question ques[num];
	for(int i =0 ; i < num;++i){
		fread(&ques[i],sizeof(struct question),1,infile);
		printf("%s \n %s \n",ques[i].q,ques[i].answer);
	}
}

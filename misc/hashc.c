#include <stdio.h>
#include <stdlib.h>
void convert(char *key,int len){
	int arr[20];
	
	for(int i = 0; i < len;i+=2){
		char temp[2];
		temp[0] = key[i];
		temp[1] = key[i+1];
		arr[i/2] = strtol(temp,NULL,16);
		printf("%02x\n",arr[i/2]);
	}
}
int main(void){

  char key[40] = "45126879ae4f157076fee52d4b801be686763421";
  convert(key,40);
}

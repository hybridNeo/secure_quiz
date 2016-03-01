#include <stdio.h>
int main(void){
	char a[6] = "hello";
	printf("Before Encryption %s \n",a);
	int secret = 7;//this is our secret
	for(int i=0;i< 6;++i){
		a[i] += secret;
	}
	printf("Encrypted string %s \n", a);

}

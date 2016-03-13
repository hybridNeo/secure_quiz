#include "md5.h"
const char *md5sum(const char *chaine, size_t len)
{
     struct md5_ctx ctx;
     unsigned char digest(16);
     md5_init(&ctx);
     ctx.size = len?len:strlen(chaine);
     strcpy(ctx.buf, chaine);
     md5_update(&ctx);
     md5_final(digest, &ctx);
     return digest;
}
int main(void){
	printf("sum is %s ",md5sum("a",1));
}

/*
 * @author - Rahul Mahadev
 * Helper library for secure quiz
 */
char* string_copy(char *,const char *);
char* string_copy(char *dst,const char *src){
	char *ptr;
	ptr = dst;
	while((*dst++ = *src++));
	return ptr;
}
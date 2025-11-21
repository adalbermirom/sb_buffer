
/*
     gcc -o test ../src/sb_buffer.c -I../include test.c

*/

#include "sb_buffer.h"

int main(int argc, char **argv){
	SB_Buffer b;
	int ret;
	/* Inicialização */
	ret = sb_buffer_init(&b);
	if (!ret) return 1;
	
	/* Adição de strings */
	ret = sb_buffer_add_lstring(&b, "Hello", 5);
	if (!ret) return 1;
	
	ret = sb_buffer_add_literal(&b, " World");
	if (!ret) return 1;
	
	ret = sb_buffer_add_string(&b, " using SB_Buffer!");
	if (!ret) return 1;
	
	/* get string content and length*/
	printf("Output: %s\n", b.str);
	/* length */
	printf("Length: %zu\n", b.len); 
	
	/* SB_INIT: alias to SB_Buffer b; sb_buffer_init(&b);
	 * Use it only inside functions!
	 * */
	SB_Buffer SB_INIT(dest);
	
	//sb_buffer_init(&dest);
	
	ret = sb_buffer_copy(&b, &dest);
	
	if (!ret){
		fprintf(stderr, "Error in copy function.\n");
		return 1;
	}
	
	
	printf("Output copy: %s\n", dest.str);
	printf("Length copy: %zu\n", dest.len);
	
	/* Clear*/
	sb_buffer_finalize(&b);
	sb_buffer_finalize(&dest);
	
	
	return 0;
}

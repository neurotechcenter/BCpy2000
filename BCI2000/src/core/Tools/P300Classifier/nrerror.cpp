#include"nrerror.h"

void nrerror(const char* error_text)
{
	fprintf(stderr, "%s\n", error_text);
	exit(1);
}

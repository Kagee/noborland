#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <fyut.h>

#include<libgen.h>

char * _ffullpath(char *buffer, const char *pathname, size_t maxlen) {
	UT_StrCopy(buffer, pathname, maxlen);
}

int main(int argc, char** args) {
	if(argc < 2) {
		exit(1);
	}
	//	printf("Input: %s\n", args[1]);
	
   char absPath[_MAX_PATH], absPath2[_MAX_PATH];
   _fullpath (absPath, args[1], _MAX_PATH);
   _ffullpath(absPath2, args[1], _MAX_PATH); 
   if (!(strcmp(absPath, absPath2) == 0)) {
	printf("THEY DIFFER:\n");
	printf("PRIO: '%s'\n", absPath);
	printf("FREE: '%s'\n\n", absPath2);   		
   } else {
	printf("BOTH: '%s'\n\n", absPath);
   }
}


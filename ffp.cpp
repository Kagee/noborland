#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <fyut.h>
#include <new> // nothrow
#include<libgen.h>
/*
Description     _fullpath converts the relative path name name 'pathname'
                to a fully qualified pathname, stored in 'buffer'.  The
                     relative path can contain ".\" and "..".

                The maximum size of the supplied buffer is 'maxlen'.
                If the fully qualified path is longer than 'maxlen',
                NULL is returned.  The buffer should be at least _MAX_PATH
                bytes long (this constant is defined in stdlib.h).

                If 'buffer' is NULL, a buffer to store the fully qualified
                path is allocated and is returned.  The calling program
                must free this buffer with GlobalFree(() when it is no longer needed.

                If the pathname does not specify a disk drive, the current
i                drive is used.

 Allocate a temporary buffer to hold the fully qualified path.
if ((tempbuf = (char*)UT_MALLOC(_MAX_PATH*2+1)) == NULL)
      return (NULL);


return (char*)(realloc(tempbuf,len));
*/

char * _ffullpath(char *buffer, const char *pathname, size_t maxlen) {
	if(maxlen < _MAX_PATH) {
		return NULL;
	}
	char * wpath;
	wpath = new (std::nothrow) char[strlen(pathname) +1];
	if (wpath == NULL) { // could not allocate memory
		return NULL;
	}
	char * wpath2;
	wpath2 = new (std::nothrow) char[strlen(pathname) +1];
	if (wpath2 == NULL) {
		delete [] wpath;
		return NULL;
	}
	char * wpath3; // working path
    wpath3 = new (std::nothrow) char[strlen(pathname) +1];
    if (wpath3 == NULL) {
        delete [] wpath;
		delete [] wpath2;
        return NULL;
    }
	int PATHLEN = strlen(pathname);
	buffer[0] = '\0';
	strcpy(wpath, pathname);	
	//strcpy(wpath2, pathname);
    //strcpy(wpath3, pathname);
	
	char prev[5];
	prev[0] = UT_SLASH; prev[1] = '.';  prev[2] = '.';  
	prev[3] = UT_SLASH; prev[4] = '\0';

	char same[4];
	same[0] = UT_SLASH; same[1] = '.';
	same[2] = UT_SLASH; same[3] = '\0';
	char * prevPos = NULL;
	int i = -1;
	while ((prevPos = strstr(wpath, prev)) != NULL) {
		printf("AFTER PREV: %s\n", (prevPos+strlen(prev)));
		while(prevPos > wpath && prevPos[i] != UT_SLASH) {
			prevPos[i--] = '\0';
		}
		
		printf("BEFORE PREV: %s\n", wpath);
		UT_StrCopy(wpath2, wpath, PATHLEN);
		strncat(wpath2, (prevPos+strlen(prev)), PATHLEN);
		UT_StrCopy(wpath, wpath2, PATHLEN);
		printf("NO PREV: %s\n", wpath2);
		i = -1;
	}
	
	if(wpath[0] != UT_SLASH) {
		char cwd[_MAX_PATH];
		if(getcwd(cwd, _MAX_PATH) == NULL) {
			delete [] wpath;
		    delete [] wpath2;
		    delete [] wpath3;
			return NULL;
		}
		strncat(cwd, "/", _MAX_PATH-(strlen(cwd)+1));
		UT_StrCopy(buffer, cwd, _MAX_PATH);
		printf("CWD: %s\n", cwd);
	}

	
	printf("PATH: %s\n", wpath);
	if(wpath[0] == '/') {
		UT_StrCopy(buffer, wpath+1, maxlen);
	} else {
		UT_StrCopy(buffer, wpath, maxlen);
	}
	delete [] wpath;
	delete [] wpath2;
	delete [] wpath3;
	return buffer;
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


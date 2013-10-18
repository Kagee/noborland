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
	wpath = new (std::nothrow) char[_MAX_PATH];
	if (wpath == NULL) { // could not allocate memory
		return NULL;
	}
	
	char * wpath2;
	wpath2 = new (std::nothrow) char[_MAX_PATH];
	if (wpath2 == NULL) {
		delete [] wpath;
		return NULL;
	}

	buffer[0] = '\0';
	strncpy(wpath, pathname, _MAX_PATH);	
	strncpy(wpath2, pathname, _MAX_PATH);

	/* Make relativer paths of ./foo-paths */
	if(pathname[0] == '.' && pathname[1] == UT_SLASH) {
		strncpy(wpath, pathname+2, _MAX_PATH);
		strncpy(wpath, pathname+2, _MAX_PATH);
	}

	char * prevPos = NULL;

	char prev[5] = { UT_SLASH, '.', '.', UT_SLASH, '\0' };

	int i = -1;
	/* Remove /../ and parent folder */
	while ((prevPos = strstr(wpath, prev)) != NULL) {
		/* Walk forward in the string until first slash or start of string */
		while((prevPos + i) >= wpath && prevPos[i] != UT_SLASH) {
			prevPos[i--] = '\0';
		}
		UT_StrCopy(wpath2, wpath, _MAX_PATH);
		strncat(wpath2, (prevPos+strlen(prev)), _MAX_PATH);
		UT_StrCopy(wpath, wpath2, _MAX_PATH);
		i = -1;
	}

	char same[4] = { UT_SLASH, '.', UT_SLASH, '\0' };

	/* replace /./ with nothing */
	while ((prevPos = strstr(wpath, same)) != NULL) {
        prevPos[1] = '\0';
		UT_StrCopy(wpath2, wpath, _MAX_PATH);
        strncat(wpath2, (prevPos+strlen(same)), _MAX_PATH);
        UT_StrCopy(wpath, wpath2, _MAX_PATH);
        i = -1;
    }
	
	/* prepend cwd on relative paths */
	if(wpath[0] != UT_SLASH) {
		if(getcwd(wpath2, _MAX_PATH) == NULL) {
			delete [] wpath;
		    delete [] wpath2;
			return NULL;
		}
		/* getcwd gives ut the path w/o ending slash */
		char slash[2] =  {UT_SLASH, '\0'};
		strncat(wpath2, slash, _MAX_PATH-(strlen(wpath2)+1));
		strncat(wpath2, wpath, _MAX_PATH)-(strlen(wpath2)+1);
		UT_StrCopy(wpath, wpath2, _MAX_PATH);
	}

	
	//printf("PATH: %s\n", wpath);
	int endLen = strlen(wpath);
	int startAt = 0;
	if(wpath[0] == '/') { 
		endLen--;
		startAt = 1;
	}

	if (endLen < maxlen) {
       	UT_StrCopy(buffer, (wpath+startAt), maxlen);
		delete [] wpath;
    	delete [] wpath2;
		return buffer;
	} else {
		delete [] wpath;
    	delete [] wpath2;
		return NULL;
	}
}

int main(int argc, char** args) {
	if(argc < 2) {
		exit(1);
	}
	//	printf("Input: %s\n", args[1]);
	
   char absPath[_MAX_PATH], absPath2[_MAX_PATH];
	absPath[0] = '\0';
	absPath2[0] = '\0';
	_ffullpath(absPath2, args[1], _MAX_PATH); 
	_fullpath (absPath, args[1], _MAX_PATH);
	if (!(strcmp(absPath, absPath2) == 0)) {
		printf("THEY DIFFER:\n");
		printf("PRIO: '%s'\n", absPath);
		printf("FREE: '%s'\n\n", absPath2);   		
	} else {
		printf("BOTH: '%s'\n\n", absPath);
	}
}


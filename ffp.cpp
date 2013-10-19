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

	strncpy(wpath, pathname, _MAX_PATH);	
	strncpy(wpath2, pathname, _MAX_PATH);

	/* Make relativer paths of ./foo-paths */
	if(pathname[0] == '.' && pathname[1] == UT_SLASH) {
		strncpy(wpath, pathname+2, _MAX_PATH);
		strncpy(wpath, pathname+2, _MAX_PATH);
	}

	char * prevPos = NULL;
	
	char same[4] = { UT_SLASH, '.', UT_SLASH, '\0' }; // e.g. "/./"
    /* replace /./ with nothing */
    while ((prevPos = strstr(wpath, same)) != NULL) {
        prevPos[1] = '\0';
        UT_StrCopy(wpath2, wpath, _MAX_PATH);
        strncat(wpath2, (prevPos+strlen(same)), _MAX_PATH);
        UT_StrCopy(wpath, wpath2, _MAX_PATH);
    }
	char prev[5] = { UT_SLASH, '.', '.', UT_SLASH, '\0' }; // e.g "/../"

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
	
	/* prepend cwd on relative paths */
	if(wpath[0] != UT_SLASH) {
		if(getcwd(wpath2, _MAX_PATH) == NULL) {
			delete [] wpath;
		    delete [] wpath2;
			return NULL;
		}
		/* getcwd gives ut the path w/o ending slash */
		strncat(wpath2, UT_STR_SLASH, _MAX_PATH-(strlen(wpath2)+1));
		strncat(wpath2, wpath, _MAX_PATH)-(strlen(wpath2)+1);
		UT_StrCopy(wpath, wpath2, _MAX_PATH);
	}

	int endLen = strlen(wpath);
	int startAt = 0;
	if(wpath[0] == '/') { 
		endLen--;
		startAt = 1;
	}
	delete [] wpath2;
	if (endLen < maxlen && buffer != NULL) { /* we have a buffer, and we have room in buffer */
       	UT_StrCopy(buffer, (wpath+startAt), maxlen);
		delete [] wpath;
		return buffer;
	} else if (buffer == NULL) { /* we have no buffer */
		char * tempbuf;
		if ((tempbuf = (char*)UT_MALLOC(endLen+1)) != NULL) {
			UT_StrCopy(tempbuf, (wpath+startAt), (endLen+1));
			delete [] wpath;
			return tempbuf;
		} else { /* failed to alloc memory*/
			delete [] wpath;
            return NULL;
		}
	} else { /* not room in buffer and we were supposed to use the buffer */
		delete [] wpath;
		return NULL;
	}
}

int main(int argc, char** args) {
	if(argc < 2) {
		exit(1);
	}
//   char absPath[_MAX_PATH], absPath2[_MAX_PATH];
//	absPath[0] = '\0';
//	absPath2[0] = '\0';
	char * absPath; char * absPath2;
	absPath2 = _ffullpath(NULL, args[1], _MAX_PATH); 
	absPath = _fullpath (NULL, args[1], _MAX_PATH);
	if (absPath == NULL || absPath2 == NULL) {
		if (absPath == NULL)
			printf("\x1b[%dmPRIO returned NULL\x1b[0m\n\n", 31);
		if (absPath2 == NULL)
			printf("\x1b[%dmFREE returned NULL\x1b[0m\n\n", 31);
		
		return 0;
	}
	if (!(strcmp(absPath, absPath2) == 0)) {
		printf("\x1b[%dmPRIO: '%s'\n", 31, absPath);
		printf("FREE: '%s'\x1b[0m\n\n", absPath2);   		
	} else {
		printf("\x1b[%dmBOTH: '%s'\x1b[0m\n\n", 32, absPath);
	}
	UT_FREE(absPath); UT_FREE(absPath2);
}


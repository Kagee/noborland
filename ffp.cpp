#include <cstdlib> // malloc etc
#include <cstring> // strcat etc 
#include <new> // nothrow
#include <fyut.h>

/*
CH _ffullpath                    
CD ==============================================================
CD Makes a complete path from a non-complete path. Handles "..",
CD "." and will prepend the path with current working directory
CD if it is relative (does not start with slash)
CD
CD The function returns buffer, pointer to memory area containing 
CD full path if buffer was NULL, or NULL if a error occured
CD (e.g. full path was longer than maxlen and a buffer 
CD was supplied.) The pointer should be free'd by calling code.
CD
CD Parameters:
CD Type        Name             I/O  Explanation
CD -------------------------------------------------------------
CD char       *buffer           i/o  Buffer to put full path into or NULL.
CD const char *pathname          i   Pathname to expand
CD size_t     maxlen             i   Size of buffer.
CD char *                        r   buffer, pointer to char[] or NULL
CD  ==============================================================
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


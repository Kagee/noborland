#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <fyut.h>

#include<libgen.h>

//#include "test.h"

//
//
//   THIS FILE HAS BEEN DEPRECATED BY sp2.cpp
//
//
//
//


void UT_splitpath(const char *pathP, char *driveP, char *dirP,
                  char *nameP, char *extP);

void UT_freesplitpath(const char *pathP, char *driveP, char *dirP,
                  char *nameP, char *extP) {
	char tmp[PATH_MAX];
	char bName[PATH_MAX];

	(*driveP) = '\0'; /* Set driveP (always empty on linux) */
	
	strcpy(tmp, pathP);
	/* Shotcut if path is to folder to mimic */
	if(*( tmp + strlen(tmp) - 1) == '/') {
		strcpy(dirP, tmp);
		(*nameP) = '\0';
		(*extP) = '\0';
		return;
	}
    	
	if(strcmp(dirname(tmp),".\0") != 0) {
		strcpy(dirP, dirname(tmp)); /* Set dirP, append / to mimic */
		strcat(dirP, "/");
	}
	else {
		(*dirP) = '\0';
	}

	strcpy(tmp, pathP);
	strcpy(bName, basename(tmp));

	strcpy(tmp,bName);
    char* lastDot = strrchr(tmp, '.');
	if(lastDot != NULL) {
	    strcpy(extP, tmp+(lastDot-tmp)); /* Set extP */

		strcpy(tmp,pathP);
	    lastDot = strrchr(tmp, '.');
		(*lastDot)   = '\0'; /* Replace last dot with \0 */
	    strcpy(nameP, basename(tmp)); /* Set nameP */
	} else { /* No dot in basename, no extention*/
		
		(*extP) = '\0';
		strcpy(tmp,pathP);
		strcpy(nameP,basename(tmp)); /* Set nameP */
	}

}

int main(int argc, char** args) {
	if(argc < 2) {
		exit(1);
	}
//	printf("Input: %s\n", args[1]);
	
   char fil[_MAX_PATH],sdir[_MAX_PATH];
   char drive1[_MAX_DRIVE],dir1[_MAX_DIR],fname1[_MAX_FNAME],ext1[_MAX_EXT];
   char drive2[_MAX_DRIVE],dir2[_MAX_DIR],fname2[_MAX_FNAME],ext2[_MAX_EXT];

   // Splitt filnavnet
   UT_splitpath(args[1],drive1,dir1,fname1,ext1);
   printf("'%s' '%s' '%s' '%s'\n", drive1,dir1,fname1,ext1);

   UT_freesplitpath(args[1],drive2,dir2,fname2,ext2); 
   printf("'%s' '%s' '%s' '%s'\n\n", drive2,dir2,fname2,ext2);
}


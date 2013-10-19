noborland
=========
Alternatives to some borland-functions in https://github.com/kartverket/fyba/tree/master/src/UT


UT\_splitpath\_free
=================
Mimics UT_splitpath . 
Testcases can be found in the supplied makefile.
The only know failure to mimic is when the resulting dir,
fname or ext extends beyond \_MAX\_DIR, \_MAX\_FNAME and  \_MAX\_EXT

_ffullpath
==========
Mimics _fullpath. You must modify the FYBA code 
to export _fullpath for the test to work.
Testcases can be found in the supplied makefile.
No know faliures. Better handling on some paths.

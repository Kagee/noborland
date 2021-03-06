CPPFLAGS = -I /usr/local/include/fyba/ -DLINUX -DUNIX -g
LDFLAGS = -lfyut
EXTRA_TEST = # windows, long

all: sp2 ffp test

test: ffp_test sp2_test

clean:
	$(RM) sp2 sp2.o *~
	$(RM) ffp ffp.o *~

sp2: sp2.o
	$(CXX) $^ $(LDFLAGS) -o $@

ffp: ffp.o
	$(CXX) $^ $(LDFLAGS) -o $@

ffp_test: TEST_PROGNAME=valgrind -q --suppressions=_fullpath.supp ./ffp
ffp_test: ffp
	$(TEST_PROGNAME) "relative/folder"
	$(TEST_PROGNAME) "relative/file.cpp"
	$(TEST_PROGNAME) "/to/file.cpp"
	$(TEST_PROGNAME) "/to/folder/"
#	This confuses the original fullpath	
#	$(TEST_PROGNAME) "/path/../to/file.cpp"
	$(TEST_PROGNAME) "/path/from/../to/file.cpp"
	$(TEST_PROGNAME) "/path/from/../to/../file.cpp"
	$(TEST_PROGNAME) "path/from/../../to/file.cpp"
	$(TEST_PROGNAME) "path/from/../to/file.cpp"
	$(TEST_PROGNAME) "./path/from/../to/file.cpp"
	$(TEST_PROGNAME) "./path/from/./to/../file.cpp"	
#	This confuses the original fullpath   
#	$(TEST_PROGNAME) "/path/../"
#	$(TEST_PROGNAME) "/path/../foo.txt"
	$(TEST_PROGNAME) "/path/./file.cpp"
	$(TEST_PROGNAME) "horrible/./../path/again/"

sp2_test: TEST_PROGNAME=valgrind -q --suppressions=_fullpath.supp ./sp2
sp2_test: sp2
	$(TEST_PROGNAME) "foo/bat/test.cpp" 	# sensible path to file
	$(TEST_PROGNAME) "foo/bat/test"		# sensible path to file or dir
	$(TEST_PROGNAME) "foo/bat/"			# sensible path to dir
	$(TEST_PROGNAME) "test.cpp"			# relative path to file
	$(TEST_PROGNAME) "foo/bat.mobile/test.cpp" # why not 
	$(TEST_PROGNAME) "foo/bat.mobile/test"	# suuuuure ...		
	$(TEST_PROGNAME) "foo/bat/test.foo.cpp"# not that sensible path to file
	$(TEST_PROGNAME) "foo/bat/test."		# fubar filename
	$(TEST_PROGNAME) "foo/bat/test.cpp."	# even more fubar filename
	$(TEST_PROGNAME) "/foo/bat/test.cpp"   # even more fubar fe
	$(TEST_PROGNAME) "/foo/bat/test"        # sensible path to file or dir
	$(TEST_PROGNAME) "/foo/bat/"            # sensible path to dir
	$(TEST_PROGNAME) "/test.cpp"            # relative path to file
	$(TEST_PROGNAME) "/foo/bat.mobile/test.cpp" # why not 
	$(TEST_PROGNAME) "/foo/bat.mobile/test" # suuuuure ...  
	$(TEST_PROGNAME) "/foo/bat/test.foo.cpp" # not that sensible path to file
	$(TEST_PROGNAME) "/foo/bat/test."       # fubar filename
	$(TEST_PROGNAME) "/foo/bat/test.cpp."   # even more fubar filename
	$(TEST_PROGNAME) "/"
	$(TEST_PROGNAME) "."
	$(TEST_PROGNAME) "./"
	$(TEST_PROGNAME) "/."
	$(TEST_PROGNAME) "/Ås.sos"
	$(TEST_PROGNAME) "Ås.sos"
	$(TEST_PROGNAME) "/foo/bar/Åså/Adresser.sos"
	$(TEST_PROGNAME) "/foo/bar/Ås.sos"
	$(TEST_PROGNAME) ".hidden"
	$(TEST_PROGNAME) "/foo/."
	$(TEST_PROGNAME) "/foo/.hidden"
	$(TEST_PROGNAME) "/foo/.hiddenfolder/file.txt"
	$(TEST_PROGNAME) "" 					# Let's just walk the goodam plank
	$(TEST_PROGNAME) "/foo/bar:brawl/file.sos"
	$(TEST_PROGNAME) "/foo/bar/file:master.sos"
	$(TEST_PROGNAME) "/foo/bar/file.sos:txt"
ifneq (,$(findstring windows, $(TEST_ON)))
#	# WINDOWZ!!!
	$(TEST_PROGNAME) "C:\\foo\\bar\\file.sos"
	$(TEST_PROGNAME) "C:\\file.sos"
	$(TEST_PROGNAME) "C:\\foo\\bar\\file.bak.sos"
	$(TEST_PROGNAME) "C:\\foo\\bar\\"
	$(TEST_PROGNAME) "C:\\foo\\bar"
	$(TEST_PROGNAME) "C:\\foo\\bar\\file."
	$(TEST_PROGNAME) "C:\\foo\\bar.brawl\\file.sos"
endif
ifneq (,$(findstring long, $(TEST_ON)))
	$(TEST_PROGNAME) "/foo/2jxtvGTFgmdn4S6UT1uR17e22ZTyo7m6yuen5asBhuisozzq6n6KJLiqgfW4JCCymROtoWyriXQ2VVc9mFMU7GnR4rumQOjs7iSoJr7LU5QVIRgnpwmbpqQV7uhO9ff98UimM839wfuZx7e3uB9w1Up43foUwPTUsXUmdXJSARs0mlC2Xyso77iGGRtuV3v0MRnjsetQywEQaWI2kMkk88Tg/source.cpp"
	$(TEST_PROGNAME) "/foo/.hiddenfolder/2jxtvGTFgmdn4S6UT1uR17e22ZTyo7m6yuen5asBhuisozzq6n6KJLiqgfW4JCCymROtoWyriXQ2VVc9mFMU7GnR4rumQOjs7iSoJr7LU5QVIRgnpwmbpqQV7uhO9ff98UimM839wfuZx7e3uB9w1Up43foUwPTUsXUmdXJSARs0mlC2Xyso77iGGRtuV3v0MRnjsetQywEQaWI2kMkk88Tg.txt"
	$(TEST_PROGNAME) "/foo/.hiddenfolder/file.2jxtvGTFgmdn4S6UT1uR17e22ZTyo7m6yuen5asBhuisozzq6n6KJLiqgfW4JCCymROtoWyriXQ2VVc9mFMU7GnR4rumQOjs7iSoJr7LU5QVIRgnpwmbpqQV7uhO9ff98UimM839wfuZx7e3uB9w1Up43foUwPTUsXUmdXJSARs0mlC2Xyso77iGGRtuV3v0MRnjsetQywEQaWI2kMkk88Tg"
	# These guys should overflow
	$(TEST_PROGNAME) "/foo/thisIsTheLongestFrellingDIRECTORYNAMEYouHaveEverSeen2jxtvGTFgmdn4S6UT1uR17e22Z/Tyo7m6yuen5asBhuisozzq6n6KJLiqgfW4JCCymROtoWyriXQ2VVc9mFMU7GnR4rumQOjs7iSoJr7LU5QVIRgnpwmbpqQV7uhO9ff98UimM839wfuZx7e3uB9w1Up43foUwPTUsXUmdXJSARs0mlC2Xyso77iGGRtuV3v0MRnjsetQywEQaWI2kMkk88Tg/source.cpp"
	$(TEST_PROGNAME) "/foo/.hiddenfolder/thisIsTheLongestFrellingFILENAMEYouHaveEverSeen2jxtvGTFgmdn4S6UT1uR17e22ZTyo7m6yuen5asBhuisozzq6n6KJLiqgfW4JCCymROtoWyriXQ2VVc9mFMU7GnR4rumQOjs7iSoJr7LU5QVIRgnpwmbpqQV7uhO9ff98UimM839wfuZx7e3uB9w1Up43foUwPTUsXUmdXJSARs0mlC2Xyso77iGGRtuV3v0MRnjsetQywEQaWI2kMkk88Tg.txt"
	$(TEST_PROGNAME) "/foo/.hiddenfolder/file.thisIsTheLongestFrellingEXTENSIONYouHaveEverSeen2jxtvGTFgmdn4S6UT1uR17e22ZTyo7m6yuen5asBhuisozzq6n6KJLiqgfW4JCCymROtoWyriXQ2VVc9mFMU7GnR4rumQOjs7iSoJr7LU5QVIRgnpwmbpqQV7uhO9ff98UimM839wfuZx7e3uB9w1Up43foUwPTUsXUmdXJSARs0mlC2Xyso77iGGRtuV3v0MRnjsetQywEQaWI2kMkk88Tg"
endif

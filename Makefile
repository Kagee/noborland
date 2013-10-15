PROGNAME=freesplitpath
OBJFILES=freesplitpath.o

CPPFLAGS = -I /usr/local/include/fyba/ -DLINUX -DUNIX -g
LDFLAGS = -lfyut

all: $(PROGNAME)

clean:
	$(RM) $(PROGNAME) $(OBJFILES) *~

$(PROGNAME): $(OBJFILES)
	$(CXX) $^ $(LDFLAGS) -o $@
	./$(PROGNAME) "foo/bat/test.foo.cpp"
	./$(PROGNAME) "foo/bat/test"
	./$(PROGNAME) "foo/bat/"
	./$(PROGNAME) "foo/bat/test.cpp"
	./$(PROGNAME) "/foo/bat/test.cpp"
	./$(PROGNAME) "/foo/bat/test"
	./$(PROGNAME) "/foo/bat/"
	./$(PROGNAME) "foo/bat/test.foo.cpp"

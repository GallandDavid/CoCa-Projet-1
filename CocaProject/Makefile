
SRC 	= $(wildcard src/*.c)
INTF 	= $(wildcard include/*.h)
CC		= gcc
CFLAGS	= -g -Iinclude
LDLIBS	= -lz3

solver.out: $(SRC) $(INTF)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDLIBS)

solver_solution.out: $(SRC) $(INTF)
	$(CC) $(CFLAGS) $(LDLIBS) $(SRC) -DSUBJECT -o $@

Z3Example: examples/Z3Example.c src/Z3Tools.c include/Z3Tools.h
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

.PHONY: clean
clean:
	rm -rf solver.out doc doc.html

.PHONY: doc
doc:
		doxygen doxygen.config
		rm -f doc.html
		ln -s doc/html/files.html doc.html
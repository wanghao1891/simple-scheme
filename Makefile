.PHONY: clean

scheme: scheme.c
	cc -Wall -o scheme scheme.c
#	cc -Wall -ansi -o scheme scheme.c
#	-Wcomment -Wreturn-type \
#	-Wimplicit-function-declaration \
#	-Wint-conversion \
#	-Wunused-variable

clean:
	rm scheme

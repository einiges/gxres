#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xresource.h>

#include "arg.h"



struct vrv {
	char *var;
	char *res;
	XrmValue val;
};

enum shell {
	NONE,
	BOURNE,
	CSH,
};

void print(struct vrv *vrv, enum shell shell);
void usage(int status);
void die(const char *errstr, ...);



char *argv0;

int
main(int argc, char *argv[])
{
	enum shell opt_shell = NONE;

	ARGBEGIN {
		case 'b':
			opt_shell = BOURNE;
			break;
		case 'c':
			opt_shell = CSH;
			break;
		case 'n':
			opt_shell = NONE;
			break;
		case 'v':
			fprintf(stderr, "%s\n", VERSION);
			exit(EXIT_SUCCESS);
		case 'h':
			usage(EXIT_SUCCESS);
		default:
			usage(EXIT_FAILURE);
	} ARGEND;

	if (argc < 1)
		usage(EXIT_FAILURE);


	Display *disp = XOpenDisplay(NULL);
	if (!disp)
		die("Can't open display\n");

	XrmInitialize();

	char *xms = XResourceManagerString(disp);
	if (!xms)
		die("No property exists\n");

	XrmDatabase xdb = XrmGetStringDatabase(xms);
	if (!xdb)
		die("No X resource database set\n");

	struct vrv vrv[argc];
	memset(&vrv, 0, sizeof(vrv));

	for (int i = 0; i < argc; ++i)
	{
		if ((vrv[i].res = strchr(argv[i], ':')) == NULL) {
			vrv[i].var = NULL;
			vrv[i].res = argv[i];
		} else {
			vrv[i].var = argv[i];
			vrv[i].res++[0] = '\0';
		}

		if (opt_shell != NONE && vrv[i].var == NULL)
			die("Missing name for resource '%s'\n", vrv[i].res);
	}

	char *type;
	for (int i = 0; i < argc; ++i)
	{
		if (!XrmGetResource(xdb, vrv[i].res, vrv[i].res, &type, &vrv[i].val))
			die("Unknown resource '%s'\n", vrv[i].res);
	}

	for (int i = 0; i < argc; ++i)
	{
		print(&vrv[i], opt_shell);
	}

	XCloseDisplay(disp);
	return EXIT_SUCCESS;
}


void
print(struct vrv *vrv, enum shell shell)
{
	switch (shell) {
		case BOURNE:
			printf("export %s='%s'\n", vrv->var, vrv->val.addr);
			break;
		case CSH:
			printf("setenv %s '%s'\n", vrv->var, vrv->val.addr);
			break;
		case NONE:
			printf("%s\n", vrv->val.addr);
			break;
	}
}


void
usage(int status)
{
	fprintf(stderr, "usage: %s [-bcnhv] [name:]resource...\n", argv0);
	exit(status);
}


void
die(const char *errstr, ...)
{
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}


/*
 * example.c - an example module for zsh
 *
 * This file is part of zsh, the Z shell.
 *
 * Copyright (c) 1996-1997 Zolt�n Hidv�gi
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and to distribute modified versions of this software for any
 * purpose, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * In no event shall Zolt�n Hidv�gi or the Zsh Development Group be liable
 * to any party for direct, indirect, special, incidental, or consequential
 * damages arising out of the use of this software and its documentation,
 * even if Zolt�n Hidv�gi and the Zsh Development Group have been advised of
 * the possibility of such damage.
 *
 * Zolt�n Hidv�gi and the Zsh Development Group specifically disclaim any
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose.  The software
 * provided hereunder is on an "as is" basis, and Zolt�n Hidv�gi and the
 * Zsh Development Group have no obligation to provide maintenance,
 * support, updates, enhancements, or modifications.
 *
 */

#include "example.mdh"
#include "example.pro"

/**/
static int
bin_example(char *nam, char **args, char *ops, int func)
{
    unsigned char c;

    printf("Options: ");
    for (c = 32; ++c < 128;)
	if (ops[c])
	    putchar(c);
    printf("\nArguments:");
    for (; *args; args++) {
	putchar(' ');
	fputs(*args, stdout);
    }
    printf("\nName: %s\n", nam);
    return 0;
}

/**/
static int
cond_p_len(char **a, int id)
{
    char *s1 = cond_str(a, 0);

    if (a[1]) {
	long v = cond_val(a, 1);

	return strlen(s1) == v;
    } else {
	return !s1[0];
    }
}

/**/
static int
cond_i_ex(char **a, int id)
{
    char *s1 = cond_str(a, 0), *s2 = cond_str(a, 1);

    return !strcmp("example", dyncat(s1, s2));
}

/**/
static int
ex_wrapper(List list, FuncWrap w, char *name)
{
    if (strncmp(name, "example", 7))
	return 1;
    else {
	int ogd = opts[GLOBDOTS];

	opts[GLOBDOTS] = 1;
	runshfunc(list, w, name);
	opts[GLOBDOTS] = ogd;

	return 0;
    }
}

/*
 * boot_example is executed when the module is loaded.
 */

static struct builtin bintab[] = {
    BUILTIN("example", 0, bin_example, 0, -1, 0, "flags", NULL),
};

static struct conddef cotab[] = {
    CONDDEF("len", 0, cond_p_len, 1, 2, 0),
    CONDDEF("ex", CONDF_INFIX, cond_i_ex, 0, 0, 0),
};

static struct funcwrap wrapper[] = {
    WRAPDEF(ex_wrapper),
};

/**/
int
setup_example(Module m)
{
    printf("The example module has now been set up.\n");
    fflush(stdout);
    return 0;
}

/**/
int
boot_example(Module m)
{
    return !(addbuiltins(m->nam, bintab, sizeof(bintab)/sizeof(*bintab)) |
	     addconddefs(m->nam, cotab, sizeof(cotab)/sizeof(*cotab)) |
	     !addwrapper(m, wrapper));
}

#ifdef MODULE

/**/
int
cleanup_example(Module m)
{
    deletebuiltins(m->nam, bintab, sizeof(bintab)/sizeof(*bintab));
    deleteconddefs(m->nam, cotab, sizeof(cotab)/sizeof(*cotab));
    deletewrapper(m, wrapper);
    return 0;
}

/**/
int
finish_example(Module m)
{
    printf("Thank you for using the example module.  Have a nice day.\n");
    fflush(stdout);
    return 0;
}

#endif

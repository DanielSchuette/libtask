/*
 * Copyright 2005-2007 Russ Cox, Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <task.h>
#include <unistd.h>

int quiet;
int goal;
int buffer;

void
primetask(void *arg)
{
	Channel *c, *nc;
	int p, i;
	c = arg;
	p = chanrecvul(c);
	if (p > goal) {
		taskexitall(EXIT_SUCCESS);
	}
	if (!quiet) {
		printf("%d\n", p);
	}
	nc = chancreate(sizeof(unsigned long), buffer);
	taskcreate(primetask, nc, 32768);
	for (;;) {
		i = chanrecvul(c);
		if (i % p) {
			chansendul(nc, i);
		}
	}
}

void
taskmain(int argc, char **argv)
{
	int i;
	Channel *c;
	if (argc > 1) {
		goal = atoi(argv[1]);
	} else {
		goal = 100;
	}
	printf("goal=%d\n", goal);
	c = chancreate(sizeof(unsigned long), buffer);
	taskcreate(primetask, c, 32768);
	for (i = 2;; i++) {
		chansendul(c, i);
	}
}

void *
emalloc(unsigned long n)
{
	return calloc(n ,1);
}

long
lrand(void)
{
	return rand();
}

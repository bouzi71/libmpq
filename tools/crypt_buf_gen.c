/*
 *  crypt_buf_gen.c -- tool to re-create the static decryption buffer.
 *
 *  Copyright (c) 2003-2008 Maik Broemme <mbroemme@plusserver.de>
 *  Copyright (c)      2008 Georg Lukas <georg@op-co.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 *  Usage:
 *  $ make crypt_buf_gen
 *  $ ./crypt_buf_gen > crypt_buf.h
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t *buffer;

int32_t libmpq__buffer_init() {
	buffer = malloc(sizeof(uint32_t) * 0x500);

	if (!buffer)
		return -1;

	/* some common variables. */
	uint32_t seed   = 0x00100001;
	uint32_t index1 = 0;
	uint32_t index2 = 0;
	uint32_t i;

	/* initialize the decryption buffer. */
	for (index1 = 0; index1 < 0x100; index1++) {
		for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100) {

			/* some common variables. */
			uint32_t temp1, temp2;

			/* temporary copy. */
			seed  = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;

			/* temporary copy. */
			seed  = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);

			/* assign buffer. */
			buffer[index2] = (temp1 | temp2);
		}
	}

	/* if no error was found, return zero. */
	return 0;
}

int main() {
	if (libmpq__buffer_init() != 0)
		perror("libmpq__buffer_init()");
	int x;
	printf("/* DO NOT CHANGE! this file is auto-generated by crypt_buf_gen.c */\n");
	printf("static uint32_t crypt_buf[0x500] = {\n\t");
	for (x = 0; x < 0x500; x++) {
		printf("0x%08x", buffer[x]);
		if (x < 0x500 - 1) {
			if (x % 6 == 5)
				printf(",\n\t");
			else
				printf(", ");
		}
	}
	printf("\n};\n");
}

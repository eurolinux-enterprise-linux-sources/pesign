/*
 * Copyright 2011 Red Hat, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author(s): Peter Jones <pjones@redhat.com>
 */

#include <stdio.h>
#include <assert.h>

#include "libdpe.h"

static int global_error;

int
pe_errno (void)
{
	int result = global_error;
	global_error = PE_E_NOERROR;
	return result;
}

static const char msgstr[] =
{
#define PE_E_NOERROR_IDX 0
	"no error"
	"\0"
#define PE_E_UNKNOWN_ERROR_IDX \
	(PE_E_NOERROR_IDX + sizeof "no error")
	"unknown error"
	"\0"
#define PE_E_INVALID_HANDLE_IDX \
	(PE_E_UNKNOWN_ERROR_IDX + sizeof "unknown error")
	"invalid 'Pe' handle"
	"\0"
#define PE_E_NOMEM_IDX \
	(PE_E_INVALID_HANDLE_IDX + sizeof "invalid 'Pe' handle")
	"out of memory"
	"\0"
#define PE_E_INVALID_FILE_IDX \
	(PE_E_NOMEM_IDX + sizeof "out of memory")
	"invalid file descriptor"
	"\0"
#define PE_E_WRITE_ERROR_IDX \
	(PE_E_INVALID_FILE_IDX + sizeof "invalid file descriptor")
	"cannot write data to file"
	"\0"
#define PE_E_INVALID_INDEX_IDX \
	(PE_E_WRITE_ERROR_IDX + sizeof "cannot write data to file")
	"invalid section index"
	"\0"
#define PE_E_INVALID_OP_IDX \
	(PE_E_INVALID_INDEX_IDX + sizeof "invalid section index")
	"invalid operation"
	"\0"
#define PE_E_INVALID_CMD_IDX \
	(PE_E_INVALID_OP_IDX + sizeof "invalid operation")
	"invalid command"
	"\0"
#define PE_E_INVALID_OPERAND_IDX \
	(PE_E_INVALID_CMD_IDX + sizeof "invalid command")
	"invalid operand"
	"\0"
#define PE_E_WRONG_ORDER_PEHDR_IDX \
	(PE_E_INVALID_OPERAND_IDX + sizeof "invalid operand")
	"executable header not created first"
	"\0"
#define PE_E_FD_DISABLED_IDX \
	(PE_E_WRONG_ORDER_PEHDR_IDX + sizeof "executable header not created first")
	"file descriptor disabled"
	"\0"
#define PE_E_FD_MISMATCH_IDX \
	(PE_E_FD_DISABLED_IDX + sizeof "file descriptor disabled")
	"file descriptor mismatch"
	"\0"
#define PE_E_UPDATE_RO_IDX \
	(PE_E_FD_MISMATCH_IDX + sizeof "file descriptor mismatch")
	"update() for write on read-only file"
};

static const uint16_t msgidx[PE_E_NUM] =
{
	[PE_E_NOERROR] = PE_E_NOERROR_IDX,
	[PE_E_UNKNOWN_ERROR] = PE_E_UNKNOWN_ERROR_IDX,
	[PE_E_INVALID_HANDLE] = PE_E_INVALID_HANDLE_IDX,
	[PE_E_NOMEM] = PE_E_NOMEM_IDX,
	[PE_E_INVALID_FILE] = PE_E_INVALID_FILE_IDX,
	[PE_E_WRITE_ERROR] = PE_E_WRITE_ERROR_IDX,
	[PE_E_INVALID_INDEX] = PE_E_INVALID_INDEX_IDX,
	[PE_E_INVALID_OP] = PE_E_INVALID_OP_IDX,
	[PE_E_INVALID_CMD] = PE_E_INVALID_CMD_IDX,
	[PE_E_INVALID_OPERAND] = PE_E_INVALID_OPERAND_IDX,
	[PE_E_WRONG_ORDER_PEHDR] = PE_E_WRONG_ORDER_PEHDR_IDX,
	[PE_E_FD_DISABLED] = PE_E_FD_DISABLED_IDX,
	[PE_E_FD_MISMATCH] = PE_E_FD_MISMATCH_IDX,
	[PE_E_UPDATE_RO] = PE_E_UPDATE_RO_IDX,
};
#define nmsgidx ((int) (sizeof (msgidx) / sizeof (msgidx[0])))

void __libpe_seterrno(int value)
{
	global_error = value >= 0 && value <= nmsgidx
			? value : PE_E_UNKNOWN_ERROR;
}

const char *
pe_errmsg(int error)
{
	int last_error = global_error;

	if (error == 0) {
		assert(msgidx[last_error] < sizeof(msgstr));
		return last_error != 0 ? msgstr + msgidx[last_error] : NULL;
	} else if (error < -1 || error >= nmsgidx) {
		return msgstr + PE_E_UNKNOWN_ERROR_IDX;
	}

	assert (msgidx[error == -1 ? last_error : error] < sizeof (msgstr));
	return msgstr + msgidx[error == -1 ? last_error : error];
}

#include <stdlib.h>
#include <stdio.h>

#include "tmx.h"
#include "tsx.h"
#include "tmx_utils.h"

tmx_error_codes tmx_errno = E_NONE;

static char *errmsgs[] = {
	"No error",
	"Memory alloc failed",
	"Missing privileges to access the file",
	"File not found",
	"Unsupproted/Unknown map file format"
};

char custom_msg[256];

const char* tmx_strerr(void) {
	char *msg;
	switch(tmx_errno) {
		case E_NONE:   msg = errmsgs[0]; break;
		case E_ALLOC:  msg = errmsgs[1]; break;
		case E_ACCESS: msg = errmsgs[2]; break;
		case E_NOENT:  msg = errmsgs[3]; break;
		case E_FORMAT: msg = errmsgs[4]; break;
		default: msg = custom_msg;
	}
	return msg;
}

void tmx_perror(const char *pos) {
	const char *msg = tmx_strerr();
	fprintf(stderr, "%s: %s\n", pos, msg);
}

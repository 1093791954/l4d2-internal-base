#include "console.h"

#include <cstdio>
#include <windows.h>

void console::initialise()
{
	/* alloc & check console */
	if (!AllocConsole())
		return;

	/* Redirect standard input stream (stdin) to the console input */
	freopen_s(reinterpret_cast<_iobuf**>(stdin), "CONIN$", "r", stdin);

	/* Redirect standard input stream (stdout) to the console input */
	freopen_s(reinterpret_cast<_iobuf**>(stdout), "CONOUT$", "w", stdout);

	/* Redirect standard input stream (stderr) to the console input */
	freopen_s(reinterpret_cast<_iobuf**>(stderr), "CONOUT$", "w", stderr);
}

void console::shutdown()
{
	/* free inputs/outputs and etc .. */
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	/* free allocated console */
	FreeConsole();
}
// CrashDumpSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SelfDump.h"

// source
// http://greenfishblog.tistory.com/19

void crash()
{
	int *p = nullptr;

	*p = 10;
}

int main()
{

	CSelfDump dump;

	dump.RegisterExceptionFilter();

	crash();

    return 0;
}


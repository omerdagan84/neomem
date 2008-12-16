#ifndef _ATA_LOGFILE_
#define _ATA_LOGFILE_



//#ifdef _DEBUG_LOG
void _trace(char *fmt, ...)
{
	static FILE* plogfile = 0;

	if (plogfile == 0)
//		plogfile = fopen("logfile.txt", bAppend ? "a" : "w");
		plogfile = fopen("logfile.txt", "w");

	char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf(out, fmt, body);
	va_end(body);
	fputs(out, plogfile);

}


#undef TRACE
#define TRACE _trace


//#endif


#endif //_ATA_LOGFILE_


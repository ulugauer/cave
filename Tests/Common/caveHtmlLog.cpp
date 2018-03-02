/*
Copyright (c) <2017> <Udo Lugauer>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

/// @file caveHtmlLog.cpp
///       Functions for writing logs in HTML style

#include "caveHtmlLog.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>

static FILE *logFile = NULL; ///< global log file

/**
* @brief Print timestamp to log file
*
* @param[in]  buffer		must be able to hold 26 bytes
*
* @return none
*/
void CaveHtmlLog::Timestamp(char *buffer)
{
	struct tm *newTime;
	time_t systemTime;

	// print the time stamp
	if (time(&systemTime) != -1)
	{
		// Convert to local time.
		newTime = localtime(&systemTime);
		if (newTime)
		{
			strcpy(buffer, asctime(newTime));

			// asctime adds a newline, remove this
			buffer[strlen(buffer) - 1] = 0;
		}
	}
}


/**
* @brief Initialize HTML based logging
*
* @param[in]  noLogFile		if true write to stdout
* @param[in]  header		header appears in the HTML title bar
*
* @return true if successful
*/
bool CaveHtmlLog::LogInit(bool noLogFile, const char* filePrefix, const char* header)
{
	if (noLogFile)
	{
		logFile = stdout;
	}
	else
	{
		size_t prefixLength = strlen(filePrefix);
		char* fileName = new char[prefixLength + 26 + 5];
		//char fileName[prefixLength + 26 + 5]; // prefix + timestamp + posix

		strcpy(fileName, filePrefix);
		Timestamp(&fileName[strlen(fileName)]);
		strcat(fileName, ".html");
		// replace : with -
		for (unsigned int i = 0; i < strlen(fileName); i++)
		{
			if (fileName[i] == ':')
				fileName[i] = '-';
		}

		logFile = fopen(fileName, "w");
		if (!logFile)
			return false;

		fprintf(logFile,
			"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n"
			"    \"http://www.w3.org/TR/html4/loose.dtd\">\n"
			"<html>\n"
			"<head>\n");
		fprintf(logFile, "    <title>%s</title>\n", header);
		fprintf(logFile, "</head>\n"
			"<body>\n");
		fprintf(logFile, "<h1>%s</h1>\n", header);

		delete fileName;
	}

	return true;
}

/**
* @brief Cleanup HTML based logging and write end tag
*
*
* @return none
*/
void CaveHtmlLog::LogCleanup()
{
	fprintf(logFile,
		"</body>\n"
		"</html>\n"
	);

	if (logFile != stdout)
		fclose(logFile);
}

/**
* @brief Write a messeage to the log file
*
* @param[in]  message		message buffer to log
*
* @return none
*/
void CaveHtmlLog::LogMessage(const char *message, ...)
{
	va_list marker;

	va_start(marker, message);
	vfprintf(logFile, message, marker);
	va_end(marker);

	fprintf(logFile, "<br>");
}

/**
* @brief Write an image link to the log file
*
* @param[in]  imageFile		Path to image file
* @param[in]  width			Image width
* @param[in]  height		Image height
*
* @return none
*/
void CaveHtmlLog::LogThumbnailImage(const char *imageFile, int width, int height)
{
	fprintf(logFile, "<a href = %s  target = \"_blank\">\n", imageFile);
	fprintf(logFile,
		"<img src = %s height = %d width = %d> </a>\n", imageFile, width, height);
}

///< start a HTML paragraph
void CaveHtmlLog::LogBeginParagraph()
{
	fprintf(logFile, "<p>\n");
}

///< end a HTML paragraph
void CaveHtmlLog::LogEndParagraph()
{
	fprintf(logFile, "</p>\n");
}

///< write start table tag
void CaveHtmlLog::LogBeginTable()
{
	fprintf(logFile,
		"<table border=\"1\" cellpadding=\"10\">\n"
	);
}

///< write end table tag
void CaveHtmlLog::LogEndTable()
{
	fprintf(logFile, "</table>\n");
}

///< write row start tags
void CaveHtmlLog::LogBeginRow()
{
	fprintf(logFile, "    <tr><td>");
}

///< write row end tags
void CaveHtmlLog::LogEndRow()
{
	fprintf(logFile, "</td></tr>\n");
}

///< write column start tags
void CaveHtmlLog::LogNextColumn()
{
	fprintf(logFile, "</td><td>");
}

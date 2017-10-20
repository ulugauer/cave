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
#pragma once

/// @file engineLog.h
///       handle engine message logging

#include "engineDefines.h"

/** \addtogroup engine 
*  @{
*		
*/

namespace cave
{

/**
* Engine message logging handling
*/
class CAVE_INTERFACE EngineLog
{
public:
	/// The warning levels
    typedef enum
    {
        WARNING_LEVEL0 = 1,     ///< Level 0 - informational warnings
        WARNING_LEVEL1,         ///< Level 1 - severe warnings
    } logWarningLevel;
    /// The message levels
    typedef enum
    {
        MESSAGE_LEVEL0 = 1,     ///< Level 0 - informational message
        MESSAGE_LEVEL1,         ///< Level 1 - important messages
    } logMessageLevel;

	/**
	* @brief Constructor
	*
	*/
	EngineLog();

	/** destructor */
	~EngineLog();

	/**
	* @brief Print debug message
	*
	* @param level Message level
	* @param string Pointer to message to print
	* @param ... variadic parameters
	*/
	void Message(logMessageLevel level, const char *string, ...);

	/**
	* @brief Print warning message
	*
	* @param level Warnings level
	* @param string Pointer to message to print
	* @param ... variadic parameters
	*/
    void Warning(logWarningLevel level, const char *string, ...);
    
    /**
	* @brief Print error message
	*
	* @param string Pointer to message to print
	* @param ... variadic parameters
	*/
    void Error(const char *string, ...);


private:
	
	logWarningLevel _minWarningLevel;    ///< all warnings below this level are ignored
    logMessageLevel _minMessageLevel;    ///< all messages below this level are ignored
    void *_logFile;                         ///< log file handle

    /**
	* @brief Open log file
	*
	* @return true if successfuly opened
	*/
    bool Open();

	/**
	* @brief Print a timestamp to the log file
	*
	*/
    void Timestamp();

    /**
	* @brief Print message to log log file
	*
	* @param string Pointer to message to print
	*/
    void PrintToLog(const char *string);

};

}

/** @}*/

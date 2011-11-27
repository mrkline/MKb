#pragma once

#include <string>

/*!
\brief A base Exception class

Many exception types are lovingly borrowed from the .NET framework.
*/
class Exception
{
public:
	/*!
	\brief Initializes an exception with a message and other optional data
	\param exceptionMessage The message to contain within the exception.
	\param callingFunctionName The name of the function the exception comes from.
	*/
	Exception(const std::string& exceptionMessage,
			const std::string& callingFunctionName = nullptr)
		: message(exceptionMessage), callingFunction(callingFunctionName)
	{ }

	//! A void pointer to hold any extra exception data a user might want
	void* extraData;
	//! The exception message
	const std::string message;
	//! The name of the calling function
	const std::string callingFunction;
};

/*!
\brief Thrown if a method or function is not implemented.

Mostly for use as a placeholder in stubs during development.
*/
class NotImplementedException : public Exception
{
public:
	explicit NotImplementedException(const std::string& callingFunctionName)
		: Exception("This function or method has not been implemented yet", callingFunctionName)
	{ }

	NotImplementedException(const std::string& exceptionMessage,
		const std::string& callingFunctionName)
		: Exception(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown due to math-related errors
class MathException : public Exception
{
public:
	MathException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: Exception(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown if an argument to a method was invalid
class ArgumentException : public Exception
{
public:
	ArgumentException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: Exception(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown if a null argument was passed to a method that doesn't accept it.
class ArgumentNullException : public ArgumentException
{
public:
	ArgumentNullException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: ArgumentException(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown if an argument value is out of range
class ArgumentOutOfRangeException : public ArgumentException
{
public:
		ArgumentOutOfRangeException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: ArgumentException(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown if an array index is out of bounds
class IndexOutOfRangeException : public Exception
{
public:
	IndexOutOfRangeException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: Exception(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown if a method was called at an improper time
class InvalidOperationException : public Exception
{
public:
	InvalidOperationException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: Exception(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown when an IO error, such as a networking or file error, occurs
class IOException : public Exception
{
public:
	IOException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: Exception(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown when a file error occurs
class FileException : public IOException
{
public:
	FileException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: IOException(exceptionMessage, callingFunctionName)
	{ }
};

//! Thrown when a networking error occurs
class NetworkException : public IOException
{
public:
	NetworkException(const std::string& exceptionMessage,
		const std::string& callingFunctionName = nullptr)
		: IOException(exceptionMessage, callingFunctionName)
	{ }
};

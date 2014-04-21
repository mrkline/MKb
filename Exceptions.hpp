#ifndef __MK_EXCEPTIONS_HPP__
#define __MK_EXCEPTIONS_HPP__

#include <exception>
#include <string>

/**
 * \brief Provides a hierarchy of custom exception types
 *
 * Unfortunately, the standard set of exceptions is underwhelming.
 * It does not provide enough exceptions to indicate many common but specific problems,
 * nor does it provide file name and line number for debugging.
 * This hierarchy attempts to alleviate those two concerns
 * Combined with the THROW and ENFORCE macros,
 * this provides a much nicer debugging experience.
 */
namespace Exceptions
{

	/**
	 * \brief A base Exception class
	 *
	 * Most exception types are lovingly inspired by the .NET framework.
	 */
	class Exception : public std::exception {

	public:
		/**
		 * \brief Initializes an exception with a message and other optional data
		 * \param exceptionMessage The message to contain within the exception.
		 * \param file The file from which the exception was thrown
		 * \param line The line from which the exception was thrown
		 */
		Exception(const std::string& exceptionMessage,
		          const char* file, int line) :
			message(exceptionMessage),
			file(file),
			line(line),
			type("generic")
		{ }

		Exception(const Exception&) = default;

		Exception& operator=(const Exception&) = delete;

		virtual ~Exception() noexcept { }

		const char* what() const noexcept override { return message.c_str(); }

		const std::string message; ///< Exception message
		const char* file; ///< Name of file from which the exception was thrown
		const int line; ///< Line number from which the exception was thrown
		const char* type; ///< Type of exception.

	protected:

		Exception(const std::string& exceptionMessage,
		          const char* file, int line,
		          const char* exceptionType) :
			message(exceptionMessage),
			file(file),
			line(line),
			type(exceptionType)
		{ }
	};

	/**
	 * \brief Thrown if a method or function is not implemented.
	 *
	 * Mostly for use as a placeholder in stubs during development.
	 */
	class NotImplementedException : public Exception {

	public:
		NotImplementedException(const char* file, int line)
				: Exception("This function or method has not been"
				            " implemented yet",
				            file, line,
				            "not implemented")
		{ }

		NotImplementedException(const std::string& exceptionMessage,
		                        const char* file, int line)
				: Exception(exceptionMessage, file, line,
				            "not implemented")
		{ }

		NotImplementedException(const NotImplementedException&) = default;

		NotImplementedException& operator=(const NotImplementedException&) = delete;

		virtual ~NotImplementedException() noexcept { }
	};

	/// Thrown due to math-related errors
	class MathException : public Exception {

	public:
		MathException(const std::string& exceptionMessage,
		              const char* file, int line)
				: Exception(exceptionMessage, file, line, "math")
		{ }

		MathException(const MathException&) = default;

		MathException& operator=(MathException&) = delete;

		virtual ~MathException() noexcept { }
	};

	/// Thrown if an argument to a method was invalid
	class ArgumentException : public Exception {

	public:

		ArgumentException(const std::string& exceptionMessage,
		                  const char* file, int line)
				: Exception(exceptionMessage, file, line, "argument")
		{ }

		ArgumentException(const ArgumentException&) = default;

		ArgumentException& operator=(const ArgumentException&) = delete;

		virtual ~ArgumentException() noexcept { }

	protected:
		ArgumentException(const std::string& exceptionMessage,
		                  const char* file, int line,
		                  const char* exceptionType)
				: Exception(exceptionMessage, file, line, exceptionType)
		{ }
	};

	/// Thrown if a null argument was passed to a method that doesn't accept it.
	class ArgumentNullException : public ArgumentException {

	public:
		ArgumentNullException(const std::string& exceptionMessage,
		                      const char* file, int line)
				: ArgumentException(exceptionMessage, file, line,
				                    "argument null")
		{ }

		ArgumentNullException(const ArgumentNullException&) = default;

		ArgumentNullException& operator=(const ArgumentNullException&) = delete;

		virtual ~ArgumentNullException() noexcept { }
	};

	/// Thrown if an argument value is out of range
	class ArgumentOutOfRangeException : public ArgumentException {

	public:
		ArgumentOutOfRangeException(const std::string& exceptionMessage,
		                            const char* file, int line)
				: ArgumentException(exceptionMessage, file, line,
				                    "argument out of range")
		{ }

		ArgumentOutOfRangeException(const ArgumentOutOfRangeException&) = default;

		ArgumentOutOfRangeException& operator=(const ArgumentOutOfRangeException&) = delete;

		virtual ~ArgumentOutOfRangeException() noexcept { }
	};

	/// Thrown if an array index is out of bounds
	class IndexOutOfRangeException : public Exception {

	public:
		IndexOutOfRangeException(const std::string& exceptionMessage,
		                         const char* file, int line)
				: Exception(exceptionMessage, file, line, "index out of range")
		{ }

		IndexOutOfRangeException(const IndexOutOfRangeException&) = default;

		IndexOutOfRangeException& operator=(const IndexOutOfRangeException&) = delete;

		virtual ~IndexOutOfRangeException() noexcept { }
	};

	/// Thrown if an argument to a method was invalid
	class ConcurrencyException : public Exception {

	public:
		ConcurrencyException(const std::string& exceptionMessage,
		                     const char* file, int line)
				: Exception(exceptionMessage, file, line, "concurrency")
		{ }

		ConcurrencyException(const ConcurrencyException&) = default;

		ConcurrencyException& operator=(const ConcurrencyException&) = delete;

		virtual ~ConcurrencyException() noexcept { }
	};

	/// Thrown if a method was called at an improper time
	class InvalidOperationException : public Exception {

	public:
		InvalidOperationException(const std::string& exceptionMessage,
		                          const char* file, int line, const char* type = "invalid operation")
				: Exception(exceptionMessage, file, line, type)
		{ }

		InvalidOperationException(const InvalidOperationException&) = default;

		InvalidOperationException& operator=(const InvalidOperationException&) = delete;

		virtual ~InvalidOperationException() noexcept { }
	};

	/// Thrown if some user input is invalid
	class InvalidInputException : public Exception {

	public:
		InvalidInputException(const std::string& exceptionMessage,
		                      const char* file, int line)
				: Exception(exceptionMessage, file, line, "invalid operation")
		{ }

		InvalidInputException(const InvalidInputException&) = default;

		InvalidInputException& operator=(const InvalidInputException&) = delete;

		virtual ~InvalidInputException() noexcept { }
	};

	/// Thrown when an IO error, such as a networking or file error, occurs
	class IOException : public Exception {

	public:
		IOException(const std::string& exceptionMessage,
		            const char* file, int line)
				: Exception(exceptionMessage, file, line, "I/O")
		{ }

		IOException(const IOException&) = default;

		IOException& operator=(const IOException&) = delete;

		virtual ~IOException() noexcept { }

	protected:
		IOException(const std::string& exceptionMessage,
		            const char* file, int line,
		            const char* exceptionType)
				: Exception(exceptionMessage, file, line, exceptionType)
		{ }
	};

	/// Thrown when a file error occurs
	class FileException : public IOException {

	public:
		FileException(const std::string& exceptionMessage,
		              const char* file, int line)
				: IOException(exceptionMessage, file, line, "file")
		{ }

		FileException(const FileException&) = default;

		FileException& operator=(const FileException&) = delete;

		virtual ~FileException() noexcept { }
	};

	/// Thrown when a networking error occurs
	class NetworkException : public IOException {

	public:
		NetworkException(const std::string& exceptionMessage,
		                 const char* file, int line)
				: IOException(exceptionMessage, file, line, "network")
		{ }

		NetworkException(const NetworkException&) = default;

		NetworkException& operator=(const NetworkException&) = delete;

		virtual ~NetworkException() noexcept { }
	};

	/**
	  * \brief A convenience function that checks a condition and throws a given exception if it is false
	  * \tparam T The exception type to throw if the condition is not met
	  * \param cond true if the condition was met, otherwise false
	  * \param message The message to pass to the exception's constructor
	  * \param file The name of the file where the exception is being thrown
	  * \param line The line number from which the exception is being thrown.
	  */
	template <typename T = Exception>
	inline void enforce(bool cond, const std::string& message, const char* file, int line)
	{
		if (!cond)
			throw T(message, file, line);
	}

} // end namespace Exceptions

#ifdef ENFORCE
#error ENFORCE is already defined. See Exceptions.hpp
#endif
/**
 * \brief A convenience wrapper around Exceptions::enforce that provides the file and line number
 * \param ex The exception type to throw if the condition is not met
 * \param cond An expression that evaluates to a boolean condition
 * \param msg The message to pass the exception
 */
#define ENFORCE(ex, cond, msg) Exceptions::enforce<ex>(cond, msg, __FILE__, __LINE__)

#ifdef THROW
#error THROW is already defined. See Exceptions.hpp
#endif
/// A convenience wrapper that provides the file and line number to the exception's constructor
#define THROW(ex, msg) throw ex(msg, __FILE__, __LINE__)

#endif

#ifndef EXCEPTION_HPP_
# define EXCEPTION_HPP_

namespace RR
{
	class Exception : public std::exception
	{
	public:
		Exception(const char *);
		~Exception();

		virtual const char *what() const throw();
	};
}

#endif //!EXCEPTION_HPP_
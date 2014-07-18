#include <windows.h>
#include <exception>
#include "Exception.hpp"
#include "DualBuffer.hh"

using namespace RR;

template <typename T>
DualBuffer<T>::DualBuffer()
try
: _obj1(), _obj2(), _reading(&_obj1), _writing(&_obj2), _rd_ref(0), _wr_ref(0), _mutex(NULL)
{
	if ((_mutex = CreateMutex(NULL, false, NULL)) == NULL)
		throw (Exception("Can't create mutex"));
}
catch (const Exception &)
{
	throw;
}

template <typename T>
DualBuffer<T>::DualBuffer(const DualBuffer& copy)
: _obj1(copy._obj1), _obj2(copy._obj2), 
	_reading(copy._reading), _writing(copy.writing), 
	_rd_ref(copy._rd_ref), _wr_ref(copy._wr_ref), 
	_mutex(mutex)
{

}

template <typename T>
DualBuffer<T>::~DualBuffer()
{
	if (_rd_ref || _wr_ref)
		throw ("Reference(s) on DualBuffer still alive\n");
	::CloseHandle(_mutex);
	_mutex = NULL;
}

template <typename T>
DualBuffer<T>&
DualBuffer<T>::operator=(const DualBuffer& copy)
{
	if (this == &copy)
		return (*this);
	_obj1 = copy._obj1;
	_obj2 = copy._obj2;
	_reading = copy._reading;
	_writing = copy._writing;
	_rd_ref = copy._rd_ref;
	_wr_ref = copy._wr_ref;
	_mutex = copy._mutex;
	return (*this);
}

template <typename T>
T*
DualBuffer<T>::getReadAccess()
{
	::WaitForSingleObject(_mutex, INFINITE);
	++_rd_ref;
	::ReleaseMutex(_mutex);
	return (_reading);
}

template <typename T>
T*
DualBuffer<T>::getWriteAccess()
{
	::WaitForSingleObject(_mutex, INFINITE);
	++_wr_ref;
	::ReleaseMutex(_mutex);
	return (_writing);
}

template <typename T>
void
DualBuffer<T>::releaseReadAccess(T **ref)
{
	if (*ref != _reading)
	{
		*ref = 0;
		return ;
	}
	*ref = 0;
	::WaitForSingleObject(_mutex, INFINITE);
	if (--_rd_ref < 0)
		_rd_ref = 0;
	::ReleaseMutex(_mutex);
}

template <typename T>
void
DualBuffer<T>::releaseWriteAccess(T** ref)
{
	if (*ref != _writing)
	{
		*ref = 0;
		return ;
	}
	*ref = 0;
	::WaitForSingleObject(_mutex, INFINITE);
	if (--_wr_ref < 0)
		_wr_ref = 0;
	::ReleaseMutex(_mutex);
}

template <typename T>
bool
DualBuffer<T>::swap()
{
	T* temp;

	::WaitForSingleObject(_mutex, INFINITE);
	if ((_rd_ref == 0 && _wr_ref == 0))
	{
		temp = _reading;
		_reading = _writing;
		_writing = temp;
	}
	::ReleaseMutex(_mutex);	
	return (true);
}

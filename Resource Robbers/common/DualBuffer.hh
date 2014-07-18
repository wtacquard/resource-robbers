#ifndef DUALBUFFER_HH_
# define DUALBUFFER_HH_

namespace RR
{
	template <typename T>
	class DualBuffer
	{
	private:
		DualBuffer(const DualBuffer&);
		DualBuffer<T>&	operator=(const DualBuffer<T>&);

	private:
		T	_obj1;
		T	_obj2;
		T*	_reading;
		T*	_writing;
		unsigned int _rd_ref;
		unsigned int _wr_ref;
		HANDLE	_mutex;

	public:
		DualBuffer();
		~DualBuffer();

		T*		getReadAccess();
		T*		getWriteAccess();
		void	releaseReadAccess(T **ref);
		void	releaseWriteAccess(T **ref);
		bool	swap();
	};
}

#include "DualBuffer.cc"

#endif //!DUALBUFFER_HH_

//	Useage example
//		tempWBuff = wObjects.getWriteAccess();
//		tempWBuff->push_back(&vehicles[i]);
//		wObjects.releaseWriteAccess(&tempWBuff);
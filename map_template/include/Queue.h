#ifndef MY_QUEUE
#define MY_QUEUE

#include "Interfaces.h"

///@brief queue with bezier paths
class Queue : public IQueue
{
public:
	Queue(const IDB *m_DB)
	{
		;
	}

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const IMap::container &get_added_paths() const
	{
		return m_added_paths;
	}
	
	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const IMap::container &get_removed_paths() const
	{
		return m_removed_paths;
	}

	///@brief accept path
	///@param path pointer to bezier path
	virtual void accept(const IMap::iterator &path)
	{
		//всего для элемента очереди должно быть три значения
		//подтвержден, отклонен, еще не успели рассмотреть
		//обозначаем элемент подтвержденным
		;
	}

	///@brief reject path
	///@param path pointer to bezier path
	virtual void reject(const IMap::iterator &path)
	{
		//определяем очередь. можно при помощи запроса, можно при помощи добавления методов принять отклонить для каждой очереди
		//удаляем из очереди добавления, т.к. клиент отказал, а нужно согласие всех
		//переносим из очереди удаления обратно на карту, т.к. клиент отказал, а нужно согласие всех
		;
	}

protected:
	
	///@brief request all path to be accepted/rejected
	void request()
	{
		;
	}
	IMap::container m_added_paths;
	IMap::container m_removed_paths;
};


#endif
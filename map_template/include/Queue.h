#ifndef MY_QUEUE
#define MY_QUEUE

#if TESTING
#define PRIVATE_TESTABLE public
#define PROTECTED_TESTABLE public
#else
#define PRIVATE_TESTABLE private
#define PROTECTED_TESTABLE protected
#endif

#include "Interfaces.h"
namespace newmeteo {
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
			//����� ��� �������� ������� ������ ���� ��� ��������
			//�����������, ��������, ��� �� ������ �����������
			//���������� ������� ��������������
			;
		}
		virtual bool accept(int index) {
			return true;
		}
		///@brief reject path
		///@param path pointer to bezier path
		virtual void reject(const IMap::iterator &path)
		{
			//���������� �������. ����� ��� ������ �������, ����� ��� ������ ���������� ������� ������� ��������� ��� ������ �������
			//������� �� ������� ����������, �.�. ������ �������, � ����� �������� ����
			//��������� �� ������� �������� ������� �� �����, �.�. ������ �������, � ����� �������� ����
			;
		}

		virtual bool reject(int index) {
			return true;
		}

	PROTECTED_TESTABLE:

		///@brief request all path to be accepted/rejected
		void request()
		{
			;
		}
		IMap::container m_added_paths;
		IMap::container m_removed_paths;
	};

}
#endif
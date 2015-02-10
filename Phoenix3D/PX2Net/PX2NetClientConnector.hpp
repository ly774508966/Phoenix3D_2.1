/*
*
* �ļ�����	��	PX2ClientConnect.hpp
*
*/

#ifndef PX2NETCLIENTCONNECT_HPP
#define PX2NETCLIENTCONNECT_HPP

#include "PX2NetPre.hpp"
#include "PX2NetEventBuffer.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2SocketAddress.hpp"
#include "PX2StreamSocket.hpp"

namespace PX2
{

	enum
	{
		CONNSTATE_INIT = 0,
		CONNSTATE_TRYCONNECT,
		CONNSTATE_CONNECTED,
		CONNSTATE_CONN_ERROR, //���ӳ���״̬�� �ȴ�����
		CONNSTATE_WAITSERVER, //���������������Ϣ�� �ȴ��ظ���Ϣ
	};

	class ClientConnector
	{
	public:
		typedef int (ClientConnector::*ServerMsgHandleFunc)(const void *pbuffer, int buflen);

	public:
		ClientConnector(int num_msghandlers);
		virtual ~ClientConnector();

		void RegisterHandler(int msgid, ServerMsgHandleFunc msgfunc, bool need_answer=true, int other_answer_msg=-1);

		StreamSocket &GetSocket ();
		int Connect(const std::string &ip, int16_t port);
		int Connect(const SocketAddress &addr);
		int Disconnect();
		int GetConnectState ();
		void SetConnectState(int state);

		//�����������ʱ�� ���ô˺�����ͼ���������Ұ�֮ǰûȷ�Ϸ�������Ϣ�����ٷ�
		//������Ҫ����Ϣ�ط�ǰ��Ҫ���͵ĸ�λ��Ϣ�� ���ΪNULL�� �Ͳ��ᷢ��λ��Ϣ
		int Reconnect(BufferEvent *pevent=NULL); 

		template<class T>
		int SendMsgToServer(int msgid, const T &msg);

		template<class T>
		BufferEvent *CreateSendEvent(int msgid, const T &msg);

		int Update(float elapsedSeconds);

	private:
		struct ServerMsgDesc
		{
			ServerMsgHandleFunc handler;
			bool need_answer;
			int other_answermsg;
		};
		std::vector<ServerMsgDesc>mMsgHandlers;

		BufferEventQueue *mSendQue;
		BufferEventQueue *mRecvQue;

		int mConnectState;
		float mConnStateTime; //���ڴ�״̬��ʱ��

		char *mRecvBuf;
		int mRecvLen;
		char *mSendBuf;
		int mSendLen;

		SocketAddress mAddr;

		//������Ϣ�� �������Ҫ������ȷ�ϵģ� ��ֱ���ͷţ� �ȷ���������ȷ����Ϣ�����ͷ�
		//����м����������� �������ӣ��ط�������Ҫȷ����Ϣ
		std::vector<BufferEvent *>mWaitConfirmMsg; 

		StreamSocket mSocket;

	private:
		void _InternalConnect();
		void _InternalDisconnect();
		void _ConfirmMsgCome(int msgid); //��ÿ����������Ϣ�� �ж��Ƿ���֮ǰ������Ϣ�ķ�����Ϣ�� ����ǣ� �������ȴ���������Ϣ
		int _HandleServerMsg ();
		int _HandleServerBufferEvent(BufferEvent *pevent);
		int _OnReservedMsg (const void *pbuffer, int buflen);
		int _ClientOnRead();
		int _ClientOnWrite();
	};

#include "PX2NetClientConnector.inl"

	typedef Pointer0<ClientConnector> ClientConnectPtr;

}

#endif
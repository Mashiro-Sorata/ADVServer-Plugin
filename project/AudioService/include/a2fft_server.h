#ifndef A2FFT_SERVER_H
#define A2FFT_SERVER_H

//include
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include<atomic>
#include<mutex>
#include<vector>
#include "../include/audiocapture.h"
//DEBUG
#include "../include/debug.h"



#define DEFAULT_IP_LOCAL "127.0.0.1"
#define DEFAULT_IP_ANY "0.0.0.0"
#define DEFAULT_PORT 5050
#define DEFAULT_MAXCLIENTS SOMAXCONN


//������
class CA2FFTServer
{
public:
	CA2FFTServer(const char* ip, u_short port, int maxClients);
	CA2FFTServer();
	~CA2FFTServer();

	bool StartServer();
	bool ExitServer();
	bool RebootServer();

	

	//���ͻ��˵ļ��ʱ��
	static const u_short sm_Interval;
	//���ͳ���
	static const u_short sm_SendLength;
	//���������ͳ���
	static const u_short sm_MonoSendLength;
	
	static const int sm_DataIndex[64];
	static const int sm_Gap[64];

private:
	bool Initial_();
	bool StartMainLoopService_();
	bool StartBufferSenderService_();
	static unsigned int __stdcall MainLoopService_(PVOID pParam);
	static unsigned int __stdcall BufferSenderService_(PVOID pParam);
	void SendToClients_(char* buffer);
	
	static CADataCapture* sm_pAudioCapture_;

	//״̬����:1->����,0->ֹͣ
	static std::atomic<bool> sm_control_;
	//��¼���ӵ�����˵�client����
	static std::atomic<u_short> sm_clientNum_;

	//������ſͻ��˵�ַ
	static std::vector<SOCKET> sm_clientsVector_;
	//��֤clientsVector����ͬʱ����
	static std::mutex sm_clientsMutex_;

	//ÿ�δ�������ݸ���
	static const UINT32 sm_DataSize_;
	static const size_t sm_ComplexSize_;

	//���ͻ�����
	static float* sm_pSendBuffer_;
	static float* sm_pLSendBuffer_;
	static float* sm_pRSendBuffer_;

	//Server�ĵ�ַ:Ĭ��0.0.0.0
	u_long m_ip_;
	//�˿ں�:Ĭ��5050
	u_short m_port_;
	//���������
	int m_maxClients_;

	//���������׽��֣����������׽���
	SOCKET m_socketServer_;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN m_serverAddr_;

	HANDLE m_mainLoopServiceHandle_;
	unsigned int m_mainLoopServiceID_;
	HANDLE m_bufferSenderServiceHandle_;
	unsigned int m_bufferSenderServiceID_;
};

#endif // A2FFT_SERVER_H

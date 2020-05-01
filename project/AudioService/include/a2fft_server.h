#pragma once

#ifndef A2FFT_SERVER_H
#define A2FFT_SERVER_H

//include
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include<atomic>
#include<mutex>
#include<vector>
#include "../include/AudioFFT.h"
#include "../include/audiocapture.h"

//DEBUG
#include "../include/debug.h"

struct WebSocketStreamHeader {
	unsigned int header_size;				//���ݰ�ͷ��С
	int mask_offset;					//����ƫ��
	unsigned int payload_size;				//���ݴ�С
	bool fin;                                               //֡���
	bool masked;					        //����
	unsigned char opcode;					//������
	unsigned char res[3];
};

enum WS_Status
{
	WS_STATUS_CONNECT = 0,
	WS_STATUS_UNCONNECT = 1,
};


enum WS_FrameType
{
	WS_EMPTY_FRAME = 0xF0,
	WS_ERROR_FRAME = 0xF1,
	WS_TEXT_FRAME = 0x01,
	WS_BINARY_FRAME = 0x02,
	WS_PING_FRAME = 0x09,
	WS_PONG_FRAME = 0x0A,
	WS_OPENING_FRAME = 0xF3,
	WS_CLOSING_FRAME = 0x08
};




//����������
class CA2FFTServer
{
public:
	CA2FFTServer(const char* ip, u_short port, int maxconn);
	CA2FFTServer();
	~CA2FFTServer();

	virtual bool Initial();
	virtual inline bool StartServer();
	virtual inline bool ExitServer();
	virtual inline bool RebootServer();

	

	//���ͻ��˵ļ��ʱ��
	static const u_short INTERVAL;
	//Ĭ�϶˿ں�
	static const u_short PORT;
	//���ͳ���
	static const u_short SENDLENGTH;
	//���������ͳ���
	static const u_short MONOSENDLENGTH;
	
	static const int DataIndex[64];
	static const int Gap[64];

private:

	static unsigned int __stdcall MainLoopService(PVOID pParam);
	static unsigned int __stdcall BufferSenderService(PVOID pParam);
	bool StartMainLoopService();
	bool StartBufferSenderService();

	void SendToClients(char* buffer);
	
	static CADataCapture* audioCapture;

	//״̬����:1->����,0->ֹͣ
	static std::atomic<bool> control;
	//���ӵ�����˵�client����,���ٶ����̵߳�����
	static std::atomic<u_short> clientNum;

	//������ſͻ��˵�ַ
	static std::vector<SOCKET> clientsVector;
	//��֤clients����ͬʱ����
	static std::mutex clientsMutex;

	//��buff����float����ʽ��ȡ
	//static float* pfData;
	//���ݰ���λ���������ݰ������ݴ���dataSize������д���
	//static UINT32 desPtn;
	//static UINT32 srcPtn;
	//static INT32 packRem;

	//FFT
	//static audiofft::AudioFFT fft;
	//ÿ�δ�������ݸ���
	static const UINT32 dataSize;
	static const size_t complexSize;

	//static std::vector<float> lData;
	//static std::vector<float> rData;

	//static std::vector<float> lRe;
	//static std::vector<float> lImg;
	//static std::vector<float> rRe;
	//static std::vector<float> rImg;

	//Server�ĵ�ַ:Ĭ��0.0.0.0
	u_long ip_;
	//�˿ں�:Ĭ��5050
	u_short port_;
	//���������
	int maxConN_;

	//���ͻ�����
	static float* sendBuffer;
	static float* lSendBuffer;
	static float* rSendBuffer;

	//���������׽��֣����������׽���
	SOCKET socketServer_;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN serverAddr_;

	HANDLE mainLoopServiceHandle_;
	unsigned int mainLoopServiceID_;
	HANDLE bufferSenderServiceHandle_;
	unsigned int bufferSenderServiceID_;
};

#endif // A2FFT_SERVER_H

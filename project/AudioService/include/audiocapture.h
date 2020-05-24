#pragma once

#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include <Audioclient.h>
#include <mmdeviceapi.h>
#include <mutex>



#define REFTIMES_PER_SEC  10000000



class CADataCapture : public IMMNotificationClient
{
public:
	CADataCapture();
	~CADataCapture();

	HRESULT Initial();
	bool StartExInitialService();
	HRESULT Start();
	HRESULT Stop();
	HRESULT Reset();
	HRESULT GetNextPacketSize();
	HRESULT GetBuffer(float** dataBuff);
	HRESULT ReleaseBuffer();
	bool IsChanging();
	bool ReStart();

	UINT32 GetNumFramesAvailable();
	UINT32 GetPacketLength();
	DWORD GetFlags();

	//��Ӧ����ʹ��pCaptureClientʱlock
	static std::mutex sm_mutexWait;

public:
	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG STDMETHODCALLTYPE AddRef(void) override;
	ULONG STDMETHODCALLTYPE Release(void) override;

	// IMMNotificationClient
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(_In_  LPCWSTR pwstrDeviceId, _In_  DWORD dwNewState) override;
	HRESULT STDMETHODCALLTYPE OnDeviceAdded(_In_  LPCWSTR pwstrDeviceId) override;
	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(_In_  LPCWSTR pwstrDeviceId) override;
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(_In_  EDataFlow flow, _In_  ERole role, _In_  LPCWSTR pwstrDefaultDeviceId) override;
	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(_In_  LPCWSTR pwstrDeviceId, _In_  const PROPERTYKEY key) override;

private:
	HRESULT ExInitial_();
	static unsigned int __stdcall ExInitialService_(PVOID pParam);

	ULONG m_referenceCount_ = 0;

	IMMDeviceEnumerator* m_pEnumerator_;
	IMMDevice* m_pDevice_;
	IAudioClient* m_pAudioClient_;
	IAudioCaptureClient* m_pCaptureClient_;
	WAVEFORMATEX* m_pwfx_;

	ERole m_role_;

	UINT32 m_numFramesAvailable_;
	UINT32 m_packetLength_;
	DWORD m_flags_;

	//��Ӧ�ô��ڿ�ʼ״̬ʱ��start��Ϊtrue
	bool m_start_;
	//��Ĭ���豸�ı��������õĹ�����changingΪtrue
	bool m_changing_;
};

#endif // !AUDIOCAPTURE_H

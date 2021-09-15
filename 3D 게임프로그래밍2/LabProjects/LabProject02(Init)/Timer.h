#pragma once
const ULONG MAX_SAMPLE_COUNT = 50; // 50ȸ�� ������ ó���ð��� �����Ͽ� ���

class CGameTimer
{
public:
	explicit CGameTimer();
	virtual ~CGameTimer();

public:
	void			Start();
	void			Stop();
	void			Reset();
	void			Tick(float fLockFPS = 0.0f); // Ÿ�̸��� �ð� ����
	unsigned long	GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0); // ������ ����Ʈ ��ȯ
	float			GetTimerElapsed(); // �������� ��� ��� �ð��� ��ȯ

private:
	bool			m_bHardwareHasPerformanceCounter; // ��ǻ�Ͱ� Performance Counter�� ������ �ִ°�
	float			m_fTimeScale; // Scale Counter�� ��
	float			m_fTimeElapsed; // ������ ������ ���� ���� �ð� -> ���� �� �ϼ��� �����? C-L
	__int64			m_nCurrentTime; // __int64�� long long�� ���Ǿ� 8��Ʈ // ������ �ð�
	__int64			m_nLastTime; // ������ �������� �ð�
	__int64			m_nPerformanceFrequency; // ��ǻ���� Performance Frequency
	float			m_fFrameTime[MAX_SAMPLE_COUNT]; // ������ �ð��� �����ϱ� ���� �迭
	ULONG			m_nSampleCount; // ������ ������ Ƚ��

	unsigned long	m_nCurrentFrameRate; // ������ ������ ����Ʈ
	unsigned long	m_nFramesPerSecond; // �ʴ� ������ ��
	float			m_fFPSTimeElapsed; // ������ ����Ʈ ��� �ҿ� �ð�

	bool			m_bStopped;

};


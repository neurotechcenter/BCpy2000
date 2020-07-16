/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <MMSystem.h>

#include <vector>
#include <string>

struct MicDevice {
	std::string mName;
	int mDeviceID;
	WAVEINCAPS mCaps;
};

class MicRecorder
{
public:
	MicRecorder(void);
	~MicRecorder(void);

	MMRESULT EnumDevices();
	unsigned int GetNumDevices() { return mDeviceEnums.size(); }
	std::string GetDeviceName(unsigned int pDeviceId) { if (pDeviceId < mDeviceEnums.size()) return mDeviceEnums[pDeviceId].mName; return ""; }
	std::string GetSelectedDeviceName() { if (mSelectedDevice >= 0) return mDeviceEnums[mSelectedDevice].mName; return ""; }
	unsigned int GetSamplesRecorded() { return mSamplesRecorded; }
	bool IsRecording() { return mRecording; }

	MMRESULT SetRecordingFormat(unsigned int pNumChannels, unsigned char pNumBitsPerChannel, unsigned int pSamplingRate);
	MMRESULT SetDevice(std::string pDevicePartialName);
	MMRESULT SetDevice(unsigned int pDeviceID);
	void SetFileName(std::string pDestFileName) { mDestFileName = pDestFileName; }

	MMRESULT OpenSelectedDevice();
	MMRESULT OpenFileForRecording();
	MMRESULT CloseDevice();
	MMRESULT InitBuffer(unsigned int pMilliseconds, unsigned int pBufferSize);

	MMRESULT Record();
	MMRESULT Stop();
	MMRESULT Reset();

	void SetDebuggingLog(bool pValue) { mDebuggingLog = pValue; }

private:

	static void CALLBACK RecordingCallback(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

	void AddAllBuffers();
	void ProcessBuffer();
	void FreeBuffer();

	bool mRecording;
	WAVEHDR *mWaveHeaderRingBuffer;
	WAVEHDR *mWaveHeaderRingBufferPtr;
	WAVEHDR *mWaveHeaderRingBufferEnd;
	WAVEHDR *mWaveHeaderRingBufferBegin;

	unsigned int mWaveHeaderNumRingBuffers;
	unsigned int mBufferBlockSize;

	std::vector<MicDevice> mDeviceEnums;
	std::string mDestFileName;

	int mSelectedDevice;
	WAVEFORMATEX mRecordingFormat;

	unsigned int mSamplesRecorded;

	HWAVEIN mDevice;

	HMMIO mDestFile;
	MMIOINFO mDestFileInfo;

	MMCKINFO mDestFileRiffChunk;
	MMCKINFO mDestFileFmtChunk;
	MMCKINFO mDestFileDataChunk;

	bool mDebuggingLog;
};

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
#include "MicRecorder.h"

#include "BCIError.h"

MicRecorder::MicRecorder(void) {

	mDebuggingLog = false;
	mRecording = false;
	mWaveHeaderRingBuffer = NULL;
	mWaveHeaderRingBufferBegin = NULL;
	mWaveHeaderRingBufferEnd = NULL;
	mWaveHeaderNumRingBuffers = 0;
	mBufferBlockSize = 0;
	mDevice = NULL;
	mDestFile = NULL; // Note: this is a handle
	mWaveHeaderRingBufferPtr = NULL;
	mSamplesRecorded = 0;

	mRecordingFormat.wFormatTag = WAVE_FORMAT_PCM;
	mRecordingFormat.nChannels = 1;
	mRecordingFormat.wBitsPerSample = 16;
	mRecordingFormat.nBlockAlign = mRecordingFormat.wBitsPerSample / 8;
	mRecordingFormat.nSamplesPerSec = 44100;
	mRecordingFormat.nAvgBytesPerSec = mRecordingFormat.nSamplesPerSec * mRecordingFormat.nBlockAlign;

	mRecordingFormat.cbSize = 0;

	mSelectedDevice = -1;

}

MMRESULT MicRecorder::EnumDevices() {
	MMRESULT result;
	unsigned int numDevices = waveInGetNumDevs();

	mDeviceEnums.clear();

	bciout << "\nNumber of microphones: " << numDevices << "\n";

	MicDevice md;
	ZeroMemory(&md.mCaps,sizeof(WAVEINCAPS));

	for(unsigned int deviceNumber = 0; deviceNumber < numDevices; deviceNumber++) {
		//bciout << " Querying mic " << deviceNumber << "\n";

		result = waveInGetDevCaps(deviceNumber, &md.mCaps, sizeof(WAVEINCAPS));
		if(result == MMSYSERR_NOERROR) {
			md.mName = md.mCaps.szPname;
			md.mDeviceID = deviceNumber;
			bciout << "  Name: " << md.mCaps.szPname << " - Number of channels: " << md.mCaps.wChannels << "\n";
			if(mDebuggingLog) bciout << "  Capabilities:\n";
			//wprintf("    Name: %s a\n", md.mCaps.szPname);
			//wprintf("    Manufacturer ID: %i\n", md.mCaps.wMid);
			//wprintf("    Product ID: %i\n", md.mCaps.wPid);
			//wprintf("    Driver version: %i\n", md.mCaps.vDriverVersion);
			//wprintf("    Number of channels: %i\n", md.mCaps.wChannels);

			unsigned int capEnum = 1;
			//wprintf("    Formats Available:\n", md.mCaps.wChannels);
			for (int capShift = 0; capShift < 20; capShift++) {
				int isCapable = capEnum & md.mCaps.dwFormats;
				if (isCapable > 0) {
					if(mDebuggingLog) bciout << "      ";

					// Freq
					switch (capShift / 4) {
					case 0:
						if(mDebuggingLog) bciout << "11.025 kHz, ";
						break;
					case 1:
						if(mDebuggingLog) bciout << "22.05 kHz, ";
						break;
					case 2:
						if(mDebuggingLog) bciout << "44.1 kHz, ";
						break;
					case 3:
						if(mDebuggingLog) bciout << "48 kHz, ";
						break;
					case 4:
						if(mDebuggingLog) bciout << "96 kHz, ";
						break;
					}

					// Channels
					switch(capShift % 2) {
					case 0:
						if(mDebuggingLog) bciout << "Mono, ";
						break;
					case 1:
						if(mDebuggingLog) bciout << "Stereo, ";
						break;
					}

					// Bits per sample
					int numBits = (capShift % 4) > 1;
					switch(numBits) {
					case 0:
						if(mDebuggingLog) bciout << "8-bit";
						break;
					case 1:
						if(mDebuggingLog) bciout << "16-bit";
						break;
					}

					if(mDebuggingLog) bciout << "\n";
				}
				capEnum = capEnum << 1;
			}
		}
		else {
			return result;
		}

		mDeviceEnums.push_back(md);
	}

	return MMSYSERR_NOERROR;
}

void CALLBACK MicRecorder::RecordingCallback(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	////wprintf("CALLBACK [%x,%i,%i] - ",dwInstance,dwParam1,dwParam2);
	MicRecorder *mr = (MicRecorder *)dwInstance;
	switch(uMsg) {
	case MM_WIM_OPEN:
		////wprintf("Open\n");
		break;
	case MM_WIM_CLOSE:
		////wprintf("Close\n");
		break;
	case MM_WIM_DATA:
		////wprintf("Data\n");
		mr->ProcessBuffer();
		break;
	default:
		//wprintf("Got unhandled message\n",uMsg);
		break;
	}
}

MMRESULT MicRecorder::SetRecordingFormat(unsigned int pNumChannels, unsigned char pNumBitsPerChannel, unsigned int pSamplingRate) {

	if(pSamplingRate != 8000 && pSamplingRate != 11025 && pSamplingRate != 22050 && pSamplingRate != 44100 && pSamplingRate != 48000 && pSamplingRate != 96000) {
		return MMSYSERR_INVALPARAM;
	}
	if(pNumChannels != 1 && pNumChannels != 2) {
		return MMSYSERR_INVALPARAM;
	}
	if(pNumBitsPerChannel != 8 && pNumBitsPerChannel != 16) {
		return MMSYSERR_INVALPARAM;
	}

	mRecordingFormat.wFormatTag = WAVE_FORMAT_PCM;
	mRecordingFormat.nChannels = pNumChannels;
	mRecordingFormat.wBitsPerSample = pNumBitsPerChannel;
	mRecordingFormat.nBlockAlign =  mRecordingFormat.nChannels * mRecordingFormat.wBitsPerSample / 8;
	mRecordingFormat.nSamplesPerSec = pSamplingRate;
	mRecordingFormat.nAvgBytesPerSec = mRecordingFormat.nSamplesPerSec * mRecordingFormat.nBlockAlign;
	mRecordingFormat.cbSize = 0;

	if(mSelectedDevice != NULL) {
		MMRESULT result = waveInOpen(&mDevice, mSelectedDevice, &mRecordingFormat, NULL, NULL, WAVE_FORMAT_QUERY);
		if (result != MMSYSERR_NOERROR) {
			bcierr << "Could not open microphone for recording!  Check parameters\n";
		}
		return result;
	}

	return MMSYSERR_NOERROR;
}

MMRESULT MicRecorder::SetDevice(unsigned int pDeviceID) {
	if (pDeviceID < 0 || pDeviceID >= mDeviceEnums.size()) {
		return MMSYSERR_INVALPARAM;
	}
	mSelectedDevice = pDeviceID;
	return MMSYSERR_NOERROR;
}

MMRESULT MicRecorder::SetDevice(std::string pDevicePartialName) {
	for (unsigned int devID = 0; devID < mDeviceEnums.size(); devID++) {
		std::string devName = mDeviceEnums[devID].mName;
		int idx = devName.find(pDevicePartialName);
		if (idx >= 0) {
			mSelectedDevice = devID;
			return MMSYSERR_NOERROR;
		}
	}
	bcierr << "Microphone name containing " << pDevicePartialName << "not found!\n";

	return MMSYSERR_INVALPARAM;
}

MMRESULT MicRecorder::OpenSelectedDevice() {
	if (mSelectedDevice == -1) {
		return MMSYSERR_BADDEVICEID;
	}

	MMRESULT result = waveInOpen(&mDevice, mSelectedDevice, &mRecordingFormat, (DWORD_PTR)(&RecordingCallback), (DWORD_PTR)this, CALLBACK_FUNCTION);
	return result;
}

MMRESULT MicRecorder::OpenFileForRecording() {
	MMRESULT result;

	ZeroMemory(&mDestFileInfo,sizeof(MMIOINFO));
	mDestFile = mmioOpen((LPSTR) mDestFileName.c_str(),&mDestFileInfo,MMIO_WRITE | MMIO_CREATE);
	if(mDestFile == NULL) return MMSYSERR_WRITEERROR;

	ZeroMemory(&mDestFileRiffChunk, sizeof(MMCKINFO));
	mDestFileRiffChunk.fccType = mmioFOURCC('W','A','V','E');
	result = mmioCreateChunk(mDestFile, &mDestFileRiffChunk, MMIO_CREATERIFF); if(result != MMSYSERR_NOERROR) return result;

	ZeroMemory(&mDestFileFmtChunk, sizeof(MMCKINFO));
	mDestFileFmtChunk.ckid = mmioFOURCC('f','m','t',' ');
	mDestFileFmtChunk.cksize = sizeof(WAVEFORMATEX);
	result = mmioCreateChunk(mDestFile, &mDestFileFmtChunk, 0); if(result != MMSYSERR_NOERROR) return result;
	int nWritten = mmioWrite(mDestFile, (HPSTR)&mRecordingFormat, sizeof(WAVEFORMATEX)); if(nWritten <= 0) return MMSYSERR_WRITEERROR;
	result = mmioAscend(mDestFile, &mDestFileFmtChunk, 0); if(result != MMSYSERR_NOERROR) return result;

	ZeroMemory(&mDestFileDataChunk, sizeof(MMCKINFO));
	mDestFileDataChunk.ckid = mmioFOURCC('d','a','t','a');
	result = mmioCreateChunk(mDestFile, &mDestFileDataChunk, 0);
	return MMSYSERR_NOERROR;
}

MMRESULT MicRecorder::InitBuffer(unsigned int pMilliseconds, unsigned int pBufferSize) {
	if (pMilliseconds <= 0 || pBufferSize <= 0 || pBufferSize % 2 != 0) {
		return MMSYSERR_INVALPARAM;
	}
	if(mDevice == NULL) {
		return MMSYSERR_INVALHANDLE;
	}

	MMRESULT result;

	FreeBuffer();

	mBufferBlockSize = pBufferSize;


	unsigned int maxDataRecordedInBytes = unsigned int(mRecordingFormat.nAvgBytesPerSec * pMilliseconds / 1000);
	mWaveHeaderNumRingBuffers = unsigned int(maxDataRecordedInBytes / mBufferBlockSize); // CAN BE 0!!
	if(maxDataRecordedInBytes % mBufferBlockSize > 0) {
		mWaveHeaderNumRingBuffers++;
	}

	mWaveHeaderRingBuffer = new WAVEHDR[mWaveHeaderNumRingBuffers];
	mWaveHeaderRingBufferBegin = mWaveHeaderRingBuffer;
	mWaveHeaderRingBufferPtr = mWaveHeaderRingBufferBegin;
	mWaveHeaderRingBufferEnd = &mWaveHeaderRingBuffer[mWaveHeaderNumRingBuffers];

	for(unsigned int waveHeaderIdx = 0; waveHeaderIdx < mWaveHeaderNumRingBuffers; waveHeaderIdx++) {
		ZeroMemory(&mWaveHeaderRingBuffer[waveHeaderIdx],sizeof(WAVEHDR));
		mWaveHeaderRingBuffer[waveHeaderIdx].lpData = new char[mBufferBlockSize];
		ZeroMemory(mWaveHeaderRingBuffer[waveHeaderIdx].lpData,mBufferBlockSize);
		mWaveHeaderRingBuffer[waveHeaderIdx].dwBufferLength = mBufferBlockSize;

		result = waveInPrepareHeader(mDevice,&mWaveHeaderRingBuffer[waveHeaderIdx],sizeof(WAVEHDR));
		if(result != MMSYSERR_NOERROR) {
			//wprintf("ERR: Unable to prepare header: %i\n",result);
			return result;
		}
	}

	mSamplesRecorded = 0;

	return MMSYSERR_NOERROR;
}

void MicRecorder::AddAllBuffers() {
	if(mWaveHeaderRingBuffer == NULL) {
		wprintf(L"ERR: Buffer not initialized!");
	}
	mWaveHeaderRingBufferPtr = mWaveHeaderRingBufferBegin;
	for(unsigned int waveHeaderIdx = 0; waveHeaderIdx < mWaveHeaderNumRingBuffers; waveHeaderIdx++) {
		MMRESULT result = waveInAddBuffer(mDevice,&mWaveHeaderRingBuffer[waveHeaderIdx],sizeof(WAVEHDR));
		if(result != MMSYSERR_NOERROR) {
			wprintf(L"ERR: Unable to add buffer: %i\n",result);
		}
	}
}


void MicRecorder::FreeBuffer() {
	MMRESULT result;

	if(mWaveHeaderRingBuffer != NULL) {
		for(unsigned int waveHeaderIdx = 0; waveHeaderIdx < mWaveHeaderNumRingBuffers; waveHeaderIdx++) {
			result = waveInUnprepareHeader(mDevice, &mWaveHeaderRingBuffer[waveHeaderIdx], sizeof(WAVEHDR));
			if(result != MMSYSERR_NOERROR) {
				//wprintf("ERR: Unable to unprepare header, memory is (probably) leaking: %i\n",result);
			}
			delete[] mWaveHeaderRingBuffer[waveHeaderIdx].lpData;
		}

		delete [] mWaveHeaderRingBuffer;
	}

	mWaveHeaderRingBuffer = NULL;
	mWaveHeaderNumRingBuffers = 0;
	mWaveHeaderRingBufferBegin = NULL;
	mWaveHeaderRingBufferEnd = NULL;
	mWaveHeaderRingBufferPtr = NULL;
	mWaveHeaderNumRingBuffers = 0;
	mBufferBlockSize = 0;
	mSamplesRecorded = 0;
}

MMRESULT MicRecorder::CloseDevice() {
	MMRESULT result;
	FreeBuffer();
	if(mDevice != NULL) {
		result = waveInClose(mDevice);
		mDevice = NULL;
		return result;
	}

	return MMSYSERR_NOERROR;
}

MMRESULT MicRecorder::Record() {

	MMRESULT result;

	mSamplesRecorded = 0;

	if(mDevice == NULL || mWaveHeaderRingBuffer == NULL) {
		return MMSYSERR_INVALHANDLE;
	}

	AddAllBuffers();

	result = waveInStart(mDevice);
	if(result != MMSYSERR_NOERROR) {
		//wprintf("ERR: Failed to start recording: %i\n",result);
		return result;
	}
	mRecording = true;

	return result;
}

MMRESULT MicRecorder::Stop() {

	MMRESULT result;

	if(mDevice == NULL || mWaveHeaderRingBuffer == NULL) {
		return MMSYSERR_INVALHANDLE;
	}

	mRecording = false;
	result = waveInReset(mDevice);
	if(result != MMSYSERR_NOERROR) {
		//wprintf("ERR: Can't reset: %i\n",result);
		return result;
	}
	result = waveInStop(mDevice);
	if(result != MMSYSERR_NOERROR) {
		//wprintf("ERR: Can't stop recording: %i\n",result);
		return result;
	}

	mmioAscend(mDestFile, &mDestFileDataChunk, 0);
	mmioAscend(mDestFile, &mDestFileRiffChunk, 0);
	result = mmioClose(mDestFile, 0);

	return result;
}

MMRESULT MicRecorder::Reset() {
	FreeBuffer();
	mSamplesRecorded = 0;
	mRecording = false;
	if(mDestFile) {
		mmioClose(mDestFile, 0);
	}
	return MMSYSERR_NOERROR;
}

void MicRecorder::ProcessBuffer() {
	MMRESULT result;

	mSamplesRecorded += mWaveHeaderRingBufferPtr->dwBytesRecorded / mRecordingFormat.nBlockAlign;

	int nWritten = mmioWrite(mDestFile,mWaveHeaderRingBufferPtr->lpData,mWaveHeaderRingBufferPtr->dwBytesRecorded);
	if(nWritten != mWaveHeaderRingBufferPtr->dwBytesRecorded) {
		bciout << "ERR: Didn't record what was asked";
	}

	if(mRecording) {
		result = waveInAddBuffer(mDevice,mWaveHeaderRingBufferPtr,sizeof(WAVEHDR));
	}

	mWaveHeaderRingBufferPtr++;
	if(mWaveHeaderRingBufferPtr == mWaveHeaderRingBufferEnd) {
		mWaveHeaderRingBufferPtr = mWaveHeaderRingBufferBegin;
	}
}

MicRecorder::~MicRecorder(void) {
	FreeBuffer();
	CloseDevice();
}

#ifndef SOUNDCONTROLLER_H
#define SOUNDCONTROLLER_H

#include <XACT3.h>
#include <xact3d3.h>
#include <D3dx9math.h>
#include <x3daudio.h>


class SoundController
{
public:
	SoundController();  //sets the wave and soundbank filepaths
	~SoundController();
	void Init(); //initializes the XACT engine
	void PlayBGMusic(); //initializes the game's background music track
	void PlayCue(XACTINDEX iCueIndex);  //plays a cue
	void Play3DCue(XACTINDEX iCueIndex, D3DXVECTOR3* pvPosition);
	void DoWork();  //called once per frame to run the audio
	void CleanUp();  //destroy objects and free up memory
	
private:
	IXACT3Engine*						pXACT3Engine;
	X3DAUDIO_HANDLE						x3DInstance;
	IXACT3WaveBank*						pWaveBank;
	IXACT3SoundBank*					pSoundBank;
	VOID*								pbWaveBank; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
    VOID*								pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed
	XACTINDEX							dwSoundCueIndex;
	IXACT3Cue*							pCue;
	LPCWSTR								gsPath;
	LPCWSTR								sbPath;
	LPCWSTR								wbPath;
	X3DAUDIO_LISTENER					listener;
    X3DAUDIO_EMITTER					emitter;
	FLOAT32								DelayTimes[2];
	FLOAT32								MatrixCoefficients[2 * 8];
	X3DAUDIO_DSP_SETTINGS				dspSettings;
	bool								bInitialized;
	bool								bMusicPlaying;
	IXACT3Cue*							pBackgroundMusic;	

public:
    XACTINDEX				iBackgroundMusic;
	XACTINDEX				iHCIdle;

};

#endif //SOUNDCONTROLLER_H

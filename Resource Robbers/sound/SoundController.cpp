#include <iostream>
#include <sstream>
#include "Log.hpp"

#include "SoundController.h"


/****************************************************************************
* Constructor:
* Sets the soundbank and wavebank filepaths
*****************************************************************************/
SoundController::SoundController()
: pXACT3Engine(NULL), pWaveBank(NULL), pSoundBank(NULL), dwSoundCueIndex(0),
  pCue(NULL) 
{
	gsPath = L"../../RR Win Audio.xgs";
	sbPath = L"../include/Sound Bank.xsb";
	wbPath = L"../include/Wave Bank.xwb";
}

/****************************************************************************
* Destructor:
* 
*****************************************************************************/
SoundController::~SoundController() {}



/****************************************************************************
* Initialize:
* Creates and initializes the XACT engine
*****************************************************************************/
void SoundController::Init()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	XACT3CreateEngine( /*XACT_FLAG_API_DEBUG_MODE*/  0, &pXACT3Engine );

// Load the global settings file and pass it into XACTInitialize
    VOID* pGlobalSettingsData = NULL;
    DWORD dwGlobalSettingsFileSize = 0;
    bool bSuccess = false;
    DWORD dwBytesRead;
    HANDLE hFile;
	DWORD dwFileSize;
    HANDLE hMapFile;


    hFile = CreateFile( gsPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile )
    {
        dwGlobalSettingsFileSize = GetFileSize( hFile, NULL );
        if( dwGlobalSettingsFileSize != INVALID_FILE_SIZE )
        {
            // Using CoTaskMemAlloc so that XACT can clean up this data when its done
            pGlobalSettingsData = CoTaskMemAlloc( dwGlobalSettingsFileSize );
            if( pGlobalSettingsData )
            {
                if( 0 != ReadFile( hFile, pGlobalSettingsData, dwGlobalSettingsFileSize, &dwBytesRead, NULL ) )
                {
                    bSuccess = true;
                }
            }
        }
        CloseHandle( hFile );
    }
    if( !bSuccess )
    {
        if( pGlobalSettingsData )
            CoTaskMemFree( pGlobalSettingsData );
        pGlobalSettingsData = NULL;
        dwGlobalSettingsFileSize = 0;
    }


    XACT_RUNTIME_PARAMETERS xrParams = {0};
    xrParams.pGlobalSettingsBuffer = pGlobalSettingsData;
    xrParams.globalSettingsBufferSize = dwGlobalSettingsFileSize;
    xrParams.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA; // this will tell XACT to delete[] the buffer when its unneeded
    //xrParams.fnNotificationCallback = XACTNotificationCallback;
    xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;

	pXACT3Engine->Initialize( &xrParams );


	// Setup 3D audio structs
    ZeroMemory( &listener, sizeof( X3DAUDIO_LISTENER ) );
    listener.OrientFront = D3DXVECTOR3( 0, 0, 1 );
    listener.OrientTop = D3DXVECTOR3( 0, 1, 0 );
    listener.Position = D3DXVECTOR3( 0, 5, 0 );
    listener.Velocity = D3DXVECTOR3( 0, 0, 0 );

    ZeroMemory( &emitter, sizeof( X3DAUDIO_EMITTER ) );
    emitter.pCone = NULL;
    emitter.OrientFront = D3DXVECTOR3( 0, 0, 1 );
    emitter.OrientTop = D3DXVECTOR3( 0, 1, 0 );
    emitter.Position = D3DXVECTOR3( 0, 0, 0 );
    emitter.Velocity = D3DXVECTOR3( 0, 0, 0 );
    emitter.ChannelCount = 2;
    emitter.ChannelRadius = 1.0f;
    emitter.pChannelAzimuths = NULL;
    emitter.pVolumeCurve = NULL;
    emitter.pLFECurve = NULL;
    emitter.pLPFDirectCurve = NULL;
    emitter.pLPFReverbCurve = NULL;
    emitter.pReverbCurve = NULL;
    emitter.CurveDistanceScaler = 1.0f;
    emitter.DopplerScaler = NULL;

    DelayTimes[0] = 0.0f;
    DelayTimes[1] = 0.0f;

    // Init MatrixCoefficients. XACT will fill in the values
    ZeroMemory( &MatrixCoefficients, sizeof( FLOAT32 ) * 8 * 2 );

    // query number of channels on the final mix
    WAVEFORMATEXTENSIBLE wfxFinalMixFormat;
    pXACT3Engine->GetFinalMixFormat( &wfxFinalMixFormat );
	WORD nDstChannelCount = wfxFinalMixFormat.Format.nChannels;

    ZeroMemory( &dspSettings, sizeof( X3DAUDIO_DSP_SETTINGS ) );
    dspSettings.pMatrixCoefficients = MatrixCoefficients;
    dspSettings.pDelayTimes = DelayTimes;
    dspSettings.SrcChannelCount = 2;
    dspSettings.DstChannelCount = nDstChannelCount;

    // Initialize 3D settings
    XACT3DInitialize( pXACT3Engine, x3DInstance );


	// Read and register the wave bank file with XACT using memory mapped file IO
    // Memory mapped files tend to be the fastest for most situations assuming you
    // have enough virtual address space for a full map of the file
    hFile = CreateFile( wbPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
            if( hMapFile )
            {
                pbWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( pbWaveBank )
                {
                    pXACT3Engine->CreateInMemoryWaveBank( pbWaveBank, dwFileSize, 0,
                                                                       0, &pWaveBank );
                }
                CloseHandle( hMapFile ); // pbWaveBank is maintains a handle on the file so close this unneeded handle
            }
        }
        CloseHandle( hFile ); // pbWaveBank is maintains a handle on the file so close this unneeded handle
    }
    
    // Read and register the sound bank file with XACT.  Do not use memory mapped file IO because the
    // memory needs to be read/write and the working set of sound banks are small.
    hFile = CreateFile( sbPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            // Allocate the data here and free the data when recieving the sound bank destroyed notification
            pbSoundBank = new BYTE[dwFileSize];
            if( pbSoundBank )
            {
                if( 0 != ReadFile( hFile, pbSoundBank, dwFileSize, &dwBytesRead, NULL ) )
                {
                    pXACT3Engine->CreateSoundBank( pbSoundBank, dwFileSize, 0,
                                                                0, &pSoundBank );
                }
            }
        }
        CloseHandle( hFile );
    }

	bInitialized = true;

	iBackgroundMusic = pSoundBank->GetCueIndex( "Test" );
	iHCIdle = pSoundBank->GetCueIndex( "HC_Idle" );
}



/****************************************************************************
* Play Background Music:
* Starts the background music if it isn't already playing.
*****************************************************************************/
void SoundController::PlayBGMusic()
{
	if( bInitialized )
	{
		if( !bMusicPlaying)
		{
			bMusicPlaying = true;
			pSoundBank->Play( iBackgroundMusic, 0, 0, &pBackgroundMusic );
		}
	}
}



/****************************************************************************
* Play Cue:
* Plays a non-3D cue based on an index
*****************************************************************************/
void SoundController::PlayCue(XACTINDEX iCueIndex)
{
	if( bInitialized )
	{
		pSoundBank->Play( iCueIndex, 0, 0, NULL );
	}
}



/****************************************************************************
* Play 3DCue:
* Plays a 3D cue based on an index and position
*****************************************************************************/
void SoundController::Play3DCue( XACTINDEX iCueIndex, D3DXVECTOR3* pvPosition )
{
	if( bInitialized )
	{		
		emitter.Position = *pvPosition;
		
		// Play the cue as a 3D audio cue
		pSoundBank->Prepare( iCueIndex, 0, 0, &pCue );
		XACT3DCalculate( x3DInstance, &listener, &emitter, &dspSettings );
		XACT3DApply( &dspSettings, pCue );
		pCue->Play();
	}
}

/****************************************************************************
* Do Work:
* Must be called about once per frame to proces sound
*****************************************************************************/
void SoundController::DoWork()
{
	{
        listener.OrientFront = D3DXVECTOR3( 0, 0, 1 );
		listener.OrientTop = D3DXVECTOR3( 0, 1, 0 );
		listener.Position = D3DXVECTOR3( 0, 5, 0 );

        // Background music positioning
        emitter.Position = D3DXVECTOR3( 0, 0, 0 );
        XACT3DCalculate(x3DInstance, &listener, &emitter, &dspSettings);
        XACT3DApply(&dspSettings, pBackgroundMusic);
    }
	pXACT3Engine->DoWork();
}



/****************************************************************************
* Clean Up:
* Cleans up and deletes all sound files and memory. Seems to be
* having some issues and needs to be fixed.
*****************************************************************************/
void SoundController::CleanUp()
{
	// Shutdown XACT
	pXACT3Engine->ShutDown();
	pXACT3Engine = NULL;
/*
	if (pSoundBank != NULL)
	{
		pSoundBank->Destroy();
        delete pSoundBank;
		pSoundBank = NULL;
	}
    // Release memory mapped files
    if (pWaveBank != NULL)
	{
        //UnmapViewOfFile(pWaveBank);
		pWaveBank->Destroy();
		delete pWaveBank;
		pWaveBank = NULL;
	}
	if (pXACT3Engine != NULL)
    {
        pXACT3Engine->ShutDown();
		//delete pXACT3Engine;
		pXACT3Engine = NULL;
    }
  */  
    CoUninitialize();
}


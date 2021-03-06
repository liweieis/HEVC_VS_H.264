/************************************************************************
    This file is part of VLCWrapper.
    
    File:    VLCWrapper.h
    Desc.:   An simple C++-interface to libvlc.

	Author:  LiWei
	Date:    09/22/2013
	eMail:   liwei_eis@whu.edu.cn

	VLCWrapper is distributed under the Code Project Open License (CPOL).

************************************************************************/
#pragma once
#include "vlc\vlc.h"
#include "vlc\libvlc_events.h"
//#include "vlc_input_item.h"

// Typedefs for old MS compilers
typedef unsigned __int32	uint32_t;
typedef __int64             int64_t;

// Typedefs for VLC types
typedef struct libvlc_event_t           VLCEvent;                     ///< A vlc event.
typedef void (*VLCEventHandler)         (const VLCEvent *, void *);   ///< Event handler callback.

class VLCWrapper
{
public:
	VLCWrapper(void);
	~VLCWrapper(void);

	void SetAudioOutput();

	 /** Set window for media output.
    *   @param [in] pHwnd window, on Windows a HWND handle. */
    void SetOutputWindow(void* pHwnd);

    /** Register an event handler for libvlc-events.
    *   @param [in] event The event handler.
	*   @param [in] pUserData pointer to user data.
	*/
    void SetEventHandler(VLCEventHandler event, void* pUserData);

    /** Open a media file.
    *   @param [in] pMediaPathName PathName of the media file. */
    void OpenMedia(const char* pMediaPathName);

    /** Start playback. */
    void Play();

    /** Pause playback. */
    void Pause();
    
    /** Stop playback. */
    void Stop();

    /** Get length of media in milliseconds. Call this in the event handler,
    *   otherwise the result is not reliable!!!
    *   @return The media length in milliseconds. */
    int64_t GetLength();

    /** Get actual position of media in milliseconds. Call this in the event handler,
    *   otherwise the result is not reliable!!!
    *   @return The media position in milliseconds. */
    int64_t GetTime();

    /** Set new position of media in milliseconds.
    *   @param [in] newTime The new media position in milliseconds. */
    void SetTime(int64_t newTime);

    /** Mutes the audio output of playback.
    *   @param [in] mute True or false. */
    void Mute(bool mute = true);

    /** Get mute state of playback.
    *   @return True or false. */
    bool GetMute();

    /** Returns the actual audio volume.
    *   @return The actual audio volume. */
    int  GetVolume();

    /** Set the actual audio volume.
    *   @param [in] volume New volume level. */
    void SetVolume(int volume);

	libvlc_state_t GetState();

	bool GetMediaStats(libvlc_media_stats_t *p_stats);

	/*CString GetCodeInfo();*/
    void AddOption(const char *optionStr);
protected:
	libvlc_instance_t*       m_pVLCInstance;        ///< The VLC instance.
	libvlc_media_player_t*   m_pMediaPlayer;        ///< The VLC media player object.
	libvlc_media_t*          m_pMedia;              ///< The media played by the media player.
	libvlc_event_manager_t*  m_pEventManager;       ///< The event manger for the loaded media file.    
	VLCEventHandler          m_eventHandler;         ///< An event handler for the media player.

};


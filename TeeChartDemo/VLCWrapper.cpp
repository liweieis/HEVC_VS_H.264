#include "StdAfx.h"
#include "VLCWrapper.h"

void ConvertUTF8ToANSI(char* strUTF8, CString &strANSI)   
{   
	int nLen = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCTSTR)strUTF8, -1, NULL, 0); 
	//返回需要的unicode长度   
	WCHAR * wszANSI = new WCHAR[nLen+1];   
	memset(wszANSI, 0, nLen * 2 + 2);   
	nLen = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8, -1, wszANSI, nLen);    //把utf8转成unicode  
	nLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL);        //得到要的ansi长度   
	char *szANSI=new char[nLen + 1];   
	memset(szANSI, 0, nLen + 1);   
	WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, szANSI, nLen, NULL,NULL);          //把unicode转成ansi   
	strANSI = szANSI;   
	delete wszANSI;   
	delete szANSI;   
}


VLCWrapper::VLCWrapper(void) : m_pMedia(NULL),
m_eventHandler(NULL),
m_pEventManager(NULL),
m_pMediaPlayer(NULL),
m_pVLCInstance(NULL)
{
	const char * const vlc_args[] = {"--no-ignore-config", NULL};

	// init vlc modules, should be done only once
	m_pVLCInstance = libvlc_new(1/*sizeof(vlc_args) / sizeof(vlc_args[0])*/, vlc_args);

	// Create a media player playing environement
	m_pMediaPlayer = libvlc_media_player_new(m_pVLCInstance);

	// Create an event manager for the player for handling e.g. time change events
	m_pEventManager = libvlc_media_player_event_manager(m_pMediaPlayer);
}

VLCWrapper::~VLCWrapper(void)
{
	// Free the media_player
	libvlc_media_player_release(m_pMediaPlayer);
	libvlc_release(m_pVLCInstance);
}

void VLCWrapper::OpenMedia(const char* pMediaPathName)
{
	// 网络视频
	if (pMediaPathName[0] == 'h' && pMediaPathName[1] == 't' && pMediaPathName[2] == 't' && pMediaPathName[3] == 'p')
	{
		m_pMedia = libvlc_media_new_location(m_pVLCInstance, pMediaPathName);
	}
	// 本地视频
	else
	{
		m_pMedia = libvlc_media_new_path(m_pVLCInstance, pMediaPathName);
	}

	libvlc_media_player_set_media(m_pMediaPlayer, m_pMedia);
}
void VLCWrapper::AddOption(const char *optionStr)
{
	libvlc_media_add_option_flag(m_pMedia, optionStr, 2);
}

void VLCWrapper::SetOutputWindow(void* pHwnd)
{
	libvlc_media_player_set_hwnd(m_pMediaPlayer, pHwnd);
}

void VLCWrapper::SetAudioOutput()
{
	libvlc_audio_output_t *audio = libvlc_audio_output_list_get(m_pVLCInstance);
	libvlc_audio_output_set(m_pMediaPlayer, audio->psz_name);
	libvlc_audio_output_list_release(audio);
}

void VLCWrapper::SetEventHandler(VLCEventHandler event, void* pUserData)
{
	m_eventHandler = event;
	libvlc_event_attach(m_pEventManager,        
		libvlc_MediaPlayerTimeChanged,
		m_eventHandler,
		pUserData);
	libvlc_event_attach(m_pEventManager, libvlc_MediaPlayerEndReached, m_eventHandler, pUserData);
}

void VLCWrapper::Play()
{
	libvlc_media_player_play(m_pMediaPlayer);
}

void VLCWrapper::Pause()
{
	libvlc_media_player_pause(m_pMediaPlayer);
}

void VLCWrapper::Stop()
{
	libvlc_media_player_stop(m_pMediaPlayer);
}

int64_t VLCWrapper::GetLength()
{
	int64_t length = libvlc_media_player_get_length(m_pMediaPlayer);
	return length;
}

int64_t VLCWrapper::GetTime()
{
	int64_t time = libvlc_media_player_get_time(m_pMediaPlayer);    
	return time;
}

void VLCWrapper::SetTime(int64_t newTime)
{
	libvlc_media_player_set_time(m_pMediaPlayer, (libvlc_time_t)newTime);
}

void VLCWrapper::Mute( bool mute /*= true*/ )
{
	libvlc_audio_set_mute(m_pMediaPlayer, mute);
}

bool VLCWrapper::GetMute()
{
	bool bMuteState = !!libvlc_audio_get_mute(m_pMediaPlayer);
	return bMuteState;
}

int VLCWrapper::GetVolume()
{
	int volume = libvlc_audio_get_volume(m_pMediaPlayer);
	return volume;
}

void VLCWrapper::SetVolume(int volume)
{
	int ret = libvlc_audio_set_volume(m_pMediaPlayer, volume);
}

libvlc_state_t VLCWrapper::GetState()
{
	return libvlc_media_get_state(m_pMedia);
}

bool VLCWrapper::GetMediaStats(libvlc_media_stats_t *p_stats)
{
	return libvlc_media_get_stats(m_pMedia, p_stats);
}

//CString VLCWrapper::GetCodeInfo()
//{
//	CString str;
//	unsigned int width;
//	unsigned int height;
//	libvlc_video_get_size(m_pMediaPlayer, 0, &width, &height);
//
//	char *info = new char[1024];
//	memset(info, 0, 1024);
//	libvlc_media_get_codecinfo(m_pMedia, info);
//	ConvertUTF8ToANSI(info, str);
//	
//	return str;
//}

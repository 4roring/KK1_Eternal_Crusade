#include "stdafx.h"
#include "SoundManager.h"
#include <filesystem>

namespace FILESYSTEM = std::experimental::filesystem;
static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
	Release();
}

SoundSource * CSoundManager::FindSound(const std::wstring & sound_key)
{
	auto iter_find = map_sound_.find(sound_key);
	if (iter_find == map_sound_.end())
		return nullptr;

	return iter_find->second;
}

HRESULT CSoundManager::Initialize()
{
	FMOD_RESULT result = FMOD_System_Create(&ptr_system_);
	if (result != FMOD_OK) return E_FAIL;
	
	result = FMOD_System_Init(ptr_system_, CHANNEL_END, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) return E_FAIL;

	if (FAILED(LoadSoundFile())) return E_FAIL;

	return S_OK;
}

HRESULT CSoundManager::LoadSoundFile()
{
	FILESYSTEM::path file_path(TEXT("../bin/Resources/Sound"));

	if (false == FILESYSTEM::is_directory(file_path)) return E_FAIL;

	for (const auto& directory : FILESYSTEM::directory_iterator(file_path))
	{
		if (FILESYSTEM::is_regular_file(directory.status()))
		{
			file_path = directory.path();
			SoundSource* ptr_sound = nullptr;
			FMOD_RESULT result = FMOD_System_CreateSound(ptr_system_
				, converter.to_bytes(file_path).c_str(), FMOD_HARDWARE, nullptr, &ptr_sound);

			if (result == FMOD_OK)
			{
				auto& iter_find = map_sound_.find(file_path.stem());
				if (iter_find != map_sound_.end())
				{
					FMOD_Sound_Release(ptr_sound);
					continue;
				}
				map_sound_.emplace(file_path.stem(), ptr_sound);
			}
		}
	}
	file_path.clear();
	FMOD_System_Update(ptr_system_);

	return S_OK;
}

void CSoundManager::Update()
{
	FMOD_System_Update(ptr_system_);
}

void CSoundManager::PlaySound(SoundSource * ptr_sound, ChannelID channel)
{
	BOOL is_playing = FALSE;
	FMOD_Channel_IsPlaying(ptr_channel_[channel], &is_playing);
	if (is_playing)
		FMOD_System_PlaySound(ptr_system_, FMOD_CHANNEL_FREE,
			ptr_sound, FALSE, &ptr_channel_[channel + 1]);
	else
		FMOD_System_PlaySound(ptr_system_, FMOD_CHANNEL_FREE,
			ptr_sound, FALSE, &ptr_channel_[channel]);
}

void CSoundManager::PlayBGM(SoundSource * ptr_sound)
{
	FMOD_System_PlaySound(ptr_system_, FMOD_CHANNEL_FREE,
		ptr_sound, FALSE, &ptr_channel_[CHANNEL_BGM]);
}

void CSoundManager::StopSound(ChannelID channel)
{
	FMOD_Channel_Stop(ptr_channel_[channel]);
}

void CSoundManager::StopAll()
{
	for (auto& channel : ptr_channel_)
		FMOD_Channel_Stop(channel);
}

void CSoundManager::Release()
{
	for (auto& pair : map_sound_)
		FMOD_Sound_Release(pair.second);

	map_sound_.clear();

	FMOD_System_Release(ptr_system_);
	FMOD_System_Close(ptr_system_);
}

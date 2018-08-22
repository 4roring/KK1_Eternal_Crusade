#pragma once

#pragma comment(lib, "fmodex_vc.lib")

#include "Engine_Singleton.h"

typedef FMOD_SOUND SoundSource;

class CSoundManager
	: public Engine::CSingleton<CSoundManager>
{
public:
	enum ChannelID { CHANNEL_BGM
		, CHANNEL_PLAYER, CHANNEL_PLAYER_ATTACK, CHANNEL_PLAYER_ATTACK_
		, CHANNEL_ENEMY, CHANNEL_ENEMY_ATTACK, CHANNEL_ENEMY_ATTACK_
		, CHANNEL_END};

private:
	friend CSingleton;
	CSoundManager();
	virtual ~CSoundManager();

public:
	SoundSource* FindSound(const std::wstring& sound_key);

public:
	HRESULT Initialize();
	HRESULT LoadSoundFile();
	void Update();
	void PlaySound(SoundSource* ptr_sound, ChannelID channel);
	void PlayBGM(SoundSource* ptr_sound);
	void StopSound(ChannelID channel);
	void StopAll();
	void Release();

private:
	FMOD_SYSTEM* ptr_system_ = nullptr;
	FMOD_CHANNEL* ptr_channel_[CHANNEL_END] = { nullptr };
	std::unordered_map<std::wstring, SoundSource*> map_sound_;
};

static CSoundManager* Sound() { return CSoundManager::GetInstance(); }

inline Engine::CGraphicDevice* GraphicDevice()
{
	return CGraphicDevice::GetInstance();
}

inline Engine::CTimeManager* Time()
{
	return CTimeManager::GetInstance();
}

inline Engine::CInputManager* Input()
{
	return CInputManager::GetInstance();
}

inline Engine::CFontManager* Font()
{
	return CFontManager::GetInstance();
}
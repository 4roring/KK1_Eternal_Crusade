#pragma once

#include "Engine_Include.h"
#include "Logo.h"
#include "Stage.h"

class CSceneSelector
{
public:
	enum class SCENE { LOGO, TITLE, STAGE, SPECIAL_ISLAND };

public:
	explicit CSceneSelector(SCENE scene_id)
		: scene_id_(scene_id) {}
	~CSceneSelector() {}

public:
	HRESULT operator()(Engine::CScene** pp_scene, LPDIRECT3DDEVICE9 ptr_device)
	{
		switch (scene_id_)
		{
		case SCENE::LOGO:
			*pp_scene = CLogo::Create(ptr_device);
			break;
		case SCENE::TITLE:
			break;
		case SCENE::STAGE:
			*pp_scene = CStage::Create(ptr_device);
			break;
		case SCENE::SPECIAL_ISLAND:
			break;
		default:
			assert(!"Scene Change Error(scene_id_ is default)");
			break;
		}
		(*pp_scene)->InitScene();

		if (scene_id_ != SCENE::LOGO)
			(*pp_scene)->Update(Engine::Time()->GetDeltaTime());

		assert(nullptr != pp_scene && "SceneChange Failed");
		return S_OK;
	}

private:
	SCENE scene_id_;
};
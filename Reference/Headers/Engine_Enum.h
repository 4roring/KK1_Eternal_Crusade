#pragma once

namespace Engine
{
	enum class TEXTURETYPE { NORMAL, STATIC_MESH, CUBE };
	enum RENDERLAYER { LAYER_PRIORITY, LAYER_NONEALPHA, LAYER_ALPHA, LAYER_UI, LAYER_END };
}

enum class COLLTAG { PLAYER, ENEMY, PROP };
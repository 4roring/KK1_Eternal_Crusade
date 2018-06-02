#include "Shader.h"

Engine::CShader::CShader(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CShader::~CShader()
{
}

const LPD3DXEFFECT Engine::CShader::GetEffectHandle()
{
	return ptr_effect_;
}

Engine::CComponent * Engine::CShader::CloneComponent()
{
	++reference_count_;
	return this;
}

HRESULT Engine::CShader::AddShaderFile(const TCHAR * file_path)
{
	HRESULT hr = D3DXCreateEffectFromFile(ptr_device_, file_path, nullptr, nullptr, D3DXSHADER_DEBUG
		, nullptr, &ptr_effect_, &ptr_error_message_);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)ptr_error_message_->GetBufferPointer(), nullptr, MB_OK);
		assert(!"Shader Create Error!!!");
		return E_FAIL;
	}
	return S_OK;
}

void Engine::CShader::BegineShader(int pass_index)
{
	ptr_effect_->Begin(nullptr, 0);
	ptr_effect_->BeginPass(pass_index);
}

void Engine::CShader::EndShader()
{
	ptr_effect_->EndPass();
	ptr_effect_->End();
}

int Engine::CShader::Release()
{
	if (--reference_count_ == 0)
	{
		Safe_Release(ptr_error_message_);
		Safe_Release(ptr_effect_);
		return 0;
	}

	return reference_count_;
}

Engine::CShader * Engine::CShader::Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR * file_path)
{
	CShader* ptr_shader = new CShader(ptr_device);
	if (FAILED(ptr_shader->AddShaderFile(file_path)))
	{
		Safe_Release_Delete(ptr_shader);
		assert(!"Shader Create Failed!!!");
	}
	ptr_shader->AddReferenceCount();
	return ptr_shader;
}

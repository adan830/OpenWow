#include <stdafx.h>

#include "ConstantBufferDX11.h"

ConstantBufferDX11::ConstantBufferDX11(ID3D11Device2* pDevice, size_t size)
	: m_pDevice(pDevice)
	, m_BufferSize(size)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)m_BufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pBuffer)))
	{
		Log::Error("Failed to create constant buffer for shader.");
	}

	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

ConstantBufferDX11::~ConstantBufferDX11()
{}

void ConstantBufferDX11::Set(const void* data, size_t size)
{
	assert(size == m_BufferSize);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(m_pDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		Log::Error("Failed to map constant buffer.");
		return;
	}

	memcpy(mappedResource.pData, data, m_BufferSize);

	m_pDeviceContext->Unmap(m_pBuffer, 0);
}

void ConstantBufferDX11::Copy(std::shared_ptr<ConstantBuffer> other)
{
	std::shared_ptr<ConstantBufferDX11> srcBuffer = std::dynamic_pointer_cast<ConstantBufferDX11>(other);

	if (srcBuffer && srcBuffer.get() != this &&
		m_BufferSize == srcBuffer->m_BufferSize)
	{
		m_pDeviceContext->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
	{
		Log::Error("Source buffer is not compatible with this buffer.");
	}
}

void ConstantBufferDX11::Copy(std::shared_ptr<Buffer> other)
{
	Copy(std::dynamic_pointer_cast<ConstantBuffer>(other));
}

bool ConstantBufferDX11::Bind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	bool result = true;

	ID3D11Buffer* pBuffers[] = { m_pBuffer };

	switch (shaderType)
	{
	case Shader::VertexShader:
		m_pDeviceContext->VSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::TessellationControlShader:
		m_pDeviceContext->HSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::TessellationEvaluationShader:
		m_pDeviceContext->DSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::GeometryShader:
		m_pDeviceContext->GSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::PixelShader:
		m_pDeviceContext->PSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::ComputeShader:
		m_pDeviceContext->CSSetConstantBuffers(id, 1, pBuffers);
		break;
	default:
		result = false;
		break;
	}

	return result;
}

void ConstantBufferDX11::UnBind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	ID3D11Buffer* pBuffers[] = { nullptr };

	switch (shaderType)
	{
	case Shader::VertexShader:
		m_pDeviceContext->VSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::TessellationControlShader:
		m_pDeviceContext->HSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::TessellationEvaluationShader:
		m_pDeviceContext->DSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::GeometryShader:
		m_pDeviceContext->GSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::PixelShader:
		m_pDeviceContext->PSSetConstantBuffers(id, 1, pBuffers);
		break;
	case Shader::ComputeShader:
		m_pDeviceContext->CSSetConstantBuffers(id, 1, pBuffers);
		break;
	default:
		break;
	}
}
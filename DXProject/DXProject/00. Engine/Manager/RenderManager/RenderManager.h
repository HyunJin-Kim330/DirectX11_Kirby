#pragma once
#include "00. Engine/Pipeline/02. VertexShader/ConstantBuffer.h"

class BlendState;
class GameObject;
class Pipeline;
class SamplerState;

class RenderManager
{
public:
	RenderManager(HWND _hwnd);
	~RenderManager();

	void Init();
	ComPtr<ID3D11Device> GetDevice() { return device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return deviceContext; }

public:
	void Render();

private:
	vector<GameObject*> GetRenderableGameObject();  // mesh renderer������Ʈ�� ������ �ִ� ������Ʈ���� ������
	vector<GameObject*> GetRenderableCollider();
	void ReadyRender();				// ������ �� ȭ�� �ʱ�ȭ
	void RenderWithPipeline();		// ������������ ������Ʈ���� �����ϰ� �׿����� ������
	void SwapRender();				// ���߹��۸�
	void PushCameraData();			// ī�޶� constant buffer�� GPU�� ����
	void PushTransformData();		// transform constant buffer�� GPU�� ����
	void PushAnimationData();

private:
	HWND hwnd;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	D3D11_VIEWPORT viewport = {0};

	Pipeline* pipeline;

	// Camera
	CameraData cameraData;
	ConstantBuffer<CameraData>* cameraBuffer;

	// SRT
	TransformData transformData;
	ConstantBuffer<TransformData>* transformBuffer;
	
	// Animation
	AnimationData animationData;
	ConstantBuffer<AnimationData>* animationBuffer;

	SamplerState* samplerState;
	BlendState* blendState;

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();
};


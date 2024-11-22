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
	vector<GameObject*> GetRenderableGameObject();  // mesh renderer컴포넌트를 가지고 있는 오브젝트들을 가져옴
	vector<GameObject*> GetRenderableCollider();
	void ReadyRender();				// 렌더링 전 화면 초기화
	void RenderWithPipeline();		// 파이프라인을 오브젝트별로 셋팅하고 그에따라 렌더링
	void SwapRender();				// 이중버퍼링
	void PushCameraData();			// 카메라 constant buffer을 GPU에 복사
	void PushTransformData();		// transform constant buffer을 GPU에 복사
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


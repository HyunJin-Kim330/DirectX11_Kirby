#include "pch.h"
#include "00. Engine/Manager/RenderManager/RenderManager.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "00. Engine/Component/Camera/Camera.h"
#include "00. Engine/Pipeline/Pipeline.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "00. Engine/Pipeline/04. PixelShader/SamplerState.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "00. Engine/Component/Collider/Collider.h"

RenderManager::RenderManager(HWND _hwnd) : hwnd(_hwnd)
{

}

RenderManager::~RenderManager()
{

}

void RenderManager::Init()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	pipeline = new Pipeline(deviceContext);
	cameraBuffer = new ConstantBuffer<CameraData>(device, deviceContext);
	cameraBuffer->Create();
	transformBuffer = new ConstantBuffer<TransformData>(device, deviceContext);
	transformBuffer->Create();
	animationBuffer = new ConstantBuffer<AnimationData>(device, deviceContext);
	animationBuffer->Create();

	samplerState = new SamplerState(device);
	samplerState->Create();
	blendState = new BlendState(device);
	blendState->Create();
}

void RenderManager::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		nullptr,
		deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void RenderManager::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
	CHECK(hr);
}

void RenderManager::SetViewport()
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(GWinSizeX);
	viewport.Height = static_cast<float>(GWinSizeY);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void RenderManager::Render()
{
	ReadyRender();
	PushCameraData();
	RenderWithPipeline();
	SwapRender();
}

vector<GameObject*> RenderManager::GetRenderableGameObject()
{
	return GGameManager->GetSceneManager()->GetRenderableObject();
}

vector<GameObject*> RenderManager::GetRenderableCollider()
{
	return GGameManager->GetSceneManager()->GetRenderableCollider();
}

void RenderManager::ReadyRender()
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
	deviceContext->RSSetViewports(1, &viewport);
}

void RenderManager::RenderWithPipeline()		// 파이프라인를 거쳐 렌더링
{
	vector<GameObject*> renderableObjects = GetRenderableGameObject();		// 렌더링 가능한 오브젝트들만 가져옴

	for (GameObject* object : renderableObjects)
	{
		MeshRenderer* meshRenderer= object->GetMeshRenderer();
		
		transformData.matWorld = object->GetTransform()->GetWorldMatrix();
		PushTransformData();

		if (object->GetAnimator() != nullptr)		// 애니메이터 component가 없는 경우
		{
			Animator* animator = object->GetAnimator();
			Animation* animation = animator->GetPresentAnimation();
			Frame frame = animation->GetFrameByIndex(animator->GetFrameNum());
			animationData.useAnimation = 1.0f;
			animationData.offset = frame.offset;
			animationData.size = frame.size;
			animationData.textureSize = animation->GetTexture()->GetSize();
			PushAnimationData();

			pipeline->SetTexture(0, animation->GetTexture());
			pipeline->SetConstantBuffer(2, animationBuffer);
		}
		else		// 애니메이터 component 있는 경우
		{
			animationData.offset = Vec2(0.f, 0.f);
			animationData.size = Vec2(0.f, 0.f);
			animationData.textureSize = Vec2(0.f, 0.f);
			animationData.useAnimation = 0.f;
			PushAnimationData();

			pipeline->SetConstantBuffer(2, animationBuffer);
			pipeline->SetTexture(0, meshRenderer->GetTexture());
		}

		// 파이프라인을 거치기 위한 정보 셋팅
		PipelineInfo pipelineInfo;
		pipelineInfo.inputLayout = meshRenderer->GetInputLayout();
		pipelineInfo.vertexShader = meshRenderer->GetVertexShader();
		pipelineInfo.pixelShader = meshRenderer->GetPixelShader();
		pipelineInfo.blendState = blendState;
		pipelineInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline->updatePipeline(pipelineInfo);

		pipeline->SetVertexBuffer(meshRenderer->GetMesh()->GetVertexBuffer());
		pipeline->SetIndexBuffer(meshRenderer->GetMesh()->GetIndexBuffer());
		
		pipeline->SetConstantBuffer(0, cameraBuffer);
		pipeline->SetConstantBuffer(1, transformBuffer);
		pipeline->SetSamplerState(0, samplerState);

		pipeline->DrawIndexed(meshRenderer->GetMesh()->GetIndexBuffer()->GetCount(), 0, 0);		// 인덱스를 통해 draw
	}


	// Collider를 그려주는 부분
	vector<GameObject*> renderableColliders = GetRenderableCollider();
	for (GameObject* object : renderableColliders)
	{
		transformData.matWorld = object->GetTransform()->GetWorldMatrix();
		PushTransformData();

		PipelineInfo pipelineInfo;

		Shader* shader = GGameManager->GetResourceManager()->Get<Shader>(L"Collider");
		pipelineInfo.inputLayout = shader->GetInputLayout();
		pipelineInfo.vertexShader = shader->GetVertexShader();
		pipelineInfo.pixelShader = shader->GetPixelShader();
		pipelineInfo.blendState = blendState;
		pipelineInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		pipeline->updatePipeline(pipelineInfo);

		pipeline->SetVertexBuffer(object->GetCollider()->GetVertexBuffer());
		pipeline->SetConstantBuffer(0, cameraBuffer);
		pipeline->SetConstantBuffer(1, transformBuffer);
		pipeline->Draw(object->GetCollider()->GetVertices().size());
	}
}

void RenderManager::SwapRender()
{
	HRESULT hr = swapChain->Present(1, 0);
	CHECK(hr);
}

void RenderManager::PushCameraData()
{
	cameraData.matView = Camera::S_MatView;
	cameraData.matProjection = Camera::S_MatProjection;
	cameraBuffer->CopyData(cameraData);
}

void RenderManager::PushTransformData()
{
	transformBuffer->CopyData(transformData);
}

void RenderManager::PushAnimationData()
{
	animationBuffer->CopyData(animationData);
}
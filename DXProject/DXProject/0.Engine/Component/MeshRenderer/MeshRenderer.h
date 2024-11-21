#pragma once
#include "Component.h"
#include "Material.h"
#include "Mesh.h"

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer();
	~MeshRenderer();

public:
	virtual void Update() override;

	void SetMesh(Mesh* _mesh) { mesh = _mesh; }
	void SetMaterial(Material* _material) { material = _material; }
	void SetShader(Shader* _shader) { material->SetShader(_shader); }
	void SetTexture(Texture* _texture) { material->SetTexture(_texture); };
	void SetRenderingOrder(uint8 num) { renderingOrder = num; }

	auto GetMesh() { return mesh; }
	auto GetMaterial() { return material; }
	auto GetVertexShader() { return GetMaterial()->GetShader()->GetVertexShader(); }
	auto GetInputLayout() { return GetMaterial()->GetShader()->GetInputLayout(); }
	auto GetPixelShader() { return GetMaterial()->GetShader()->GetPixelShader(); }
	auto GetTexture() { return GetMaterial()->GetTexture(); }
	bool GetActive() { return active; }
	uint8 GetRenderingOrder() { return renderingOrder; }

	void ActiveMeshRenderer() { active = true; }
	void InactiveMeshRenderer() { active = false; }

private:
	Mesh* mesh = nullptr;
	Material* material = nullptr;
	bool active = true;
	uint8 renderingOrder = 0;

};

struct RendererOrder  // 렌더링 순서를 지정하기 위한 펑터
{
	bool operator()(GameObject* g1, GameObject* g2)
	{
		return g1->GetMeshRenderer()->GetRenderingOrder() < g2->GetMeshRenderer()->GetRenderingOrder();
	}
};

struct VS_INPUT
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer CameraData : register(b0)
{
	row_major matrix matView;
	row_major matrix matProjection;
}

cbuffer TransformData : register(b1)
{
	row_major matrix matWorld;
}

cbuffer AnimationData : register(b2)
{
	float2 offset;
	float2 size;
	float2 textureSize;
	float useAnimation;
}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	// WVP
	float4 position = mul(input.position, matWorld); // W
	position = mul(position, matView); // V
	position = mul(position, matProjection); // P

	output.position = position;
	output.uv = input.uv;

	if (useAnimation == 1.0f)
	{
		output.uv *= size / textureSize;
		output.uv += offset / textureSize;
	}

	return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture0.Sample(sampler0, input.uv);
	float4 text = float4(color.rgb, color.a);		// 실질적인 부분만 렌더링 하기 위해 rgb값 추가

	return text;
}
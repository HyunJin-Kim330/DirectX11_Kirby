
struct VS_INPUT
{
	float4 position : POSITION;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
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

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 position = mul(input.position, matWorld);
	position = mul(position, matView);
	position = mul(position, matProjection);

	output.position = position;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = {0, 1, 1, 1};

	return color;
}
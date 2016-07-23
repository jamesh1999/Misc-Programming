
struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer rarely : register(b0)
{
	float4x4 projectionMatrix;
};

cbuffer perObject : register(b1)
{
	float4x4 objWorldTransform;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = mul(mul(position, objWorldTransform), projectionMatrix);
	output.color = color;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}
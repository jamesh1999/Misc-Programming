Texture2D shaderTexture : register(t0);
SamplerState samplerState : register(s0);

struct VOut
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 viewDir : TEXCOORD1;
    float2 tex : TEXCOORD0;
};

cbuffer rarely : register(b0)
{
	float4x4 objWorldTransform;
	float4x4 projectionMatrix;
};

VOut VShader(float4 position : POSITION, float3 color : NORMAL, float2 tex : TEXCOORD0)
{
    VOut output;

    position.w = 1.0;
    output.position = mul(mul(position, objWorldTransform), projectionMatrix);
    output.normal = normalize(mul(float4(color, 0.0), objWorldTransform));   
    output.tex = tex;
    output.viewDir = normalize(-mul(position, objWorldTransform));

    return output;
}

float4 PShader(float4 position : SV_POSITION, float3 norm : NORMAL, float3 view : TEXCOORD1, float2 tex : TEXCOORD0) : SV_TARGET
{
	float3 lightDir = normalize(float3(0.0, 0.0, 1.0));
	float intensity = dot(-lightDir, norm);
	float4 c = float4(0.1, 0.1, 0.1, 1.0);
	float4 specular = float4(0.0,0.0,0.0,0.0);

	if(intensity > 0.0)
	{
		c += intensity * float4(0.5, 0.5, 0.5, 1.0);

        float3 reflection = normalize(lightDir + 2 * intensity * norm); 
        specular = pow(saturate(dot(reflection, view)), 12.0) * 1.2;
	}

	float4 texCol = shaderTexture.Sample(samplerState, tex);

	return texCol * c + specular;
}
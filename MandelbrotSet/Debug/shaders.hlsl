cbuffer main : register(b0)
{
	int sf;
}

struct VOut
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

VOut VSMain(float2 pos : POSITION, float2 tex : TEXCOORD)
{
	VOut output;
	output.pos = float4(pos, 0.0, 1.0);
	output.tex = tex;

	return output;
}

float4 PSMain(VOut input) : SV_TARGET
{
	float2 z = float2(0.0, 0.0);
	float4 col = float4(1.0, 1.0, 1.0, 1.0);
	for(uint i = 0; i < 1024; ++i)
	{
		z = float2((z.x * ldexp(z.x, -sf)) - (z.y * ldexp(z.y, -sf)) + input.tex.x, 2 * (z.x * ldexp(z.y, -sf)) + input.tex.y);

		if((z.x * ldexp(z.x, -sf)) + (z.y * ldexp(z.y, -sf)) / 4 < exp2(sf))
			if(i == 1023)
				col = float4(0.0, 0.0, 0.0, 1.0);
			else
				col = float4(1 - pow((i + 1) / 1025.0, 0.4), 1 - 0.7 * pow((i + 1) / 1025.0, 0.4), 1.0, 0.0);
	}

	return col;
}
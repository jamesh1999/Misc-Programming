struct VOut
{
	float4 pos : SV_POSITION;
	float2 tex : TEXOORD;
};

cbuffer rect : register(b0)
{
	int sf;
}

VOut VSMain(float2 pos : POSITION, float2 tex : TEXCOORD)
{
	VOut output;
	output.pos = float4(pos, 0.0, 1.0);
	output.tex = tex;

	return output;
}

float4 PSMain(VOut input) : SV_TARGET
{
	float2 z = float2(0, 0);
	float4 col = float4(1.0, 1.0, 1.0, 1.0);
	for(uint i = 0; i < 400; ++i)
	{
		z = float2(
			z.x * ldexp(z.x, -sf) - z.y * ldexp(z.y, -sf),
			2 * z.x * ldexp(z.y, -sf)
			) + input.tex;

		if(ldexp(z.x * ldexp(z.x, -sf) + z.y * ldexp(z.y, -sf), -sf) < 4)
		{
			if(i == 399)
				col = float4(0.0, 0.0, 0.0, 1.0);
			else
				col = float4(1 - pow((i + 1) / 401.0, 0.4), 1 - 0.7 * pow((i + 1) / 401.0, 0.4), 1.0, 0.0);
		}
	}

	return col;
}
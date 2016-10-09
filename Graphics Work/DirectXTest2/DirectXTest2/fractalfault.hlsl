struct Fault
{
	float offset;
	float gradient;
	bool gt;
}

RWTexture2D<float> tex;
StructuredBuffer<Fault> faults;

[numthreads(16, 16, 1)]
void CS_Fault()
{
	
}
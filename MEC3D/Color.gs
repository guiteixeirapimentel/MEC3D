struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

[maxvertexcount(5)]
void ColorGS(point PixelInputType input[1], inout LineStream<PixelInputType> OutputStream)
{
    //for(int i = 0; i < 3; i++)
    //{
    //   
    //}

	 OutputStream.Append(input[0]);

	 input[0].position.x += 0.01f;
	 OutputStream.Append(input[0]);

	 input[0].position.y += 0.01f;
	 input[0].position.x -= 0.01f;
	 OutputStream.Append(input[0]);

	 input[0].position.y -= 0.01f;
	 OutputStream.Append(input[0]);

	  input[0].position.y += 0.01f;
	 input[0].position.x -= 0.01f;
	 OutputStream.Append(input[0]);
}
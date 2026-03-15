struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

cbuffer CBMatrix : register(b0)
{
    matrix world;
}

PS_IN VSMain(VS_IN input)
{
    PS_IN output;

    output.pos = mul(input.pos, world);
    output.col = input.col;
    
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    return input.col;
}
cbuffer CBMatrix : register(b0)
{
    matrix lightWVP;
    matrix pading;
};

struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
};

VS_OUT VSMain(VS_IN input)
{
    VS_OUT o;
    o.pos = mul(input.pos, lightWVP);
    return o;
}

struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

cbuffer CBMatrix : register(b0)
{
    matrix world;
}

cbuffer CBMaterial : register(b1)
{
    int hasTexture;
    float3 padding; // выравнивание до 16 байт
};

Texture2D diffuseTexture : register(t0);
SamplerState samLinear : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output;

    output.pos = mul(input.pos, world);
    output.col = input.col;
    output.uv = input.uv;
    
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float4 baseColor = input.col;

    if (hasTexture == 1)
    {
        float4 texColor = diffuseTexture.Sample(samLinear, input.uv);
        baseColor *= texColor;
    }

    return baseColor;
}

struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;

    float3 worldPos : POSITION1;
    float3 normal : NORMAL0;
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

cbuffer CBLight : register(b2)
{
    float3 lightDir;
    float pad1;

    float3 cameraPos;
    float intensity;
};

Texture2D diffuseTexture : register(t0);
SamplerState samLinear : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output;

    float4 worldPos = mul(input.pos, world);

    output.pos = worldPos;
    output.worldPos = worldPos.xyz;

    // rotation-only transform
    float3x3 world3x3 = (float3x3) world;
    output.normal = normalize(mul(input.normal, world3x3));

    output.col = input.col;
    output.uv = input.uv;

    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float3 N = normalize(input.normal);
    float3 L = normalize(-lightDir);
    float3 V = normalize(cameraPos - input.worldPos);

    float3 ambient = 0.25;

    float diff = max(dot(N, L), 0.0);
    float3 diffuse = diff;

    float3 R = reflect(-L, N);
    float specFactor = pow(max(dot(R, V), 0.0), 32.0);
    float3 specular = specFactor;

    float4 baseColor = input.col;
    if (hasTexture == 1)
        baseColor *= diffuseTexture.Sample(samLinear, input.uv);

    float3 lighting = ambient + diffuse + specular;
    return float4(baseColor.rgb * lighting * intensity, baseColor.a);
}

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
    
    float4 lightPos : TEXCOORD1;
};

cbuffer CBMatrix : register(b0)
{
    matrix cameraWVP;
    matrix world;
}

cbuffer CBMaterial : register(b1)
{
    int hasTexture;
    float3 padding;
};

cbuffer CBLight : register(b2)
{
    float3 lightDir;
    float pad1;

    float3 cameraPos;
    float intensity;
};

cbuffer CBLightMatrix : register(b3)
{
    matrix lightWVP;
    matrix pading;
};

Texture2D diffuseTexture : register(t0);
SamplerState samLinear : register(s0);

Texture2D shadowMap : register(t1);
SamplerComparisonState shadowSampler : register(s1);

PS_IN VSMain(VS_IN input)
{
    PS_IN output;

    float4 worldPos = mul(input.pos, world);

    output.pos = mul(input.pos, cameraWVP);
    output.worldPos = worldPos.xyz;

    // rotation-only transform
    float3x3 world3x3 = (float3x3) world;
    output.normal = normalize(mul(input.normal, world3x3));
    
    output.lightPos = mul(input.pos, lightWVP);

    output.col = input.col;
    output.uv = input.uv;

    return output;
}

float CalculateShadow(float4 lightPos)
{
    float3 proj = lightPos.xyz / lightPos.w;

    proj.xy = proj.xy * 0.5f + 0.5f;
    proj.y = 1.0f - proj.y;

    if (proj.x < 0 || proj.x > 1 ||
        proj.y < 0 || proj.y > 1)
        return 1.0f;

    float shadow = shadowMap.SampleCmpLevelZero(
        shadowSampler,
        proj.xy,
        proj.z - 0.001f);

    return shadow;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float3 N = normalize(input.normal);
    float3 L = normalize(-lightDir);
    float3 V = normalize(cameraPos - input.worldPos);
    
    float shadow = CalculateShadow(input.lightPos);

    float3 ambient = 0.25;

    float diff = max(dot(N, L), 0.0);
    float3 diffuse = diff * shadow;

    float3 R = reflect(-L, N);
    float specFactor = pow(max(dot(R, V), 0.0), 32.0);
    float3 specular = specFactor * shadow;

    float4 baseColor = input.col;
    if (hasTexture == 1)
        baseColor *= diffuseTexture.Sample(samLinear, input.uv);

    float3 lighting = ambient + diffuse + specular;
    return float4(baseColor.rgb * lighting * intensity, baseColor.a);
}

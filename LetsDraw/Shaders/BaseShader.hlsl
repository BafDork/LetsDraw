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
    float4x4 world;
    float4x4 cameraWVP;
    float3 cameraPos;
    float padding1;
}

cbuffer CBMaterial : register(b1)
{
    int hasTexture;
    float3 padding2;
};

struct Light
{
    float3 position;
    float radius;

    float3 direction;
    float intensity;

    float3 color;
    float type;
};

#define MAX_LIGHTS 256

cbuffer LightData : register(b2)
{
    Light lights[MAX_LIGHTS];
    int lightCount;
    float3 padding;
};

cbuffer ShadowData : register(b3)
{
    float4x4 lightWVP;
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
    float3x3 normalMatrix = (float3x3) world;
    output.normal = normalize(mul(input.normal, normalMatrix));
    
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
    float3 V = normalize(cameraPos - input.worldPos);

    float shadow = CalculateShadow(input.lightPos);

    float3 totalLighting = 0.0f;

    for (int i = 0; i < lightCount; i++)
    {
        Light light = lights[i];

        float3 L;
        float attenuation = 1.0f;

        // Directional
        if (light.type == 0)
        {
            L = normalize(-light.direction);
        }
        // Point
        else if (light.type == 1)
        {
            float3 toLight = light.position - input.worldPos;
            float dist = length(toLight);

            L = toLight / dist;

            attenuation = saturate(1.0f - dist / light.radius);
            attenuation *= attenuation;
        }

        float3 lightColor = light.color * light.intensity * attenuation;

        // Diffuse
        float diff = max(dot(N, L), 0.0f);
        float3 diffuse = diff * lightColor;

        // Specular
        float3 R = reflect(-L, N);
        float spec = pow(max(dot(R, V), 0.0f), 32.0f);
        float3 specular = spec * lightColor;

        // Тени только для directional
        if (light.type == 0)
        {
            diffuse *= shadow;
            specular *= shadow;
        }

        totalLighting += diffuse + specular;
    }

    // Ambient
    float3 ambient = float3(0.2f, 0.2f, 0.2f);

    // Base color
    float4 baseColor = input.col;

    if (hasTexture == 1)
    {
        baseColor *= diffuseTexture.Sample(samLinear, input.uv);
    }

    float3 finalColor = baseColor.rgb * (ambient + totalLighting);

    return float4(finalColor, baseColor.a);
}

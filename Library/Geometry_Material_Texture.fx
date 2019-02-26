//グローバル
matrix g_mW;
matrix g_mWVP;
//float4 g_vLightDir= { 0,0,-1,0 };
float4 g_Ambient;
float4 g_Diffuse;
float4 g_Specular;
float3 g_vLight;
float3 g_vEye;

Texture2D g_texDecal;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//バーテックスバッファー出力構造体
struct VS_OUTPUT
{	
    float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float3 Light : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
};

//
//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
	output.Normal=mul(Norm,(float3x3)  g_mW);
	output.Pos=mul(Pos,g_mWVP);
	//ライト方向
	output.Light=g_vLight;
	//視線ベクトル
	float3 PosWorld = normalize(mul(Pos,g_mW));
    output.EyeVector = g_vEye - PosWorld;
	
	float3 Normal = normalize(output.Normal);
    float3 LightDir = normalize(output.Light);
    float3 ViewDir = normalize(output.EyeVector); 
    float4 NL = saturate(dot(Normal, LightDir)); 
    
    float3 Reflect = normalize(2 * NL * Normal - LightDir);
    float4 specular =pow(saturate(dot(Reflect, ViewDir)), 4); 

    output.Color= g_Diffuse * NL + specular;
	
	//テクスチャー座標
	output.Tex=Tex;

    return output;
}

//
//ピクセルシェーダー
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 color=g_texDecal.Sample( samLinear, input.Tex );
	color+=input.Color;

    return color;
}

//
//テクニック
//
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}


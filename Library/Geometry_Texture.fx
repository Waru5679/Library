//グローバル
matrix g_mWVP;		
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
	float2 Tex : TEXCOORD3;
};

//
//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION ,float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
		
	//位置
	output.Pos=mul(Pos,g_mWVP);

	//テクスチャー座標
	output.Tex=Tex;

    return output;
}

//
//ピクセルシェーダー
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 color = g_texDecal.Sample( samLinear, input.Tex );
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


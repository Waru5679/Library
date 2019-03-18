//�O���[�o��
matrix g_mWVP;		
Texture2D g_texDecal;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{	
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD3;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS( float4 Pos : POSITION ,float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
		
	//�ʒu
	output.Pos=mul(Pos,g_mWVP);

	//�e�N�X�`���[���W
	output.Tex=Tex;

    return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 color = g_texDecal.Sample( samLinear, input.Tex );
    return color;
}

//
//�e�N�j�b�N
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


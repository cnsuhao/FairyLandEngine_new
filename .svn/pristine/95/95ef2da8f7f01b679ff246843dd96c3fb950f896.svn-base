//============================================================================
// DefaultShader.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// DefaultShader Implementation
//----------------------------------------------------------------------------

StringPtr DefaultShader::s_ASM_PS_Gray =
	L"ps.1.1\n"
	L"tex t0\n"
	L"dp3 r0, t0, c0\n"
	L"mul r0, r0, c1\n"
	L"mov r0.a, t0\n";

StringPtr DefaultShader::s_HLSL_VS_DepthAsColor =
	L"float4x4 world_view_proj_matrix: register(c0);\n"
	L"float threshold : register(c4);\n"

	L"struct VS_OUTPUT\n"
	L"{\n"
	L"	float4 Pos: POSITION;\n"
	L"	float4 Dep: COLOR0;\n"
	L"};\n"

	L"VS_OUTPUT vs_main( float4 Pos: POSITION )\n"
	L"{\n"
	L"	VS_OUTPUT Out = (VS_OUTPUT) 0;\n"
	L"	Out.Pos = mul(world_view_proj_matrix, Pos);\n"
	L"	Out.Dep = 1.0 - Out.Pos.z / threshold;\n"
	L"	return Out;\n"
	L"}\n";

StringPtr DefaultShader::s_HLSL_PS_Silhouette =
	L"sampler2D input : register(s0);\n"
	L"float4 color : register(c0);\n"
	L"float threshold : register(c1);\n"

	L"struct PS_INPUT\n"
	L"{\n"
	L"	float2 TexCoord0 : TEXCOORD0;\n"
	L"	float2 TexCoord1 : TEXCOORD1;\n"
	L"	float2 TexCoord2 : TEXCOORD2;\n"
	L"};\n"

	L"struct PS_OUTPUT\n"
	L"{\n"
	L"	float4 Color : COLOR0;\n"
	L"};\n"

	L"PS_OUTPUT ps_main( PS_INPUT In )\n"
	L"{\n"
	L"	PS_OUTPUT Out;\n"

	//L"	// Sample neighbor pixels\n"
	//L"	float s00 = tex2D(input, float2(In.TexCoord1.x, In.TexCoord1.y)).r;\n"
	//L"	float s01 = tex2D(input, float2(In.TexCoord0.x, In.TexCoord1.y)).r;\n"
	//L"	float s02 = tex2D(input, float2(In.TexCoord2.x, In.TexCoord1.y)).r;\n"

	//L"	float s10 = tex2D(input, float2(In.TexCoord1.x, In.TexCoord0.y)).r;\n"
	//L"	float s12 = tex2D(input, float2(In.TexCoord2.x, In.TexCoord0.y)).r;\n"

	//L"	float s20 = tex2D(input, float2(In.TexCoord1.x, In.TexCoord2.y)).r;\n"
	//L"	float s21 = tex2D(input, float2(In.TexCoord0.x, In.TexCoord2.y)).r;\n"
	//L"	float s22 = tex2D(input, float2(In.TexCoord2.x, In.TexCoord2.y)).r;\n"

	//L"	float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;\n"
	//L"	float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;\n"

	//L"	float s10 = tex2D(input, float2(In.TexCoord1.x, In.TexCoord0.y)).r;\n"
	//L"	float s12 = tex2D(input, float2(In.TexCoord2.x, In.TexCoord0.y)).r;\n"
	//L"	float s01 = tex2D(input, float2(In.TexCoord0.x, In.TexCoord1.y)).r;\n"
	//L"	float s21 = tex2D(input, float2(In.TexCoord0.x, In.TexCoord2.y)).r;\n"

	//L"	float sobelX = s10 - s12;\n"
	//L"	float sobelY = s01 - s21;\n"

	L"	float s00 = tex2D(input, float2(In.TexCoord1.x, In.TexCoord1.y)).r;\n"
	L"	float s01 = tex2D(input, float2(In.TexCoord0.x, In.TexCoord1.y)).r;\n"
	L"	float s10 = tex2D(input, float2(In.TexCoord1.x, In.TexCoord0.y)).r;\n"
	L"	float s11 = tex2D(input, float2(In.TexCoord0.x, In.TexCoord0.y)).r;\n"

	L"	float sobelX = s11 - s00;\n"
	L"	float sobelY = s10 - s01;\n"

	L"	// Find edge, skip sqrt() to improve performance ...\n"
	L"	float edgeSqr = (sobelX * sobelX + sobelY * sobelY);\n"

	L"	// ... and threshold against a squared value instead.\n"
	L"	Out.Color = (edgeSqr > threshold) * color;\n"

	L"	return Out;\n"
	L"}\n";

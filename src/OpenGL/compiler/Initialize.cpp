//
// Copyright (c) 2002-2013 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

//
// Create strings that declare built-in definitions, add built-ins that
// cannot be expressed in the files, and establish mappings between 
// built-in functions and operators.
//

#include "Initialize.h"

#include "intermediate.h"

void InsertBuiltInFunctions(GLenum type, const ShBuiltInResources &resources, TSymbolTable &symbolTable)
{
	TType *float1 = new TType(EbtFloat);
	TType *float2 = new TType(EbtFloat, 2);
	TType *float3 = new TType(EbtFloat, 3);
	TType *float4 = new TType(EbtFloat, 4);
	TType *genType = new TType(EbtGenType);

    TType *int1 = new TType(EbtInt);
    TType *uint1 = new TType(EbtUInt);
    TType *genIType = new TType(EbtGenIType);
    TType *genUType = new TType(EbtGenUType);
    TType *genBType = new TType(EbtGenBType);

    //
    // Angle and Trigonometric Functions.
    //
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpRadians, genType, "radians", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpDegrees, genType, "degrees", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpSin, genType, "sin", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpCos, genType, "cos", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpTan, genType, "tan", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpAsin, genType, "asin", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpAcos, genType, "acos", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpAtan, genType, "atan", genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpAtan, genType, "atan", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpSinh, genType, "sinh", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpCosh, genType, "cosh", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTanh, genType, "tanh", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpAsinh, genType, "asinh", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpAcosh, genType, "acosh", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpAtanh, genType, "atanh", genType);

    //
    // Exponential Functions.
    //
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpPow, genType, "pow", genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpExp, genType, "exp", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpLog, genType, "log", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpExp2, genType, "exp2", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpLog2, genType, "log2", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpSqrt, genType, "sqrt", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpInverseSqrt, genType, "inversesqrt", genType);

    //
    // Common Functions.
    //
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpAbs, genType, "abs", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpAbs, genIType, "abs", genIType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpSign, genType, "sign", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpSign, genIType, "sign", genIType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpFloor, genType, "floor", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTrunc, genType, "trunc", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpRound, genType, "round", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpRoundEven, genType, "roundEven", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpCeil, genType, "ceil", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpFract, genType, "fract", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMod, genType, "mod", genType, float1);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMod, genType, "mod", genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMin, genType, "min", genType, float1);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMin, genType, "min", genType, genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMin, genIType, "min", genIType, genIType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMin, genIType, "min", genIType, int1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMin, genUType, "min", genUType, genUType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMin, genUType, "min", genUType, uint1);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMax, genType, "max", genType, float1);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMax, genType, "max", genType, genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMax, genIType, "max", genIType, genIType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMax, genIType, "max", genIType, int1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMax, genUType, "max", genUType, genUType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMax, genUType, "max", genUType, uint1);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpClamp, genType, "clamp", genType, float1, float1);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpClamp, genType, "clamp", genType, genType, genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpClamp, genIType, "clamp", genIType, int1, int1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpClamp, genIType, "clamp", genIType, genIType, genIType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpClamp, genUType, "clamp", genUType, uint1, uint1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpClamp, genUType, "clamp", genUType, genUType, genUType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMix, genType, "mix", genType, genType, float1);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMix, genType, "mix", genType, genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpStep, genType, "step", genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpStep, genType, "step", float1, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpSmoothStep, genType, "smoothstep", genType, genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpSmoothStep, genType, "smoothstep", float1, float1, genType);

    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpIsNan, genBType, "isnan", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpIsInf, genBType, "isinf", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpFloatBitsToInt, genIType, "floatBitsToInt", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpFloatBitsToUint, genUType, "floatBitsToUint", genType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpIntBitsToFloat, genType, "intBitsToFloat", genIType);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpUintBitsToFloat, genType, "uintBitsToFloat", genUType);

    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpPackSnorm2x16, uint1, "packSnorm2x16", float2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpPackUnorm2x16, uint1, "packUnorm2x16", float2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpPackHalf2x16, uint1, "packHalf2x16", float2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpUnpackSnorm2x16, float2, "unpackSnorm2x16", uint1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpUnpackUnorm2x16, float2, "unpackUnorm2x16", uint1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpUnpackHalf2x16, float2, "unpackHalf2x16", uint1);

    //
    // Geometric Functions.
    //
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpLength, float1, "length", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpDistance, float1, "distance", genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpDot, float1, "dot", genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpCross, float3, "cross", float3, float3);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpNormalize, genType, "normalize", genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpFaceForward, genType, "faceforward", genType, genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpReflect, genType, "reflect", genType, genType);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpRefract, genType, "refract", genType, genType, float1);

	TType *mat2 = new TType(EbtFloat, 2, 2);
	TType *mat2x3 = new TType(EbtFloat, 2, 3);
	TType *mat2x4 = new TType(EbtFloat, 2, 4);
	TType *mat3x2 = new TType(EbtFloat, 3, 2);
	TType *mat3 = new TType(EbtFloat, 3, 3);
	TType *mat3x4 = new TType(EbtFloat, 3, 4);
	TType *mat4x2 = new TType(EbtFloat, 4, 2);
	TType *mat4x3 = new TType(EbtFloat, 4, 3);
	TType *mat4 = new TType(EbtFloat, 4, 4);

    //
    // Matrix Functions.
    //
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMul, mat2, "matrixCompMult", mat2, mat2);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMul, mat3, "matrixCompMult", mat3, mat3);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpMul, mat4, "matrixCompMult", mat4, mat4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMul, mat2x3, "matrixCompMult", mat2x3, mat2x3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMul, mat3x2, "matrixCompMult", mat3x2, mat3x2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMul, mat2x4, "matrixCompMult", mat2x4, mat2x4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMul, mat4x2, "matrixCompMult", mat4x2, mat4x2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMul, mat3x4, "matrixCompMult", mat3x4, mat3x4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpMul, mat4x3, "matrixCompMult", mat4x3, mat4x3);

    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat2, "outerProduct", float2, float2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat3, "outerProduct", float3, float3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat4, "outerProduct", float4, float4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat2x3, "outerProduct", float3, float2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat3x2, "outerProduct", float2, float3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat2x4, "outerProduct", float4, float2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat4x2, "outerProduct", float2, float4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat3x4, "outerProduct", float4, float3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpOuterProduct, mat4x3, "outerProduct", float3, float4);

    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat2, "transpose", mat2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat3, "transpose", mat3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat4, "transpose", mat4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat2x3, "transpose", mat3x2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat3x2, "transpose", mat2x3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat2x4, "transpose", mat4x2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat4x2, "transpose", mat2x4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat3x4, "transpose", mat4x3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpTranspose, mat4x3, "transpose", mat3x4);

    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpDeterminant, float1, "determinant", mat2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpDeterminant, float1, "determinant", mat3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpDeterminant, float1, "determinant", mat4);

    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpInverse, mat2, "inverse", mat2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpInverse, mat3, "inverse", mat3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpInverse, mat4, "inverse", mat4);


	TType *bool1 = new TType(EbtBool);
	TType *vec = new TType(EbtVec);
	TType *ivec = new TType(EbtIVec);
	TType *uvec = new TType(EbtUVec);
	TType *bvec = new TType(EbtBVec);

    //
    // Vector relational functions.
    //
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpLessThan, bvec, "lessThan", vec, vec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpLessThan, bvec, "lessThan", ivec, ivec);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpLessThan, bvec, "lessThan", uvec, uvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpLessThanEqual, bvec, "lessThanEqual", vec, vec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpLessThanEqual, bvec, "lessThanEqual", ivec, ivec);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpLessThanEqual, bvec, "lessThanEqual", uvec, uvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpGreaterThan, bvec, "greaterThan", vec, vec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpGreaterThan, bvec, "greaterThan", ivec, ivec);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpGreaterThan, bvec, "greaterThan", uvec, uvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpGreaterThanEqual, bvec, "greaterThanEqual", vec, vec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpGreaterThanEqual, bvec, "greaterThanEqual", ivec, ivec);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpGreaterThanEqual, bvec, "greaterThanEqual", uvec, uvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpVectorEqual, bvec, "equal", vec, vec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpVectorEqual, bvec, "equal", ivec, ivec);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpVectorEqual, bvec, "equal", uvec, uvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpVectorEqual, bvec, "equal", bvec, bvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpVectorNotEqual, bvec, "notEqual", vec, vec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpVectorNotEqual, bvec, "notEqual", ivec, ivec);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, EOpVectorNotEqual, bvec, "notEqual", uvec, uvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpVectorNotEqual, bvec, "notEqual", bvec, bvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpAny, bool1, "any", bvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpAll, bool1, "all", bvec);
    symbolTable.insertBuiltIn(COMMON_BUILTINS, EOpVectorLogicalNot, bvec, "not", bvec);

	TType *sampler2D = new TType(EbtSampler2D);
	TType *samplerCube = new TType(EbtSamplerCube);
	TType *sampler3D = new TType(EbtSampler3D);

    //
    // Texture Functions for GLSL ES 1.0
    //
    symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2D", sampler2D, float2);
    symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProj", sampler2D, float3);
    symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProj", sampler2D, float4);
    symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "textureCube", samplerCube, float3);
	symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture3D", sampler3D, float3);

	if(resources.OES_EGL_image_external)
    {
        TType *samplerExternalOES = new TType(EbtSamplerExternalOES);

        symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2D", samplerExternalOES, float2);
        symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProj", samplerExternalOES, float3);
        symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProj", samplerExternalOES, float4);
		symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture3D", samplerExternalOES, float3);
    }

	if(type == GL_FRAGMENT_SHADER)
	{
        symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2D", sampler2D, float2, float1);
        symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProj", sampler2D, float3, float1);
        symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProj", sampler2D, float4, float1);
        symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "textureCube", samplerCube, float3, float1);

		if(resources.OES_standard_derivatives)
		{
			symbolTable.insertBuiltIn(ESSL1_BUILTINS, EOpDFdx, "GL_OES_standard_derivatives", genType, "dFdx", genType);
			symbolTable.insertBuiltIn(ESSL1_BUILTINS, EOpDFdy, "GL_OES_standard_derivatives", genType, "dFdy", genType);
			symbolTable.insertBuiltIn(ESSL1_BUILTINS, EOpFwidth,"GL_OES_standard_derivatives", genType, "fwidth", genType);
		}
	}

	if(type == GL_VERTEX_SHADER)
	{
		symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DLod", sampler2D, float2, float1);
		symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProjLod", sampler2D, float3, float1);
		symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture2DProjLod", sampler2D, float4, float1);
		symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "textureCubeLod", samplerCube, float3, float1);
		symbolTable.insertBuiltIn(ESSL1_BUILTINS, float4, "texture3DLod", sampler3D, float3, float1);
	}

    TType *gvec4 = new TType(EbtGVec4);

    TType *gsampler2D = new TType(EbtGSampler2D);
    TType *gsamplerCube = new TType(EbtGSamplerCube);
    TType *gsampler3D = new TType(EbtGSampler3D);
    TType *gsampler2DArray = new TType(EbtGSampler2DArray);

    //
    // Texture Functions for GLSL ES 3.0
    //
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsampler2D, float2);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsampler3D, float3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsamplerCube, float3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsampler2DArray, float3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureProj", gsampler2D, float3);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureProj", gsampler2D, float4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureProj", gsampler3D, float4);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureLod", gsampler2D, float2, float1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureLod", gsampler3D, float3, float1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureLod", gsamplerCube, float3, float1);
    symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureLod", gsampler2DArray, float3, float1);

    if(type == GL_FRAGMENT_SHADER)
    {
        symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsampler2D, float2, float1);
        symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsampler3D, float3, float1);
        symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsamplerCube, float3, float1);
        symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "texture", gsampler2DArray, float3, float1);
        symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureProj", gsampler2D, float3, float1);
        symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureProj", gsampler2D, float4, float1);
        symbolTable.insertBuiltIn(ESSL3_BUILTINS, gvec4, "textureProj", gsampler3D, float4, float1);
    }

    //
    // Depth range in window coordinates
    //
	TTypeList *members = NewPoolTTypeList();
	TTypeLine near = {new TType(EbtFloat, EbpHigh, EvqGlobal, 1), 0};
	TTypeLine far = {new TType(EbtFloat, EbpHigh, EvqGlobal, 1), 0};
	TTypeLine diff = {new TType(EbtFloat, EbpHigh, EvqGlobal, 1), 0};
	near.type->setFieldName("near");
	far.type->setFieldName("far");
	diff.type->setFieldName("diff");
	members->push_back(near);
	members->push_back(far);
	members->push_back(diff);
	TVariable *depthRangeParameters = new TVariable(NewPoolTString("gl_DepthRangeParameters"), TType(members, "gl_DepthRangeParameters"), true);
	symbolTable.insert(COMMON_BUILTINS, *depthRangeParameters);
	TVariable *depthRange = new TVariable(NewPoolTString("gl_DepthRange"), TType(members, "gl_DepthRangeParameters"));
	depthRange->setQualifier(EvqUniform);
	symbolTable.insert(COMMON_BUILTINS, *depthRange);

    //
    // Implementation dependent built-in constants.
    //
    symbolTable.insertConstInt(COMMON_BUILTINS, "gl_MaxVertexAttribs", resources.MaxVertexAttribs);
    symbolTable.insertConstInt(COMMON_BUILTINS, "gl_MaxVertexUniformVectors", resources.MaxVertexUniformVectors);
    symbolTable.insertConstInt(COMMON_BUILTINS, "gl_MaxVertexTextureImageUnits", resources.MaxVertexTextureImageUnits);
    symbolTable.insertConstInt(COMMON_BUILTINS, "gl_MaxCombinedTextureImageUnits", resources.MaxCombinedTextureImageUnits);
    symbolTable.insertConstInt(COMMON_BUILTINS, "gl_MaxTextureImageUnits", resources.MaxTextureImageUnits);
    symbolTable.insertConstInt(COMMON_BUILTINS, "gl_MaxFragmentUniformVectors", resources.MaxFragmentUniformVectors);
    symbolTable.insertConstInt(ESSL1_BUILTINS, "gl_MaxVaryingVectors", resources.MaxVaryingVectors);
    symbolTable.insertConstInt(ESSL1_BUILTINS, "gl_MaxDrawBuffers", resources.MaxDrawBuffers);
    symbolTable.insertConstInt(ESSL3_BUILTINS, "gl_MaxVertexOutputVectors", resources.MaxVertexOutputVectors);
    symbolTable.insertConstInt(ESSL3_BUILTINS, "gl_MaxFragmentInputVectors", resources.MaxFragmentInputVectors);
    symbolTable.insertConstInt(ESSL3_BUILTINS, "gl_MinProgramTexelOffset", resources.MinProgramTexelOffset);
    symbolTable.insertConstInt(ESSL3_BUILTINS, "gl_MaxProgramTexelOffset", resources.MaxProgramTexelOffset);
}

void IdentifyBuiltIns(GLenum shaderType,
                      const ShBuiltInResources &resources,
                      TSymbolTable &symbolTable)
{
    //
    // First, insert some special built-in variables that are not in 
    // the built-in header files.
    //
    switch(shaderType)
	{
    case GL_FRAGMENT_SHADER:
        symbolTable.insert(COMMON_BUILTINS, *new TVariable(NewPoolTString("gl_FragCoord"), TType(EbtFloat, EbpMedium, EvqFragCoord,   4)));
        symbolTable.insert(COMMON_BUILTINS, *new TVariable(NewPoolTString("gl_FrontFacing"), TType(EbtBool,  EbpUndefined, EvqFrontFacing, 1)));
        symbolTable.insert(COMMON_BUILTINS, *new TVariable(NewPoolTString("gl_PointCoord"), TType(EbtFloat, EbpMedium, EvqPointCoord,  2)));
        symbolTable.insert(ESSL1_BUILTINS, *new TVariable(NewPoolTString("gl_FragColor"), TType(EbtFloat, EbpMedium, EvqFragColor,   4)));
        symbolTable.insert(ESSL1_BUILTINS, *new TVariable(NewPoolTString("gl_FragData[gl_MaxDrawBuffers]"), TType(EbtFloat, EbpMedium, EvqFragData,    4)));
        break;
    case GL_VERTEX_SHADER:
        symbolTable.insert(COMMON_BUILTINS, *new TVariable(NewPoolTString("gl_Position"), TType(EbtFloat, EbpHigh, EvqPosition,    4)));
        symbolTable.insert(COMMON_BUILTINS, *new TVariable(NewPoolTString("gl_PointSize"), TType(EbtFloat, EbpMedium, EvqPointSize,   1)));
        symbolTable.insert(ESSL3_BUILTINS, *new TVariable(NewPoolTString("gl_InstanceID"), TType(EbtInt, EbpHigh, EvqInstanceID, 1)));
        break;
    default: assert(false && "Language not supported");
    }

    // Finally add resource-specific variables.
    switch(shaderType)
    {
    case GL_FRAGMENT_SHADER:
		{
            // Set up gl_FragData.  The array size.
            TType fragData(EbtFloat, EbpMedium, EvqFragData, 4, 1, true);
            fragData.setArraySize(resources.MaxDrawBuffers);
            symbolTable.insert(ESSL1_BUILTINS, *new TVariable(NewPoolTString("gl_FragData"), fragData));
		}
		break;
    default: break;
    }
}

void InitExtensionBehavior(const ShBuiltInResources& resources,
                           TExtensionBehavior& extBehavior)
{
    if(resources.OES_standard_derivatives)
        extBehavior["GL_OES_standard_derivatives"] = EBhUndefined;
	if(resources.OES_fragment_precision_high)
        extBehavior["GL_FRAGMENT_PRECISION_HIGH"] = EBhUndefined;
    if(resources.OES_EGL_image_external)
        extBehavior["GL_OES_EGL_image_external"] = EBhUndefined;
}

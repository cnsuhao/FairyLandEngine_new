//============================================================================
// FairyLandGraphics.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

#pragma warning( disable : 4819 )

#define _WIN32_WINNT 0x0500

#include "windows.h"
#include "gdiplus.h"
#include "d3d9.h"
#include "d3dx9.h"

#include "gl.h"
#include "glext.h"
#include "glu.h"

#define FREEIMAGE_LIB
#include "freeimage.h"

#include "FairyLand.h"

#include "Graphics/DefaultShader.h"

#include "Graphics/VertexElement.h"
#include "Graphics/RenderAPI.h"
#include "Graphics/RenderAPID3D9.h"
#include "Graphics/RenderAPIOpenGL.h"

#include "Graphics/VertexDeclaration.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/GeometryData.h"
#include "Graphics/GeometryFactory.h"

#include "Graphics/Surface.h"
#include "Graphics/Texture.h"

#include "Graphics/FontTexture.h"
#include "Graphics/TextureFactory.h"

#include "Graphics/ShaderConst.h"
#include "Graphics/RenderMethod.h"
#include "Graphics/SamplerMethod.h"
#include "Graphics/TextureMethod.h"
#include "Graphics/TechniqueUnit.h"
#include "Graphics/TechniqueFactory.h"

#include "Graphics/Animation.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshAnimation.h"
#include "Graphics/VertexAnimation.h"
#include "Graphics/Skeleton.h"
#include "Graphics/SkeletonAnimation.h"
#include "Graphics/ModelFactory.h"

#include "Graphics/Particle.h"
#include "Graphics/ParticleEmitter.h"
#include "Graphics/ParticleGravity.h"
#include "Graphics/ParticleUpdater.h"
#include "Graphics/ParticleBlocker.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/ParticlePlayer.h"
#include "Graphics/ParticleFactory.h"

#include "Graphics/TagPoint.h"
#include "Graphics/PhysicsFactory.h"

#include "Graphics/RenderScene.h"
#include "Graphics/FrustumScene.h"
#include "Graphics/SceneFactory.h"

#include "Graphics/OverlayClipper.h"
#include "Graphics/SurfaceProcessor.h"

#include "Graphics/DeviceToken.h"
#include "Graphics/RenderToken.h"
#include "Graphics/RenderUnit.h"
#include "Graphics/RenderSet.h"
#include "Graphics/RenderLayer.h"
#include "Graphics/RenderQueue.h"

#include "Graphics/RenderDevice.h"
#include "Graphics/RenderConfig.h"
#include "Graphics/RenderStatistics.h"
#include "Graphics/GraphicsManager.h"

#pragma comment( lib, "gdiplus" )
#pragma comment( lib, "d3dx9" )
#pragma comment( lib, "opengl32" )
#pragma comment( lib, "glu32" )

using namespace FL;
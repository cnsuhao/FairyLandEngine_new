#pragma once
#pragma warning( disable : 4288 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4748 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include "FairyLandConfig.h"

#include "Common/Typedef.h"
#include "Common/Diagnose.h"
#include "Common/Interlocked.h"
#include "Common/Lock.h"

#include "Common/Singleton.h"
#include "Common/ReferencedObject.h"
#include "Common/AsyncManager.h"
#include "Common/StringPtr.h"
#include "Common/String.h"
#include "Common/DataType.h"
#include "Common/TypeChecker.h"

#include "Common/Pair.h"
#include "Common/Triple.h"
#include "Common/Quadruple.h"
#include "Common/Array.h"
#include "Common/Stack.h"
#include "Common/Queue.h"
#include "Common/Link.h"
#include "Common/List.h"
#include "Common/Pool.h"
#include "Common/Heap.h"
#include "Common/Hash.h"
#include "Common/StringHash.h"
#include "Common/Tree.h"
#include "Common/FixedTree.h"
#include "Common/Dictionary.h"
#include "Common/FixedAllocater.h"
#include "Common/DataOwner.h"

#include "Common/CallStack.h"
#include "Common/Exception.h"
#include "Common/Memory.h"
#include "Common/Allocater.h"
#include "Common/RawArray.h"
#include "Common/RawString.h"
#include "Common/SSEHelper.h"

#include "Common/QuickSort.h"
#include "Common/BinarySearch.h"

#include "Common/Math.h"
#include "Common/Time.h"
#include "Common/Address.h"
#include "Common/Color.h"
#include "Common/Point.h"
#include "Common/Rect.h"
#include "Common/Vector2.h"
#include "Common/Vector3.h"
#include "Common/Vector4.h"
#include "Common/Quaternion.h"
#include "Common/EulerAngles.h"
#include "Common/Matrix2.h"
#include "Common/Matrix3.h"
#include "Common/Matrix4.h"
#include "Common/Matrix3x4.h"
#include "Common/Matrix4Adjustor.h"
#include "Common/Vector3SSE.h"
#include "Common/Vector4SSE.h"
#include "Common/Matrix3SSE.h"
#include "Common/Matrix4SSE.h"

#include "Common/Region.h"
#include "Common/Triangle.h"
#include "Common/Plane.h"
#include "Common/AxisAlignedBox.h"
#include "Common/Box.h"
#include "Common/Sphere.h"
#include "Common/Ray.h"
#include "Common/Frustum.h"
#include "Common/Camera.h"
#include "Common/Viewport.h"
#include "Common/Intersection.h"

#include "Common/KernelObject.h"
#include "Common/SyncObject.h"
#include "Common/CompletionPort.h"
#include "Common/System.h"
#include "Common/Thread.h"
#include "Common/Pipe.h"
#include "Common/File.h"
#include "Common/FileMapping.h"
#include "Common/MemFile.h"
#include "Common/BinFile.h"
#include "Common/TxtFile.h"
#include "Common/CfgFile.h"
#include "Common/IniFile.h"
#include "Common/FileSystem.h"
#include "Common/FileFinder.h"
#include "Common/Library.h"
#include "Common/Console.h"
#include "Common/Registry.h"
#include "Common/Clipboard.h"
#include "Common/MainWindow.h"
#include "Common/ViewWindow.h"
#include "Common/MonitorWindow.h"

#include "Common/Global.h"
#include "Common/Script.h"
#include "Common/Variable.h"
#include "Common/Monitor.h"

#include "Common/Model.h"
#include "Common/ModelHelper.h"
#include "Common/ModelLoader.h"
#include "Common/ModelSaver.h"

#include "Common/TxtParser.h"
#include "Common/HyperTextParser.h"
#include "Common/RegularExpression.h"
#include "Common/StringFormatter.h"

#include "Common/TinyVariable.h"
#include "Common/Expression.h"

#include "Common/Language.h"
#include "Common/QueryTable.h"
#include "Common/Encryption.h"
#include "Common/Compression.h"
#include "Common/IFairyLandWorker.h"
#ifdef _WIN32
#include "Common/FairyLandWin32Worker.h"
#endif

#include "Storage/IDataStream.h"
#include "Storage/IDataArchive.h"
#include "Storage/IFileDownloader.h"
#include "Storage/IResourceCache.h"
#include "Storage/IResourceManager.h"
#include "Storage/IResourceDownloader.h"
#include "Storage/IResourceReplacer.h"

#include "Graphics/IRenderable.h"

#include "Graphics/IVertexDeclaration.h"
#include "Graphics/IVertexBuffer.h"
#include "Graphics/IIndexBuffer.h"
#include "Graphics/IGeometryFactory.h"

#include "Graphics/ISurface.h"
#include "Graphics/ITexture.h"
#include "Graphics/IFontTexture.h"
#include "Graphics/ITextureFactory.h"

#include "Graphics/IRenderSet.h"
#include "Graphics/IShaderConst.h"
#include "Graphics/IRenderMethod.h"
#include "Graphics/ISamplerMethod.h"
#include "Graphics/ITextureMethod.h"
#include "Graphics/ITechniqueFactory.h"

#include "Graphics/IAnimation.h"
#include "Graphics/IMesh.h"
#include "Graphics/IMeshAnimation.h"
#include "Graphics/IVertexAnimation.h"
#include "Graphics/ISkeleton.h"
#include "Graphics/ISkeletonAnimation.h"
#include "Graphics/IModelFactory.h"

#include "Graphics/IParticleSystem.h"
#include "Graphics/IParticlePlayer.h"
#include "Graphics/IParticleFactory.h"

#include "Graphics/ITagPoint.h"
#include "Graphics/IPhysicsFactory.h"

#include "Graphics/IRenderScene.h"
#include "Graphics/ISceneFactory.h"

#include "Graphics/IRenderDevice.h"
#include "Graphics/IRenderConfig.h"
#include "Graphics/IRenderStatistics.h"
#include "Graphics/IGraphicsManager.h"

#include "Sound/ISoundBuffer.h"
#include "Sound/ISoundChannel.h"
#include "Sound/ISoundDevice.h"

#include "Network/ITCPSocket.h"
#include "Network/IUDPSocket.h"
#include "Network/INetworkStatistics.h"
#include "Network/INetworkSession.h"
#include "Network/INetworkManager.h"

#include "Database/IDatabaseRecord.h"
#include "Database/IDatabaseConnection.h"
#include "Database/IDatabaseManager.h"

//#include "Script/IScriptManager.h"

#include "FairyLandGlobal.h"
#ifndef _DEBUG
#pragma comment( lib, "FairyLandCommon" )
#else
#pragma comment( lib, "FairyLandCommon_d")
#endif

#pragma comment( linker, "/nodefaultlib:libc" )
#ifndef _DEBUG
#pragma comment( linker, "/nodefaultlib:LIBCMTD" )
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#define PS_DELETE( p )		{ delete p; p = 0; }
#define PS_DELETEARRAY( p )	{ delete[] p; p = 0; }
#define PS_RELEASE( p )		{ p->Release( ); p = 0; }

/*
#define FL_APP(cls) INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return startApp<cls>(); }
template <class AppClass>
int startApp()
{
	try
	{
		AppClass* app = AppClass::GetSingletonPtr( );
		app->prepare();
		if (app->create())
		{
			app->run();
			app->destroy();
		}
		delete app;
		app = 0;
	}
	catch (FL::Exception& _e)
	{
		//MessageBoxA( NULL, _e.getFullDescription().c_str(), "An exception has occured", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		MessageBoxA( NULL, _e.getFullDescription().c_str(), "An exception has occured", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		throw;
	}
	return 0;
}


namespace FL
{
	class FairyLandApplaction:public Singleton<FairyLandApplaction>
	{
	public:
		FairyLandApplaction()
		{
			mainwindow.SetInitializeCallback( OnInitializeCallback );
			mainwindow.SetFinalizeCallback( OnFinalizeCallback );
			mainwindow.SetMessageCallback( OnMessageCallback );
			mainwindow.SetIdleCallback( OnIdleCallback );
		}
		virtual ~FairyLandApplaction(){}
		virtual void prepare()=0;
		virtual bool create()=0;
		virtual void run()=0;
		virtual void destroy()=0;
	protected:
		MainWindow mainwindow;
	};
}
*/
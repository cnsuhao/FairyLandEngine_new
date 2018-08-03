#include "FairyLandCommon.h"

namespace FL{

_bool OnInitializeCallback( )
{	
	FairyLandWin32Worker* g_pWin32Worker=(FairyLandWin32Worker*)GetFairyLandWorkerPointer();
	return g_pWin32Worker->OnInitialize();
}

_void OnFinalizeCallback( )
{
	FairyLandWin32Worker* g_pWin32Worker=(FairyLandWin32Worker*)GetFairyLandWorkerPointer();
	g_pWin32Worker->OnFinalize();
}

_bool OnMessageCallback( _dword msgid, _dword wparam, _dword lparam )
{
	FairyLandWin32Worker* g_pWin32Worker=(FairyLandWin32Worker*)GetFairyLandWorkerPointer();
	return g_pWin32Worker->OnMessage(msgid,wparam,lparam);
}

_bool OnIdleCallback( _dword elapse )
{
	if ( elapse == 0 )
		return _true;
	FairyLandWin32Worker* g_pWin32Worker=(FairyLandWin32Worker*)GetFairyLandWorkerPointer();
	return g_pWin32Worker->OnIdle(elapse);
}
_bool FairyLandWin32Worker::Create(IFairyLandApplication* pApp)
{
	m_MainWindow.SetInitializeCallback( OnInitializeCallback );
	m_MainWindow.SetFinalizeCallback( OnFinalizeCallback );
	m_MainWindow.SetMessageCallback( OnMessageCallback );
	m_MainWindow.SetIdleCallback( OnIdleCallback );
	m_pApp=(FairyLandWin32Application*)pApp;
	m_pApp->SetMainWindow(&m_MainWindow);
	return m_pApp->Create();
}

_void FairyLandWin32Worker::Run()
{
	m_MainWindow.Show( );
	m_MainWindow.Run( _false );
}

_void FairyLandWin32Worker::Close()
{
	m_MainWindow.Close( );
	
}

_bool FairyLandWin32Worker::OnInitialize()
{
	return m_pApp->OnInitialize();
}

_void FairyLandWin32Worker::OnFinalize()
{
	m_pApp->OnFinalize();
}

_bool FairyLandWin32Worker::OnMessage(_dword msgid, _dword wparam, _dword lparam)
{
	return m_pApp->OnMessage(msgid,wparam,lparam);
}

_bool FairyLandWin32Worker::OnIdle(_dword elapse)
{
	//return m_pApp->OnIdle(elapse);
	m_pApp->Update(elapse);
	m_pApp->Render();
	return _true;
}

_void FairyLandWin32Worker::Release()
{
	delete this;
}

FairyLandWin32Worker::~FairyLandWin32Worker()
{
	GetFairyLandGlobal( ).mFairyLandWorker = _null;
}

_void FairyLandWin32Application::SetMainWindow(MainWindow* pWin)
{
	m_pWindow = pWin;
}

}




IFairyLandWorker* CreateFairyLandWorker( )
{
	IFairyLandWorker* fairylandworker = GetFairyLandWorkerPointer( );

	if ( fairylandworker != _null )
		return fairylandworker;

	fairylandworker = (IFairyLandWorker*)new FairyLandWin32Worker( );
	GetFairyLandGlobal( ).mFairyLandWorker = fairylandworker;

	PS_TRACE( L"[KL] FairyLandWorker Created\r\n" )

		return fairylandworker;
}
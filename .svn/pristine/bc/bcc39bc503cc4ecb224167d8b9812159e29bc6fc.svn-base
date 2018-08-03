#pragma once
namespace FL
{
	class FairyLandWin32Application:public IFairyLandApplication
	{
	public:
		FairyLandWin32Application(){}
		virtual ~FairyLandWin32Application(){}
	public:
		virtual _bool		Create()=0;
		virtual _bool		OnInitialize()=0;
		virtual _void		OnFinalize()=0;
		virtual _bool		OnMessage( _dword msgid, _dword wparam, _dword lparam )=0;
		//virtual _bool		OnIdle( _dword elapse )=0;
		virtual _void		Update( _dword elapse )=0;
		virtual _void		Render()=0;
	protected:
		MainWindow*		m_pWindow;
	private:
		_void		SetMainWindow(MainWindow* pWin);

		friend class FairyLandWin32Worker;
	};
	class FairyLandWin32Worker:public IFairyLandWorker
	{
	public:
		FairyLandWin32Worker(){}
		virtual ~FairyLandWin32Worker();
	public:
		_bool	Create(IFairyLandApplication* pApp);
		_void	Run();	
		_void	Close();
		virtual _void   Release();

		_bool OnInitialize();
		_void OnFinalize();
		_bool OnMessage( _dword msgid, _dword wparam, _dword lparam );
		_bool OnIdle( _dword elapse );

	protected:
		MainWindow		m_MainWindow;
		FairyLandWin32Application*	m_pApp;
	};
}
#define FL_APP(appclsptr) \
	_long __stdcall WinMain( HINSTANCE instance, HINSTANCE, _chara* cmdline, _long ) \
{ \
	Trace::SetTraceTarget( Trace::_TARGET_TRACEFILE ); \
	FairyLandWin32Worker* pWorker=(FairyLandWin32Worker*)CreateFairyLandWorker(); \
	if(pWorker->Create(appclsptr)==_true) \
{ \
	pWorker->Run(); \
	pWorker->Close(); \
}	\
	FreeAllFairyLandSingleton( ); \
	return 0; \
}
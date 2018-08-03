#include "FairyLandLib.h"
#include <Windows.h>
using namespace FL;

IRenderDevice*		g_pDevice = _null;
ITextureFactory*	g_pTexFactory = _null;
IFontTexture*		g_pFontTexture = _null;
ITexture*			g_pTexture1 = _null;

IResourceCache*		g_pResCache = _null;
IResourceManager*	g_pResMgr   = _null;


class MyApp:public FairyLandWin32Application
{
public:
	MyApp(){}
	virtual ~MyApp(){}
public:
	virtual _bool	Create()
	{
		return m_pWindow->Create( L"FairyLand Render Demo", Point( 800, 600 ) );
	}
	virtual _bool		OnInitialize()
	{
		g_pResCache = CreateResourceCache( );
		FL_ASSERT( g_pResCache != _null )

			g_pResMgr = CreateResourceManager( );
		FL_ASSERT( g_pResMgr != _null )
			g_pResMgr ->InsertSearchPath( L"..\\Sample\\PSSampleRender\\Resource");
		g_pResCache->SetCacheTimeout( 1000 );

		GetResourceManagerPointer( )->OpenPEResArchive( L"", L"" );
		g_pResMgr->OpenFileArchive( L".", L"" );
		g_pResMgr->OpenFileArchive( L"Resource", L"" );
		g_pResMgr->CreateMemArchive( L"FLTemp", L"FLTemp" );

		FL_ASSERT(CreateGraphicsManager()!=NULL);

		GetGraphicsManagerPointer()->CreateRenderDevice( MainWindow::GetMainWindow( ), IRenderDevice::_DEVICE_HARDWARE_NOVSYNC );
		g_pDevice = GetRenderDevicePointer();
		FL_ASSERT( g_pDevice != _null )

			g_pTexFactory = GetTextureFactoryPointer();
		FL_ASSERT( g_pTexFactory != _null )

			g_pFontTexture		=	g_pTexFactory->CreateFontTexture( L"ËÎÌו", 12, 0, _false, _false, _false );
		g_pTexture1			=   g_pTexFactory->CreateTexture( L"1440tj1.jpg" );

		g_pFontTexture->SetFontColor( Color::cYellow );
		g_pDevice->SetCamera( Camera( Vector3( 100.0f, 100.0f, 100.0f ), Vector3::cOrigin, Vector3::cZAxis ) );



		return _true;
	}
	virtual _void		OnFinalize()
	{
		g_pTexFactory->ReleaseTexture(g_pTexture1);
		g_pTexFactory->ReleaseFontTexture( g_pFontTexture );
	}
	virtual _bool		OnMessage( _dword msgid, _dword wparam, _dword lparam )
	{
		return _true;
	}
	virtual _void Update(_dword elapse )
	{
		if ( elapse == 0 )
			return ;
		g_pResMgr->Update( elapse );
		g_pResCache->Update( elapse );
	}
	virtual _void		Render()
	{
		
		if ( g_pDevice->IsDeviceLost( ) == _true )
		{
			g_pDevice->ResetDevice( );
			return ;
		}
		Rect clientrect = m_pWindow->GetClientRect( );
		g_pDevice->ResizeDevice( clientrect.Width( ), clientrect.Height( ) );
		g_pDevice->SetProjectionTransform( Matrix4::CreatePerspectiveFovLH( Math::cPi / 4.0f,
			(_float) clientrect.Width( ) / (_float) clientrect.Height( ), 0.1f, 5000.0f ) );

		g_pDevice->SetClearColor( Color::cDarkBlue );
		g_pDevice->ClearBuffer( );

		
		Vector2 pos1(0,0);
		Vector2 pos2(800,600);

		Vector2 tex1(0,0);
		Vector2 tex2(1,1);
		FL::OverlayRectangleImage img( pos1, pos2, tex1, tex2, Color::cWhite, g_pTexture1 );
		g_pDevice->Render(img);
//		g_pDevice->Render( PrimitiveAxis( Vector3::cOrigin, 100.0f ) );
		g_pDevice->Present( );
	}
};

MyApp myApp;
FL_APP(&myApp)

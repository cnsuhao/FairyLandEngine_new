//============================================================================
// RenderAPID3D9.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

#ifdef _DIRECTX9_RENDER

//----------------------------------------------------------------------------
// RenderAPID3D9 Implementation
//----------------------------------------------------------------------------

RenderAPID3D9::RenderAPID3D9( )
{
	mRGBAFormat				= _false;

	mD3D					= _null;
	mD3DDevice				= _null;
	mDefaultSwapChain		= _null;
	mDefaultRenderTarget	= _null;
	mDefaultDepthStencil	= _null;
	mCurrentRenderTarget	= _null;
	mCurrentDepthStencil	= _null;
}

_dword RenderAPID3D9::CreateDeviceHelper( _handle window, _dword devicetouse, _dword devicetype, _bool vsync, _bool preservefpu )
{
	D3DPRESENT_PARAMETERS presentparam	= { 0 };
	presentparam.BackBufferFormat		= D3DFMT_UNKNOWN;
	presentparam.BackBufferCount		= 1;
	presentparam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	presentparam.hDeviceWindow			= (HWND) window;
	presentparam.Windowed				= _true;
	presentparam.EnableAutoDepthStencil	= _true;
	presentparam.AutoDepthStencilFormat	= D3DFMT_D24S8;
	presentparam.PresentationInterval	= vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

	_dword result = ( (IDirect3D9*) mD3D )->CreateDevice( devicetouse, (D3DDEVTYPE) devicetype, (HWND) window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | ( preservefpu ? D3DCREATE_FPU_PRESERVE : 0 ), &presentparam, (IDirect3DDevice9**) &mD3DDevice );

	if ( result != 0 )
	{
		result = ( (IDirect3D9*) mD3D )->CreateDevice( devicetouse, (D3DDEVTYPE) devicetype, (HWND) window,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | ( preservefpu ? D3DCREATE_FPU_PRESERVE : 0 ), &presentparam, (IDirect3DDevice9**) &mD3DDevice );
	}

	return result;
}

_dword RenderAPID3D9::GetAPIType( ) const
{
	return _API_D3D9;
}

_bool RenderAPID3D9::Initialize( _handle window, _dword devicetype )
{
	if ( mD3DLibrary.Load( L"D3D9.dll" ) == _false )
	{
		if ( mD3DLibrary.Load( System::GetWindowPath( ) + L"\\system32\\d3d9.dll" ) == _false )
			return _false;
	}

	typedef _void* (_stdcall *D3DCreateDevice)( _dword );

	D3DCreateDevice createdevice = (D3DCreateDevice) mD3DLibrary.GetFunction( L"Direct3DCreate9" );
	if ( createdevice == _null )
		return _false;

	if ( ( mD3D = (*createdevice)( D3D_SDK_VERSION ) ) == _null )
		return _false;

	_bool preservefpu = _false;

	if ( devicetype & IRenderDevice::_DEVICE_FPU_PRESERVE )
		preservefpu = _true;

	devicetype &= IRenderDevice::_DEVICE_MASK;

	if ( devicetype > IRenderDevice::_DEVICE_MAX )
		devicetype = IRenderDevice::_DEVICE_MAX;

	_dword devicetouse = 0, d3ddevice = D3DDEVTYPE_HAL;

	// Try to use PerfHud device.
	if ( devicetype == IRenderDevice::_DEVICE_HARDWARE_PERFHUD )
	{
		for ( _dword i = 0; i < ( (IDirect3D9*) mD3D )->GetAdapterCount( ); i ++ )
		{
			D3DADAPTER_IDENTIFIER9 adapterid;
			if ( ( (IDirect3D9*) mD3D )->GetAdapterIdentifier( i, 0, &adapterid ) != 0 )
				continue;

			_char buffer[1024];
			StringFormatter::AnsiToUnicode( buffer, 1024, adapterid.Description );

			// Found a perf hud device.
			if ( StringPtr( buffer ).SearchL2R( L"PerfHUD" ) != -1 )
			{
				devicetouse = i;
				d3ddevice   = D3DDEVTYPE_REF;
				break;
			}
		}
	}

	while ( _true )
	{
		_dword result = CreateDeviceHelper( window, devicetouse, d3ddevice,
			devicetype & IRenderDevice::_DEVICE_HARDWARE_VSYNC, preservefpu );

		// Create device succeeded.
		if ( mD3DDevice != _null )
			break;

		// Device not available, maybe locking screen, so wait and try again.
		//if ( result == D3DERR_NOTAVAILABLE )
		//{
		//	System::Sleep( 1000 );

		//	// Recreate D3D.
		//	( (IDirect3D9*) mD3D )->Release( );

		//	if ( ( mD3D = (*createdevice)( D3D_SDK_VERSION ) ) == _null )
		//		return _false;

		//	continue;
		//}

		// Create device failed.
		if ( mD3DDevice == _null )
		{
			( (IDirect3D9*) mD3D )->Release( );
			mD3D = _null;

			break;
		}
	}

	if ( mD3D == _null )
		return _false;

	PS_TRACE1( L"[RD] CreateD3DDevice At Thread %d\r\n", System::GetCurrentThreadID( ) )

	// Get swap chian.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->GetSwapChain( 0, (IDirect3DSwapChain9**) &mDefaultSwapChain ) != 0 )
		return _false;

	// Get render target surface.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->GetRenderTarget( 0, (IDirect3DSurface9**) &mDefaultRenderTarget ) != 0 )
		return _false;

	// Get depth stencil surface.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->GetDepthStencilSurface( (IDirect3DSurface9**) &mDefaultDepthStencil ) != 0 )
		return _false;

	for ( _dword i = 0; i < 8; i ++ )
	{
		( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	}

	// Disable lighting.
	( (IDirect3DDevice9*) mD3DDevice )->SetRenderState( D3DRS_LIGHTING, _false );

	Rect clientrect;
	::GetClientRect( (HWND) window, (RECT*) &clientrect );

	mDeviceType		= devicetype;
	mWindow			= window;
	mDeviceWidth	= clientrect.Width( );
	mDeviceHeight	= clientrect.Height( );

	return _true;
}

_void RenderAPID3D9::Finalize( )
{
	if ( mD3DDevice != _null )
		( (IDirect3DDevice9*) mD3DDevice )->Release( );

	if ( mD3D != _null )
		( (IDirect3D9*) mD3D )->Release( );
}

_bool RenderAPID3D9::IsDeviceLost( )
{
	if ( mD3DDevice == _null )
		return _false;

	return ( (IDirect3DDevice9*) mD3DDevice )->TestCooperativeLevel( ) != 0;
}

_bool RenderAPID3D9::ResizeDevice( _dword width, _dword height )
{
	if ( width == 0 || height == 0 )
		return _false;

	if ( mDeviceWidth == width && mDeviceHeight == height )
		return _true;

	if ( mDefaultRenderTarget != _null )
		( (IDirect3DSurface9*) mDefaultRenderTarget )->Release( );

	if ( mDefaultDepthStencil != _null )
		( (IDirect3DSurface9*) mDefaultDepthStencil )->Release( );

	if ( mDefaultSwapChain != _null )
		( (IDirect3DSwapChain9*) mDefaultSwapChain )->Release( );

	mDefaultRenderTarget	= _null;
	mDefaultDepthStencil	= _null;
	mDefaultSwapChain		= _null;

	D3DPRESENT_PARAMETERS presentparam	= { 0 };
	presentparam.BackBufferFormat		= D3DFMT_UNKNOWN;
	presentparam.BackBufferCount		= 1;
	presentparam.MultiSampleType		= D3DMULTISAMPLE_NONE;
	presentparam.MultiSampleQuality		= 0;
	presentparam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	presentparam.hDeviceWindow			= (HWND) mWindow;
	presentparam.Windowed				= _true;
	presentparam.EnableAutoDepthStencil	= _true;
	presentparam.AutoDepthStencilFormat	= D3DFMT_D24S8;
	presentparam.PresentationInterval	= mDeviceType == IRenderDevice::_DEVICE_HARDWARE_NOVSYNC ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_ONE;

	// Create swap chain.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->CreateAdditionalSwapChain( (D3DPRESENT_PARAMETERS*) &presentparam, (IDirect3DSwapChain9**) &mDefaultSwapChain ) != 0 )
		return _false;

	// Get render target surface.
	if ( ( (IDirect3DSwapChain9*) mDefaultSwapChain )->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, (IDirect3DSurface9**) &mDefaultRenderTarget ) != 0 )
		return _false;

	// Set as current render target surface.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->SetRenderTarget( 0, (IDirect3DSurface9*) mDefaultRenderTarget ) != 0 )
		return _false;

	// Create depth stencil surface.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->CreateDepthStencilSurface( width, height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, _true, (IDirect3DSurface9**) &mDefaultDepthStencil, _null ) != 0 )
		return _false;

	// Set as current depth stencil surface.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->SetDepthStencilSurface( (IDirect3DSurface9*) mDefaultDepthStencil ) != 0 )
		return _false;

	mDeviceWidth	= width;
	mDeviceHeight	= height;

	return _true;
}

_bool RenderAPID3D9::ResetDevice( )
{
	//for ( _dword i = 0; i < mRenderTargetArray.Number( ); i ++ )
	//{
	//	Texture* rendertarget = mRenderTargetArray[i].mRenderTarget;

	//	if ( rendertarget->GetD3DInterface( ) == _null )
	//		continue;

	//	rendertarget->ReleaseD3DInterface( );
	//}

	//for ( _dword i = 0; i < mDepthStencilArray.Number( ); i ++ )
	//{
	//	Surface* depthstencil = mDepthStencilArray[i].mDepthStencil;

	//	if ( depthstencil->GetD3DInterface( ) == _null )
	//		continue;

	//	( (IDirect3DSurface9*) depthstencil->GetD3DInterface( ) )->Release( );

	//	depthstencil->SetD3DInterface( _null );
	//}

	if ( mDefaultRenderTarget != _null )
		( (IDirect3DSurface9*) mDefaultRenderTarget )->Release( );

	if ( mDefaultDepthStencil != _null )
		( (IDirect3DSurface9*) mDefaultDepthStencil )->Release( );

	if ( mDefaultSwapChain != _null )
		( (IDirect3DSwapChain9*) mDefaultSwapChain )->Release( );

	mDefaultRenderTarget	= _null;
	mDefaultDepthStencil	= _null;
	mDefaultSwapChain		= _null;

	D3DPRESENT_PARAMETERS presentparam	= { 0 };
	presentparam.BackBufferFormat		= D3DFMT_UNKNOWN;
	presentparam.BackBufferCount		= 1;
	presentparam.MultiSampleType		= D3DMULTISAMPLE_NONE;
	presentparam.MultiSampleQuality		= 0;
	presentparam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	presentparam.hDeviceWindow			= (HWND) mWindow;
	presentparam.Windowed				= _true;
	presentparam.EnableAutoDepthStencil	= _true;
	presentparam.AutoDepthStencilFormat	= D3DFMT_D24S8;
	presentparam.PresentationInterval	= mDeviceType == IRenderDevice::_DEVICE_HARDWARE_VSYNC ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

	if ( ( (IDirect3DDevice9*) mD3DDevice )->Reset( (D3DPRESENT_PARAMETERS*) &presentparam ) != 0 )
		return _false;

	// Get swap chian.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->GetSwapChain( 0, (IDirect3DSwapChain9**) &mDefaultSwapChain ) != 0 )
		return _false;

	// Get render target surface.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->GetRenderTarget( 0, (IDirect3DSurface9**) &mDefaultRenderTarget ) != 0 )
		return _false;

	// Get depth stencil surface.
	if ( ( (IDirect3DDevice9*) mD3DDevice )->GetDepthStencilSurface( (IDirect3DSurface9**) &mDefaultDepthStencil ) != 0 )
		return _false;

	for ( _dword i = 0; i < 8; i ++ )
	{
		( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	}

	// Disable lighting.
	( (IDirect3DDevice9*) mD3DDevice )->SetRenderState( D3DRS_LIGHTING, _false );

	// Set transforms.
	//( (IDirect3DDevice9*) mD3DDevice )->SetTransform( D3DTS_WORLD, (const D3DMATRIX*) &Matrix4::cIdentity );
	//( (IDirect3DDevice9*) mD3DDevice )->SetTransform( D3DTS_VIEW, (const D3DMATRIX*) &mViewTransform );
	//( (IDirect3DDevice9*) mD3DDevice )->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*) &mProjectionTransform );

	// Set viewport.
	//( (IDirect3DDevice9*) mD3DDevice )->SetViewport( (D3DVIEWPORT9*) &mViewport );

	//for ( _dword i = 0; i < mRenderTargetArray.Number( ); i ++ )
	//{
	//	Texture* rendertarget = mRenderTargetArray[i].mRenderTarget;

	//	if ( rendertarget->GetD3DInterface( ) != _null )
	//		continue;

	//	_void* d3dtexture = _null;
	//	( (IDirect3DDevice9*) mD3DDevice )->CreateTexture( mRenderTargetArray[i].mWidth, mRenderTargetArray[i].mHeight,
	//		1, D3DUSAGE_RENDERTARGET, (D3DFORMAT) mRenderTargetArray[i].mFormat, D3DPOOL_DEFAULT, (IDirect3DTexture9**) &d3dtexture, _null );

	//	rendertarget->ResetD3DInterface( d3dtexture );
	//}

	//for ( _dword i = 0; i < mDepthStencilArray.Number( ); i ++ )
	//{
	//	Surface* depthstencil = mDepthStencilArray[i].mDepthStencil;

	//	if ( depthstencil->GetD3DInterface( ) != _null )
	//		continue;

	//	_void* d3dsurface = _null;
	//	( (IDirect3DDevice9*) mD3DDevice )->CreateDepthStencil( mDepthStencilArray[i].mWidth,
	//		mDepthStencilArray[i].mHeight, (D3DFORMAT) mDepthStencilArray[i].mFormat, D3DMULTISAMPLE_NONE, 0, 1, (IDirect3DSurface9**) &d3dsurface, _null );

	//	depthstencil->SetD3DInterface( d3dsurface );
	//}

	Rect clientrect;
	::GetClientRect( (HWND) mWindow, (RECT*) &clientrect );

	mDeviceWidth	= clientrect.Width( );
	mDeviceHeight	= clientrect.Height( );

	return _true;
}

_void RenderAPID3D9::ClearBuffer( _bool color, _bool depth, _bool stencil )
{
	_dword flags = 0;

	if ( color == _true )
		flags |= D3DCLEAR_TARGET;

	if ( depth == _true )
		flags |= D3DCLEAR_ZBUFFER;

	if ( stencil == _true )
		flags |= D3DCLEAR_STENCIL;

	( (IDirect3DDevice9*) mD3DDevice )->Clear( 0, _null, flags, mClearColor, mClearDepth, mClearStencil );
}

_void RenderAPID3D9::BeginScene( )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->BeginScene( );
}

_void RenderAPID3D9::EndScene( )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->EndScene( );
}

_void RenderAPID3D9::SetViewport( const Viewport& viewport )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetViewport( (D3DVIEWPORT9*) &viewport );
}

_void RenderAPID3D9::GetViewport( Viewport& viewport )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->GetViewport( (D3DVIEWPORT9*) &viewport );
}

_void RenderAPID3D9::SetWorldTransform( const Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetTransform( D3DTS_WORLD, (const D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::GetWorldTransform( Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->GetTransform( D3DTS_WORLD, (D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::SetViewTransform( const Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetTransform( D3DTS_VIEW, (const D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::GetViewTransform( Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->GetTransform( D3DTS_VIEW, (D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::SetProjectionTransform( const Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::GetProjectionTransform( Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::SetTextureTransform( _dword index, const Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetTransform( (D3DTRANSFORMSTATETYPE) ( D3DTS_TEXTURE0 + index ), (const D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::GetTextureTransform( _dword index, Matrix4& matrix )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->GetTransform( (D3DTRANSFORMSTATETYPE) ( D3DTS_TEXTURE0 + index ), (D3DMATRIX*) &matrix );
}

_void RenderAPID3D9::EnableRenderState( _dword state, _dword value )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetRenderState( (D3DRENDERSTATETYPE) state, value );
}

_void RenderAPID3D9::DisableRenderState( _dword state )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetRenderState( (D3DRENDERSTATETYPE) state,
		TechniqueFactory::GetDefaultRenderState( state ) );
}

_void RenderAPID3D9::EnableSamplerState( _dword stage, _dword state, _dword value )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( stage, (D3DSAMPLERSTATETYPE) state, value );
}

_void RenderAPID3D9::DisableSamplerState( _dword stage, _dword state )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetSamplerState( stage, (D3DSAMPLERSTATETYPE) state,
		TechniqueFactory::GetDefaultSamplerState( stage, state ) );
}

_void RenderAPID3D9::EnableTextureState( _dword stage, _dword state, _dword value )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetTextureStageState( stage, (D3DTEXTURESTAGESTATETYPE) state, value );
}

_void RenderAPID3D9::DisableTextureState( _dword stage, _dword state )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetTextureStageState( stage, (D3DTEXTURESTAGESTATETYPE) state,
		TechniqueFactory::GetDefaultTextureState( stage, state ) );
}

_void* RenderAPID3D9::CreateVertexDeclaration( _void* vertexdecl, const Array< VertexElement >& elementarray )
{
	if ( mD3DDevice == _null )
		return _null;

	if ( vertexdecl == _null )
	{
		_dword elementnumber = elementarray.Number( );
		if ( elementnumber == 0 )
			return _null;

		D3DVERTEXELEMENT9 vertexelements[ 32 ];

		if ( elementnumber >= 32 )
			elementnumber = 31;

		// Translate into d3d9 vertex elements.
		for ( _dword i = 0; i < elementnumber; i ++ )
		{
			const VertexElement& element = elementarray[i];

			_byte type = 0;

			// Translate into d3d9 vertex element type.
			switch ( element.mType )
			{
				case IVertexDeclaration::_TYPE_FLOAT1:	type = D3DDECLTYPE_FLOAT1; break;
				case IVertexDeclaration::_TYPE_FLOAT2:	type = D3DDECLTYPE_FLOAT2; break;
				case IVertexDeclaration::_TYPE_FLOAT3:	type = D3DDECLTYPE_FLOAT3; break;
				case IVertexDeclaration::_TYPE_FLOAT4:	type = D3DDECLTYPE_FLOAT4; break;
				case IVertexDeclaration::_TYPE_COLOR:	type = D3DDECLTYPE_D3DCOLOR; break;
				case IVertexDeclaration::_TYPE_SHORT2:	type = D3DDECLTYPE_SHORT2; break;
				case IVertexDeclaration::_TYPE_SHORT4:	type = D3DDECLTYPE_SHORT4; break;
				case IVertexDeclaration::_TYPE_UBYTE4:	type = D3DDECLTYPE_UBYTE4; break;
				case IVertexDeclaration::_TYPE_UNUSED:	type = D3DDECLTYPE_UNUSED; break;
			}

			_byte usage = 0, index = 0;

			// Translate into d3d9 vertex element usage.
			switch ( element.mUsage )
			{
				case IVertexDeclaration::_USAGE_POSITION:		usage = D3DDECLUSAGE_POSITION; index = (_byte) element.mIndex; break;
				case IVertexDeclaration::_USAGE_BLEND_WEIGHTS:	usage = D3DDECLUSAGE_BLENDWEIGHT; break;
				case IVertexDeclaration::_USAGE_BLEND_INDICES:	usage = D3DDECLUSAGE_BLENDINDICES; break;
				case IVertexDeclaration::_USAGE_NORMAL:			usage = D3DDECLUSAGE_NORMAL; index = (_byte) element.mIndex; break;
				case IVertexDeclaration::_USAGE_DIFFUSE:		usage = D3DDECLUSAGE_COLOR; index = 0; break;
				case IVertexDeclaration::_USAGE_SPECULAR:		usage = D3DDECLUSAGE_COLOR; index = 1; break;
				case IVertexDeclaration::_USAGE_TEXCOORD:		usage = D3DDECLUSAGE_TEXCOORD; index = (_byte) element.mIndex; break;
				case IVertexDeclaration::_USAGE_BINORMAL:		usage = D3DDECLUSAGE_BINORMAL; break;
				case IVertexDeclaration::_USAGE_TANGENT:		usage = D3DDECLUSAGE_TANGENT; break;
				case IVertexDeclaration::_USAGE_POSITIONT:		usage = D3DDECLUSAGE_POSITIONT; break;
			}

			// Set vertex element.
			vertexelements[i].Stream		= (_word) element.mSource;
			vertexelements[i].Offset		= (_word) element.mOffset;
			vertexelements[i].Type			= type;
			vertexelements[i].Method		= 0;
			vertexelements[i].Usage			= usage;
			vertexelements[i].UsageIndex	= index;
		}

		// Set the last vertex element.
		vertexelements[ elementnumber ].Stream		= 0xFF;
		vertexelements[ elementnumber ].Offset		= 0;
		vertexelements[ elementnumber ].Type		= D3DDECLTYPE_UNUSED;
		vertexelements[ elementnumber ].Method		= 0;
		vertexelements[ elementnumber ].Usage		= 0;
		vertexelements[ elementnumber ].UsageIndex	= 0;

		( (IDirect3DDevice9*) mD3DDevice )->CreateVertexDeclaration( vertexelements, (IDirect3DVertexDeclaration9**) &vertexdecl );
	}

	// Create vertex declaration object.
	if ( vertexdecl == _null )
		return _null;

	( (IDirect3DDevice9*) mD3DDevice )->SetVertexDeclaration( (IDirect3DVertexDeclaration9*) vertexdecl );

	return vertexdecl;
}

_void RenderAPID3D9::ReleaseVertexDeclaration( _void* vertexdecl )
{
	if ( vertexdecl == _null )
		return;

	( (IDirect3DVertexDeclaration9*) vertexdecl )->Release( );
}

_void* RenderAPID3D9::CreateVertexBuffer( _dword type, _dword size )
{
	if ( mD3DDevice == _null )
		return _null;

	_void* d3dbuffer = _null;

	// Without lock, D3DPOOL_MANAGED is 30% faster ( better for GPU )
	// With lock, D3DPOOL_SYSTEMMEM is 20% faster ( better for CPU )
	_dword pool = type == IGeometryFactory::_TYPE_DYNAMIC ? D3DPOOL_SYSTEMMEM : D3DPOOL_MANAGED;

	// Create d3d vertex buffer.
	( (IDirect3DDevice9*) mD3DDevice )->CreateVertexBuffer( size, 0, 0,
		(D3DPOOL) pool, (IDirect3DVertexBuffer9**) &d3dbuffer, _null );

	return d3dbuffer;
}

_void* RenderAPID3D9::LockVertexBuffer( _void* vertexbuffer, _dword offset, _dword size, _dword flag )
{
	if ( vertexbuffer == _null )
		return _null;

	_void* buffer = _null; _dword d3dflag = 0;

	if ( flag & IGeometryFactory::_FLAG_DISCARD )
		d3dflag |= D3DLOCK_DISCARD;

	if ( flag & IGeometryFactory::_FLAG_READONLY )
		d3dflag |= D3DLOCK_READONLY;

	( (IDirect3DVertexBuffer9*) vertexbuffer )->Lock( offset, size, &buffer, d3dflag );

	return buffer;
}

_void RenderAPID3D9::UnlockVertexBuffer( _void* vertexbuffer )
{
	if ( vertexbuffer == _null )
		return;

	( (IDirect3DVertexBuffer9*) vertexbuffer )->Unlock( );
}

_void RenderAPID3D9::SetVertexBuffer( _void* vertexbuffer, _dword vertexsize )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetStreamSource( 0, (IDirect3DVertexBuffer9*) vertexbuffer, 0, vertexsize );
}

_void RenderAPID3D9::ReleaseVertexBuffer( _void* vertexbuffer )
{
	if ( vertexbuffer == _null )
		return;

	( (IDirect3DVertexBuffer9*) vertexbuffer )->Release( );
}

_void* RenderAPID3D9::CreateIndexBuffer( _dword type, _dword size, _dword indexsize )
{
	if ( mD3DDevice == _null )
		return _null;

	_void* d3dbuffer = _null;

	_dword format = indexsize == sizeof( _word ) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
	_dword pool   = type == IGeometryFactory::_TYPE_DYNAMIC ? D3DPOOL_SYSTEMMEM : D3DPOOL_MANAGED;

	// Create d3d index buffer.
	( (IDirect3DDevice9*) mD3DDevice )->CreateIndexBuffer( size, 0,
		(D3DFORMAT) format, (D3DPOOL) pool, (IDirect3DIndexBuffer9**) &d3dbuffer, _null );

	return d3dbuffer;
}

_void* RenderAPID3D9::LockIndexBuffer( _void* indexbuffer, _dword offset, _dword size, _dword flag )
{
	if ( indexbuffer == _null )
		return _null;

	_void* buffer = _null; _dword d3dflag = 0;

	if ( flag & IGeometryFactory::_FLAG_DISCARD )
		d3dflag |= D3DLOCK_DISCARD;

	if ( flag & IGeometryFactory::_FLAG_READONLY )
		d3dflag |= D3DLOCK_READONLY;

	( (IDirect3DIndexBuffer9*) indexbuffer )->Lock( offset, size, &buffer, d3dflag );

	return buffer;
}

_void RenderAPID3D9::UnlockIndexBuffer( _void* indexbuffer )
{
	if ( indexbuffer == _null )
		return;

	( (IDirect3DIndexBuffer9*) indexbuffer )->Unlock( );
}

_void RenderAPID3D9::SetIndexBuffer( _void* indexbuffer, _dword indexsize )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetIndices( (IDirect3DIndexBuffer9*) indexbuffer );
}

_void RenderAPID3D9::ReleaseIndexBuffer( _void* indexbuffer )
{
	if ( indexbuffer == _null )
		return;

	( (IDirect3DIndexBuffer9*) indexbuffer )->Release( );
}

_bool RenderAPID3D9::GetTextureInfo( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip )
{
	if ( mD3DDevice == _null )
		return _false;

	// Get texture information.
	D3DXIMAGE_INFO imageinfo;
	if ( ::D3DXGetImageInfoFromFileInMemory( buffer, length, &imageinfo ) != 0 )
		return _false;

	width	= imageinfo.Width;
	height	= imageinfo.Height;
	format	= APIFormat2PSDFormat( imageinfo.Format );
	mip		= imageinfo.MipLevels;

	return _true;
}

_void* RenderAPID3D9::CreateTexture( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip )
{
	if ( mD3DDevice == _null )
		return _null;

	// Get texture information.
	D3DXIMAGE_INFO imageinfo;
	if ( ::D3DXGetImageInfoFromFileInMemory( buffer, length, &imageinfo ) != 0 )
		return _null;

	if ( format != 0 )
		imageinfo.Format = (D3DFORMAT) PSDFormat2APIFormat( format );

	// Use width as scale.
	if ( width > 0 )
	{
		// Resize texture width.
		imageinfo.Width /= width;
		if ( imageinfo.Width == 0 )
			imageinfo.Width = 1;

		// Resize texture height.
		imageinfo.Height /= width;
		if ( imageinfo.Height == 0 )
			imageinfo.Height = 1;
	}

	// Create texture from memory buffer.
	_void* d3dtexture = _null;
	::D3DXCreateTextureFromFileInMemoryEx( (IDirect3DDevice9*) mD3DDevice, buffer, length, imageinfo.Width, imageinfo.Height,
		mip, 0, imageinfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, _null, _null, (IDirect3DTexture9**) &d3dtexture );

	if ( d3dtexture != _null )
	{
		mip = ( (IDirect3DTexture9*) d3dtexture )->GetLevelCount( );

		D3DSURFACE_DESC d3dsurfacedesc;
		if ( ( (IDirect3DTexture9*) d3dtexture )->GetLevelDesc( 0, &d3dsurfacedesc ) == 0 )
		{
			width	= d3dsurfacedesc.Width;
			height	= d3dsurfacedesc.Height;
			format	= APIFormat2PSDFormat( d3dsurfacedesc.Format );
		}
	}

	return d3dtexture;
}

_void* RenderAPID3D9::CreateTexture( _dword width, _dword height, _dword format, _dword mip )
{
	if ( mD3DDevice == _null )
		return _null;

	_dword d3dformat = PSDFormat2APIFormat( format );
	if ( d3dformat == 0 || d3dformat == -1 )
		return _null;

	// Create texture by parameters.
	_void* d3dtexture = _null;
	( (IDirect3DDevice9*) mD3DDevice )->CreateTexture( width, height, mip, 0,
		(D3DFORMAT) d3dformat, D3DPOOL_MANAGED, (IDirect3DTexture9**) &d3dtexture, _null );

	return d3dtexture;
}

_void* RenderAPID3D9::CreateRenderTexture( _dword width, _dword height, _dword format )
{
	// Get d3d texture format.
	_dword d3dformat = PSDFormat2APIFormat( format );
	if ( d3dformat == 0 || d3dformat == -1 )
		return _null;

	// Create texture by parameters.
	_void* d3dtexture = _null;
	( (IDirect3DDevice9*) mD3DDevice )->CreateTexture( width, height, 1, D3DUSAGE_RENDERTARGET,
		(D3DFORMAT) d3dformat, D3DPOOL_DEFAULT, (IDirect3DTexture9**) &d3dtexture, _null );

	return d3dtexture;
}

_void* RenderAPID3D9::CreateDepthTexture( _dword width, _dword height )
{
	if ( mD3DDevice == _null )
		return _null;

	// Create texture by parameters.
	_void* d3dtexture = _null;
	( (IDirect3DDevice9*) mD3DDevice )->CreateTexture( width, height, 1, D3DUSAGE_DEPTHSTENCIL,
		D3DFMT_D24S8, D3DPOOL_DEFAULT, (IDirect3DTexture9**) &d3dtexture, _null );

	return d3dtexture;
}

_void* RenderAPID3D9::LockTexture( _void* texture, _dword format, _dword level, _dword& pitch, const Rect& lockrect )
{
	if ( texture == _null )
		return _null;

	D3DLOCKED_RECT d3dlockedrect = { 0 };
	( (IDirect3DTexture9*) texture )->LockRect( level, &d3dlockedrect, (const RECT*) &lockrect, 0 );

	pitch = d3dlockedrect.Pitch;

	return d3dlockedrect.pBits;
}

_void RenderAPID3D9::UnlockTexture( _void* texture, _dword format, _dword level, _void* buffer, const Rect& lockrect )
{
	if ( texture == _null )
		return;

	( (IDirect3DTexture9*) texture )->UnlockRect( level );
}

_void RenderAPID3D9::SetTexture( _dword stage, _void* texture )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetTexture( stage, (IDirect3DTexture9*) texture );
}

_void RenderAPID3D9::SaveTexture( _void* texture, StringPtr filename, _dword format )
{
	if ( texture == _null )
		return;

	_dword d3dformat = -1;

	switch ( format )
	{
		case ITextureFactory::_FORMAT_BMP: d3dformat = D3DXIFF_BMP; break;
		case ITextureFactory::_FORMAT_JPG: d3dformat = D3DXIFF_JPG; break;
		case ITextureFactory::_FORMAT_PNG: d3dformat = D3DXIFF_PNG; break;
		case ITextureFactory::_FORMAT_DDS: d3dformat = D3DXIFF_DDS; break;
		case ITextureFactory::_FORMAT_DIB: d3dformat = D3DXIFF_DIB; break;
	}

	if ( d3dformat == -1 )
		return;

	::D3DXSaveTextureToFile( filename, (D3DXIMAGE_FILEFORMAT) d3dformat, (IDirect3DTexture9*) texture, _null );
}

_void RenderAPID3D9::GenerateMipmaps( _void* texture )
{
	if ( texture == _null )
		return;

	( (IDirect3DTexture9*) texture )->GenerateMipSubLevels( );
}

_void RenderAPID3D9::ReleaseTexture( _void* texture )
{
	if ( texture == _null )
		return;

	( (IDirect3DTexture9*) texture )->Release( );
}

_void* RenderAPID3D9::GetSurface( _void* texture, _dword level )
{
	if ( texture == _null )
		return _null;

	_void* d3dsurface = _null;
	( (IDirect3DTexture9*) texture )->GetSurfaceLevel( level, (IDirect3DSurface9**) &d3dsurface );

	return d3dsurface;
}

_void* RenderAPID3D9::CreateSurface( _dword width, _dword height, _dword format )
{
	format = PSDFormat2APIFormat( format );

	// Create surface from memory.
	_void* d3dsurface = _null;
	( (IDirect3DDevice9*) mD3DDevice )->CreateOffscreenPlainSurface( width, height, (D3DFORMAT) format, D3DPOOL_SYSTEMMEM, (IDirect3DSurface9**) &d3dsurface, _null );

	return d3dsurface;
}

_void* RenderAPID3D9::CreateSurface( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format )
{
	// Get surface information.
	D3DXIMAGE_INFO imageinfo;
	if ( ::D3DXGetImageInfoFromFileInMemory( buffer, length, &imageinfo ) != 0 )
		return _null;

	// Create surface from memory.
	_void* d3dsurface = _null;
	( (IDirect3DDevice9*) mD3DDevice )->CreateOffscreenPlainSurface( imageinfo.Width, imageinfo.Height, imageinfo.Format, D3DPOOL_SCRATCH, (IDirect3DSurface9**) &d3dsurface, _null );

	if ( d3dsurface == _null )
		return _null;

	::D3DXLoadSurfaceFromFileInMemory( (IDirect3DSurface9*) d3dsurface, _null, _null, buffer, length, _null, D3DX_FILTER_NONE, 0xFF000000, _null );

	width	= imageinfo.Width;
	height	= imageinfo.Height;
	format	= APIFormat2PSDFormat( imageinfo.Format );

	return d3dsurface;
}

_void* RenderAPID3D9::LockSurface( _void* surface, _dword format, _dword& pitch, const Rect& lockrect )
{
	if ( surface == _null )
		return _null;

	D3DLOCKED_RECT d3dlockedrect = { 0 };
	( (IDirect3DSurface9*) surface )->LockRect( &d3dlockedrect, (const RECT*) &lockrect, 0 );

	pitch = d3dlockedrect.Pitch;

	return d3dlockedrect.pBits;
}

_void RenderAPID3D9::UnlockSurface( _void* surface, _dword format, _void* buffer, const Rect& lockrect )
{
	if ( surface == _null )
		return;

	( (IDirect3DSurface9*) surface )->UnlockRect( );
}

_void RenderAPID3D9::ReleaseSurface( _void* surface )
{
	if ( surface == _null )
		return;

	( (IDirect3DSurface9*) surface )->Release( );
}

_dword RenderAPID3D9::GetRenderTargetFormat( ) const
{
	if ( mDefaultRenderTarget == _null )
		return 0;

	D3DSURFACE_DESC surfacedesc = { (D3DFORMAT) 0 };
	( (IDirect3DSurface9*) mDefaultRenderTarget )->GetDesc( &surfacedesc );

	return APIFormat2PSDFormat( surfacedesc.Format );
}

_void* RenderAPID3D9::CreateRenderTargetSurface( _dword width, _dword height, _dword format )
{
	if ( mD3DDevice == _null )
		return _null;

	_dword d3dformat = PSDFormat2APIFormat( format );
	if ( d3dformat == 0 || d3dformat == -1 )
		return _null;

	_void* d3dsurface = _null;
	( (IDirect3DDevice9*) mD3DDevice )->CreateRenderTarget( width, height, (D3DFORMAT) d3dformat, D3DMULTISAMPLE_NONE, 0, FALSE, (IDirect3DSurface9**) &d3dsurface, _null );

	return d3dsurface;
}

_void* RenderAPID3D9::GetDefaultRenderTargetSurface( )
{
	return mDefaultRenderTarget;
}

_void* RenderAPID3D9::GetCurrentRenderTargetSurface( )
{
	return mCurrentRenderTarget;
}

_void RenderAPID3D9::SetRenderTargetSurface( _void* rendertarget )
{
	if ( mD3DDevice == _null )
		return;

	mCurrentRenderTarget = rendertarget;
	( (IDirect3DDevice9*) mD3DDevice )->SetRenderTarget( 0, (IDirect3DSurface9*) rendertarget );
}

_dword RenderAPID3D9::GetDepthStencilFormat( ) const
{
	if ( mDefaultDepthStencil == _null )
		return 0;

	D3DSURFACE_DESC surfacedesc = { (D3DFORMAT) 0 };
	( (IDirect3DSurface9*) mDefaultDepthStencil )->GetDesc( &surfacedesc );

	return APIFormat2PSDFormat( surfacedesc.Format );
}

_void* RenderAPID3D9::CreateDepthStencilSurface( _dword width, _dword height )
{
	if ( mD3DDevice == _null )
		return _null;

	_void* d3dsurface = _null;
	( (IDirect3DDevice9*) mD3DDevice )->CreateDepthStencilSurface( width, height, D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0, TRUE, (IDirect3DSurface9**) &d3dsurface, _null );

	return d3dsurface;
}

_void* RenderAPID3D9::GetDefaultDepthStencilSurface( )
{
	return mDefaultDepthStencil;
}

_void* RenderAPID3D9::GetCurrentDepthStencilSurface( )
{
	return mCurrentDepthStencil;
}

_void RenderAPID3D9::SetDepthStencilSurface( _void* depthstencil )
{
	if ( mD3DDevice == _null )
		return;

	mCurrentDepthStencil = depthstencil;
	( (IDirect3DDevice9*) mD3DDevice )->SetDepthStencilSurface( (IDirect3DSurface9*) depthstencil );
}

_void RenderAPID3D9::SetMaterial( _dword diffuse, _dword ambient, _dword specular, _dword emissive, _float power )
{
	if ( mD3DDevice == _null )
		return;

	D3DMATERIAL9 material;
	material.Power = power;
	Memory::MemCpy( &material.Diffuse,  &Color( diffuse ),  sizeof( Color ) );
	Memory::MemCpy( &material.Ambient,  &Color( ambient ),  sizeof( Color ) );
	Memory::MemCpy( &material.Specular, &Color( specular ), sizeof( Color ) );
	Memory::MemCpy( &material.Emissive, &Color( emissive ), sizeof( Color ) );

	( (IDirect3DDevice9*) mD3DDevice )->SetMaterial( &material );
}

_void RenderAPID3D9::SetLight( _dword index, _void* light )
{
	if ( mD3DDevice == _null )
		return;

	if ( light != _null )
	{
		( (IDirect3DDevice9*) mD3DDevice )->SetLight( index, (D3DLIGHT9*) light );
		( (IDirect3DDevice9*) mD3DDevice )->LightEnable( index, _true );
	}
	else
	{
		( (IDirect3DDevice9*) mD3DDevice )->LightEnable( index, _false );
	}
}

_dword RenderAPID3D9::GetVertexShaderVersion( ) const
{
	if ( mD3DDevice == _null )
		return 0;

	D3DCAPS9 caps;
	( (IDirect3DDevice9*) mD3DDevice )->GetDeviceCaps( &caps );

	return caps.VertexShaderVersion;
}

_void* RenderAPID3D9::CreateVertexShader( StringPtr code )
{
	if ( mD3DDevice == _null )
		return _null;

	_chara* ansicode = StringFormatter::UnicodeToAnsi( _null, 0, code );

	// Assemble shader code.
	ID3DXBuffer* buffer = _null;
	if ( ::D3DXAssembleShader( ansicode, (_long) ::strlen( ansicode ) + 1, _null, _null, 0, &buffer, _null ) != 0 )
	{
		delete[] ansicode;
		return _null;
	}

	delete[] ansicode;

	// Create d3d vertex shader interface.
	_void* d3dvertexshader = 0;
	if ( ( (IDirect3DDevice9*) mD3DDevice )->CreateVertexShader( (_dword*) buffer->GetBufferPointer( ), (IDirect3DVertexShader9**) &d3dvertexshader ) != 0 )
	{
		buffer->Release( );
		return _null;
	}

	buffer->Release( );

	return d3dvertexshader;
}

_void* RenderAPID3D9::CreateVertexShader( StringPtr code, StringPtr function, StringPtr profile )
{
	if ( mD3DDevice == _null )
		return _null;

	_chara* ansicode		= StringFormatter::UnicodeToAnsi( _null, 0, code );
	_chara* ansifunction	= StringFormatter::UnicodeToAnsi( _null, 0, function );
	_chara* ansiprofile		= StringFormatter::UnicodeToAnsi( _null, 0, profile );

	// Assemble shader code.
	ID3DXBuffer* buffer = _null;
	if ( ::D3DXCompileShader( ansicode, (_long) ::strlen( ansicode ) + 1, _null, _null, ansifunction, ansiprofile, 0, &buffer, _null, _null ) != 0 )
	{
		delete[] ansicode;
		delete[] ansifunction;
		delete[] ansiprofile;
		return _null;
	}

	delete[] ansicode;
	delete[] ansifunction;
	delete[] ansiprofile;

	// Create d3d vertex shader interface.
	_void* d3dvertexshader = 0;
	if ( ( (IDirect3DDevice9*) mD3DDevice )->CreateVertexShader( (_dword*) buffer->GetBufferPointer( ), (IDirect3DVertexShader9**) &d3dvertexshader ) != 0 )
	{
		buffer->Release( );
		return _null;
	}

	buffer->Release( );

	return d3dvertexshader;
}

_void RenderAPID3D9::SetVertexShader( _void* vertexshader )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetVertexShader( (IDirect3DVertexShader9*) vertexshader );
}

_void RenderAPID3D9::SetVertexShaderConstant( _dword index, const _float* constant, _dword number )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetVertexShaderConstantF( index, constant, number );
}

_void RenderAPID3D9::ReleaseVertexShader( _void* vertexshader )
{
	if ( vertexshader == _null )
		return;

	( (IDirect3DVertexShader9*) vertexshader )->Release( );
}

_dword RenderAPID3D9::GetPixelShaderVersion( ) const
{
	if ( mD3DDevice == _null )
		return 0;

	D3DCAPS9 caps;
	( (IDirect3DDevice9*) mD3DDevice )->GetDeviceCaps( &caps );

	return caps.PixelShaderVersion;
}

_void* RenderAPID3D9::CreatePixelShader( StringPtr code )
{
	if ( mD3DDevice == _null )
		return _null;

	_chara* ansicode = StringFormatter::UnicodeToAnsi( _null, 0, code );

	// Assemble shader code.
	ID3DXBuffer* buffer = _null;
	if ( ::D3DXAssembleShader( ansicode, (_long) ::strlen( ansicode ) + 1, _null, _null, 0, &buffer, _null ) != 0 )
	{
		delete[] ansicode;
		return _null;
	}

	delete[] ansicode;

	// Create d3d pixel shader interface.
	_void* d3dpixelshader = 0;
	if ( ( (IDirect3DDevice9*) mD3DDevice )->CreatePixelShader( (_dword*) buffer->GetBufferPointer( ), (IDirect3DPixelShader9**) &d3dpixelshader ) != 0 )
	{
		buffer->Release( );
		return _null;
	}

	buffer->Release( );

	return d3dpixelshader;
}

_void* RenderAPID3D9::CreatePixelShader( StringPtr code, StringPtr function, StringPtr profile )
{
	if ( mD3DDevice == _null )
		return _null;

	_chara* ansicode		= StringFormatter::UnicodeToAnsi( _null, 0, code );
	_chara* ansifunction	= StringFormatter::UnicodeToAnsi( _null, 0, function );
	_chara* ansiprofile		= StringFormatter::UnicodeToAnsi( _null, 0, profile );

	// Assemble shader code.
	ID3DXBuffer* buffer = _null;
	if ( ::D3DXCompileShader( ansicode, (_long) ::strlen( ansicode ) + 1, _null, _null, ansifunction, ansiprofile, 0, &buffer, _null, _null ) != 0 )
	{
		delete[] ansicode;
		delete[] ansifunction;
		delete[] ansiprofile;

		return _null;
	}

	delete[] ansicode;
	delete[] ansifunction;
	delete[] ansiprofile;

	// Create d3d pixel shader interface.
	_void* d3dpixelshader = 0;
	if ( ( (IDirect3DDevice9*) mD3DDevice )->CreatePixelShader( (_dword*) buffer->GetBufferPointer( ), (IDirect3DPixelShader9**) &d3dpixelshader ) != 0 )
	{
		buffer->Release( );
		return _null;
	}

	buffer->Release( );

	return d3dpixelshader;
}

_void RenderAPID3D9::SetPixelShader( _void* pixelshader )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetPixelShader( (IDirect3DPixelShader9*) pixelshader );
}

_void RenderAPID3D9::SetPixelShaderConstant( _dword index, const _float* constant, _dword number )
{
	if ( mD3DDevice == _null )
		return;

	( (IDirect3DDevice9*) mD3DDevice )->SetPixelShaderConstantF( index, constant, number );
}

_void RenderAPID3D9::ReleasePixelShader( _void* pixelshader )
{
	if ( pixelshader == _null )
		return;

	( (IDirect3DPixelShader9*) pixelshader )->Release( );
}

_void RenderAPID3D9::RenderPrimitive( _dword type, _dword svertex, _dword pnumber )
{
	if ( mD3DDevice == _null )
		return;

	_dword primitivetype = 0, vertexnumber = 0, polygonnumber = 0;

	switch ( type )
	{
		case Geometry::_GEOMETRY_POINTLIST:
		{
			primitivetype	= D3DPT_POINTLIST;
			vertexnumber	= pnumber;

			break;
		}

		case Geometry::_GEOMETRY_LINELIST:
		{
			primitivetype	= D3DPT_LINELIST;
			vertexnumber	= pnumber * 2;

			break;
		}

		case Geometry::_GEOMETRY_LINESTRIP:
		{
			primitivetype	= D3DPT_LINESTRIP;
			vertexnumber	= pnumber + 1;

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLELIST:
		{
			primitivetype	= D3DPT_TRIANGLELIST;
			vertexnumber	= pnumber * 3;
			polygonnumber	= pnumber;

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLESTRIP:
		{
			primitivetype	= D3DPT_TRIANGLESTRIP;
			vertexnumber	= pnumber + 2;
			polygonnumber	= pnumber;

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLEFAN:
		{
			primitivetype	= D3DPT_TRIANGLEFAN;
			vertexnumber	= pnumber + 2;
			polygonnumber	= pnumber;

			break;
		}
	}

	( (RenderStatistics&) GetRenderStatistics( ) ).DrawPrimitive( vertexnumber, polygonnumber );

	( (IDirect3DDevice9*) mD3DDevice )->DrawPrimitive( (D3DPRIMITIVETYPE) primitivetype, svertex, pnumber );
}

_void RenderAPID3D9::RenderPrimitive( _dword type, _dword svertex, _dword sindex, _dword vnumber, _dword pnumber )
{
	if ( mD3DDevice == _null )
		return;

	_dword primitivetype = 0, vertexnumber = 0, polygonnumber = 0;

	switch ( type )
	{
		case Geometry::_GEOMETRY_POINTLIST:
		{
			primitivetype	= D3DPT_POINTLIST;
			vertexnumber	= pnumber;

			break;
		}

		case Geometry::_GEOMETRY_LINELIST:
		{
			primitivetype	= D3DPT_LINELIST;
			vertexnumber	= vnumber;

			break;
		}

		case Geometry::_GEOMETRY_LINESTRIP:
		{
			primitivetype	= D3DPT_LINESTRIP;
			vertexnumber	= vnumber;

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLELIST:
		{
			primitivetype	= D3DPT_TRIANGLELIST;
			vertexnumber	= vnumber;
			polygonnumber	= pnumber;

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLESTRIP:
		{
			primitivetype	= D3DPT_TRIANGLESTRIP;
			vertexnumber	= vnumber;
			polygonnumber	= pnumber;

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLEFAN:
		{
			primitivetype	= D3DPT_TRIANGLEFAN;
			vertexnumber	= vnumber;
			polygonnumber	= pnumber;

			break;
		}
	}

	( (RenderStatistics&) GetRenderStatistics( ) ).DrawPrimitive( vertexnumber, polygonnumber );

	( (IDirect3DDevice9*) mD3DDevice )->DrawIndexedPrimitive( (D3DPRIMITIVETYPE) primitivetype, svertex, 0, vnumber, sindex, pnumber );
}

_void RenderAPID3D9::Present( _handle window )
{
	( (IDirect3DSwapChain9*) mDefaultSwapChain )->Present( _null, _null, (HWND) window, _null, 0 );
}

_dword RenderAPID3D9::APIFormat2PSDFormat( _dword format ) const
{
	switch ( format )
	{
		case D3DFMT_UNKNOWN:
			return ITextureFactory::_FORMAT_UNKNOWN;

		case D3DFMT_R8G8B8:
			return ITextureFactory::_FORMAT_R8G8B8;

		case D3DFMT_A8R8G8B8:
			return ITextureFactory::_FORMAT_A8R8G8B8;

		case D3DFMT_X8R8G8B8:
			return ITextureFactory::_FORMAT_X8R8G8B8;

		case D3DFMT_A4R4G4B4:
			return ITextureFactory::_FORMAT_A4R4G4B4;

		case D3DFMT_X4R4G4B4:
			return ITextureFactory::_FORMAT_X4R4G4B4;

		case D3DFMT_R5G6B5:
			return ITextureFactory::_FORMAT_R5G6B5;

		case D3DFMT_A1R5G5B5:
			return ITextureFactory::_FORMAT_A1R5G5B5;

		case D3DFMT_DXT1:
			return ITextureFactory::_FORMAT_DXT1;

		case D3DFMT_DXT2:
			return ITextureFactory::_FORMAT_DXT2;

		case D3DFMT_DXT3:
			return ITextureFactory::_FORMAT_DXT3;

		case D3DFMT_DXT4:
			return ITextureFactory::_FORMAT_DXT4;

		case D3DFMT_DXT5:
			return ITextureFactory::_FORMAT_DXT5;

		case D3DFMT_A8L8:
			return ITextureFactory::_FORMAT_A8L8;

		case D3DFMT_D24S8:
			return ITextureFactory::_FORMAT_D24S8;
	}

	return -1;
}

_dword RenderAPID3D9::PSDFormat2APIFormat( _dword format ) const
{
	switch ( format )
	{
		case ITextureFactory::_FORMAT_UNKNOWN:
			return D3DFMT_UNKNOWN;

		case ITextureFactory::_FORMAT_R8G8B8:
			return D3DFMT_R8G8B8;

		case ITextureFactory::_FORMAT_A8R8G8B8:
			return D3DFMT_A8R8G8B8;

		case ITextureFactory::_FORMAT_X8R8G8B8:
			return D3DFMT_X8R8G8B8;

		case ITextureFactory::_FORMAT_A4R4G4B4:
			return D3DFMT_A4R4G4B4;

		case ITextureFactory::_FORMAT_X4R4G4B4:
			return D3DFMT_X4R4G4B4;

		case ITextureFactory::_FORMAT_R5G6B5:
			return D3DFMT_R5G6B5;

		case ITextureFactory::_FORMAT_A1R5G5B5:
			return D3DFMT_A1R5G5B5;

		case ITextureFactory::_FORMAT_DXT1:
			return D3DFMT_DXT1;

		case ITextureFactory::_FORMAT_DXT2:
			return D3DFMT_DXT2;

		case ITextureFactory::_FORMAT_DXT3:
			return D3DFMT_DXT3;

		case ITextureFactory::_FORMAT_DXT4:
			return D3DFMT_DXT4;

		case ITextureFactory::_FORMAT_DXT5:
			return D3DFMT_DXT5;

		case ITextureFactory::_FORMAT_A8L8:
			return D3DFMT_A8L8;

		case ITextureFactory::_FORMAT_D24S8:
			return D3DFMT_D24S8;
	}

	return -1;
}

#endif
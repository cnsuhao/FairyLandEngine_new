//============================================================================
// RenderAPI.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderAPI
//----------------------------------------------------------------------------

class RenderAPI
{
public:
	enum _API_TYPE
	{
		_API_D3D9	= 1,
		_API_OGL	= 2,
	};

protected:
	_handle		mWindow;
	_dword		mDeviceType;
	_dword		mDeviceWidth;
	_dword		mDeviceHeight;

	_bool		mRGBAFormat;

	_dword		mClearColor;
	_float		mClearDepth;
	_dword		mClearStencil;

	RenderAPI( );

public:
	inline _dword ConvertColor( _dword color ) const
		{ return mRGBAFormat ? ( color & 0xFF000000 ) | ( ( color & 0x00FF0000 ) >> 16 ) | ( color & 0x0000FF00 ) | ( ( color & 0x000000FF ) << 16 ) : color; }

	inline _handle GetDeviceHandle( ) const
		{ return mWindow; }

	inline _dword GetDeviceWidth( ) const
		{ return mDeviceWidth; }

	inline _dword GetDeviceHeight( ) const
		{ return mDeviceHeight; }

	inline _void SetClearColor( _dword color )
		{ mClearColor = color; }

	inline _void SetClearDepth( _float depth )
		{ mClearDepth = depth; }

	inline _void SetClearStencil( _dword stencil )
		{ mClearStencil = stencil; }

	static _dword	GetPixelSize( _dword format );
	static _dword	GetSurfaceSize( _dword width, _dword height, _dword format, _dword mip );

	virtual _dword	GetAPIType( ) const = 0;

	virtual _bool	Initialize( _handle window, _dword devicetype ) = 0;
	virtual _void	Finalize( ) = 0;

	virtual _bool	IsDeviceLost( ) = 0;
	virtual _bool	ResizeDevice( _dword width, _dword height ) = 0;
	virtual _bool	ResetDevice( ) = 0;

	virtual _void	ClearBuffer( _bool color, _bool depth, _bool stencil ) = 0;
	virtual _void	BeginScene( ) = 0;
	virtual _void	EndScene( ) = 0;

	virtual _void	SetViewport( const Viewport& viewport ) = 0;
	virtual _void	GetViewport( Viewport& viewport ) = 0;

	virtual _void	SetWorldTransform( const Matrix4& matrix ) = 0;
	virtual _void	GetWorldTransform( Matrix4& matrix ) = 0;

	virtual _void	SetViewTransform( const Matrix4& matrix ) = 0;
	virtual _void	GetViewTransform( Matrix4& matrix ) = 0;

	virtual _void	SetProjectionTransform( const Matrix4& matrix ) = 0;
	virtual _void	GetProjectionTransform( Matrix4& matrix ) = 0;

	virtual _void	SetTextureTransform( _dword index, const Matrix4& matrix ) = 0;
	virtual _void	GetTextureTransform( _dword index, Matrix4& matrix ) = 0;

	virtual _void	EnableRenderState( _dword state, _dword value ) = 0;
	virtual _void	DisableRenderState( _dword state ) = 0;

	virtual _void	EnableTextureState( _dword stage, _dword state, _dword value ) = 0;
	virtual _void	DisableTextureState( _dword stage, _dword state ) = 0;

	virtual _void	EnableSamplerState( _dword stage, _dword state, _dword value ) = 0;
	virtual _void	DisableSamplerState( _dword stage, _dword state ) = 0;

	virtual _void*	CreateVertexDeclaration( _void* vertexdecl, const Array< VertexElement >& elementarray ) = 0;
	virtual _void	ReleaseVertexDeclaration( _void* vertexdecl ) = 0;

	virtual _void*	CreateVertexBuffer( _dword type, _dword size ) = 0;
	virtual _void*	LockVertexBuffer( _void* vertexbuffer, _dword offset, _dword length, _dword flag ) = 0;
	virtual _void	UnlockVertexBuffer( _void* vertexbuffer ) = 0;
	virtual _void	SetVertexBuffer( _void* vertexbuffer, _dword vertexsize ) = 0;
	virtual _void	ReleaseVertexBuffer( _void* vertexbuffer ) = 0;

	virtual _void*	CreateIndexBuffer( _dword type, _dword size, _dword indexsize ) = 0;
	virtual _void*	LockIndexBuffer( _void* indexbuffer, _dword offset, _dword length, _dword flag ) = 0;
	virtual _void	UnlockIndexBuffer( _void* indexbuffer ) = 0;
	virtual _void	SetIndexBuffer( _void* indexbuffer, _dword indexsize ) = 0;
	virtual _void	ReleaseIndexBuffer( _void* indexbuffer ) = 0;

	virtual _bool	GetTextureInfo( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip ) = 0;
	virtual _void*	CreateTexture( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip ) = 0;
	virtual _void*	CreateTexture( _dword width, _dword height, _dword format, _dword mip ) = 0;
	virtual _void*	CreateRenderTexture( _dword width, _dword height, _dword format ) = 0;
	virtual _void*	CreateDepthTexture( _dword width, _dword height ) = 0;
	virtual _void*	LockTexture( _void* texture, _dword format, _dword level, _dword& pitch, const Rect& lockrect ) = 0;
	virtual _void	UnlockTexture( _void* texture, _dword format, _dword level, _void* buffer, const Rect& lockrect ) = 0;
	virtual _void	SetTexture( _dword stage, _void* texture ) = 0;
	virtual _void	SaveTexture( _void* texture, StringPtr filename, _dword format ) = 0;
	virtual _void	GenerateMipmaps( _void* texture ) = 0;
	virtual _void	ReleaseTexture( _void* texture ) = 0;

	virtual _void*	GetSurface( _void* texture, _dword level ) = 0;
	virtual _void*	CreateSurface( _dword width, _dword height, _dword format ) = 0;
	virtual _void*	CreateSurface( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format ) = 0;
	virtual _void*	LockSurface( _void* surface, _dword format, _dword& pitch, const Rect& lockrect ) = 0;
	virtual _void	UnlockSurface( _void* surface, _dword format, _void* buffer, const Rect& lockrect ) = 0;
	virtual _void	ReleaseSurface( _void* surface ) = 0;

	virtual _dword	GetRenderTargetFormat( ) const = 0;
	virtual _void*	CreateRenderTargetSurface( _dword width, _dword height, _dword format ) = 0;
	virtual _void*	GetDefaultRenderTargetSurface( ) = 0;
	virtual _void*	GetCurrentRenderTargetSurface( ) = 0;
	virtual _void	SetRenderTargetSurface( _void* surface ) = 0;

	virtual _dword	GetDepthStencilFormat( ) const = 0;
	virtual _void*	CreateDepthStencilSurface( _dword width, _dword height ) = 0;
	virtual _void*	GetDefaultDepthStencilSurface( ) = 0;
	virtual _void*	GetCurrentDepthStencilSurface( ) = 0;
	virtual _void	SetDepthStencilSurface( _void* surface ) = 0;

	virtual _void	SetMaterial( _dword diffuse, _dword ambient, _dword specular, _dword emissive, _float power ) = 0;
	virtual _void	SetLight( _dword index, _void* light ) = 0;

	virtual _dword	GetVertexShaderVersion( ) const = 0;
	virtual _void*	CreateVertexShader( StringPtr code ) = 0;
	virtual _void*	CreateVertexShader( StringPtr code, StringPtr function, StringPtr profile ) = 0;
	virtual _void	SetVertexShader( _void* vertexshader ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const _float* constant, _dword number ) = 0;
	virtual _void	ReleaseVertexShader( _void* vertexshader ) = 0;

	virtual _dword	GetPixelShaderVersion( ) const = 0;
	virtual _void*	CreatePixelShader( StringPtr code ) = 0;
	virtual _void*	CreatePixelShader( StringPtr code, StringPtr function, StringPtr profile ) = 0;
	virtual _void	SetPixelShader( _void* pixelshader ) = 0;
	virtual _void	SetPixelShaderConstant( _dword index, const _float* constant, _dword number ) = 0;
	virtual _void	ReleasePixelShader( _void* pixelshader ) = 0;

	virtual _void	RenderPrimitive( _dword type, _dword svertex, _dword pnumber ) = 0;
	virtual _void	RenderPrimitive( _dword type, _dword svertex, _dword sindex, _dword vnumber, _dword pnumber ) = 0;

	virtual _void	Present( _handle window ) = 0;

	virtual _dword	APIFormat2PSDFormat( _dword format ) const = 0;
	virtual _dword	PSDFormat2APIFormat( _dword format ) const = 0;
};

}
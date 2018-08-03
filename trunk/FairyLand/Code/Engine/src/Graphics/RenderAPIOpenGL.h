//============================================================================
// RenderAPIOpenGL.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

#ifdef _OPENGL_RENDER

namespace FL
{

//----------------------------------------------------------------------------
// RenderAPIOpenGL
//----------------------------------------------------------------------------

class RenderAPIOpenGL : public RenderAPI
{
private:
	_handle					mWindowDC;
	_handle					mRenderContext;

	Array< VertexElement >*	mVertexElementArray;

	_dword					mVertexSize;
	_dword					mVertexBuffer;

	_dword					mIndexSize;
	_dword					mIndexBuffer;

	Matrix4					mWorldTranform;

	_dword	BeforeRender( );
	_void	AfterRender( _dword format );
	_dword	GetGLFormat( _dword format );

public:
	RenderAPIOpenGL( );

	virtual _dword	GetAPIType( ) const;

	virtual _bool	Initialize( _handle window, _dword devicetype );
	virtual _void	Finalize( );

	virtual _bool	IsDeviceLost( );
	virtual _bool	ResizeDevice( _dword width, _dword height );
	virtual _bool	ResetDevice( );

	virtual _void	ClearBuffer( _bool color, _bool depth, _bool stencil );
	virtual _void	BeginScene( );
	virtual _void	EndScene( );

	virtual _void	SetViewport( const Viewport& viewport );
	virtual _void	GetViewport( Viewport& viewport );

	virtual _void	SetWorldTransform( const Matrix4& matrix );
	virtual _void	GetWorldTransform( Matrix4& matrix );

	virtual _void	SetViewTransform( const Matrix4& matrix );
	virtual _void	GetViewTransform( Matrix4& matrix );

	virtual _void	SetProjectionTransform( const Matrix4& matrix );
	virtual _void	GetProjectionTransform( Matrix4& matrix );

	virtual _void	SetTextureTransform( _dword index, const Matrix4& matrix );
	virtual _void	GetTextureTransform( _dword index, Matrix4& matrix );

	virtual _void	EnableRenderState( _dword state, _dword value );
	virtual _void	DisableRenderState( _dword state );

	virtual _void	EnableSamplerState( _dword stage, _dword state, _dword value );
	virtual _void	DisableSamplerState( _dword stage, _dword state );

	virtual _void	EnableTextureState( _dword stage, _dword state, _dword value );
	virtual _void	DisableTextureState( _dword stage, _dword state );

	virtual _void*	CreateVertexDeclaration( _void* vertexdecl, const Array< VertexElement >& elementarray );
	virtual _void	ReleaseVertexDeclaration( _void* vertexdecl );

	virtual _void*	CreateVertexBuffer( _dword type, _dword size );
	virtual _void*	LockVertexBuffer( _void* vertexbuffer, _dword offset, _dword length, _dword flag );
	virtual _void	UnlockVertexBuffer( _void* vertexbuffer );
	virtual _void	SetVertexBuffer( _void* vertexbuffer, _dword vertexsize );
	virtual _void	ReleaseVertexBuffer( _void* vertexbuffer );

	virtual _void*	CreateIndexBuffer( _dword type, _dword size, _dword indexsize );
	virtual _void*	LockIndexBuffer( _void* indexbuffer, _dword offset, _dword length, _dword flag );
	virtual _void	UnlockIndexBuffer( _void* indexbuffer );
	virtual _void	SetIndexBuffer( _void* indexbuffer, _dword indexsize );
	virtual _void	ReleaseIndexBuffer( _void* indexbuffer );

	virtual _bool	GetTextureInfo( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip );
	virtual _void*	CreateTexture( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip );
	virtual _void*	CreateTexture( _dword width, _dword height, _dword format, _dword mip );
	virtual _void*	CreateRenderTexture( _dword width, _dword height, _dword format );
	virtual _void*	CreateDepthTexture( _dword width, _dword height );
	virtual _void*	LockTexture( _void* texture, _dword format, _dword level, _dword& pitch, const Rect& lockrect );
	virtual _void	UnlockTexture( _void* texture, _dword format, _dword level, _void* buffer, const Rect& lockrect );
	virtual _void	SetTexture( _dword stage, _void* texture );
	virtual _void	SaveTexture( _void* texture, StringPtr filename, _dword format );
	virtual _void	GenerateMipmaps( _void* texture );
	virtual _void	ReleaseTexture( _void* texture );

	virtual _void*	GetSurface( _void* texture, _dword level );
	virtual _void*	CreateSurface( _dword width, _dword height, _dword format );
	virtual _void*	CreateSurface( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format );
	virtual _void*	LockSurface( _void* surface, _dword format, _dword& pitch, const Rect& lockrect );
	virtual _void	UnlockSurface( _void* surface, _dword format, _void* buffer, const Rect& lockrect );
	virtual _void	ReleaseSurface( _void* surface );

	virtual _dword	GetRenderTargetFormat( ) const;
	virtual _void*	CreateRenderTargetSurface( _dword width, _dword height, _dword format );
	virtual _void*	GetDefaultRenderTargetSurface( );
	virtual _void*	GetCurrentRenderTargetSurface( );
	virtual _void	SetRenderTargetSurface( _void* surface );

	virtual _dword	GetDepthStencilFormat( ) const;
	virtual _void*	CreateDepthStencilSurface( _dword width, _dword height );
	virtual _void*	GetDefaultDepthStencilSurface( );
	virtual _void*	GetCurrentDepthStencilSurface( );
	virtual _void	SetDepthStencilSurface( _void* surface );

	virtual _void	SetMaterial( _dword diffuse, _dword ambient, _dword specular, _dword emissive, _float power );
	virtual _void	SetLight( _dword index, _void* light );

	virtual _dword	GetVertexShaderVersion( ) const;
	virtual _void*	CreateVertexShader( StringPtr code );
	virtual _void*	CreateVertexShader( StringPtr code, StringPtr function, StringPtr profile );
	virtual _void	SetVertexShader( _void* vertexshader );
	virtual _void	SetVertexShaderConstant( _dword index, const _float* constant, _dword number );
	virtual _void	ReleaseVertexShader( _void* vertexshader );

	virtual _dword	GetPixelShaderVersion( ) const;
	virtual _void*	CreatePixelShader( StringPtr code );
	virtual _void*	CreatePixelShader( StringPtr code, StringPtr function, StringPtr profile );
	virtual _void	SetPixelShader( _void* pixelshader );
	virtual _void	SetPixelShaderConstant( _dword index, const _float* constant, _dword number );
	virtual _void	ReleasePixelShader( _void* pixelshader );

	virtual _void	RenderPrimitive( _dword type, _dword svertex, _dword pnumber );
	virtual _void	RenderPrimitive( _dword type, _dword svertex, _dword sindex, _dword vnumber, _dword pnumber );

	virtual _void	Present( _handle window );

	virtual _dword	APIFormat2PSDFormat( _dword format ) const;
	virtual _dword	PSDFormat2APIFormat( _dword format ) const;
};

};

#endif
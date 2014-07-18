#include "Button.h"

Button::Button( void ) : color(0), message(NULL) {}

Button::Button( const D3DCOLOR _color, const PRECT pLocation, 
    const WCHAR _message[] ) : color(_color), message(_message)
{
    if( pLocation )
        location = *pLocation;
}

Button::Button( const Button &button ) : color(button.color),
    location(button.location), message(button.message) {}

int Button::draw( const LPD3DXFONT pd3dxFont, 
    const LPD3DXSPRITE pd3dxSprite ) 
{
    PRECT pLocation = &location;
    //CHECK( drawBorder( pd3dxSprite ) );
    //MessageBox( DXUTGetHWND(), message, L"Message", MB_OK );
    return pd3dxFont->DrawText( pd3dxSprite, message, -1, pLocation,
        DT_CENTER | DT_NOCLIP | DT_SINGLELINE | DT_TOP, color );
}

int Button::setAttributes( const D3DCOLOR _color, const PRECT pLocation, 
    const WCHAR _message[] )
{
    if( !pLocation )
        return FALSE;

    if( !_message )
        return FALSE;

    color = _color;
    location = *pLocation;
    message = _message;

    return TRUE;
}

void Button::setColor( const D3DCOLOR _color )
{
    color = _color;
}

Button::~Button()
{
}

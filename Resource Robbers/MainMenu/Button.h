#ifndef BUTTON_H
#define BUTTON_H

#include "DXUT.H"

#define BUT_COLORSELECTED 0xFFFFFFFF
#define BUT_COLORDEFAULT 0xFF0000FF

class Button
{
    D3DCOLOR color;
    RECT location;
    const WCHAR *message;

    public:
    Button( void );
    Button( const D3DCOLOR color, const PRECT location, 
        const WCHAR message[] );
    Button( const Button &button );
    ~Button();
    int draw( const LPD3DXFONT pd3dxFont, 
        const LPD3DXSPRITE pd3dxSprite );

    int setAttributes( const D3DCOLOR color, const PRECT location, 
        const WCHAR message[] );
    void setColor( const D3DCOLOR color );
};

#endif //BUTTON_H

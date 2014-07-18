#ifndef MENU_H
#define MENU_H

#include "Button.h"

#define MENU_COLORDEFAULT 0xffffffff

#define TRUE 1
#define FALSE 0

typedef enum MenuResult
{
	MENU_ERROR = -2,
	MENU_NOCHANGE = -1
	//non-negative values represent which button was selected.
} MenuResult;

class Menu
{
    Button *buttons;
    D3DCOLOR color;
    RECT location;
    size_t occupancy;
    size_t size;
    int selectedButton;

    public:
    Menu( void );
    Menu( const size_t buttonsMax, const RECT &location );
    Menu( const Menu &menu );
    ~Menu();
    HRESULT draw( const LPD3DXFONT pd3dxFont, const LPD3DXSPRITE pd3dxSprite );

    int insert( const WCHAR message[], const LPD3DXFONT pd3dxFont );
    int readInput( UINT nChar );
    void setColor( D3DCOLOR color );
    void setLocation( const RECT &location );
    void setSize( const size_t size );
    static void setTexture( const LPDIRECT3DTEXTURE9 pTexture );
};

#endif //MENU_H

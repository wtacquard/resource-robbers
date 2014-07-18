#include "Menu.h"

Menu::Menu( void ) : buttons(NULL), color(MENU_COLORDEFAULT), 
    occupancy(0), size(0), selectedButton(0) {}

Menu::Menu( const size_t buttonsMax, const RECT &_location ) : 
    location(_location), occupancy(0), size(buttonsMax), selectedButton(0)
{
    buttons = new Button[buttonsMax];
}

Menu::Menu( const Menu &menu ) : 
    location(menu.location), occupancy(menu.occupancy), size(menu.size),
    selectedButton(menu.selectedButton)
{
    if( size > 0 )
    {
        buttons = new Button[size];

        unsigned int index;
        for( index = 0; index < size; index++ )
        {
            buttons[index] = Button( menu.buttons[index] );
        }
    }
}

HRESULT Menu::draw( const LPD3DXFONT pd3dxFont, 
    const LPD3DXSPRITE pd3dxSprite )
{
	HRESULT hr;
    unsigned int index;

    if( buttons == NULL )
        return E_FAIL;

    D3DXMATRIX transformMatrix;
    pd3dxSprite->SetTransform(D3DXMatrixIdentity(&transformMatrix));

    int dHeight = 0;

    for(index = 0; index < occupancy; index++)
    {
        V((dHeight += buttons[index].draw( pd3dxFont, pd3dxSprite)));
    }

    if( dHeight == 0 )
        return E_FAIL;
/*
    const unsigned int height = ( occupancy - 1 ) * dHeight;
    const unsigned int halfHeight = (unsigned int) height / 2;

    const unsigned int halfWidth = 
        (unsigned int)(location.right - location.left) / 2;

    const unsigned int sideDrawRectWidth =
        (unsigned int)(sideDrawRect.right - sideDrawRect.left);

    D3DXVECTOR2 center = D3DXVECTOR2(
        (float)( location.right + location.left ) / 2,
        (float)( location.top + location.top + height ) / 2 );
    
    D3DXVECTOR2 drawLocation = center;

    for( index = 0; 
        index < halfWidth + sideDrawRectWidth; 
        index += sideDrawRectWidth )
    {
        drawLocation.y = center.y - ( halfHeight + sideDrawRectWidth / 2 );
        drawLocation.x = center.x + index;

        CHECK( drawBorderSegment( color, drawLocation, pd3dxSprite, 
            MENU_BORDERTOPROTATE ) );

        drawLocation.y = center.y + ( halfHeight + sideDrawRectWidth / 2 );
        drawLocation.x = center.x + index;

        CHECK( drawBorderSegment( color, drawLocation, pd3dxSprite, 
            MENU_BORDERBOTTOMROTATE ) );

        if( index == 0 )
            continue;

        drawLocation.y = center.y + ( halfHeight + sideDrawRectWidth / 2 );
        drawLocation.x = center.x - index;

        CHECK( drawBorderSegment( color, drawLocation, pd3dxSprite, 
            MENU_BORDERBOTTOMROTATE ) );

        drawLocation.y = center.y - ( halfHeight + sideDrawRectWidth / 2 );
        drawLocation.x = center.x - index;

        CHECK( drawBorderSegment( color, drawLocation, pd3dxSprite, 
            MENU_BORDERTOPROTATE ) );
    }
*/

    return S_OK;
}

/*
HRESULT Menu::drawBorderSegment( const D3DCOLOR color, 
    const D3DXVECTOR2 &drawLocation, 
    const LPD3DXSPRITE pd3dxSprite,
    const float rotationAngle )
{
        CHECK( drawSprite( color, pd3dxSprite, &sideDrawRect, pTexture,
            drawLocation, rotationAngle ) );

        return drawSprite( color, pd3dxSprite, &sideDrawRectShader, pTexture,
            drawLocation, rotationAngle );
}
*/

int Menu::insert( const WCHAR message[], const LPD3DXFONT pd3dxFont )
{
	HRESULT hr;
    if( occupancy >= Menu::size || !message || !size || !buttons )
        return FALSE;

    int dy;
    dy = pd3dxFont->DrawText( NULL, message, -1, &location,
        DT_CENTER | DT_NOCLIP | DT_SINGLELINE | DT_TOP | DT_CALCRECT, 0);

    RECT newButtonLocation = location;
    newButtonLocation.top += occupancy * ( dy );

    V( buttons[occupancy].setAttributes( BUT_COLORDEFAULT, 
        &newButtonLocation, message ) );

    if((long)occupancy == selectedButton)
        buttons[occupancy].setColor( BUT_COLORSELECTED );

    occupancy++;

    return TRUE;
}

int Menu::readInput( UINT nChar )
{
    if( !buttons )
        return MENU_ERROR;

    if( nChar == VK_UP )
    {
        buttons[selectedButton].setColor( BUT_COLORDEFAULT );
        selectedButton--;
        if( selectedButton < 0 )
            selectedButton += occupancy;
        buttons[selectedButton].setColor( BUT_COLORSELECTED );
    }
    else if( nChar == VK_DOWN )
    {
        buttons[selectedButton].setColor( BUT_COLORDEFAULT );
        selectedButton++;
        selectedButton %= occupancy;
        buttons[selectedButton].setColor( BUT_COLORSELECTED );
    }
    else if( nChar == VK_RETURN )
        return selectedButton;

    return MENU_NOCHANGE;
}

void Menu::setColor( D3DCOLOR _color )
{
    color = _color;
}

void Menu::setLocation( const RECT &_location )
{
    location = _location;
}

void Menu::setSize( const size_t _size )
{
    size = _size;
    delete[] buttons;
    buttons = new Button[size];
    occupancy = 0;
}

/*
void Menu::setTexture( LPDIRECT3DTEXTURE9 _pTexture )
{
    pTexture = _pTexture;
}
*/

Menu::~Menu( void )
{
    delete[] buttons;
}

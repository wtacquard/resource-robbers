#include "Hovercraft.h"

Hovercraft::Hovercraft()
: life(100), maxLife(100), thrust(10.0f)
{
	life = maxLife;
}

Hovercraft::~Hovercraft()
{
}

void Hovercraft::applyDamage(int damage)
{
	if(damage > 0)
	{
		if((unsigned int)damage > life)
			life = 0;
		else
			life -= damage;
	}
	else
		life -= damage;
}
void Hovercraft::applyThrust(D3DXVECTOR3* direction)
{
	D3DXVECTOR3 scaledDirection = *direction * thrust;
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);
	applyForce(&center, &scaledDirection);
}

std::string& Hovercraft::serializeGameState(std::string& buffer)
{
	std::string objXml;
	Object::serialize(objXml);
	std::ostringstream os;
	os << "<game>" << 
		objXml <<
		"<type>hovercraft</type>" <<
		"<life>" << life << "</life>"
	"</game>";
	buffer += os.str();
	return buffer;
}
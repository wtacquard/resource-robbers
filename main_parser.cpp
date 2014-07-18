#include "DXUT.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include "common/Serializable.h"
#include "server/Object.h"
#include "game/WorldObject.h"
#include "game/DisplayObject.h"
#include "common/DualBuffer.hh"
#include "common/typedef.h"
#include "game/Merger.hpp"


int main(int argc, char **argv)
{
	Merger::Buffers mem;

	Merger m (mem);
	WorldObject *o = new DisplayObject();
	
	Merger::Buffer* wr = mem.getWriteAccess();
	wr->insert(std::make_pair<ID, WorldObject*>(42, o));
	mem.releaseWriteAccess(&wr);
	mem.swap();
	m.update("<physics><object_id>42</object_id><position><x>1</x><y>2</y><z>3</z></position><orientation><w>4</w><x>5</x><y>6</y><z>7</z></orientation></physics><physics><");


	return (0);
}
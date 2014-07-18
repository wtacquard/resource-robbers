#include "Resource.h"

Resource::Resource(int amount)
: amount(amount)
{
}

int Resource::getAmount()
{
	return amount;
}
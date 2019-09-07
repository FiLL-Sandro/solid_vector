#include "mathematic.h"

u_long multipl_ulong_double(u_long x, double y)
{
	return (u_long)(round(((double)x) * y) + 1.l);
}

double div_ulong_ulong(u_long x, u_long y)
{
	return round((double)x / (double)y);
}

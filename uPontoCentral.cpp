//---------------------------------------------------------------------------

#pragma hdrstop

#include "uPontoCentral.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

PontoCentral::PontoCentral(){
	x = y = 0;
}

PontoCentral::PontoCentral(double nx, double ny){
	x = nx;
	y = ny;
}

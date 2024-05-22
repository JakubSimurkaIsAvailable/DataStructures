#include <libds/amt/hierarchy.h>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/heap_monitor.h>
#include <Windows.h>
#include "NacitavacUJ.h"
#include "AlgoritmusNaPredikat.h"
#include "Program.h"
#include <crtdbg.h>
//#pragma execution_character_set( "utf-8" )
int main()
{
	{
		Program p;
		p.loop();
	}
	initHeapMonitor();
	return 0;
}
		

		

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


extern void set_bus_clk_32m();
extern void set_bus_clk_40m();
extern void set_bus_clk_48m();
extern void set_bus_clk_64m();
extern void set_bus_clk_80m();

void set_bus_clk_32m()
{
    CLKSEL_PLLSEL = 0;
	PLLCTL_PLLON = 1;
    
    SYNR = 0X40 | 0X03;
	REFDV = 0X80 | 0X01;
    POSTDIV = 0X00;
	
    _asm(nop);
	_asm(nop);	
	
	while(!CRGFLG_LOCK)
	{
		;
	}
	CLKSEL_PLLSEL = 1;
}

void set_bus_clk_40m()
{
    CLKSEL_PLLSEL = 0;
	PLLCTL_PLLON = 1;
    
    SYNR = 0X40 | 0X04;
	REFDV = 0X80 | 0X01;
    POSTDIV = 0X00;
	
    _asm(nop);
	_asm(nop);	
	
	while(!CRGFLG_LOCK)
	{
		;
	}
	CLKSEL_PLLSEL = 1;
}

void set_bus_clk_48m()
{
    CLKSEL_PLLSEL = 0;
	PLLCTL_PLLON = 1;
    
    SYNR =0xc0 | 0x05; 
	REFDV=0x80 | 0x01;
    POSTDIV = 0X00;
	
    _asm(nop);
	_asm(nop);	
	
	while(!CRGFLG_LOCK)
	{
		;
	}
	CLKSEL_PLLSEL = 1;
}

void set_bus_clk_64m()
{
    CLKSEL_PLLSEL = 0;
	PLLCTL_PLLON = 1;
    
    SYNR = 0XC0 | 0X07;
	REFDV = 0X80 | 0X01;
    POSTDIV = 0X00;
	
    _asm(nop);
	_asm(nop);	
	
	while(!CRGFLG_LOCK)
	{
		;
	}
	CLKSEL_PLLSEL = 1;
}

void set_bus_clk_80m()
{
    CLKSEL_PLLSEL = 0;
	PLLCTL_PLLON = 1;
    
    SYNR = 0XC0 | 0X08;
	REFDV = 0X80 | 0X01;
    POSTDIV = 0X00;
	
    _asm(nop);
	_asm(nop);	
	
	while(!CRGFLG_LOCK)
	{
		;
	}
	CLKSEL_PLLSEL = 1;
}

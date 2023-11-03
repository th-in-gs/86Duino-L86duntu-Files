/*
  TimerOne.cpp - DM&P Vortex86 TimerOne library
  Copyright (c) 2014 Android Lin <acen@dmp.com.tw>. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  (If you need a commercial license, please contact soc@dmp.com.tw 
   to get more information.)
*/

#ifndef TIMERONE_cpp
#define TIMERONE_cpp

#include "TimerOne.h"
#include <stdio.h>
#include "io.h"
#include "mcm.h"

#if defined (DMP_DOS_DJGPP)
  #include "irq.h"
#endif

TimerOne Timer1;

#if defined (DMP_DOS_DJGPP)
TimerRealTimeClock TimerRTC;

static int mc = 3, md = 2;

/*****************/
/*   Interrupt   */                                                                       
/*****************/
static int mcint_offset[3] = {0, 8, 16};
DMP_INLINE(void) clear_INTSTATUS(void) {
    mc_outp(mc, 0x04, 0xffL << mcint_offset[md]); //for EX
}

DMP_INLINE(void) disable_MCINT(void) {
    mc_outp(mc, 0x00, mc_inp(mc, 0x00) & ~(0xffL << mcint_offset[md]));  // disable mc interrupt
    mc_outp(MC_GENERAL, 0x38, mc_inp(MC_GENERAL, 0x38) | (1L << mc));
}

DMP_INLINE(void) enable_MCINT(unsigned long used_int) {
	mc_outp(MC_GENERAL, 0x38, mc_inp(MC_GENERAL, 0x38) & ~(1L << mc));
	mc_outp(mc, 0x00, (mc_inp(mc, 0x00) & ~(0xffL<<mcint_offset[md])) | (used_int << mcint_offset[md]));
}

// TimerOne ISR
static char* isrname_one = "TimerOne";
DMP_INLINE(int) timer1_isr_handler(int irq, void* data) {
    if((mc_inp(mc, 0x04) & (PULSE_END_INT << mcint_offset[md])) == 0) return ISR_NONE;
    
		mc_outp(mc, 0x04, (PULSE_END_INT << mcint_offset[md]));
    
    if(Timer1.isrCallback != NULL) Timer1.isrCallback();
     
    return ISR_HANDLED;
}

DMP_INLINE(bool) init_mc_irq(void) {
	static bool timerOneInitInt = false;
	if(timerOneInitInt == false)
	{	
		if(irq_InstallISR(GetMCIRQ(), timer1_isr_handler, isrname_one) == false)
		{
		    printf("irq_install fail\n"); return false;
		}
		timerOneInitInt = true;
	}
    return true;
}
#endif

DMP_INLINE(void) pwmInit(int mcn, int mdn) {
	mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_CANCEL);
	mcpwm_SetOutMask(mcn, mdn, MCPWM_HMASK_NONE + MCPWM_LMASK_NONE);
	mcpwm_SetOutPolarity(mcn, mdn, MCPWM_HPOL_NORMAL + MCPWM_LPOL_NORMAL);
	mcpwm_SetDeadband(mcn, mdn, 0L);
	mcpwm_ReloadOUT_Unsafe(mcn, mdn, MCPWM_RELOAD_NOW);
	
	mcpwm_SetWaveform(mcn, mdn, MCPWM_EDGE_A0I1);
	mcpwm_SetSamplCycle(mcn, mdn, 1999L); // sample cycle: 20ms
}

static bool timerOneInit = false;
DMPAPI(void) TimerOne::initialize(long microseconds) {
	timerOneInit = true;
	setPeriod(microseconds);
}

#define YES    (1)
#define NO     (2)
static int mc_md_inuse[PINS];
static int isPwmInit[PINS];
static unsigned long _fperiod[PINS]; // used for freePwm() and disableFreePwm()
static unsigned long _fduty[PINS]; // used for freePwm() and disableFreePwm()
static unsigned long _duty[PINS];

TimerOne::TimerOne() {
	isrCallback = NULL;
	timer1Enable = false;
}

DMPAPI(void) TimerOne::setPeriod(long microseconds)	{ // AR modified for atomic access
	int mcn, mdn, i;
	if(timerOneInit == false || microseconds <= 0) return;
	
	periodMicroseconds = microseconds > 0 ? microseconds : 1;
	pwmPeriod = microseconds;
	fixedperiod = microseconds*SYSCLK;

#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)	
	if(timer1Enable == true) // if call attachInterrupt() or start(), timer1Enable is ture
	{
		mcpwm_SetWidth(mc, md, fixedperiod-1L, 0L);
	    mcpwm_ReloadPWM(mc, md, MCPWM_RELOAD_PEREND);
	}
#endif

#if defined (DMP_LINUX)
	lockMCM(mcn, mdn);
#endif
	for(i=0; i<PINS; i++)
	{
		if(mc_md_inuse[i] == YES) // if call pwm()...
		{
			mcn = arduino_to_mc_md[0][i];
	    	mdn = arduino_to_mc_md[1][i];
			if(mcpwm_ReadReloadPWM(mcn, mdn) != 0) mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_CANCEL);
			mcpwm_SetWidth(mcn, mdn, fixedperiod-1L, _duty[i]-1L);
		    mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_PEREND);
		}
	}
#if defined (DMP_LINUX)
	unLockMCM(mcn, mdn);
#endif
}

DMP_INLINE(void) safeClosePwmModule(int mcn, int mdn, unsigned long period) {
	if(mcpwm_ReadReloadPWM(mcn, mdn) != 0) mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_CANCEL);
	mcpwm_SetWidth(mcn, mdn, period-1L, 0L);
	mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_PEREND);
	while(mcpwm_ReadReloadPWM(mcn, mdn) != 0L);
	while(mcpwm_ReadSTATREG2(mcn, mdn) > (period - 1L));
	mcpwm_Disable(mcn, mdn);
}

DMPAPI(void) TimerOne::setPwmDuty(char pin, int duty) {
	int mcn, mdn;
	unsigned short crossbar_ioaddr = sb_Read16(0x64)&0xfffe;

	if(pin < 0 || pin >= PINS || timerOneInit == false) return;

	mcn = arduino_to_mc_md[0][pin];
    mdn = arduino_to_mc_md[1][pin];
    
    if(mcn == NOPWM || mdn == NOPWM) return;

#if defined (DMP_LINUX)
	lockMCM(mcn, mdn);
#endif

	if(duty <= 0)
	{
		if(mc_md_inuse[pin] == YES)
		{
			safeClosePwmModule(mcn, mdn, fixedperiod);
			mc_md_inuse[pin] = NO;
		}
		#if defined (DMP_LINUX)
			unLockMCM(mcn, mdn);
		#endif
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
		return;
	}

#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)		
	io_DisableINT();
#endif
    if(mc_md_inuse[pin] == 0 || mc_md_inuse[pin] == NO)
	{
		pwmInit(mcn, mdn);
		
		if (pin <= 9)
			io_outpb(crossbar_ioaddr + 2, 0x01); // GPIO port2: 0A, 0B, 0C, 3A
		else if (pin > 28)
	    	io_outpb(crossbar_ioaddr, 0x03); // GPIO port0: 2A, 2B, 2C, 3C
		else
			io_outpb(crossbar_ioaddr + 3, 0x02); // GPIO port3: 1A, 1B, 1C, 3B
    }

	if(mcpwm_ReadReloadPWM(mcn, mdn) != 0) mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_CANCEL);
	_duty[pin] = (fixedperiod*duty)/1024L;
	mcpwm_SetWidth(mcn, mdn, fixedperiod-1L, _duty[pin]-1L);
	mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_PEREND);

#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)		
    io_RestoreINT();
#endif
	
    if(isPwmInit[pin] == 1 && (mc_md_inuse[pin] == 0 || mc_md_inuse[pin] == NO))
    {
    	io_outpb(crossbar_ioaddr + 0x90 + pinMap[pin], 0x08); // Enable PWM pin
		mcpwm_Enable(mcn, mdn);
		mc_md_inuse[pin] = YES;
    }

#if defined (DMP_LINUX)
	unLockMCM(mcn, mdn);
#endif
}

DMPAPI(void) TimerOne::pwm(char pin, int duty, long microseconds) { // expects duty cycle to be 10 bit (1024)
	int mcn, mdn;
	if(pin < 0 || pin >= PINS || timerOneInit == false) return;
	
	mcn = arduino_to_mc_md[0][pin];
	mdn = arduino_to_mc_md[1][pin];
	  
	if(mcn == NOPWM || mdn == NOPWM) return;
	
	isPwmInit[pin] = 1; // for Arduino compatibility

	if(microseconds > 0) setPeriod(microseconds);

  	setPwmDuty(pin, duty);
}

DMPAPI(void) TimerOne::freePwm(char pin, unsigned long duty, unsigned long microseconds) {
	int mcn, mdn;
	unsigned short crossbar_ioaddr = sb_Read16(0x64)&0xfffe;
	if(pin < 0 || pin >= PINS || microseconds == 0) return;
	
	mcn = arduino_to_mc_md[0][pin];
	mdn = arduino_to_mc_md[1][pin];
	  
	if(mcn == NOPWM || mdn == NOPWM) return;

#if defined (DMP_LINUX)
	lockMCM(mcn, mdn);
#endif

	if(duty == 0)
	{
		if(mc_md_inuse[pin] == YES)
		{
			safeClosePwmModule(mcn, mdn, microseconds*SYSCLK);
			mc_md_inuse[pin] = NO;
		}
		#if defined (DMP_LINUX)
			unLockMCM(mcn, mdn);
		#endif
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
		return;
	}

	_fperiod[pin] = microseconds*SYSCLK;
	_fduty[pin] = duty*SYSCLK;
#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)		
	io_DisableINT();
#endif
    if(mc_md_inuse[pin] == 0 || mc_md_inuse[pin] == NO)
	{
		pwmInit(mcn, mdn);
		
		if (pin <= 9)
			io_outpb(crossbar_ioaddr + 2, 0x01); // GPIO port2: 0A, 0B, 0C, 3A
		else if (pin > 28)
	    	io_outpb(crossbar_ioaddr, 0x03); // GPIO port0: 2A, 2B, 2C, 3C
		else
			io_outpb(crossbar_ioaddr + 3, 0x02); // GPIO port3: 1A, 1B, 1C, 3B
    }

	if(mcpwm_ReadReloadPWM(mcn, mdn) != 0) mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_CANCEL);

	mcpwm_SetWidth(mcn, mdn, _fperiod[pin]-1L, _fduty[pin]-1L);
	mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_PEREND);

#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)		
    io_RestoreINT();
#endif
	
    if(mc_md_inuse[pin] == 0 || mc_md_inuse[pin] == NO)
    {
    	io_outpb(crossbar_ioaddr + 0x90 + pinMap[pin], 0x08); // Enable PWM pin
		mcpwm_Enable(mcn, mdn);
		mc_md_inuse[pin] = YES;
    }

#if defined (DMP_LINUX)
	unLockMCM(mcn, mdn);
#endif
}

DMPAPI(void) TimerOne::disablePwm(char pin) {
	unsigned short crossbar_ioaddr = sb_Read16(0x64)&0xfffe;
	int mcn, mdn;
	
	if(pin < 0 || pin >= PINS || timerOneInit == false) return;
	
	mcn = arduino_to_mc_md[0][pin];
    mdn = arduino_to_mc_md[1][pin];
    
    if(mcn == NOPWM || mdn == NOPWM) return;

#if defined (DMP_LINUX)
	lockMCM(mcn, mdn);
#endif	
	if(mc_md_inuse[pin] == YES)
	{
		safeClosePwmModule(mcn, mdn, fixedperiod);
		mc_md_inuse[pin] = NO;
		isPwmInit[pin] = 0;
		io_outpb(crossbar_ioaddr + 0x90 + pinMap[pin], 0x01); // Disable PWM pin (To be GPIO)
	}
#if defined (DMP_LINUX)
	unLockMCM(mcn, mdn);
#endif
}

DMPAPI(void) TimerOne::disableFreePwm(char pin) {
	unsigned short crossbar_ioaddr = sb_Read16(0x64)&0xfffe;
	int mcn, mdn;
	
	if(pin < 0 || pin >= PINS) return;
	
	mcn = arduino_to_mc_md[0][pin];
    mdn = arduino_to_mc_md[1][pin];
    
    if(mcn == NOPWM || mdn == NOPWM) return;

#if defined (DMP_LINUX)
	lockMCM(mcn, mdn);
#endif	
	if(mc_md_inuse[pin] == YES)
	{
		safeClosePwmModule(mcn, mdn, _fperiod[pin]);
		mc_md_inuse[pin] = NO;
		io_outpb(crossbar_ioaddr + 0x90 + pinMap[pin], 0x01); // Disable PWM pin (To be GPIO)
	}
#if defined (DMP_LINUX)
	unLockMCM(mcn, mdn);
#endif
}
 
DMPAPI(void) TimerOne::attachInterrupt(void (*isr)(void), long microseconds) {
#if defined (DMP_DOS_DJGPP)
	int i;
	if(timerOneInit == false || isr == NULL) return;
	mcpwm_Disable(mc, md);
	
	if(microseconds > 0)                       
	{
		setPeriod(microseconds);
	} 
	
	pwmInit(mc, md);
	disable_MCINT();
    
	clear_INTSTATUS();
	if(init_mc_irq() == false)
	{
		printf("Init MC IRQ fail\n");
		return;
    }
    enable_MCINT(PULSE_END_INT);

	mcpwm_SetWidth(mc, md, fixedperiod-1L, 0L);
	
	io_DisableINT();
	isrCallback = isr; // register the user's callback with the real ISR
	io_RestoreINT();
	
	mcpwm_Enable(mc, md);
	timer1Enable = true;
#elif defined (DMP_LINUX)
	if(microseconds < 0)
		microseconds = periodMicroseconds;

	attachTimerInterrupt(12, isr, microseconds);
#endif
}

DMPAPI(void) TimerOne::detachInterrupt() {
#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)
	if(timerOneInit == false || timer1Enable == false) return;
	disable_MCINT(); // timer continues to count without calling the isr
#elif defined (DMP_LINUX)
	detachTimerInterrupt();
#endif
}

DMPAPI(void) TimerOne::resume() {
	start();
}

DMPAPI(void) TimerOne::restart() { // Depricated - Public interface to start at zero - Lex 10/9/2011
	start();
}

DMPAPI(void) TimerOne::start() { // AR addition, renamed by Lex to reflect it's actual role
	int mcn, mdn, i;

#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)
	if(timer1Enable == false)
	{
		mcpwm_SetWidth(mc, md, fixedperiod-1L, 0L);
		mcpwm_Enable(mc, md);
		timer1Enable = true;
	}
#endif

	for(i=0; i<PINS; i++)
	{
		if(mc_md_inuse[i] == NO)
		{
			mcn = arduino_to_mc_md[0][i];
			mdn = arduino_to_mc_md[1][i];
			#if defined (DMP_LINUX)
			lockMCM(mcn, mdn);
			#endif

			if(mcpwm_ReadReloadPWM(mcn, mdn) != 0) mcpwm_ReloadPWM(mcn, mdn, MCPWM_RELOAD_CANCEL);
			mcpwm_SetWidth(mcn, mdn, fixedperiod-1L, _duty[i]-1L);
			mcpwm_Enable(mcn, mdn);
			mc_md_inuse[i] = YES;

			#if defined (DMP_LINUX)
			unLockMCM(mcn, mdn);
			#endif
		}
	}
}

DMPAPI(void) TimerOne::stop() {
	int mcn, mdn, i;

#if defined (DMP_DOS_BC) || defined (DMP_DOS_DJGPP)
	if(timer1Enable == true)
	{
		mcpwm_Disable(mc, md);
		timer1Enable = false;
	}
#endif


	for(i=0; i<PINS; i++)
	{
		if(mc_md_inuse[i] == YES)
		{
			mcn = arduino_to_mc_md[0][i];
			mdn = arduino_to_mc_md[1][i];
			#if defined (DMP_LINUX)
			lockMCM(mcn, mdn);
			#endif

			mcpwm_Disable(mcn, mdn);
			mc_md_inuse[i] = NO;

			#if defined (DMP_LINUX)
			unLockMCM(mcn, mdn);
			#endif
		}
	}
}

DMPAPI(unsigned long) TimerOne::read() {	//returns the value of the timer in microseconds
#if defined (DMP_DOS_DJGPP)
	unsigned long tmp = mcpwm_ReadSTATREG2(mc, md);
	return ((fixedperiod - tmp) / SYSCLK);
#endif
}

#if defined (DMP_DOS_DJGPP)

/* for debugging
unsigned int testcount = 0;
unsigned int TimerRealTimeClock::readTestCount(void) {
	unsigned int tmp;
	io_DisableINT();
	tmp = testcount;
	io_EnableINT();
	return tmp;
}*/

#define RTCIRQ    (8)
TimerRealTimeClock::TimerRealTimeClock() {
	isrCallback = NULL;
	timerRTCEnable = false;
}

static char* isrname_rtc = "TimerRTC";
DMP_INLINE(int) timerrtc_isr_handler(int irq, void* data) {
	unsigned char tmp;
	io_outpb(0x70, 0x0C); // enable NMI and read RTC register C 
	tmp = io_inpb(0x71); // clear RTC interrupt state

	if((tmp & 0x40) > 0 && TimerRTC.isrCallback != NULL) TimerRTC.isrCallback();
    if((tmp & 0x70) == 0) return ISR_NONE;

    return ISR_HANDLED;
}

DMP_INLINE(unsigned char) inpb_cmos(unsigned char reg) {
    unsigned char tmp;
	io_DisableINT();
    io_outpb(0x70, 0x80 | reg); // disable NMI (by setting the 0x80 bit) and assign a RTC register address
    tmp = io_inpb(0x71);
    io_RestoreINT();
    return tmp;
}

DMP_INLINE(void) outpb_cmos(unsigned char reg, unsigned char data) {
    io_DisableINT();
    io_outpb(0x70, 0x80 | reg); // disable NMI (by setting the 0x80 bit) and assign a RTC register address
    io_outpb(0x71, data);
    io_RestoreINT();
}

static bool timerRTCInit = false;
DMPAPI(void) TimerRealTimeClock::initialize(long microseconds) {
    unsigned char tmp;
    timerRTCInit = true;
    setPeriod(microseconds);
    
    tmp = inpb_cmos(0x0A);
    outpb_cmos(0x0A, (tmp & 0xF0) | _freq);
}

DMPAPI(void) TimerRealTimeClock::attachInterrupt(void (*isr)(void), long microseconds) {
    unsigned char tmp;
    
    if(timerRTCInit == false || isr == NULL) return;
    
    if(timerRTCEnable == false)
	{
		if(irq_Setting(RTCIRQ, IRQ_EDGE_TRIGGER) == false)
	    {
	        printf("MCM IRQ Setting fail\n"); return;
	    }
		if(irq_InstallISR(RTCIRQ, timerrtc_isr_handler, isrname_rtc) == false)
		{
		    printf("irq_install fail\n"); return;
		}
	}
	
    if(microseconds > 0) setPeriod(microseconds);
    
    io_DisableINT();
    isrCallback = isr;
    io_RestoreINT();
    
	tmp = inpb_cmos(0x0B);
    outpb_cmos(0x0B, tmp | 0x40);
    timerRTCEnable = true;
}

DMPAPI(void) TimerRealTimeClock::detachInterrupt() {
    unsigned char tmp;
    if(timerRTCInit == false || timerRTCEnable == false) return;
    
	tmp = inpb_cmos(0x0B);
    outpb_cmos(0x0B, tmp & ~(0x40));
    
    irq_UninstallISR(RTCIRQ, (void*)isrname_rtc);
    timerRTCEnable = false;
}

DMPAPI(void) TimerRealTimeClock::setPeriod(long microseconds) {
	if(timerRTCInit == false || microseconds <= 0) return;
	
	if    (microseconds < 183L)    _freq = 3;
	else if(microseconds < 366L)    _freq = 4;
	else if(microseconds < 732L)    _freq = 5;
	else if(microseconds < 1464L)   _freq = 6;
	else if(microseconds < 2929L)   _freq = 7;
	else if(microseconds < 5859L)   _freq = 8;
	else if(microseconds < 11718L)  _freq = 9;
	else if(microseconds < 23437L)  _freq = 10;
	else if(microseconds < 46875L)  _freq = 11;
	else if(microseconds < 93750L)  _freq = 12;
	else if(microseconds < 187500L) _freq = 13;
	else if(microseconds < 375000L) _freq = 14;
	else if(microseconds < 500000L) _freq = 15;
	else                            _freq = 15;
}
#endif

#endif
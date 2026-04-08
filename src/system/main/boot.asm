; -------------------------------------------------------------------------
; Main CPU boot program
; -------------------------------------------------------------------------

	include	"src/include/main_cpu.inc"

	org	RAM_START
	
; -------------------------------------------------------------------------
; Security block
; -------------------------------------------------------------------------

	if REGION=JAPAN
		include	"src/system/main/security_japan.asm"
	elseif REGION=USA
		include	"src/system/main/security_usa.asm"
	else
		include	"src/system/main/security_europe.asm"
	endif

; -------------------------------------------------------------------------
; Main function
; -------------------------------------------------------------------------

	move	#$2700,sr					; Stop interrupts
	move.w	#_LEVEL4,HBLANK_USER				; Set user H-BLANK interrupt

	lea	COMM_CMDS,a0					; Clear communication flags
	moveq	#0,d0
	move.b	d0,MAIN_FLAG-COMM_CMDS(a0)
	move.l	d0,(a0)+
	move.l	d0,(a0)+
	move.l	d0,(a0)+
	move.l	d0,(a0)+

	GIVE_WORD_ACCESS					; Wait for main program file to be read
	WAIT_WORD_ACCESS

	lea	LoadProgram(pc),a0				; Program loader
	lea	RAM_START+$F700,a1				; Destination buffer
	move.w	#(LoadProgramEnd-LoadProgram)/2-1,d0		; Size of program loader

.Copy:
	move.w	(a0)+,(a1)+					; Copy program loader
	dbf	d0,.Copy					; Loop until finished

	jmp	RAM_START+$F700.w				; Jump to program loader

; -------------------------------------------------------------------------
; Program loader
; -------------------------------------------------------------------------

LoadProgram:
	obj	RAM_START+$F700
	lea	WORD_START,a0					; Source buffer
	lea	RAM_START,a1					; Destination buffer
	move.w	#filesize("out/files/MAIN.M68")/16-1,d0		; Size of program

.Copy:
	move.l	(a0)+,(a1)+					; Copy program 
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	dbf	d0,.Copy					; Loop until finished

	jmp	RAM_START					; Jump to program
	objend
LoadProgramEnd:

; -------------------------------------------------------------------------

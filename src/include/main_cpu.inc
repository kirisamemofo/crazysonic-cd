; -------------------------------------------------------------------------
; Main CPU definitions and macros
; -------------------------------------------------------------------------

	include	"src/include/common.inc"

; -------------------------------------------------------------------------
; Memory map
; -------------------------------------------------------------------------

; Mega CD area
MCD_START		EQU	$000000				; Mega CD area start
MCD_SIZE		EQU	$400000				; Mega CD area size
MCD_END			EQU	MCD_START+MCD_SIZE		; Mega CD area end

; Cartridge
CART_START		EQU	$400000				; Cartridge area start
CART_SIZE		EQU	$400000				; Cartridge area size
CART_END		EQU	CART_START+CART_SIZE		; Cartridge area end

; BIOS
BIOS_START		EQU	MCD_START			; BIOS area start
BIOS_SIZE		EQU	$20000				; BIOS area size
BIOS_END		EQU	BIOS_START+BIOS_SIZE		; BIOS area end

; Program RAM bank
PRG_START		EQU	MCD_START+$20000		; Program RAM bank area start
PRG_SIZE		EQU	$20000				; Program RAM bank area size
PRG_END			EQU	PRG_START+PRG_SIZE		; Program RAM bank area end

; Word RAM
WORD_START		EQU	MCD_START+$200000		; Word RAM area start
WORD_SIZE_2M		EQU	$40000				; Word RAM area size (2M)
WORD_SIZE_1M		EQU	$20000				; Word RAM area size (1M/1M)
WORD_END_2M		EQU	WORD_START+WORD_SIZE_2M		; Word RAM area end (2M)
WORD_END_1M		EQU	WORD_START+WORD_SIZE_1M		; Word RAM area end (1M/1M)

; Word RAM image
IMAGE_START		EQU	MCD_START+$220000		; Word RAM image area start (1M/1M)
IMAGE_SIZE		EQU	$20000				; Word RAM image area size (1M/1M)
IMAGE_END		EQU	IMAGE_START+IMAGE_SIZE		; Word RAM image area end (1M/1M)

; Z80 RAM
Z80_START		EQU	$A00000				; Z80 RAM area start
Z80_SIZE		EQU	$2000				; Z80 RAM area size
Z80_END			EQU	Z80_START+Z80_SIZE		; Z80 RAM area end

; YM2612 registers
YM_ADDR_0		EQU	$A04000				; YM2612 address port 0
YM_DATA_0		EQU	$A04001				; YM2612 data port 0
YM_ADDR_1		EQU	$A04002				; YM2612 address port 1
YM_DATA_1		EQU	$A04003				; YM2612 data port 1

; I/O registers
VERSION			EQU	$A10001				; Hardware version
IO_DATA_1		EQU	$A10003				; I/O port 1 data
IO_DATA_2		EQU	$A10005				; I/O port 2 data
IO_DATA_3		EQU	$A10007				; I/O port 3 data
IO_CTRL_1		EQU	$A10009				; I/O port 1 control
IO_CTRL_2		EQU	$A1000B				; I/O port 2 control
IO_CTRL_3		EQU	$A1000D				; I/O port 3 control

; Z80 registers
Z80_BUS			EQU	$A11100				; Z80 bus request
Z80_RESET		EQU	$A11200				; Z80 reset

; Gate array registers
GATE_ARRAY		EQU	$A12000				; Gate array registers start
SUB_INT2		EQU	GATE_ARRAY			; Sub CPU level 2 interrupt status/request
	BIT_IFL2:		EQU	0
	BIT_IEN2:		EQU	7
SUB_BUS			EQU	GATE_ARRAY+1			; Sub CPU bus reset/request
	BIT_SRES:		EQU	0
	BIT_SBRQ:		EQU	1
SUB_PROTECT		EQU	GATE_ARRAY+2			; Sub CPU memory protection
MEMORY_MODE		EQU	GATE_ARRAY+3			; Memory mode
	BIT_RET:		EQU	0
	BIT_DMNA:		EQU	1
	BIT_MODE:		EQU	2
CDC_MODE		EQU	GATE_ARRAY+4			; CDC mode
	BIT_DSR:		EQU	6
	BIT_EDT:		EQU	7
HBLANK_USER		EQU	GATE_ARRAY+6			; H-INT user vector
CDC_HOST		EQU	GATE_ARRAY+8			; CDC host data
STOP_WATCH		EQU	GATE_ARRAY+$C			; Stop watch
COMM_FLAGS		EQU	GATE_ARRAY+$E			; Communication flags
MAIN_FLAG		EQU	GATE_ARRAY+$E			; Main CPU communication flag
SUB_FLAG		EQU	GATE_ARRAY+$F			; Sub CPU communication flag
COMM_CMDS		EQU	GATE_ARRAY+$10			; Communication commands
COMM_CMD_0		EQU	GATE_ARRAY+$10			; Communication command 0
COMM_CMD_1		EQU	GATE_ARRAY+$12			; Communication command 1
COMM_CMD_2		EQU	GATE_ARRAY+$14			; Communication command 2
COMM_CMD_3		EQU	GATE_ARRAY+$16			; Communication command 3
COMM_CMD_4		EQU	GATE_ARRAY+$18			; Communication command 4
COMM_CMD_5		EQU	GATE_ARRAY+$1A			; Communication command 5
COMM_CMD_6		EQU	GATE_ARRAY+$1C			; Communication command 6
COMM_CMD_7		EQU	GATE_ARRAY+$1E			; Communication command 7
COMM_STATS		EQU	GATE_ARRAY+$20			; Communication statuses
COMM_STAT_0		EQU	GATE_ARRAY+$20			; Communication status 0
COMM_STAT_1		EQU	GATE_ARRAY+$22			; Communication status 1
COMM_STAT_2		EQU	GATE_ARRAY+$24			; Communication status 2
COMM_STAT_3		EQU	GATE_ARRAY+$26			; Communication status 3
COMM_STAT_4		EQU	GATE_ARRAY+$28			; Communication status 4
COMM_STAT_5		EQU	GATE_ARRAY+$2A			; Communication status 5
COMM_STAT_6		EQU	GATE_ARRAY+$2C			; Communication status 6
COMM_STAT_7		EQU	GATE_ARRAY+$2E			; Communication status 7

; VDP registers
VDP_DATA		EQU	$C00000				; VDP data port
VDP_CTRL		EQU	$C00004				; VDP control port
VDP_HV			EQU	$C00008				; VDP H/V counter
PSG_CTRL		EQU	$C00011				; PSG control port

; Work RAM
RAM_START		EQU	$FFFF0000			; Work RAM start
RAM_SIZE		EQU	$10000				; Work RAM size
RAM_END			EQU	RAM_START+RAM_SIZE		; Work RAM end

; Work RAM addresses
_EXCPT			EQU	RAM_START+$FD00			; Exception
_LEVEL6			EQU	RAM_START+$FD06			; V-BLANK interrupt
_LEVEL4			EQU	RAM_START+$FD0C			; H-BLANK interrupt
_LEVEL2			EQU	RAM_START+$FD12			; External interrupt
_TRAP00			EQU	RAM_START+$FD18			; TRAP #00
_TRAP01			EQU	RAM_START+$FD1E			; TRAP #01
_TRAP02			EQU	RAM_START+$FD24			; TRAP #02
_TRAP03			EQU	RAM_START+$FD2A			; TRAP #03
_TRAP04			EQU	RAM_START+$FD30			; TRAP #04
_TRAP05			EQU	RAM_START+$FD36			; TRAP #05
_TRAP06			EQU	RAM_START+$FD3C			; TRAP #06
_TRAP07			EQU	RAM_START+$FD42			; TRAP #07
_TRAP08			EQU	RAM_START+$FD48			; TRAP #08
_TRAP09			EQU	RAM_START+$FD4E			; TRAP #09
_TRAP10			EQU	RAM_START+$FD54			; TRAP #10
_TRAP11			EQU	RAM_START+$FD5A			; TRAP #11
_TRAP12			EQU	RAM_START+$FD60			; TRAP #12
_TRAP13			EQU	RAM_START+$FD66			; TRAP #13
_TRAP14			EQU	RAM_START+$FD6C			; TRAP #14
_TRAP15			EQU	RAM_START+$FD72			; TRAP #15
_CHKERR			EQU	RAM_START+$FD78			; CHK exception
_ADRERR			EQU	RAM_START+$FD7E			; Address error
_CODERR			EQU	RAM_START+$FD7E			; Illegal instruction
_DIVERR			EQU	RAM_START+$FD84			; Division by zero
_TRPERR			EQU	RAM_START+$FD8A			; TRAPV exception
_NOCOD0			EQU	RAM_START+$FD90			; Line A emulator
_NOCOD1			EQU	RAM_START+$FD96			; Line F emulator
_SPVERR			EQU	RAM_START+$FD9C			; Privilege violation
_TRACE			EQU	RAM_START+$FDA2			; TRACE exception
_BURAM			EQU	RAM_START+$FDAE			; Cartridge Backup RAM handler

; Program RAM (Sub CPU)
SUB_PRG_START		EQU	$000000				; Program RAM area start
SUB_PRG_SIZE		EQU	$80000				; Program RAM area size
SUB_PRG_END		EQU	SUB_PRG_START+SUB_PRG_SIZE	; Program RAM area end

; Word RAM (Sub CPU)
SUB_WORD_START_2M	EQU	$080000				; Word RAM area start (2M)
SUB_WORD_START_1M	EQU	$0C0000				; Word RAM area start (1M/1M)
SUB_WORD_END_2M		EQU	SUB_WORD_START_2M+WORD_SIZE_2M	; Word RAM area end (2M)
SUB_WORD_END_1M		EQU	SUB_WORD_START_1M+WORD_SIZE_1M	; Word RAM area end (1M/1M)

; -------------------------------------------------------------------------
; Backup RAM function codes
; -------------------------------------------------------------------------

BRMINIT			EQU	$00				; Initialize Backup RAM handling
BRMSTAT			EQU	$01				; Get Backup RAM status
BRMSERCH		EQU	$02				; Search Backup RAM
BRMREAD			EQU	$03				; Read from Backup RAM
BRMWRITE		EQU	$04				; Write to Backup RAM
BRMDEL			EQU	$05				; Delete data from Backup RAM
BRMFORMAT		EQU	$06				; Format Backup RAM
BRMDIR			EQU	$07				; Get directory in Backup RAM
BRMVERIFY		EQU	$08				; Verify Backup RAM data

; -------------------------------------------------------------------------
; VDP command base values
; -------------------------------------------------------------------------

VRAM_WRITE_CMD		EQU	$40000000			; VRAM write
CRAM_WRITE_CMD		EQU	$C0000000			; CRAM write
VSRAM_WRITE_CMD		EQU	$40000010			; VSRAM write
VRAM_READ_CMD		EQU	$00000000			; VRAM read
CRAM_READ_CMD		EQU	$00000020			; CRAM read
VSRAM_READ_CMD		EQU	$00000010			; VSRAM read
VRAM_DMA_CMD		EQU	$40000080			; VRAM DMA
CRAM_DMA_CMD		EQU	$C0000080			; CRAM DMA
VSRAM_DMA_CMD		EQU	$40000090			; VSRAM DMA
VRAM_COPY_CMD		EQU	$000000C0			; VRAM DMA copy

; -------------------------------------------------------------------------
; Check if the Sub CPU's level 2 interrupt is enabled
; 0 = Disabled
; 1 = Enabled
; -------------------------------------------------------------------------

CHECK_INT2 macro
	btst	#BIT_IEN2,SUB_INT2
	endm
	
; -------------------------------------------------------------------------
; Request a level 2 interrupt on the Sub CPU
; -------------------------------------------------------------------------

REQUEST_INT2 macro
	bset	#BIT_IFL2,SUB_INT2
	endm

; -------------------------------------------------------------------------
; Give Word RAM access to the Sub CPU (2M)
; -------------------------------------------------------------------------

GIVE_WORD_ACCESS macro
.Give\@:
	bset	#BIT_DMNA,MEMORY_MODE
	beq.s	.Give\@
	endm

; -------------------------------------------------------------------------
; Check Word RAM access (2M)
; 0 = No access
; 1 = Access
; -------------------------------------------------------------------------

CHECK_WORD_ACCESS macro
	btst	#BIT_RET,MEMORY_MODE
	endm

; -------------------------------------------------------------------------
; Wait for Word RAM access (2M)
; -------------------------------------------------------------------------

WAIT_WORD_ACCESS macro
.Wait\@:
	CHECK_WORD_ACCESS
	beq.s	.Wait\@
	endm

; -------------------------------------------------------------------------
; Check Word RAM bank access (1M/1M)
; 0 = Bank 0
; 1 = Bank 1
; -------------------------------------------------------------------------

CHECK_WORD_BANK macro
	btst	#BIT_RET,MEMORY_MODE
	endm

; -------------------------------------------------------------------------
; Wait for Word RAM bank 0 access (1M/1M)
; -------------------------------------------------------------------------

WAIT_WORD_BANK_0 macro
.Wait\@:
	CHECK_WORD_BANK
	bne.s	.Wait\@
	endm

; -------------------------------------------------------------------------
; Wait for Word RAM bank 1 access (1M/1M)
; -------------------------------------------------------------------------

WAIT_WORD_BANK_1 macro
.Wait\@:
	CHECK_WORD_BANK
	beq.s	.Wait\@
	endm

; -------------------------------------------------------------------------
; Check if 2M mode
; 0 = 2M mode
; 1 = 1M/1M mode
; -------------------------------------------------------------------------

CHECK_2M_MODE macro
	btst	#BIT_MODE,MEMORY_MODE
	endm

; -------------------------------------------------------------------------
; Wait for 2M mode
; -------------------------------------------------------------------------

WAIT_2M_MODE macro
.Wait\@:
	CHECK_2M_MODE
	bne.s	.Wait\@
	endm

; -------------------------------------------------------------------------
; Wait for 1M/1M mode
; -------------------------------------------------------------------------

WAIT_1M_MODE macro
.Wait\@:
	CHECK_2M_MODE
	beq.s	.Wait\@
	endm

; -------------------------------------------------------------------------
; Reqeust Z80 bus access
; -------------------------------------------------------------------------

REQUEST_Z80 macro
	move.w	#$100,Z80_BUS
	endm

; -------------------------------------------------------------------------
; Wait for Z80 bus acknowledgement
; -------------------------------------------------------------------------

WAIT_Z80 macro
.Wait\@:
	btst	#0,Z80_BUS
	bne.s	.Wait\@
	endm

; -------------------------------------------------------------------------
; Stop the Z80 and get bus access
; -------------------------------------------------------------------------

STOP_Z80 macro
	REQUEST_Z80
	WAIT_Z80
	endm

; -------------------------------------------------------------------------
; Start the Z80 and release bus access
; -------------------------------------------------------------------------

START_Z80 macro
	move.w	#0,Z80_BUS
	endm

; -------------------------------------------------------------------------
; Start Z80 reset
; -------------------------------------------------------------------------

RESET_Z80_ON macro
	move.w	#0,Z80_RESET
	ror.b	#8,d0
	endm

; -------------------------------------------------------------------------
; Stop Z80 reset
; -------------------------------------------------------------------------

RESET_Z80_OFF macro
	move.w	#$100,Z80_RESET
	endm

; -------------------------------------------------------------------------
; Wait for a VDP DMA to finish
; -------------------------------------------------------------------------
; PARAMETERS:
;	ctrl - VDP control port (optional)
; -------------------------------------------------------------------------

WAIT_DMA macro ctrl
.Wait\@:
	if narg>0
		move	\ctrl,ccr
	else
		move	VDP_CTRL,ccr
	endif
	bvs.s	.Wait\@
	endm

; -------------------------------------------------------------------------
; VDP command instruction
; -------------------------------------------------------------------------
; PARAMETERS:
;	ins  - Instruction
;	addr - Address in VDP memory
;	type - Type of VDP memory
;	rwd  - VDP command
;	dest - Destination
; -------------------------------------------------------------------------

VDP_CMD macro ins, addr, type, rwd, dest
	local cmd
	cmd: = (\type\_\rwd\_CMD)|(((\addr)&$3FFF)<<16)|((\addr)/$4000)
	if narg=5
		\ins	#\#cmd,\dest
	else
		\ins	cmd
	endif
	endm

; -------------------------------------------------------------------------
; VDP command instruction (low word)
; -------------------------------------------------------------------------
; PARAMETERS:
;	ins  - Instruction
;	addr - Address in VDP memory
;	type - Type of VDP memory
;	rwd  - VDP command
;	dest - Destination
; -------------------------------------------------------------------------

VDP_CMD_LO macro ins, addr, type, rwd, dest
	local cmd
	cmd: = ((\type\_\rwd\_CMD)&$FFFF)|((\addr)/$4000)
	if narg=5
		\ins	#\#cmd,\dest
	else
		\ins	cmd
	endif
	endm

; -------------------------------------------------------------------------
; VDP command instruction (high word)
; -------------------------------------------------------------------------
; PARAMETERS:
;	ins  - Instruction
;	addr - Address in VDP memory
;	type - Type of VDP memory
;	rwd  - VDP command
;	dest - Destination
; -------------------------------------------------------------------------

VDP_CMD_HI macro ins, addr, type, rwd, dest
	local cmd
	cmd: = ((\type\_\rwd\_CMD)>>16)|((\addr)&$3FFF)
	if narg=5
		\ins	#\#cmd,\dest
	else
		\ins	cmd
	endif
	endm

; -------------------------------------------------------------------------
; VDP DMA from 68000 memory to VDP memory
; -------------------------------------------------------------------------
; PARAMETERS:
;	src  - Source address in 68000 memory
;	dest - Destination address in VDP memory
;	len  - Length of data in bytes
;	type - Type of VDP memory
;	ctrl - VDP control port (optional)
; -------------------------------------------------------------------------

DMA_68K macro src, dest, len, type, ctrl
	if narg>4
		move.l	#$94009300|((((\len)/2)&$FF00)<<8)|(((\len)/2)&$FF),\ctrl
		move.l	#$96009500|((((\src)/2)&$FF00)<<8)|(((\src)/2)&$FF),\ctrl
		move.w	#$9700|(((\src)>>17)&$7F),\ctrl
		VDP_CMD_HI move.w,\dest,\type,DMA,\ctrl
		VDP_CMD_LO move.w,\dest,\type,DMA,-(sp)
		move.w	(sp)+,\ctrl
	else
		move.l	#$94009300|((((\len)/2)&$FF00)<<8)|(((\len)/2)&$FF),VDP_CTRL
		move.l	#$96009500|((((\src)/2)&$FF00)<<8)|(((\src)/2)&$FF),VDP_CTRL
		move.w	#$9700|(((\src)>>17)&$7F),VDP_CTRL
		VDP_CMD_HI move.w,\dest,\type,DMA,VDP_CTRL
		VDP_CMD_LO move.w,\dest,\type,DMA,-(sp)
		move.w	(sp)+,VDP_CTRL
	endif
	endm

; -------------------------------------------------------------------------
; Fill VRAM with byte
; (Auto-increment should be set to 1 beforehand)
; -------------------------------------------------------------------------
; PARAMETERS:
;	byte - Byte to fill VRAM with
;	addr - Address in VRAM
;	len  - Length of fill in bytes
;	ctrl - VDP control port (optional)
;	data - VDP data port (optional)
; -------------------------------------------------------------------------

VRAM_FILL macro byte, addr, len, ctrl, data
	if narg>3
		move.l	#$94009300|((((\len)-1)&$FF00)<<8)|(((\len)-1)&$FF),\ctrl
		move.w	#$9780,\ctrl
		VDP_CMD move.l,\addr,VRAM,DMA,\ctrl
		move.w	#(\byte)<<8,\data
		WAIT_DMA \ctrl
	else
		move.l	#$94009300|((((\len)-1)&$FF00)<<8)|(((\len)-1)&$FF),VDP_CTRL
		move.w	#$9780,VDP_CTRL
		VDP_CMD move.l,\addr,VRAM,DMA,VDP_CTRL
		move.w	#(\byte)<<8,VDP_DATA
		WAIT_DMA
	endif
	endm

; -------------------------------------------------------------------------
; Copy a region of VRAM to a location in VRAM
; (Auto-increment should be set to 1 beforehand)
; -------------------------------------------------------------------------
; PARAMETERS:
;	src  - Source address in VRAM
;	dest - Destination address in VRAM
;	len  - Length of copy in bytes
;	ctrl - VDP control port (optional)
; -------------------------------------------------------------------------

VRAM_COPY macro src, dest, len, ctrl
	if narg>3
		move.l	#$94009300|((((\len)-1)&$FF00)<<8)|(((\len)-1)&$FF),\ctrl
		move.l	#$96009500|(((\src)&$FF00)<<8)|((\src)&$FF),\ctrl
		move.w	#$97C0,\ctrl
		VDP_CMD move.l,\dest,VRAM,COPY,\ctrl
		WAIT_DMA \ctrl
	else
		move.l	#$94009300|((((\len)-1)&$FF00)<<8)|(((\len)-1)&$FF),VDP_CTRL
		move.l	#$96009500|(((\src)&$FF00)<<8)|((\src)&$FF),VDP_CTRL
		move.w	#$97C0,VDP_CTRL
		VDP_CMD move.l,\dest,VRAM,COPY,VDP_CTRL
		WAIT_DMA
	endif
	endm

; -------------------------------------------------------------------------

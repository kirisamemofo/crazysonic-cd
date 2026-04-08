; -------------------------------------------------------------------------
; Sub CPU system program
; -------------------------------------------------------------------------

	include	"src/include/sub_cpu.inc"
	include	"src/system/sub/file.inc"

	org	SP_START

; -------------------------------------------------------------------------
; Module header
; -------------------------------------------------------------------------

.Header:
	dc.b	'MAIN       '					; Module name
	dc.b	0						; Has offset table
	dc.w	$100						; Version
	dc.w	0						; Type
	dc.l	0						; Next module
	dc.l	0						; Module size (unused)
	dc.l	.Offsets-.Header				; Start address
	dc.l	0						; Work RAM address (unused)

; -------------------------------------------------------------------------
; Offset table
; -------------------------------------------------------------------------

.Offsets:
	dc.w	SystemInit-.Offsets				; Initialization
	dc.w	SystemMain-.Offsets					; Main
	dc.w	SystemInt2-.Offsets				; Mega Drive interrupt
	dc.w	SystemNull-.Offsets				; Undefined
	dc.w	0						; End of offset table

; -------------------------------------------------------------------------
; Initialization
; -------------------------------------------------------------------------

SystemInit:
	SET_2M_MODE						; Set to 2M mode

	lea	biosParams.w,a0					; Initialize the drive
	BIOS_DRVINIT
	
.WaitReady:
	BIOS_CDBSTAT						; Is the BIOS ready?
	move.b	(a0),d0
	andi.b	#$F0,d0
	bne.s	.WaitReady					; If not, wait
	
	lea	COMM_STATS.w,a0					; Clear communication flags
	moveq	#0,d0
	move.b	d0,SUB_FLAG-COMM_STATS(a0)
	move.l	d0,(a0)+
	move.l	d0,(a0)+
	move.l	d0,(a0)+
	move.l	d0,(a0)+
	
	bra.w	InitPCM						; Initialize PCM driver

; -------------------------------------------------------------------------
; Null routine
; -------------------------------------------------------------------------

SystemNull:
	rts

; -------------------------------------------------------------------------
; Main routine
; -------------------------------------------------------------------------

SystemMain:
	bsr.w	ReadFileIndex					; Read file index

	lea	MainProgram(pc),a0				; Load main program into PRG-RAM
	lea	PRG_END-$8000,a2
	bsr.w	ReadFile
	
	WAIT_WORD_ACCESS					; Wait for Word RAM access

	lea	PRG_END-$8000,a0				; Source buffer
	lea	WORD_START_2M,a1				; Destination buffer
	move.w	#filesize("out/files/MAIN.M68")/16-1,d0		; Size of program

.Copy:
	move.l	(a0)+,(a1)+					; Copy program 
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	dbf	d0,.Copy

	GIVE_WORD_ACCESS					; Swap Word RAM access

; -------------------------------------------------------------------------

.MainLoop:
	moveq	#0,d0						; Get command
	move.b	MAIN_FLAG.w,d0
	beq.s	.MainLoop					; If there's not one yet, wait
	st	SUB_FLAG.w					; Processing command

	add.w	d0,d0						; Run command
	add.w	d0,d0
	movea.l	.Commands-4(pc,d0.w),a0
	jsr	(a0)

.WaitMainDone:
	tst.b	MAIN_FLAG.w					; Wait for the Main CPU to be ready to send more commands
	bne.s	.WaitMainDone
	clr.b	SUB_FLAG.w					; Not processing a command

	bra.s	.MainLoop					; Loop
	
; -------------------------------------------------------------------------

.Commands:
	dc.l	Cmd_ReadFile					; Read file
	dc.l	Cmd_PlayCDDA					; Play CDDA
	dc.l	Cmd_LoopCDDA					; Loop CDDA
	dc.l	Cmd_StopCDDA					; Stop CDDA
	dc.l	Cmd_PauseCDDA					; Pause CDDA
	dc.l	Cmd_UnpauseCDDA					; Unpause CDDA
	dc.l	Cmd_RunModule					; Run module
	dc.l	Cmd_WriteByte					; Write byte
	dc.l	Cmd_WriteWord					; Write word
	dc.l	Cmd_WriteLong					; Write longword
	dc.l	Cmd_CallFunc					; Call function
	dc.l	Cmd_CheckCDDA					; Check CDDA playing

; -------------------------------------------------------------------------

MainProgram:
	dc.b	"MAIN.M68", 0
	even
	
; -------------------------------------------------------------------------
; Read file command (requires Word RAM access)
; -------------------------------------------------------------------------

Cmd_ReadFile:
	lea	.fileNameBuf(pc),a0				; Get file name
	lea	COMM_CMD_0.w,a1
	move.l	(a1),(a0)
	move.l	4(a1),4(a0)
	move.l	8(a1),8(a0)
	move.l	$C(a1),$C(a0)

	move.b	#'A',SUB_FLAG.w					; Request read address

.WaitAddress:
	cmpi.b	#'A',MAIN_FLAG.w				; Wait for read address
	bne.s	.WaitAddress

	movea.l	(a1),a2						; Get read address
	bsr.w	ReadFile					; Read file
	scs	COMM_STAT_0.w					; Set operation result
	move.l	d0,COMM_STAT_2.w

	GIVE_WORD_ACCESS					; Swap Word RAM access

	move.b	#'D',SUB_FLAG.w					; Done
	rts
	
; -------------------------------------------------------------------------

.fileNameBuf:
	dcb.b	16, 0
	
; -------------------------------------------------------------------------
; Play CDDA
; -------------------------------------------------------------------------

Cmd_PlayCDDA:
	st	cddaPlaying
	BIOS_MSCSTOP
	lea	biosParams.w,a0
	move.w	COMM_CMD_0.w,(a0)
	BIOS_MSCPLAY1
	rts
	
; -------------------------------------------------------------------------
; Loop CDDA
; -------------------------------------------------------------------------

Cmd_LoopCDDA:
	st	cddaPlaying
	BIOS_MSCSTOP
	lea	biosParams.w,a0
	move.w	COMM_CMD_0.w,(a0)
	BIOS_MSCPLAYR
	rts
	
; -------------------------------------------------------------------------
; Stop CDDA
; -------------------------------------------------------------------------

Cmd_StopCDDA:
	clr.b	cddaPlaying
	BIOS_MSCSTOP
	rts
	
; -------------------------------------------------------------------------
; Pause CDDA
; -------------------------------------------------------------------------

Cmd_PauseCDDA:
	tst.b	cddaPlaying
	beq.s	.End
	BIOS_MSCPAUSEON

.End:
	rts
	
; -------------------------------------------------------------------------
; Unpause CDDA
; -------------------------------------------------------------------------

Cmd_UnpauseCDDA:
	tst.b	cddaPlaying
	beq.s	.End
	BIOS_MSCPAUSEOFF

.End:
	rts

; -------------------------------------------------------------------------
; Run module
; -------------------------------------------------------------------------

Cmd_RunModule:
	jmp	PRG_START+$10000

; -------------------------------------------------------------------------
; Write a byte to Sub CPU memory
; -------------------------------------------------------------------------

Cmd_WriteByte:
	movea.l	COMM_CMD_0.w,a0
	move.b	COMM_CMD_2.w,(a0)
	rts

; -------------------------------------------------------------------------
; Write a word to Sub CPU memory
; -------------------------------------------------------------------------

Cmd_WriteWord:
	movea.l	COMM_CMD_0.w,a0
	move.w	COMM_CMD_2.w,(a0)
	rts

; -------------------------------------------------------------------------
; Write a longword to Sub CPU memory
; -------------------------------------------------------------------------

Cmd_WriteLong:
	movea.l	COMM_CMD_0.w,a0
	move.l	COMM_CMD_2.w,(a0)
	rts
	
; -------------------------------------------------------------------------
; Make the Sub CPU call a function
; -------------------------------------------------------------------------

Cmd_CallFunc:
	move.w	COMM_CMD_2.w,d0
	movea.l	COMM_CMD_0.w,a0
	jmp	(a0)

; -------------------------------------------------------------------------
; Check if CDDA is playing
; -------------------------------------------------------------------------

Cmd_CheckCDDA:
	BIOS_CDBSTAT						; Get BIOS status
	move.w	(a0),d0
	bmi.s	.NotPlaying					; If the CD isn't ready, branch
	andi.w	#$FF00,d0					; Is CDDA playing?
	cmpi.w	#$100,d0
	beq.s	.Playing					; If so, branch

.NotPlaying:
	clr.b	COMM_STAT_0.w					; Not playing
	rts

.Playing:
	st	COMM_STAT_0.w					; Playing
	rts

; -------------------------------------------------------------------------
; Mega Drive interrupt
; -------------------------------------------------------------------------

SystemInt2:
	movem.l	d0-a6,-(sp)					; Run routine
	move.l	int2Routine(pc),d0
	beq.s	.End
	movea.l	d0,a0
	jsr	(a0)
	
.End:
	movem.l	(sp)+,d0-a6
	rts
	
; -------------------------------------------------------------------------

int2Routine:
	dc.l	0

; -------------------------------------------------------------------------
; BIOS function parameters
; -------------------------------------------------------------------------

biosParams:
	dc.b	$01, $FF, $00, $00
	dc.b	$00, $00, $00, $00

; -------------------------------------------------------------------------
; Variables
; -------------------------------------------------------------------------

cddaPlaying:
	dc.b	0
	even

; -------------------------------------------------------------------------
; Libraries
; -------------------------------------------------------------------------

	include	"src/system/sub/disc.asm"
	include	"src/system/sub/file.asm"
	include	"src/system/sub/pcm.asm"

; -------------------------------------------------------------------------

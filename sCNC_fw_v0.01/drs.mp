NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   0026   001E =     30. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0026  __func_lit_start
       0044  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0044   0019 =     25. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0044  __idata_start
       005D  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   005E   1024 =   4132. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       002F  __start
       002F  __text_start
       004E  _exit
       004F  _main
       00BE  _port_init
       00CA  _watchdog_init
       00CE  _timer0_init
       00D5  _timer1_init
       00E6  _timer0_ovf_isr
       0108  _uart0_init
       0114  _uart0_rx_isr
       011D  _init_devices
       0129  _show_new_version
       013A  _t5ms
       013B  _InputFromHostStateMachine
       01BC  _ProcessCommand
       050D  _dly_t1_us
       0555  _x_steps
       05AB  _y_steps
       0601  _z_steps
       0657  _IsOverWater
       067B  _IsUnderWater
       069F  _IsHotWaterFull
       06B4  _GetRlyAddWaterSts
       06B7  _SetRlyAddWater
       06CF  _GetRlyAddHotSts
       06D3  _SetRlyAddHot
       06EB  _SetLedPwr
       06F1  _SetLedLwt
       06F7  _SetLedRun
       06FD  _SetLedErr
       0703  _SetSpk
       0713  _SetAllOff
       0722  _TurnOnAdPwr
       0726  _TurnOffAdPwr
       072A  _GetOnoffKeyPin
       072F  _SetErrState
       0734  _DoErr
       0752  _init_serial
       0753  _wait_us
       0754  _uart_tx
       0761  _uart_rx
       0763  _hex_to_bin
       076D  _hexit
       0778  _uart_2hex
       0784  _debug_led
       0789  _put_dword
       078A  push_arg4
       078C  push_arg2
       078F  empy16s
       079F  empy32s
       079F  empy32u
       07CB  pop_gset2
       07CD  pop_gset5
       07CF  push_gset5
       07D1  push_gset4
       07D3  push_gset3
       07D5  push_gset2
       07D7  push_gset1
       07DA  pop_gset1
       07DB  pop
       07EE  long_prolog
       07FC  long_epilog
       0807  tstzero1
       080D  tstzero2
       0813  push_lset
       082A  pop_lset
       0841  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0060   0019 =     25. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0060  __data_start
       0060  _bInputFromHostState
       0061  _bPwrOnCnt
       0062  _x_delay
       0066  _y_delay
       006A  _z_delay
       006E  _bStopLoop
       006F  _bDrsStatus
       0070  _wHotWaterCnt
       0072  _wAddWaterCnt
       0074  _wErrAlarmCnt
       0076  _bErrRetryCnt
       0077  _bHotRetryCnt
       0078  _bWaterRetryCnt
       0079  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   0079   0018 =     24. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0079  __bss_start
       007A  _bComValue
       007B  _bComParam
       007C  _in_buffer
       0084  _bTaskFlag
       0085  _wAdValue
       0091  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0018 =     24. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\icc\lib\crtAVR.o	[ crtavr.s ]
main.o	[ main.c ]
drs.o	[ drs.c ]
serial.o	[ serial.c ]
<library>	[ asave.s, emul16s.s, emul32.s, gpop2.s, gpop5.s, gpush.s, longutil.s, lpush.s ]

User Global Definitions

ram_end = 0x45f
hwstk_size = 0x10

User Base Address Definitions

func_lit = 0x26
data = 0x60
eeprom:1.512


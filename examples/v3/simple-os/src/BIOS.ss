; This program represents a tiny simple BIOS for our Os
; Some Assembler Tuning(Nessicary)
&&_Assembler:
   v_size: 1000000
   start_index: 0
; ------------------------------

%include "processor_info.ss"
;%include "kernal.ss"
;%include "os.ss"

~led:
    mov sdx 4
    mov sfc 2
    invoke 0x6      ; export pin 4

    mov tmp 1
    mov sfc 0
    invoke 0x6     ; set direction

    mov sfc 1
    invoke 0x5     ; send signal 1(keep power led on while Os is running)
    ret

~led_act:                ; send a quick signal signifying that the Os is active(only when hardware interactions are performed)
   rmov ip led_act_b     ; store func backtrack
   mov sdx 14
   mov sfc 2
   invoke 0x6            ; export pin 14

   mov tmp 1
   mov sfc 0
   invoke 0x6            ; set direction

   mov scx 400           ; sleep for 400 mills
   mov sfc 1
   invoke 0x5            ; send signal 1

   sleep 1
   mov tmp 0
   invoke 0x5            ; send signal 0

   mov sdx 14
   mov sfc 1
   invoke 0x6            ; unexport pin 14
   ret

~BIOS:
     call led                   ; send positive signal to led pin 4 to signify power
     call led_act
     cp led_act led_act_b
     mov sdx 0x39         ; store the processor magic number
     call test

     loadbl load false
     loadi goal 1
     mov i1 bool
     same load boot_value goal
     ndo load                   ; check processor pesponse
      print 38 "Invalid Magic number. Shutting down..."
      halt
     end
     rm goal
     rm load

     print 15 "Loading BIOS..."
     call led_act
     cp led_act led_act_b
     ret

~main:
     call BIOS    ; Load BIOS
     ;call kernal  ; Run kernal
     ;call Os      : Start Os
     ret

call main

; -------------------------------------------------
; after Os shutdown turn power led off and halt system
   mov sdx 14
   mov tmp 0
   invoke 0x5            ; send signal 0

   mov sfc 1
   invoke 0x6            ; unexport pin 4

   halt
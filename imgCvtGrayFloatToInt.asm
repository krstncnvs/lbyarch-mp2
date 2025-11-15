section .data
    scalevalue dd 255.0

section .text
default rel
bits 64

global imgCvtGrayFloatToInt

imgCvtGrayFloatToInt:
    mov r10d, r8d            ; total pixels(r10d) = height(r8d) * width(r9d)
    imul r10d, r9d
    xor r11d, r11d           ; loop index (r11d) = 0
    movss xmm1, [scalevalue] ; xmm1 = 255.0

convert_loop:
    cmp r11d, r10d
    jge done
    movss xmm0, [rcx + r11 * 4] ; load float ; rcx: *floatPixels
    mulss xmm0, xmm1            ; *255
    cvtss2si eax, xmm0          ; convert to int
    mov [rdx + r11], al         ; store byte ; rdx: *intPixels
    inc r11d
    jmp convert_loop

done:
    ret
_functionB:
    pushl    %ebp           ; saves EBP of caller function 
                            ; - i.e. EBP of functionA to stack
    movl    %esp, %ebp      ; set EBP with current ESP. 
                            ; i.e. new stack frame is set for functionB
    subl    $16, %esp       ; allocates 12 bytes temporary storage 
                            ; + 4 bytes for one local variable in stack
    movl    -4(%ebp), %eax  ; access local variable 'j' using ebp - 4 
                            ; and copy it to eax
    leave                   ; reverse the stack frame 
    ret                     ; return back to functionB 
_functionA:
    pushl    %ebp           ; saves EBP of caller function
                            ; - i.e. EBP of main to stack
    movl    %esp, %ebp      ; set EBP with current ESP. 
                            ; i.e. new current stack frame is set for functionA 
    subl    $20, %esp       ; allocates 12 bytes temporary storage 
                            ; + 8 bytes for two local variables in stack 
    movl    -4(%ebp), %eax  ; access local variable 'a' using ebp - 4 
                            ; and copy it to eax
    movl    %eax, (%esp)    ; move eax contents to esp. This is used as 
                            ; the input parameter in functionB
    call    _functionB      ; calls functionB
    movl    %eax, -8(%ebp)  ; post return from functionB, 
                            ; saves functionBs return value from eax to 
                            ; local variable 'b' @ ebp - 8
    movl    -8(%ebp), %eax  ; copies value of 'b' to eax as 
                            ; return value from functionA
    leave                   ; reverse the stack frame 
    ret                     ; return back to main 
.text
.syntax unified /* Unified Assembly Syntax - UAL */
.thumb /* Thumb Instr. Set */
.cpu cortex-m0plus
.type My_Div, %function
.global My_Div /* Make the function visible from outside */
// R0: Dividendo, actualizado cada iteración tras restarlle o divisor
// R1: Divisor
// R2: Cociente
My_Div:
    div:
    Adds R2, R2, #1
    Subs R0, R0, R1
    Bpl div
    Subs R0, R2, #1
    BX LR
.end

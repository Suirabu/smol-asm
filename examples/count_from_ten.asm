start:
    push 10

loop:
    dup
    drop

    push 1
    sub

    dup
    push 0
    cmp
    jne loop

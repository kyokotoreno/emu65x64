.org $3ffff000

_start:
    lda $0

brk:
nmi:
reset:
irq:
    rti

.org $3fffffc0

.dq $0 ; reserved
.dq $0 ; reserved
.dq $0 ; cop
.dq brk
.dq $0
.dq nmi
.dq reset
.dq irq

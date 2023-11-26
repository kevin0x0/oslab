.code16

.global _start

.equ SETUPSEG, 0x07e0
.equ STACKSEG, 0x17e0

.text


_start:
mov $SETUPSEG, %ax
mov %ax, %ds
mov %ax, %es
mov $STACKSEG, %ax
mov %ax, %ss
xor %sp, %sp

.equ msglen, endmsg - msg
mov $msglen, %cx
mov $msg, %bx
call print_str

# get harddisk parameter
cld
xor %ax, %ax
mov %ax, %ds
lds 0x0104, %si
mov $hdisk_param, %di
mov $16, %cx
rep movsb

mov %es, %ax
mov %ax, %ds

.equ hdiskinfolen, end_hdinfo - hdisk_info
mov $hdiskinfolen, %cx
mov $hdisk_info, %bx
call print_str

mov cylinders, %bx
call print_hex

mov $0x20, %al
mov $0x0e, %ah
mov $0x07, %bl
int $0x10
xor %bx, %bx
mov headers, %bl
call print_hex

mov $0x20, %al
mov $0x0e, %ah
mov $0x07, %bl
int $0x10
xor %bx, %bx
mov sectors, %bl
call print_hex

call print_nl

inf_loop:
jmp inf_loop


# %bx length
# %cx address
print_str:
push %bp
push %ax
push %cx
# set string length and address
mov %bx, %bp

# read current cursor position
mov $3, %ah
xor %bh, %bh
int $0x10 # now cursor position is in dh and dl

pop %cx
# write string
mov $0x0c, %bl
mov $0x00, %bh
mov $0x13, %ah
mov $0x01, %al  # move cursor
int $0x10
pop %ax
pop %bp
ret


# %bx number

print_hex:
push %ax
mov    $4, %cx
print_digit:
rol   $4, %bx
mov   $0xe0f, %ax
and   %bl, %al
add   $0x30, %al
cmp   $0x3a, %al
jl     outp
add    $0x07, %al
outp:
int    $0x10
loop   print_digit
pop %ax
ret

print_nl:
push %bx
push %cx
mov $2, %cx
mov nl_str, %bx
call print_str
pop %cx
pop %bx
ret


msg:
.ascii "now we are in setup..."
nl_str:
.byte 13,10
endmsg:

hdisk_param:
cylinders:  .word 0
headers:    .word 0
            .word 0
            .word 0
            .word 0
            .word 0
            .word 0
sectors:    .byte 0, 0

hdisk_info:
.ascii "hard disk info(cylinders, headers, sectors): "
end_hdinfo:


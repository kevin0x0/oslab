.code16

.global _start

.equ BOOTSEG, 0x07c0
.equ STACKSEG, 0x17c0

.text

ljmp $BOOTSEG, $_start


_start:
mov $BOOTSEG, %ax
mov %ax, %ds
mov %ax, %es
mov $STACKSEG, %ax
mov %ax, %ss
xor %sp, %sp

# read current cursor position
mov $3, %ah
xor %bh, %bh
int $0x10 # now cursor position is in dh and dl

# write string
.equ msglen, endmsg - msg
mov $msglen, %cx
mov $msg, %bp
mov $0x0c, %bl
mov $0x00, %bh
mov $0x13, %ah
mov $0x01, %al  # move cursor
int $0x10

load_setup:
mov $0x02, %ah
mov $2, %al  # load two sectors
mov $0, %dh  # driver 0
mov $0, %dl  # head 0
mov $0x0002, %cx # sector 2, track 2
mov $0x0200, %bx # put the setup after boot
int $0x13
jnc ok_load
# failed, reset floppy driver
mov    $0x0000, %dx
mov    $0x0000, %ax
int    $0x13
jmp load_setup

ok_load:
ljmp $BOOTSEG, $512


msg:
.ascii "Albert is booting..."
.byte 13,10
endmsg:

.org 510
boot_flag:
.word 0xAA55

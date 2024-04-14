	.EXPORT errno
	.EXPORT exit
	.EXPORT __text, __data

	.IMPORT acu_init
	.IMPORT __exit
	.IMPORT _fpuinit

	.DATA
__data:
errno:
    .DC.W   0
        
    .CODE
__text:
	JSR		_fpuinit
	JMP		acu_init

exit:
	JMP		__exit
.TEXT

        EXPORT __disableInstructionCache
        EXPORT __enableInstructionCache
        EXPORT __getCacr

__disableInstructionCache:
        NOP
        MOVEC cacr, d0
        ANDI.W  #$f6f6, d0
        MOVEC d0, cacr
        
        RTS

__enableInstructionCache:
        NOP
        MOVEC cacr, d0
        ORI.W  #$0909, d0
        MOVEC d0, cacr
        
        RTS

__getCacr:

        MOVEC cacr, d0
        
        RTS
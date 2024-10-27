.TEXT

        EXPORT __clearInstructionCache
        EXPORT __disableInstructionCache
		EXPORT __enableInstructionCache
		EXPORT __getCacr

__clearInstructionCache:

		MOVEC cacr, d0
		ORI.W #$08, d0
		MOVEC d0, cacr
		
		RTS

__disableInstructionCache:
		MOVEC cacr, d0
		ANDI.W  #$f6f6, d0
		MOVEC d0, cacr
		
		RTS

__enableInstructionCache:

		MOVEC cacr, d0
		ORI.W  #$0909, d0
		MOVEC d0, cacr
		
		RTS

__getCacr:

		MOVEC cacr, d0
		
		RTS
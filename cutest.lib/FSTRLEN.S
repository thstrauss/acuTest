.TEXT

        EXPORT fstrlen

.MODULE fstrlen
                                              
        MOVEA.L   A0,A1
        MOVE.L	  #$7F7F7F7F, D2                                                      
        BRA.B     .four                                                   
.loop:  ADDQ.W    #4,A1                                                      
.four:  MOVE.L    (A1), D0
		MOVE.L    D0, D1                                                      
        AND.L     D2, D1                                               
        ADD.L     D2, D1                                              
        OR.L      D0, D1                                                      
        OR.L      D2, D1                                              
        NOT.L     D1                                                         
        BNE.B     .loope
        ADDQ.W    #4,A1
        MOVE.L    (A1), D0                                                
        MOVE.L    D0, D1                                                      
        AND.L     D2, D1                                               
        ADD.L     D2, D1                                              
        OR.L      D0, D1                                                      
        OR.L      D2, D1                                              
        NOT.L     D1                                                         
        BNE.B     .loope
        ADDQ.W    #4,A1
        MOVE.L    (A1), D0                                                
        MOVE.L    D0, D1                                                      
        AND.L     D2, D1                                               
        ADD.L     D2, D1                                              
        OR.L      D0, D1                                                      
        OR.L      D2, D1                                              
        NOT.L     D1                                                         
        BEQ.B     .loop                                                    
.loope:	TST.B     (A1)+
		BEQ.B     .end                                                                              
		TST.B     (A1)+
		BEQ.B     .end                                                                              
		TST.B     (A1)+
		BEQ.B     .end                                                                              
        ADDQ.L    #1, A1                                                                     
.end:   MOVE.L    A1, D0                                                      
		SUB.L     A0, D0
		SUBQ.L    #1, D0
		RTS 
.ENDMOD	   
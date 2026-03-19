/*
find the path "Microsoft Visual Studio 9.0\Common7\Packages\Debugger" autoexp.dat 
copy them to [Visualizer] 

SKEngine2::SKString{
		preview
		(
			[$c.m_pBuffer,s]
		)
}
SKEngine2::SKName{
		preview
		(

			[$c.m_String]

		)
}
SKEngine2::SKUsedName{
		preview
		(
			[$c.m_pName.m_pObject]
		)
}
SKEngine2::SKBitArray{
	children   
	(   
		#array   
		(   

			expr : ($e.m_pBuffer[$i / 32] >> ($i % 32)),     
			size : ($e.m_uiSize * 32) 
		)  : (bool)($e & 1) 
	)   

	preview   
	(    
		#(    
			"[m_uiCurUse is", 
			($e.m_uiSize * 32) ,
			"](",  
			#array   
				(   
					expr : ($e.m_pBuffer[$i / 32] >> ($i % 32)), 
					size : ($e.m_uiSize * 32)
				): (bool)($e & 1),    
			")"  
		)   
 

	)
}
SKEngine2::SKArray<*,*>{
	children   
		(   
		#array   
				(   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
			   )   
	   )   

	   preview   
	   (    
		#(    
			   "[m_uiCurUse is", 
			   $c.m_uiCurUse ,
			   "]",
			   "[m_uiBufferNum is",    
			   $c.m_uiBufferNum ,   
			   "](",    

		#array   
			   (   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
			   ),    
			   ")"  
		)   
	   )  

}
SKEngine2::SKArrayOrder<*,*>{
	children   
		(   
			#array   
				(   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
				)   
		)   

		preview   
		(    
		 #(    
				"[m_uiCurUse is", 
				$c.m_uiCurUse ,
				"]",
				"[m_uiBufferNum is",    
				$c.m_uiBufferNum ,   
				"](",    

			#array   
				(   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
				),    
				")"  
		 )   
		)  

}

SKEngine2::SKMap<*,*>{
	children   
	(   
		#array   
		(   
		expr : $c.m_pBuffer[$i],     
		size : $c.m_uiCurUse   
		)   
	)   

	preview   
	(    
		#(    
			"[m_uiCurUse is", 
			$c.m_uiCurUse ,
			"]",
			"[m_uiBufferNum is",    
			$c.m_uiBufferNum ,   
			"](",    

			#array   
			(   
			expr : $c.m_pBuffer[$i],     
			size : $c.m_uiCurUse   
			),    
			")"  
		)   
	)  

}
SKEngine2::SKMapOrder<*,*>{
	children   
	(   
		#array   
		(   
		expr : $c.m_pBuffer[$i],     
		size : $c.m_uiCurUse   
		)   
	)   

	preview   
	(    
		#(    
			"[m_uiCurUse is", 
			$c.m_uiCurUse ,
			"]",
			"[m_uiBufferNum is",    
			$c.m_uiBufferNum ,   
			"](",    

			#array   
			(   
			expr : $c.m_pBuffer[$i],     
			size : $c.m_uiCurUse   
			),    
			")"  
		)   
	)  

}
*/
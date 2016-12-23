# #############################################################################################
# Archivo de especificacion								      #
#											      #	
# la sintaxis a utilizar aca es:							      #
# operator <opname> alias <opalias> pattern <syntax>                                          #
#           [<implicit parameters>]                                                           #
#                                                                                             #
#	opname: nombre del operador                                                           #
#	opalias: Nombre del operador como texto                                               #
#	syntax: sintaxis del operador                                                         #
#	implicit parameters: use of types for function arguments                              #
#                                                                                             #
#	La sintaxis puede ser:                                                                #
#	op()		operador prefijo sin argumento                                        #  
#	op(_)		operador prefijo con un argumento                                     #
#	op(_,_)		operador prefijo con numero arbitrario de argumentos                  #
#	_ infixop _	operador infijo                                                       #
#	_op		operador postfijo con un argumento                                    #
#	_ _op		operador postfijo con dos argumentos                                  #
#       ...                                                                                   #
#       _ op[<paralist>] operador posfijo con un operador y parametros adicionales            #
#                                                                                             #
#                                                                                             #
#                                                                                             #
###############################################################################################

operator nodosMasArcos alias nodosMasArcos pattern op(_)
operator CantNodos alias CantNodos pattern op(_)
operator CantArcos alias CantArcos pattern op(_)
operator radio alias radio pattern op(_)
operator diameter alias diameter pattern op(_)


operator nodoMasPopular alias nodoMasPopular pattern op(_)

operator central alias central pattern op(_)


operator relacion alias relacion pattern op(_,_,_)

operator common alias common pattern op(_,_,_)

operator nodoPertenece alias nodoPertenece pattern op(_,_)

#ifndef FTL_BUILD_WARNING_CHECK_H
#define FTL_BUILD_WARNING_CHECK_H

#pragma once


//-------- Disable Begin --------
#pragma warning(disable: 4018)  //'expression' : signed/unsigned mismatch
//#pragma warning(disable: 4062)  //enumerator 'identifier' in switch of enum 'enumeration' is not handled
#pragma warning(disable: 4063)  //case 'identifier' is not a valid value for switch of enum 'enumeration'
//#pragma warning(disable: 4065 ) //switch statement contains 'default' but no 'case' labels
#pragma warning(disable: 4100)  //'identifier' : unreferenced formal parameter
#pragma warning(disable: 4189)  //'identifier' : local variable is initialized but not referenced
#pragma warning(disable: 4244)	//'argument' : conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  //'conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4389)  //'operator' : signed/unsigned mismatch
#pragma warning(disable: 4800)  //'type' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4996)	//'function': was declared deprecated

//-------- Disable End --------

#pragma warning(error: 4002)  //too many actual parameters for macro 'identifier'
#pragma warning(error: 4003)  //not enough actual parameters for macro 'identifier'
#pragma warning(error: 4005)  //macro redefinition
#pragma warning(error: 4010)  //single-line comment contains line-continuation character
#pragma warning(error: 4013)  //'function' undefined; assuming extern returning int
#pragma warning(error: 4020)  //'function' : too many actual parameters
#pragma warning(error: 4022)  //'function' : pointer mismatch for actual parameter 'number'
#pragma warning(error: 4023)  //'symbol' : based pointer passed to unprototyped function : parameter number
#pragma warning(error: 4024)  //'function' : different types for formal and actual parameter 'number'
#pragma warning(error: 4028)  //formal parameter 'number' different from declaration
#pragma warning(error: 4029)  //declared formal parameter list different from definition
#pragma warning(error: 4030)  //first formal parameter list longer than the second list
#pragma warning(error: 4031)  //second formal parameter list longer than the first list
#pragma warning(error: 4033)  //'function' must return a value
#pragma warning(error: 4034)  //sizeof returns 0
#pragma warning(error: 4045)  //'array' : array bounds overflow
#pragma warning(error: 4047)  //'operator' : 'identifier1' differs in levels of indirection from 'identifier2'
#pragma warning(error: 4052)  //function declarations different; one contains variable arguments
#pragma warning(error: 4053)  //one void operand for '?:'
#pragma warning(error: 4056)  //overflow in floating point constant arithmetic
#pragma warning(error: 4060)  //switch statement contains no 'case' or 'default' labels
//#pragma warning(error: 4062)  //enumerator 'identifier' in switch of enum 'enumeration' is not handled
//#pragma warning(error: 4063)  //case 'identifier' is not a valid value for switch of enum 'enumeration'
#pragma warning(error: 4064)  //switch of incomplete enum 'enumeration'
#pragma warning(error: 4065 ) //switch statement contains 'default' but no 'case' labels
#pragma warning(error: 4066)  //characters beyond first in wide-character constant ignored
#pragma warning(error: 4067)  //unexpected tokens following preprocessor directive - expected a newline
#pragma warning(error: 4076)  //'typemod' : can not be used with type 'typename'
#pragma warning(error: 4081)  //expected 'token1'; found 'token2'
#pragma warning(error: 4083)  //expected 'token'; found identifier 'identifier'
#pragma warning(error: 4087)  //'function' : declared with 'void' parameter list
#pragma warning(error: 4088)  //'function' : pointer mismatch in actual parameter 'number', formal parameter 'number'
#pragma warning(error: 4089)  //'function' : different types in actual parameter 'number', formal parameter 'number'
#pragma warning(error: 4090)  //'operation' : different 'modifier' qualifiers
#pragma warning(error: 4096)  //'a': interface is not a COM interface; will not be emitted to IDL
#pragma warning(error: 4098)  //'function' : void function returning a value
//#pragma warning(error: 4099)  //'identifier' : type name first seen using 'objecttype1' now seen using 'objecttype2'
//#pragma warning(error: 4100)  //'identifier' : unreferenced formal parameter
#pragma warning(error: 4101)  //'identifier' : unreferenced local variable
#pragma warning(error: 4102)  //'label' : unreferenced label
#pragma warning(error: 4103)  //'filename' : alignment changed after including header, may be due to missing #pragma pack(pop)
#pragma warning(error: 4109)  //unexpected identifier 'identifier'
#pragma warning(error: 4115)  //'type' : named type definition in parentheses
#pragma warning(error: 4116)  //unnamed type definition in parentheses
//#pragma warning(error: 4119)  //different bases 'base1' and 'base2' specified
//#pragma warning(error: 4121)  //'symbol' : alignment of a member was sensitive to packing
//#pragma warning(error: xxxx)  //

//#pragma warning(error: 4189)  //'identifier' : local variable is initialized but not referenced
#pragma warning(error: 4239)  //nonstandard extension used : 'token' : conversion from 'type' to 'type'
#pragma warning(error: 4240)  //nonstandard extension used : access to 'classname' now defined to be 'access specifier', previously it was defined to be 'access specifier'
//#pragma warning(error: 4251)  //'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
//#pragma warning(error: 4275)  //non DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#pragma warning(error: 4390)  //empty controlled statement found; is this the intent
#pragma warning(error: 4701)  //Potentially uninitialized local variable 'name' used
#pragma warning(error: 4706)  //assignment within conditional expression
#pragma warning(error: 4715)  //not all control paths return a value
#pragma warning(error: 4930)  //prototyped function not called (was a variable definition intended?)



//Visual Studio(Team): Anaylze -> Run Code Analyze on Only Xxx
#pragma warning(error: 6001)  //using uninitialized memory <variable>
#pragma warning(disable: 6011)  //dereferencing NULL pointer <name>
#pragma warning(disable: 6031)  //return value ignored: <function> could return unexpected value
#pragma warning(error: 6057)   //buffer overrun due to number of characters/number of bytes mismatch in call to <function>
#pragma warning(disable: 6067)  //parameter <number> in call to <function> must be the address of the string
#pragma warning(error: 6202)  //buffer overrun for <variable>, which is possibly stack allocated, in call to <function>: length <size> exceeds buffer size <max>
#pragma warning(disable: 6211)  //Leaking memory <pointer> due to an exception. Consider using a local catch block to clean up memory
//#pragma warning(error: 6246)  //Local declaration of <variable> hides declaration of same name in outer scope. Additional Information: See previous declaration at <location>.
#pragma warning(disable: 6255)  //_alloca indicates failure by raising a stack overflow exception. Consider using _malloca instead
#pragma warning(disable: 6258)  //using TerminateThread does not allow proper thread clean up
#pragma warning(error: 6262)  //Function uses <constant> bytes of stack: exceeds /analyze:stacksize<constant>. Consider moving some data to heap
#pragma warning(error: 6263)  //using _alloca in a loop; this can quickly overflow stack

#pragma warning(disable: 6262)  //httpclient.cpp
//#pragma warning(error: 6262)    //Function uses <constant> bytes of stack: exceeds /analyze:stacksize<constant>. Consider moving some data to heap

#pragma warning(error: 6280)    //<variable> is allocated with <function>, but deleted with <function>
#pragma warning(disable: 6284)  //object passed as parameter '%d' when string is required in call to <function>.
#pragma warning(error: 6287)   //redundant code: the left and right sub-expressions are identical
//#pragma warning(error: 6309)  //argument <number> is null: it does not adhere to function specification of <function>
#pragma warning(disable: 6326)  //potential comparison of a constant with another constant
#pragma warning(error: 6335)    //leaking process information handle <handlename>
//#pragma warning(error: 6336)    //arithmetic operator has precedence over question operator, use parentheses to clarify intent 
//#pragma warning(error: 6385)    //invalid data: accessing <buffer name>, the readable size is <size1> bytes, but <size2> bytes may be read
//#pragma warning(error: 6386)    //buffer overrun: accessing <buffer name>, the writable size is <size1> bytes, but <size2> bytes may be written
#pragma warning(disable: 6387)  //<argument> may be <value>: this does not adhere to the specification for the function <function name>
#pragma warning(disable: 6400)  //Using <function name> to perform a case-insensitive compare to constant string <string name>. Yields unexpected results in non-English locales



#endif  //FTL_BUILD_WARNING_CHECK_H
/****************************************************************************************

	File		externC.h
	Why:		Macros for the "extern C" declarations.
	OS:			-
	Created:	2020-12-11

History
-------

When		Who				What
-----------------------------------------------------------------------------------------
2020-12-11	Thomas			Created. Copied from ubf_type_definitions.h.
2024-07-09	Thomas			Master version is now in folder cunilog.

****************************************************************************************/

/*
	This file is maintained as part of Cunilog. See https://github.com/cunilog .
*/

/*
	This code is covered by the MIT License. See https://opensource.org/license/mit .

	Copyright (c) 2024, 2025 Thomas

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the Software
	without restriction, including without limitation the rights to use, copy, modify,
	merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be included in all copies
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef EXTERN_C_H
#define EXTERN_C_H

/*
	BEGIN_C_DECLS		END_C_DECLS
	EXTERN_C_BEGIN		EXTERN_C_END

	Lets C++ compilers know that this is C, not C++.

	Idea and original version nicked from
	https://code.woboq.org/userspace/glibc/misc/sys/cdefs.h.html#117 .

	Leading double underscores removed to get rid of compiler warnings.
	Two different flavours can be used: BEGIN_C_DECLS and END_C_DECLS,
	and EXTERN_C_BEGIN and EXTERN_C_END.
*/
// BEGIN_C_DECLS and END_C_DECLS.
#ifdef	__cplusplus
	#ifndef BEGIN_C_DECLS
	#define BEGIN_C_DECLS        extern "C"	{
	#endif
	#ifndef END_C_DECLS
	#define END_C_DECLS						}
	#endif
#else
	#ifndef BEGIN_C_DECLS
	#define BEGIN_C_DECLS
	#endif
	#ifndef END_C_DECLS
	#define END_C_DECLS
	#endif
#endif
// EXTERN_C_BEGIN and EXTERN_C_END.
#ifdef	__cplusplus
	#ifndef EXTERN_C_BEGIN
	#define EXTERN_C_BEGIN       extern "C"	{
	#endif
	#ifndef EXTERN_C_END
	#define EXTERN_C_END					}
	#endif
#else
	#ifndef EXTERN_C_BEGIN
	#define EXTERN_C_BEGIN
	#endif
	#ifndef EXTERN_C_END
	#define EXTERN_C_END
	#endif
#endif

#endif														// Of #ifndef EXTERN_C_H.

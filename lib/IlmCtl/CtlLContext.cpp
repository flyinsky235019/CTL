///////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013 Academy of Motion Picture Arts and Sciences 
// ("A.M.P.A.S."). Portions contributed by others as indicated.
// All rights reserved.
// 
// A worldwide, royalty-free, non-exclusive right to copy, modify, create
// derivatives, and use, in source and binary forms, is hereby granted, 
// subject to acceptance of this license. Performance of any of the 
// aforementioned acts indicates acceptance to be bound by the following 
// terms and conditions:
//
//  * Copies of source code, in whole or in part, must retain the 
//    above copyright notice, this list of conditions and the 
//    Disclaimer of Warranty.
//
//  * Use in binary form must retain the above copyright notice, 
//    this list of conditions and the Disclaimer of Warranty in the
//    documentation and/or other materials provided with the distribution.
//
//  * Nothing in this license shall be deemed to grant any rights to 
//    trademarks, copyrights, patents, trade secrets or any other 
//    intellectual property of A.M.P.A.S. or any contributors, except 
//    as expressly stated herein.
//
//  * Neither the name "A.M.P.A.S." nor the name of any other 
//    contributors to this software may be used to endorse or promote 
//    products derivative of or based on this software without express 
//    prior written permission of A.M.P.A.S. or the contributors, as 
//    appropriate.
// 
// This license shall be construed pursuant to the laws of the State of 
// California, and any disputes related thereto shall be subject to the 
// jurisdiction of the courts therein.
//
// Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND 
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT ARE DISCLAIMED. IN NO 
// EVENT SHALL A.M.P.A.S., OR ANY CONTRIBUTORS OR DISTRIBUTORS, BE LIABLE 
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, RESITUTIONARY, 
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
// THE POSSIBILITY OF SUCH DAMAGE.
//
// WITHOUT LIMITING THE GENERALITY OF THE FOREGOING, THE ACADEMY 
// SPECIFICALLY DISCLAIMS ANY REPRESENTATIONS OR WARRANTIES WHATSOEVER 
// RELATED TO PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS IN THE ACADEMY 
// COLOR ENCODING SYSTEM, OR APPLICATIONS THEREOF, HELD BY PARTIES OTHER 
// THAN A.M.P.A.S., WHETHER DISCLOSED OR UNDISCLOSED.
///////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
//	class LContext
//
//-----------------------------------------------------------------------------

#include <CtlLContext.h>
#include <CtlModule.h>
#include <iostream>
#include <CtlMessage.h>

using namespace std;

namespace Ctl {


LContext::LContext (istream &file, Module *module, SymbolTable &symtab):
    _file (file),
    _module (module),
    _symtab (symtab)
{
    // empty
}


LContext::~LContext ()
{
    // empty
}


const string &
LContext::fileName () const
{
    return _module->fileName();
}


void
LContext::foundError (int lineNumber, Error e)
{
    LineError le = {lineNumber,e};
    _lineErrors.insert(le);
}


void
LContext::declareError (int lineNumber, Error e)
{
    LineError le = {lineNumber,e};
    _declErrors.insert(le);
}


bool
LContext::errorDeclared (int lineNumber, Error e)
{
    LineError le = {lineNumber,e};
    return (_declErrors.find(le) != _declErrors.end());
}


void
LContext::catchErrors ()
{
    for(set<LineError>::iterator it = _declErrors.begin();
	it != _declErrors.end();)
    {
	set<LineError>::iterator found = _lineErrors.find(*it);
	set<LineError>::iterator itErase = it;
	it++;

	// if the declared error is found, erase it
	if( found != _lineErrors.end() )
	{
	    _lineErrors.erase(found);
	    _declErrors.erase(itErase);
	}
    }
}


int 	
LContext::numErrors () const   
{
    return _lineErrors.size() + _declErrors.size();
}


void
LContext::printDeclaredErrors () const
{
    if(_declErrors.size() > 0)
    {
	for(set<LineError>::const_iterator it = _declErrors.begin();
	    it != _declErrors.end();
	    it++)
	{
	    MESSAGE_LN(fileName(), it->lineNumber, 
		       "Declared error not found: @error" << it->error << "\n");
	}
    }
}


ArrayTypePtr	
LContext::newArrayType (const DataTypePtr &baseType, 
			SizeVector sizes,
			ArrayTypeUsage usage /* = NON_PARAMETER */)
{
    DataTypePtr elementType = baseType;
    for(int depth = sizes.size() - 1; depth >= 0; depth--)
    {
	elementType = newArrayType(elementType, sizes[depth], usage);
    }
    return elementType;
}

} // namespace Ctl

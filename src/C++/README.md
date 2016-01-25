#EmailValidation Class#


A C++ class implementation of email validation class.


Example
---------



    
    #include <iostream>
	#include <windows.h>
	#include "EmailValidator.h"
	using namespace std;

	int main()
	{
	    EmailValidator ev;
	
	    if(ev.validate("valid.ipv6v4.addr@[IPv6:aaaa:aaaa:aaaa:aaaa:aaaa:aaaa:127.0.0.1]"))
	        MessageBoxA(NULL,"Valid", "Email Status", MB_OK || MB_ICONINFORMATION);
	    else
	        MessageBoxA(NULL,"Invalid", "Email Status", MB_OK || MB_ICONERROR);
	
	    return 0;
	}

License
----------
    Copyright (c) 2015 Mbadiwe Nnaemeka Ronald ron2tele@gmail.com

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the author be held liable for any damages
    arising from the use of this software.
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:
    
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation must be
    specified.
    
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
    
    3. This notice may not be removed or altered from any source distribution.
        
        
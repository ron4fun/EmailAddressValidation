/*
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

    3. This notice must not be removed or altered from any source distribution.
*/

#include <iostream>
#include <wchar.h>
#include <windows.h>
#include <vector>
#include "EmailValidator.h"

using namespace std;

int main()
{
    vector<wchar_t *> ValidAddresses =
    {
        L"\"Abc\\@def\"@example.com",
        L"\"Fred Bloggs\"@example.com",
        L"\"Joe\\\\Blow\"@example.com",
        L"\"Abc@def\"@example.com",
        L"customer/department=shipping@example.com",
        L"$A12345@example.com",
        L"!def!xyz%abc@example.com",
        L"_somename@example.com",
        L"valid.ipv4.addr@[123.1.72.10]",
        L"valid.ipv6.addr@[IPv6:0::1]",
        L"valid.ipv6.addr@[IPv6:2607:f0d0:1002:51::4]",
        L"valid.ipv6.addr@[IPv6:fe80::230:48ff:fe33:bc33]",
        L"valid.ipv6v4.addr@[IPv6:aaaa:aaaa:aaaa:aaaa:aaaa:aaaa:127.0.0.1]",

        // examples from wikipedia
        L"niceandsimple@example.com",
        L"very.common@example.com",
        L"a.little.lengthy.but.fine@dept.example.com",
        L"disposable.style.email.with+symbol@example.com",
        L"user@[IPv6:2001:db8:1ff::a0b:dbd0]",
        L"\"much.more unusual\"@example.com",
        L"\"very.unusual.@.unusual.com\"@example.com",
        L"\"very.(),:;<>[]\\\".VERY.\\\"very@\\\\ \\\"very\\\".unusual\"@strange.example.com",
        L"postbox@com",
        L"admin@mailserver1",
        L"!#$%&'*+-/=?^_`{}|~@example.org",
        L"\"()<>[]:,;@\\\\\\\"!#$%&'*+-/=?^_`{}| ~.a\"@example.org",
        L"\" \"@example.org"
    };

    vector<wchar_t *> InvalidAddresses =
    {
        L"",
        L"invalid",
        L"invalid@",
        L"invalid @",
        L"invalid@[555.666.777.888]",
        L"invalid@[IPv6:123456]",
        L"invalid@[127.0.0.1.]",
        L"invalid@[127.0.0.1].",
        L"invalid@[127.0.0.1]x",

        // examples from wikipedia
        L"Abc.example.com",
        L"A@b@c@example.com",
        L"a\"b(c)d,e:f;g<h>i[j\\k]l@example.com",
        L"just\"not\"right@example.com",
        L"this is\"not\\allowed@example.com",
        L"this\\ still\\\"not\\\\allowed@example.com",

        // examples of real (invalid) input from real users.
        L"No longer available.",
        L"Moved."
    };

    vector<wchar_t *> ValidInternationalAddresses =
    {
        L"伊昭傑@郵件.商務", // Chinese
        L"राम@मोहन.ईन्फो", // Hindi
        L"юзер@екзампл.ком", // Ukranian
        L"θσερ@εχαμπλε.ψομ", // Greek
    };

    for(size_t i = 0; i < ValidAddresses.size(); i++)
    {
        if(!EmailValidator::validate(ValidAddresses[i]))
        {
            cout << "Test 1 failed..." << endl;
            MessageBoxW(NULL, ValidAddresses[i], L"Report", MB_OK || MB_ICONINFORMATION);
        } // end if
    } // end for

    for(size_t i = 0; i < InvalidAddresses.size(); i++)
    {
        if(EmailValidator::validate(InvalidAddresses[i]))
        {
            cout << "Test 2 failed..." << endl;
            MessageBoxW(NULL, InvalidAddresses[i], L"Report", MB_OK || MB_ICONINFORMATION);
        } // end if
    } // end for

    for(size_t i = 0; i < ValidInternationalAddresses.size(); i++)
    {
        if(!EmailValidator::validate(ValidInternationalAddresses[i], true))
        {
            cout << "Test 3 failed..." << endl;
            MessageBoxW(NULL, ValidInternationalAddresses[i], L"Report", MB_OK || MB_ICONINFORMATION);
        } // end if
    } // end for

    cout << "Testing Done!" << endl;

    return 0;
}

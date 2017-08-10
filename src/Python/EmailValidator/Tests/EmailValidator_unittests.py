# -*- coding: utf-8 -*-

"""
Thanks to Jeffrey Stedfast <jeff@xamarin.com> for this testcases.
"""
"""
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
"""

import unittest

from EmailValidator import *

class EmailValidatorTests(unittest.TestCase):
    def setUp(self):
        self.ValidAddresses = ["\"Abc\\@def\"@example.com",
                        "\"Fred Bloggs\"@example.com",
                        "\"Joe\\\\Blow\"@example.com",
                        "\"Abc@def\"@example.com",
                        "customer/department=shipping@example.com",
                        "$A12345@example.com",
                        "!def!xyz%abc@example.com",
                        "_somename@example.com",
                        "valid.ipv4.addr@[123.1.72.10]",
                        "valid.ipv6.addr@[IPv6:0::1]",
                        "valid.ipv6.addr@[IPv6:2607:f0d0:1002:51::4]",
                        "valid.ipv6.addr@[IPv6:fe80::230:48ff:fe33:bc33]",
                        "valid.ipv6v4.addr@[IPv6:aaaa:aaaa:aaaa:aaaa:aaaa:aaaa:127.0.0.1]",

                        # examples from wikipedia
                        "niceandsimple@example.com",
                        "very.common@example.com",
                        "a.little.lengthy.but.fine@dept.example.com",
                        "disposable.style.email.with+symbol@example.com",
                        "user@[IPv6:2001:db8:1ff::a0b:dbd0]",
                        "\"much.more unusual\"@example.com",
                        "\"very.unusual.@.unusual.com\"@example.com",
                        "\"very.(),:;<>[]\\\".VERY.\\\"very@\\\\ \\\"very\\\".unusual\"@strange.example.com",
                        "postbox@com",
                        "admin@mailserver1",
                        "!#$%&'*+-/=?^_`{}|~@example.org",
                        "\"()<>[]:,;@\\\\\\\"!#$%&'*+-/=?^_`{}| ~.a\"@example.org",
                        "\" \"@example.org"
                          ]

        self.InvalidAddresses = ["",
			"invalid",
			"invalid@",
			"invalid @",
			"invalid@[555.666.777.888]",
			"invalid@[IPv6:123456]",
			"invalid@[127.0.0.1.]",
			"invalid@[127.0.0.1].",
			"invalid@[127.0.0.1]x",

			# examples from wikipedia
			"Abc.example.com",
			"A@b@c@example.com",
			"a\"b(c)d,e:f;g<h>i[j\\k]l@example.com",
			"just\"not\"right@example.com",
			"this is\"not\\allowed@example.com",
			"this\\ still\\\"not\\\\allowed@example.com",

			# examples of real (invalid) input from real users.
			"No longer available.",
			"Moved."
                            ]

        self.ValidInternationalAddresses = [
			u"伊昭傑@郵件.商務",    # Chinese
			u"राम@मोहन.ईन्फो",       # Hindi
			u"юзер@екзампл.ком", # Ukranian
			u"θσερ@εχαμπλε.ψομ", # Greek
                        ]

    
    def test_TestValidAddresses(self):
        for addr in self.ValidAddresses:
            self.assertTrue(EmailValidator.validate(addr))

    def test_TestInvalidAddresses(self):
        for addr in self.InvalidAddresses:
            self.assertFalse(EmailValidator.validate(addr))

    def test_TestValidInternationalAddresses(self):
        for addr in self.ValidInternationalAddresses:
            self.assertTrue(EmailValidator.validate(addr, True))

  
if __name__ ==  '__main__':
    unittest.main()

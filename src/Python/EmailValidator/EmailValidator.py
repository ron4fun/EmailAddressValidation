# -*- coding: utf-8 -*-

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
    
    3. This notice may not be removed or altered from any source distribution.
"""

from ctypes import pointer, c_int

class EmailValidator(object):
    _AtomCharacters = "!#$%&'*+-/=?^_`{|}~"

    @staticmethod
    def validate(email, allowInternational=False):
        placeValue = c_int(0)
        s = pointer(placeValue)
        index = s.contents

        if (email is None):
            raise AssertionError("email is null")

        if (len(email) == 0):
            return False

        if (not EmailValidator.__skipWord(email, index, allowInternational) or index.value >= len(email)):
            return False

        while (email[index.value] == '.'):
            index.value += 1

            if (index.value >= len(email)):
                return False

            if (not EmailValidator.__skipWord(email, index, allowInternational)):
                return False

            if (index.value >= len(email)):
                return False

        if (index.value + 1 >= len(email) or email[index.value] != '@'):
            return False
        
        index.value += 1

        if (email[index.value] != '['):
            # domain
            if (not EmailValidator.__skipDomain(email, index, allowInternational)):
                return False
            return index.value == len(email)

        # address literal
        index.value += 1

        # we need at least 8 more characters
        if (index.value + 8 >= len(email)):
            return False

        ipv6 = (email[index.value:index.value+5]).lower()

        if (ipv6 == "ipv6:"):
            index.value += len("IPv6:")
            if (not EmailValidator.__skipIPv6Literal(email, index)):
                return False;
        else:
            if (not EmailValidator.__skipIPv4Literal(email, index)):
                return False

        if (index.value >= len(email) or email[index.value] != ']'):
            return False

        index.value += 1

        return index.value == len(email)

    @staticmethod
    def __isLetterOrDigit(c):
        if c.isdigit() or c.isalpha():
            return True
        return False

    @staticmethod
    def __isAtom(c, allowInternational):
        if ord(c) < 128:
            check = True if EmailValidator._AtomCharacters.find(c) != -1 else False
            return EmailValidator.__isLetterOrDigit(c) or check
        return allowInternational

    @staticmethod
    def __isDomain(c, allowInternational):
        if ord(c) < 128:
            return EmailValidator.__isLetterOrDigit(c) or c == '-'
        return allowInternational

    @staticmethod
    def __skipAtom(text, index, allowInternational):	
        startIndex = index.value
        while (index.value < len(text) and EmailValidator.__isAtom(text[index.value], allowInternational)):
            index.value += 1
        return index.value > startIndex

    @staticmethod
    def __skipSubDomain(text, index, allowInternational):
        if (not EmailValidator.__isDomain(text[index.value], allowInternational) or text[index.value] == '-'):
            return False
        index.value += 1
        while (index.value < len(text) and EmailValidator.__isDomain(text[index.value], allowInternational)):
            index.value += 1
        return True

    @staticmethod
    def __skipDomain(text, index, allowInternational):
        if (not EmailValidator.__skipSubDomain(text, index, allowInternational)):
                return False
        while(index.value < len(text) and text[index.value] == '.'):
            index.value += 1
            if (index.value == len(text)):
                return False
            if (not EmailValidator.__skipSubDomain(text, index, allowInternational)):
                return False
        return True

    @staticmethod
    def __skipQuoted(text, index, allowInternational):
        escaped = False

        # skip over leading '"'
        index.value += 1

        while(index.value < len(text)):
            if (ord(text[index.value]) >= 128 and not allowInternational):
                return False
            if (text[index.value] == '\\'):
                escaped = not escaped
            elif (not escaped):
                if (text[index.value] == '"'):
                    break
            else:
                escaped = False;
            index.value += 1
            
        if (index.value >= len(text) or text[index.value] != '"'):
            return False
        index.value += 1
        return True

    @staticmethod
    def __skipWord(text, index, allowInternational):
        if (text[index.value] == '"'):
                return EmailValidator.__skipQuoted(text, index, allowInternational)
        return EmailValidator.__skipAtom(text, index, allowInternational)

    @staticmethod
    def __isHexDigit(c):
        s = "ABCDEFabcdef0123456789"
        if s.find(c) != -1:
            return True
        return False

    @staticmethod
    def __skipIPv4Literal(text, index):
        groups = 0

        while (index.value < len(text) and groups < 4):
            startIndex = index.value
            value = 0

            while(index.value < len(text) and ord(text[index.value]) >= ord('0') and ord(text[index.value]) <= ord('9')):
                value = (value * 10) + (ord(text[index.value]) - ord('0'))
                index.value += 1

            if(index.value == startIndex and index.value - startIndex > 3 or value > 255):
                return False

            groups += 1

            if (groups < 4 and index.value < len(text) and text[index.value] == '.'):
                index.value += 1

        return groups == 4

    @staticmethod
    def __skipIPv6Literal(text, index):
        compact = False
        colons = 0

        while (index.value < len(text)):
            startIndex = index.value

            while(index.value < len(text) and EmailValidator.__isHexDigit(text[index.value])):
                index.value += 1

            if (index.value >= len(text)):
                break

            if (index.value > startIndex and colons > 2 and text[index.value] == '.'):
                # IPv6v4
                index.value = startIndex

                if (not EmailValidator.__skipIPv4Literal(text, index)):
                    return False;
                break

            count = index.value - startIndex
            if (count > 4):
                return False

            if (text[index.value] != ':'):
                break

            startIndex = index.value
            while (index.value < len(text) and text[index.value] == ':'):
                index.value += 1

            count = index.value - startIndex
            if (count > 2):
                return False

            if (count == 2):
                if (compact):
                    return False

                compact = True
                colons += 2
            else:
                colons += 1

        if (colons < 2):
            return False;

        if (compact):
            return colons < 6

        return colons < 7


if __name__ == '__main__':
    print EmailValidator.validate(u"чебурашка@ящик-с-апельсинами.рф", True)

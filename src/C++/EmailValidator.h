/*
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
*/

#ifndef EMAILVALIDATOR_H
#define EMAILVALIDATOR_H

#include <cstring>
#include <wchar.h>
#include <iomanip>

using namespace std;

class EmailValidator
{
    static const char *AtomCharacters;

    public:
        static bool validate(const wchar_t *email, const bool allowInternational = false);
        //static bool validate(const string &email, bool allowInternational = false)
        //{
        //    return validate(email.c_str(), allowInternational);
        //} // end function validate

    private:
        static void lowerCase(wchar_t *str)
        {
            unsigned int i = 0;
            for (; i < wcslen(str); i++)
            {
                if(isalpha(str[i]))
                    str[i] = towlower(str[i]);
            } // end for
        } // end func lowerCase

        static bool isLetterOrDigit(const unsigned int c)
        {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
        } // end func isLetterOrDigit

        static bool isAtom(const unsigned int c, const bool allowInternational)
        {
            return c < 128 ? isLetterOrDigit(c) || strchr(AtomCharacters, c): allowInternational;
        } // end func isAtom

        static bool isDomain(const unsigned int c, const bool allowInternational)
        {
            return c < 128 ? isLetterOrDigit(c) || c == '-' : allowInternational;
        } // end func isDomain

        static bool skipAtom(const wchar_t *text, int * const index, const bool allowInternational)
		{
			unsigned int startIndex = *index;

			while (*index < wcslen(text) && isAtom((unsigned int)(text[*index]), allowInternational))
				(*index)++;

			return *index > startIndex;
		} // end func skipAtom

		static bool skipSubDomain(const wchar_t *text, int * const index, const bool allowInternational)
		{
			if (!isDomain(text[*index], allowInternational) || (unsigned int)(text[*index]) == '-')
				return false;

			(*index)++;

			while (*index < wcslen(text) && isDomain((unsigned int)(text[*index]), allowInternational))
				(*index)++;

			return true;
		} // end func skipSubDomain

		static bool skipDomain(const wchar_t *text, int * const index, const bool allowInternational)
		{
			if (!skipSubDomain(text, index, allowInternational))
				return false;

			while(*index < wcslen(text) && (unsigned int)(text[*index]) == '.') {
				(*index)++;

				if (*index == wcslen(text))
					return false;

				if (!skipSubDomain(text, index, allowInternational))
					return false;
			} // end while

			return true;
		} // end func skipDomain

		static bool skipQuoted(const wchar_t *text, int * const index, const bool allowInternational)
		{
			bool escaped = false;

			// skip over leading '"'
			(*index)++;

			while(*index < wcslen(text)) {
				if ((unsigned int)(text[*index]) >= 128 && !allowInternational)
					return false;

				if ((unsigned int)(text[*index]) == '\\') {
					escaped = !escaped;
				} else if (!escaped) {
					if ((unsigned int)(text[*index]) == '"')
						break;
				} else {
					escaped = false;
				} // end else

				(*index)++;
			} // end while

			if (*index >= wcslen(text) || (unsigned int)(text[*index]) != '"')
				return false;

			(*index)++;

			return true;
		} // end func skipQuoted

		static bool skipWord(const wchar_t *text, int * const index, const bool allowInternational)
		{
			if ((unsigned int)(text[*index]) == '"')
				return skipQuoted(text, index, allowInternational);

			return skipAtom(text, index, allowInternational);
		} // end func skipWord

        static bool isHexDigit(const unsigned int c)
		{
			return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9');
		} // end func isHexDigit

		static bool skipIPv4Literal(const wchar_t *text, int * const index);
		static bool skipIPv6Literal(const wchar_t *text, int * const index);
};

#endif // EMAILVALIDATOR_H

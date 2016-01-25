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
#include <iomanip>
using namespace std;

class EmailValidator
{
    static const char *AtomCharacters;

    public:
        EmailValidator();

        bool validate(const char *email, bool allowInternational = false);

    private:
        void lowerCase(char *str)
        {
            int i = 0;
            for (; i < strlen(str); i++)
            {
                if(isalpha(str[i]))
                    str[i] = tolower(str[i]);
            } // end for
        } // end func lowerCase

        bool isLetterOrDigit(const char c)
        {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
        } // end func isLetterOrDigit

        bool isAtom(const char c, bool allowInternational)
        {
            return c < 128 ? isLetterOrDigit(c) || strchr(AtomCharacters, c): allowInternational;
        } // end func isAtom

        bool isDomain(const char c, bool allowInternational)
        {
            return c < 128 ? isLetterOrDigit(c) || c == '-' : allowInternational;
        } // end func isDomain

        bool skipAtom(const char *text, int * const index, bool allowInternational)
		{
			int startIndex = *index;

			while (*index < strlen(text) && isAtom(text[*index], allowInternational))
				(*index)++;

			return *index > startIndex;
		} // end func skipAtom

		bool skipSubDomain(const char *text, int * const index, bool allowInternational)
		{
			if (!isDomain(text[*index], allowInternational) || text[*index] == '-')
				return false;

			(*index)++;

			while (*index < strlen(text) && isDomain(text[*index], allowInternational))
				(*index)++;

			return true;
		} // end func skipSubDomain

		bool skipDomain(const char *text, int * const index, bool allowInternational)
		{
			if (!skipSubDomain(text, index, allowInternational))
				return false;

			while(*index < strlen(text) && text[*index] == '.') {
				(*index)++;

				if (*index == strlen(text))
					return false;

				if (!skipSubDomain(text, index, allowInternational))
					return false;
			} // end while

			return true;
		} // end func skipDomain

		bool skipQuoted(const char *text, int * const index, bool allowInternational)
		{
			bool escaped = false;

			// skip over leading '"'
			(*index)++;

			while(*index < strlen(text)) {
				if (text[*index] >= 128 && !allowInternational)
					return false;

				if (text[*index] == '\\') {
					escaped = !escaped;
				} else if (!escaped) {
					if (text[*index] == '"')
						break;
				} else {
					escaped = false;
				} // end else

				(*index)++;
			} // end while

			if (*index >= strlen(text) || text[*index] != '"')
				return false;

			(*index)++;

			return true;
		} // end func skipQuoted

		bool skipWord(const char *text, int * const index, bool allowInternational)
		{
			if (text[*index] == '"')
				return skipQuoted(text, index, allowInternational);

			return skipAtom(text, index, allowInternational);
		} // end func skipWord

        bool isHexDigit(char c)
		{
			return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9');
		} // end func isHexDigit

		bool skipIPv4Literal(const char *text, int * const index);
		bool skipIPv6Literal(const char *text, int * const index);
};

#endif // EMAILVALIDATOR_H

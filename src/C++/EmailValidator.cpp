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

#include "EmailValidator.h"
#include <stdexcept>
using namespace std;

const char *EmailValidator::AtomCharacters = "!#$%&'*+-/=?^_`{|}~";

bool EmailValidator::skipIPv4Literal(const wchar_t *text, int * const index)
{
    int groups = 0;

    while (*index < wcslen(text) && groups < 4) {
        int startIndex = *index;
        unsigned int value = 0;

        while(*index < wcslen(text) && (unsigned int)(text[*index]) >= '0' && (unsigned int)(text[*index]) <= '9') {
            value = (value * 10) + ((unsigned int)(text[*index]) - '0');
            (*index)++;
        } // end while

        if(*index == startIndex || *index - startIndex > 3 || value > 255)
            return false;

        groups++;

        if (groups < 4 && *index < wcslen(text) && (unsigned int)(text[*index]) == '.')
            (*index)++;
    } // end while

    return groups == 4;
} // end func skipIPv4Literal

// This needs to handle the following forms:
//
// IPv6-addr = IPv6-full / IPv6-comp / IPv6v4-full / IPv6v4-comp
// IPv6-hex  = 1*4HEXDIG
// IPv6-full = IPv6-hex 7(":" IPv6-hex)
// IPv6-comp = [IPv6-hex *5(":" IPv6-hex)] "::" [IPv6-hex *5(":" IPv6-hex)]
//             ; The "::" represents at least 2 16-bit groups of zeros
//             ; No more than 6 groups in addition to the "::" may be
//             ; present
// IPv6v4-full = IPv6-hex 5(":" IPv6-hex) ":" IPv4-address-literal
// IPv6v4-comp = [IPv6-hex *3(":" IPv6-hex)] "::"
//               [IPv6-hex *3(":" IPv6-hex) ":"] IPv4-address-literal
//             ; The "::" represents at least 2 16-bit groups of zeros
//             ; No more than 4 groups in addition to the "::" and
//             ; IPv4-address-literal may be present

bool EmailValidator::skipIPv6Literal(const wchar_t *text, int * const index)
{
    bool compact = false;
    int colons = 0;

    while (*index < wcslen(text)) {
        int startIndex = *index;

        while(*index < wcslen(text) && isHexDigit(text[*index]))
            (*index)++;

        if (*index >= wcslen(text))
            break;

        if (*index > startIndex && colons > 2 && (unsigned int)(text[*index]) == '.') {
            // IPv6v4
            *index = startIndex;

            if (!skipIPv4Literal(text, index))
                return false;

            break;
        } // end if

        int count = *index - startIndex;
        if (count > 4)
            return false;

        if ((unsigned int)(text[*index]) != ':')
            break;

        startIndex = *index;
        while (*index < wcslen(text) && (unsigned int)(text[*index]) == ':')
            (*index)++;

        count = *index - startIndex;
        if (count > 2)
            return false;

        if (count == 2) {
            if (compact)
                return false;

            compact = true;
            colons += 2;
        } else {
            colons++;
        } // end else
    }

    if (colons < 2)
        return false;

    if (compact)
        return colons < 6;

    return colons < 7;
} // end func skipIPv6Literal

/// <summary>
/// Validate the specified email address.
/// </summary>
/// <remarks>
/// <para>Validates the syntax of an email address.</para>
/// <para>If <paramref name="allowInternational"/> is <value>true</value>, then the validator
/// will use the newer International Email standards for validating the email address.</para>
/// </remarks>
/// <returns><c>true</c> if the email address is valid; otherwise <c>false</c>.</returns>
/// <param name="email">An email address.</param>
/// <param name="allowInternational"><value>true</value> if the validator should allow international characters; otherwise, <value>false</value>.</param>
/// <exception cref="System.ArgumentNullException">
/// <paramref name="email"/> is <c>null</c>.
/// </exception>

bool EmailValidator::validate(const wchar_t *email, bool allowInternational)
{
    int index = 0;

    if (email == NULL)
        throw invalid_argument("email is null");

    if (wcslen(email) == 0)
        return false;

    if (!skipWord(email, &index, allowInternational) || index >= wcslen(email))
        return false;

    while ((unsigned int)(email[index]) == '.') {
        index++;

        if (index >= wcslen(email))
            return false;

        if (!skipWord(email, &index, allowInternational))
            return false;

        if (index >= wcslen(email))
            return false;
    }

    if (index + 1 >= wcslen(email) || (unsigned int)(email[index++]) != '@')
        return false;

    if ((unsigned int)(email[index]) != '[') {
        // domain
        if (!skipDomain(email, &index, allowInternational))
            return false;

        return index == wcslen(email);
    } // end if

    // address literal
    index++;

    // we need at least 8 more characters
    if (index + 8 >= wcslen(email))
        return false;

    wchar_t *ipv6 = new wchar_t[7]; //email.Substring (index, 5);
    wmemset(ipv6, 0, 7);
    wmemcpy(ipv6, &email[index], 5);

    lowerCase(ipv6);
    if (wcscmp(ipv6, L"ipv6:") != -1) {
        index += wcslen(L"IPv6:");
        if (!skipIPv6Literal(email, &index))
            return false;
    } else {
        if (!skipIPv4Literal(email, &index))
            return false;
    }

    if (index >= wcslen(email) || (unsigned int)(email[index++]) != ']')
        return false;

    if(ipv6)
        delete ipv6;

    return index == wcslen(email);

} // end func validate


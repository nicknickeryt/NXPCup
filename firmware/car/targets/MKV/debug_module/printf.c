///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2018, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdint.h>
#include "printf.h"


// ntoa conversion buffer size, this must be big enough to hold
// one converted numeric number including padded zeros (dynamically created on stack)
// 32 byte is a good default
#define PRINTF_NTOA_BUFFER_SIZE    32U

// ftoa conversion buffer size, this must be big enough to hold
// one converted float number including padded zeros (dynamically created on stack)
// 32 byte is a good default
#define PRINTF_FTOA_BUFFER_SIZE    32U

// define this to support floating point (%f)
#define PRINTF_SUPPORT_FLOAT

// define this to support long long types (%llu or %p)
#define PRINTF_SUPPORT_LONG_LONG

// define this to support the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
#define PRINTF_SUPPORT_PTRDIFF_T


///////////////////////////////////////////////////////////////////////////////
typedef enum {
    FORMAT_FLAG_LEFT_ZEROPAD = 1 << 0, // '0' flag
    FORMAT_FLAG_JUSTIFY_LEFT = 1 << 1, // '-' flag
    FORMAT_FLAG_FORCE_SIGN = 1 << 2, // '+' flag
    FORMAT_FLAG_BLANK_SIGN = 1 << 3, // ' ' (space) flag
    FORMAT_FLAG_INDICATE_BASE = 1 << 4, // '#' flag
    FORMAT_FLAG_PRECISION_SPECIFIED = 1 << 5, // '.' precision found
    FORMAT_FLAG_WIDTH_SPECIFIED = 1 << 6, // Number after '%'
    FORMAT_FLAG_UPPERCASE_HEX = 1 << 7, // 'X' flag

    FORMAT_FLAG_TYPE_CHAR = 1 << 8,
    FORMAT_FLAG_TYPE_SHORT = 1 << 9,
    FORMAT_FLAG_TYPE_LONG = 1 << 10,
    FORMAT_FLAG_TYPE_LONG_LONG = 1 << 11,

    FORMAT_FLAG_NEGATIVE_VALUE = 1 << 12,
} FormatFlag;




// output function type
typedef void (*out_fct_type)(char character, void* buffer, size_t idx, size_t maxlen);


// wrapper (used as buffer) for output function type
typedef struct {
    void  (*fct)(char character, void* arg);
    void* arg;
} out_fct_wrap_type;


// internal null output
static inline void _out_null(char character, void* buffer, size_t idx, size_t maxlen)
{
    (void)character; (void)buffer; (void)idx; (void)maxlen;
}


// internal output function wrapper
static inline void _out_fct(char character, void* buffer, size_t idx, size_t maxlen)
{
    (void)idx; (void)maxlen;
    // buffer is the output fct pointer
    ((out_fct_wrap_type*)buffer)->fct(character, ((out_fct_wrap_type*)buffer)->arg);
}


// internal strlen
// \return The length of the string (excluding the terminating 0)
static inline unsigned int _strlen(const char* str)
{
    const char* s;
    for (s = str; *s; ++s);
    return (unsigned int)(s - str);
}


// internal test if char is a digit (0-9)
// \return true if char is a digit
static inline bool _is_digit(char ch)
{
    return (ch >= '0') && (ch <= '9');
}


// internal ASCII string to unsigned int conversion
static inline unsigned int _atoi(const char** str) {
    unsigned int i = 0U;
    while (_is_digit(**str)) {
        i = i * 10U + (unsigned int)(*((*str)++) - '0');
    }
    return i;
}


// internal itoa format
static size_t _ntoa_format(out_fct_type out, char* buffer, size_t idx, size_t maxlen, char* buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, FormatFlag flags)
{
    const size_t start_idx = idx;

    // pad leading zeros
    while (!(flags & FORMAT_FLAG_JUSTIFY_LEFT) && (len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
        buf[len++] = '0';
    }
    while (!(flags & FORMAT_FLAG_JUSTIFY_LEFT) && (flags & FORMAT_FLAG_LEFT_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
        buf[len++] = '0';
    }

    // handle hash
    if (flags & FORMAT_FLAG_INDICATE_BASE) {
        if (((len == prec) || (len == width)) && (len > 0U)) {
            len--;
            if ((base == 16U) && (len > 0U)) {
                len--;
            }
        }
        if ((base == 16U) && !(flags & FORMAT_FLAG_UPPERCASE_HEX) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'x';
        }
        if ((base == 16U) &&  (flags & FORMAT_FLAG_UPPERCASE_HEX) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'X';
        }
        if (len < PRINTF_NTOA_BUFFER_SIZE) {
            buf[len++] = '0';
        }
    }

    // handle sign
    if ((len == width) && (negative || (flags & FORMAT_FLAG_FORCE_SIGN) || (flags & FORMAT_FLAG_BLANK_SIGN))) {
        len--;
    }
    if (len < PRINTF_NTOA_BUFFER_SIZE) {
        if (negative) {
            buf[len++] = '-';
        }
        else if (flags & FORMAT_FLAG_FORCE_SIGN) {
            buf[len++] = '+';  // ignore the space if the '+' exists
        }
        else if (flags & FORMAT_FLAG_BLANK_SIGN) {
            buf[len++] = ' ';
        }
    }

    // pad spaces up to given width
    if (!(flags & FORMAT_FLAG_JUSTIFY_LEFT) && !(flags & FORMAT_FLAG_LEFT_ZEROPAD)) {
        for (size_t i = len; i < width; i++) {
            out(' ', buffer, idx++, maxlen);
        }
    }

    // reverse string
    for (size_t i = 0U; i < len; i++) {
        out(buf[len - i - 1U], buffer, idx++, maxlen);
    }

    // append pad spaces up to given width
    if (flags & FORMAT_FLAG_JUSTIFY_LEFT) {
        while (idx - start_idx < width) {
            out(' ', buffer, idx++, maxlen);
        }
    }

    return idx;
}


// internal itoa for 'long' type
static size_t _ntoa_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width, FormatFlag flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    // write if precision != 0 and value is != 0
    if (!(flags & FORMAT_FLAG_PRECISION_SPECIFIED) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] = digit < 10 ? '0' + digit : (flags & FORMAT_FLAG_UPPERCASE_HEX ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}


// internal itoa for 'long long' type
#if defined(PRINTF_SUPPORT_LONG_LONG)
static size_t _ntoa_long_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long long value, bool negative, unsigned long long base, unsigned int prec, unsigned int width, FormatFlag flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    // write if precision != 0 and value is != 0
    if (!(flags & FORMAT_FLAG_PRECISION_SPECIFIED) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] = digit < 10 ? '0' + digit : (flags & FORMAT_FLAG_UPPERCASE_HEX ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}
#endif  // PRINTF_SUPPORT_LONG_LONG


#if defined(PRINTF_SUPPORT_FLOAT)
static size_t _ftoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, FormatFlag flags)
{
    char buf[PRINTF_FTOA_BUFFER_SIZE];
    size_t len  = 0U;
    double diff = 0.0;

    // if input is larger than thres_max, revert to exponential
    const double thres_max = (double)0x7FFFFFFF;

    // powers of 10
    static const double pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

    // test for negative
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = 0 - value;
    }

    // set default precision to 6, if not set explicitly
    if (!(flags & FORMAT_FLAG_PRECISION_SPECIFIED)) {
        prec = 6U;
    }
    // limit precision to 9, cause a prec >= 10 can lead to overflow errors
    while ((len < PRINTF_FTOA_BUFFER_SIZE) && (prec > 9U)) {
        buf[len++] = '0';
        prec--;
    }

    int whole = (int)value;
    double tmp = (value - whole) * pow10[prec];
    unsigned long frac = (unsigned long)tmp;
    diff = tmp - frac;

    if (diff > 0.5) {
        ++frac;
        // handle rollover, e.g. case 0.99 with prec 1 is 1.0
        if (frac >= pow10[prec]) {
            frac = 0;
            ++whole;
        }
    }
    else if ((diff == 0.5) && ((frac == 0U) || (frac & 1U))) {
        // if halfway, round up if odd, OR if last digit is 0
        ++frac;
    }

    // TBD: for very large numbers switch back to native sprintf for exponentials. Anyone want to write code to replace this?
    // Normal printf behavior is to print EVERY whole number digit which can be 100s of characters overflowing your buffers == bad
    if (value > thres_max) {
        return 0U;
    }

    if (prec == 0U) {
        diff = value - (double)whole;
        if (diff > 0.5) {
            // greater than 0.5, round up, e.g. 1.6 -> 2
            ++whole;
        }
        else if ((diff == 0.5) && (whole & 1)) {
            // exactly 0.5 and ODD, then round up
            // 1.5 -> 2, but 2.5 -> 2
            ++whole;
        }
    }
    else {
        unsigned int count = prec;
        // now do fractional part, as an unsigned number
        while (len < PRINTF_FTOA_BUFFER_SIZE) {
            --count;
            buf[len++] = (char)(48U + (frac % 10U));
            if (!(frac /= 10U)) {
                break;
            }
        }
        // add extra 0s
        while ((len < PRINTF_FTOA_BUFFER_SIZE) && (count-- > 0U)) {
            buf[len++] = '0';
        }
        if (len < PRINTF_FTOA_BUFFER_SIZE) {
            // add decimal
            buf[len++] = '.';
        }
    }

    // do whole part, number is reversed
    while (len < PRINTF_FTOA_BUFFER_SIZE) {
        buf[len++] = (char)(48 + (whole % 10));
        if (!(whole /= 10)) {
            break;
        }
    }

    // pad leading zeros
    while (!(flags & FORMAT_FLAG_JUSTIFY_LEFT) && (flags & FORMAT_FLAG_LEFT_ZEROPAD) && (len < width) && (len < PRINTF_FTOA_BUFFER_SIZE)) {
        buf[len++] = '0';
    }

    // handle sign
    if ((len == width) && (negative || (flags & FORMAT_FLAG_FORCE_SIGN) || (flags & FORMAT_FLAG_BLANK_SIGN))) {
        len--;
    }
    if (len < PRINTF_FTOA_BUFFER_SIZE) {
        if (negative) {
            buf[len++] = '-';
        }
        else if (flags & FORMAT_FLAG_FORCE_SIGN) {
            buf[len++] = '+';  // ignore the space if the '+' exists
        }
        else if (flags & FORMAT_FLAG_BLANK_SIGN) {
            buf[len++] = ' ';
        }
    }

    // pad spaces up to given width
    if (!(flags & FORMAT_FLAG_JUSTIFY_LEFT) && !(flags & FORMAT_FLAG_LEFT_ZEROPAD)) {
        for (size_t i = len; i < width; i++) {
            out(' ', buffer, idx++, maxlen);
        }
    }

    // reverse string
    for (size_t i = 0U; i < len; i++) {
        out(buf[len - i - 1U], buffer, idx++, maxlen);
    }

    // append pad spaces up to given width
    if (flags & FORMAT_FLAG_JUSTIFY_LEFT) {
        while (idx < width) {
            out(' ', buffer, idx++, maxlen);
        }
    }

    return idx;
}
#endif  // PRINTF_SUPPORT_FLOAT


// internal vsnprintf
static int _vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* format, va_list va)
{
    unsigned int n;
    size_t idx = 0U;

    if (!buffer) {
        // use null output function
        out = _out_null;
    }

    while (*format)
    {
        // format specifier?  %[flags][width][.precision][length]
        if (*format != '%') {
            // no
            out(*format, buffer, idx++, maxlen);
            format++;
            continue;
        }
        else {
            // yes, evaluate it
            format++;
        }

        // evaluate flags
        FormatFlag flags = 0;
        do {
            switch (*format) {
                case '0': flags |= FORMAT_FLAG_LEFT_ZEROPAD; format++; n = 1U; break;
                case '-': flags |= FORMAT_FLAG_JUSTIFY_LEFT;    format++; n = 1U; break;
                case '+': flags |= FORMAT_FLAG_FORCE_SIGN;    format++; n = 1U; break;
                case ' ': flags |= FORMAT_FLAG_BLANK_SIGN;   format++; n = 1U; break;
                case '#': flags |= FORMAT_FLAG_INDICATE_BASE;    format++; n = 1U; break;
                default :                                   n = 0U; break;
            }
        } while (n);

        // evaluate width field
        unsigned int width = 0;
        if (_is_digit(*format)) {
            width = _atoi(&format);
        }
        else if (*format == '*') {
            const int w = va_arg(va, int);
            if (w < 0) {
                flags |= FORMAT_FLAG_JUSTIFY_LEFT;    // reverse padding
                width = (unsigned int)-w;
            }
            else {
                width = (unsigned int)w;
            }
            format++;
        }

        // evaluate precision field
        unsigned int precision = 0;
        if (*format == '.') {
            flags |= FORMAT_FLAG_PRECISION_SPECIFIED;
            format++;
            if (_is_digit(*format)) {
                precision = _atoi(&format);
            }
            else if (*format == '*') {
                precision = (unsigned int)va_arg(va, int);
                format++;
            }
        }

        // evaluate length field
        switch (*format) {
            case 'l' :
                flags |= FORMAT_FLAG_TYPE_LONG;
                format++;
                if (*format == 'l') {
                    flags |= FORMAT_FLAG_PRECISION_SPECIFIED;
                    format++;
                }
                break;
            case 'h' :
                flags |= FORMAT_FLAG_TYPE_SHORT;
                format++;
                if (*format == 'h') {
                    flags |= FORMAT_FLAG_TYPE_CHAR;
                    format++;
                }
                break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
            case 't' :
                flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FORMAT_FLAG_TYPE_LONG : FORMAT_FLAG_PRECISION_SPECIFIED);
                format++;
                break;
#endif
            case 'j' :
                flags |= (sizeof(intmax_t) == sizeof(long) ? FORMAT_FLAG_TYPE_LONG : FORMAT_FLAG_PRECISION_SPECIFIED);
                format++;
                break;
            case 'z' :
                flags |= (sizeof(size_t) == sizeof(long) ? FORMAT_FLAG_TYPE_LONG : FORMAT_FLAG_PRECISION_SPECIFIED);
                format++;
                break;
            default :
                break;
        }

        // evaluate specifier
        switch (*format) {
            case 'd' :
            case 'i' :
            case 'u' :
            case 'x' :
            case 'X' :
            case 'o' :
            case 'b' : {
                // set the base
                unsigned int base;
                if (*format == 'x' || *format == 'X') {
                    base = 16U;
                }
                else if (*format == 'o') {
                    base =  8U;
                }
                else if (*format == 'b') {
                    base =  2U;
                    flags &= ~FORMAT_FLAG_INDICATE_BASE;   // no hash for bin format
                }
                else {
                    base = 10U;
                    flags &= ~FORMAT_FLAG_INDICATE_BASE;   // no hash for dec format
                }
                // uppercase
                if (*format == 'X') {
                    flags |= FORMAT_FLAG_UPPERCASE_HEX;
                }

                // no plus or space flag for u, x, X, o, b
                if ((*format != 'i') && (*format != 'd')) {
                    flags &= ~(FORMAT_FLAG_FORCE_SIGN | FORMAT_FLAG_BLANK_SIGN);
                }

                // convert the integer
                if ((*format == 'i') || (*format == 'd')) {
                    // signed
                    if (flags & FORMAT_FLAG_PRECISION_SPECIFIED) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                        const long long value = va_arg(va, long long);
                        idx = _ntoa_long_long(out, buffer, idx, maxlen, (unsigned long long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
#endif
                    }
                    else if (flags & FORMAT_FLAG_TYPE_LONG) {
                        const long value = va_arg(va, long);
                        idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                    }
                    else {
                        const int value = (flags & FORMAT_FLAG_TYPE_CHAR) ? (char)va_arg(va, int) : (flags & FORMAT_FLAG_TYPE_SHORT) ? (short int)va_arg(va, int) : va_arg(va, int);
                        idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                    }
                }
                else {
                    // unsigned
                    if (flags & FORMAT_FLAG_PRECISION_SPECIFIED) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                        idx = _ntoa_long_long(out, buffer, idx, maxlen, va_arg(va, unsigned long long), false, base, precision, width, flags);
#endif
                    }
                    else if (flags & FORMAT_FLAG_TYPE_LONG) {
                        idx = _ntoa_long(out, buffer, idx, maxlen, va_arg(va, unsigned long), false, base, precision, width, flags);
                    }
                    else {
                        const unsigned int value = (flags & FORMAT_FLAG_TYPE_CHAR) ? (unsigned char)va_arg(va, unsigned int) : (flags & FORMAT_FLAG_TYPE_SHORT) ? (unsigned short int)va_arg(va, unsigned int) : va_arg(va, unsigned int);
                        idx = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
                    }
                }
                format++;
                break;
            }
#if defined(PRINTF_SUPPORT_FLOAT)
            case 'f' :
            case 'F' :
                idx = _ftoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
                format++;
                break;
#endif  // PRINTF_SUPPORT_FLOAT
            case 'c' : {
                unsigned int l = 1U;
                // pre padding
                if (!(flags & FORMAT_FLAG_JUSTIFY_LEFT)) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, maxlen);
                    }
                }
                // char output
                out((char)va_arg(va, int), buffer, idx++, maxlen);
                // post padding
                if (flags & FORMAT_FLAG_JUSTIFY_LEFT) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, maxlen);
                    }
                }
                format++;
                break;
            }

            case 's' : {
                char* p = va_arg(va, char*);
                unsigned int l = _strlen(p);
                // pre padding
                if (flags & FORMAT_FLAG_PRECISION_SPECIFIED) {
                    l = (l < precision ? l : precision);
                }
                if (!(flags & FORMAT_FLAG_JUSTIFY_LEFT)) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, maxlen);
                    }
                }
                // string output
                while ((*p != 0) && (!(flags & FORMAT_FLAG_PRECISION_SPECIFIED) || precision--)) {
                    out(*(p++), buffer, idx++, maxlen);
                }
                // post padding
                if (flags & FORMAT_FLAG_JUSTIFY_LEFT) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, maxlen);
                    }
                }
                format++;
                break;
            }

            case 'p' : {
                width = sizeof(void*) * 2U;
                flags |= FORMAT_FLAG_LEFT_ZEROPAD | FORMAT_FLAG_UPPERCASE_HEX;
#if defined(PRINTF_SUPPORT_LONG_LONG)
                const bool is_ll = sizeof(uintptr_t) == sizeof(long long);
                if (is_ll) {
                    idx = _ntoa_long_long(out, buffer, idx, maxlen, (uintptr_t)va_arg(va, void*), false, 16U, precision, width, flags);
                }
                else {
#endif
                    idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)((uintptr_t)va_arg(va, void*)), false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
                }
#endif
                format++;
                break;
            }

            case '%' :
                out('%', buffer, idx++, maxlen);
                format++;
                break;

            default :
                out(*format, buffer, idx++, maxlen);
                format++;
                break;
        }
    }

    // termination
    out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

    // return written chars without terminating \0
    return (int)idx;
}


///////////////////////////////////////////////////////////////////////////////


int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    const out_fct_wrap_type out_fct_wrap = { out, arg };
    const int ret = _vsnprintf(_out_fct, (char*)&out_fct_wrap, (size_t)-1, format, va);
    va_end(va);
    return ret;
}

int vfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, va_list va)
{
    const out_fct_wrap_type out_fct_wrap = { out, arg };
    const int ret = _vsnprintf(_out_fct, (char*)&out_fct_wrap, (size_t)-1, format, va);
    return ret;
}



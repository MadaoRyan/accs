/*
 *
 *
 * Useful Macros
 *
 *
 * FileName: util_macro.h
 *
 *  <Date>        <Author>       <Auditor>     <Desc>
 */
#ifndef _UTIL_MACRO_H_20101104140701_
#define _UTIL_MACRO_H_20101104140701_
/*--------------------------- Include files -----------------------------*/

/*--------------------------- Macro define ------------------------------*/
/*
 * doubly linked list macros (non-circular)
 */
#define DL_PREPEND(head,add)                                                  \
do {                                                                          \
    (add)->next = head;                                                       \
    if (head) {                                                               \
        (add)->prev = (head)->prev;                                           \
        (head)->prev = (add);                                                 \
    } else {                                                                  \
        (add)->prev = (add);                                                  \
    }                                                                         \
    (head) = (add);                                                           \
} while (0)

#define DL_APPEND(head,add)                                                   \
do {                                                                          \
    if (head) {                                                               \
        (add)->prev = (head)->prev;                                           \
        (head)->prev->next = (add);                                           \
        (head)->prev = (add);                                                 \
        (add)->next = NULL;                                                   \
    } else {                                                                  \
        (head) = (add);                                                       \
        (head)->prev = (head);                                                \
        (head)->next = NULL;                                                  \
    }                                                                         \
} while (0);

#define DL_DELETE(head,del)                                                   \
do {                                                                          \
    if ((del)->prev == (del)) {                                               \
        (head) = NULL;                                                        \
    } else if ((del)==(head)) {                                               \
        (del)->next->prev = (del)->prev;                                      \
        (head) = (del)->next;                                                 \
    } else {                                                                  \
        (del)->prev->next = (del)->next;                                      \
        if ((del)->next) {                                                    \
            (del)->next->prev = (del)->prev;                                  \
        } else {                                                              \
            (head)->prev = (del)->prev;                                       \
        }                                                                     \
    }                                                                         \
} while (0);

#define DL_FOREACH(head,el)                                                   \
    for (el = head; el; el = el->next)

/* this version is safe for deleting the elements during iteration */
#define DL_FOREACH_SAFE(head,el,tmp)                                          \
    for ((el) = (head); (el) && (tmp = (el)->next, 1); (el) = tmp)

#define DL_SEARCH_SCALAR(head,out,field,val)                                  \
do {                                                                          \
    DL_FOREACH(head,out) {                                                    \
        if ((out)->field == (val)) break;                                     \
    }                                                                         \
} while(0)

#define DL_SEARCH(head,out,elt,cmp)                                           \
do {                                                                          \
    DL_FOREACH(head,out) {                                                    \
        if ((cmp(out,elt)) == 0) break;                                       \
    }                                                                         \
} while(0)

/*
 * bitstring
 */
/* bitstr_t	sBit[bit_size(20)]; */
typedef unsigned char bitstr_t;

/* internal macros, do not use it */
/* byte of the bitstring bit is in */
#define _bit_byte(bit)	((bit) >> 3)

/* mask for the bit within its byte */
#define _bit_mask(bit)	(1 << ((bit)&0x7))

/* external macros */
/* bytes in a bitstring of nbits bits */
#define bit_size(nbits)			((((nbits) - 1) >> 3) + 1)

/* is bit N of bitstring name set? */
#define bit_test(name, bit)		((name)[_bit_byte(bit)] & _bit_mask(bit))

/* set bit N of bitstring name */
#define bit_set(name, bit)		(name)[_bit_byte(bit)] |= _bit_mask(bit)

/* clear bit N of bitstring name */
#define bit_clear(name, bit)	(name)[_bit_byte(bit)] &= ~_bit_mask(bit)

/* set bits start ... stop in bitstring */
#define bit_nset(name, start, stop)                                           \
do {                                                                          \
    register bitstr_t *_name = name;                                          \
    register int _start = start, _stop = stop;                                \
    register int _startbyte = _bit_byte(_start);                              \
    register int _stopbyte = _bit_byte(_stop);                                \
    if (_startbyte == _stopbyte) {                                            \
        _name[_startbyte] |= ((0xff << (_start&0x7)) &                        \
                              (0xff >> (7 - (_stop&0x7))));                   \
    } else {                                                                  \
        _name[_startbyte] |= 0xff << ((_start)&0x7);                          \
        while (++_startbyte < _stopbyte)                                      \
            _name[_startbyte] = 0xff;                                         \
        _name[_stopbyte] |= 0xff >> (7 - (_stop&0x7));                        \
    }                                                                         \
} while(0)

/* clear bits start ... stop in bitstring */
#define bit_nclear(name, start, stop)                                         \
do {                                                                          \
    register bitstr_t *_name = name;                                          \
    register int _start = start, _stop = stop;                                \
    register int _startbyte = _bit_byte(_start);                              \
    register int _stopbyte = _bit_byte(_stop);                                \
    if (_startbyte == _stopbyte) {                                            \
        _name[_startbyte] &= ((0xff >> (8 - (_start&0x7))) |                  \
                              (0xff << ((_stop&0x7) + 1)));                   \
    } else {                                                                  \
        _name[_startbyte] &= 0xff >> (8 - (_start&0x7));                      \
        while (++_startbyte < _stopbyte)                                      \
            _name[_startbyte] = 0;                                            \
        _name[_stopbyte] &= 0xff << ((_stop&0x7) + 1);                        \
    }                                                                         \
} while(0)

/* find first bit clear in name */
#define bit_ffc(name, nbits, value)                                           \
do {                                                                          \
    register bitstr_t *_name = name;                                          \
    register int _byte, _nbits = nbits;                                       \
    register int _stopbyte = _bit_byte(_nbits), _value = -1;                  \
    for (_byte = 0; _byte <= _stopbyte; ++_byte)                              \
        if (_name[_byte] != 0xff) {                                           \
            _value = _byte << 3;                                              \
            for (_stopbyte = _name[_byte]; (_stopbyte&0x1);                   \
                ++_value, _stopbyte >>= 1);                                   \
            break;                                                            \
        }                                                                     \
    *(value) = _value;                                                        \
} while(0)

/* find first bit set in name */
#define bit_ffs(name, nbits, value)                                           \
do {                                                                          \
    register bitstr_t *_name = name;                                          \
    register int _byte, _nbits = nbits;                                       \
    register int _stopbyte = _bit_byte(_nbits), _value = -1;                  \
    for (_byte = 0; _byte <= _stopbyte; ++_byte)                              \
        if (_name[_byte]) {                                                   \
            _value = _byte << 3;                                              \
            for (_stopbyte = _name[_byte]; !(_stopbyte&0x1);                  \
                ++_value, _stopbyte >>= 1);                                   \
            break;                                                            \
        }                                                                     \
    *(value) = _value;                                                        \
} while(0)

/* print bit string to a buffer */
#define bit_print(name, nbits, buf)                                           \
do {                                                                          \
    register int _nbits;                                                      \
    for (_nbits = 0; _nbits < nbits; _nbits++)                                \
        buf[_nbits] = bit_test(name, _nbits) ? '1': '0';                      \
} while(0)

#endif /*_UTIL_MACRO_H_20101104140701_*/
/*-----------------------------  End ------------------------------------*/

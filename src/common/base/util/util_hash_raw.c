#include	"util_hash.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stddef.h>
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef struct UT_hash_bucket {
    struct UT_hash_handle *hh_head;
    unsigned count;
    unsigned expand_mult;
} UT_hash_bucket;
typedef struct UT_hash_table {
    UT_hash_bucket *buckets;
    unsigned num_buckets, log2_num_buckets;
    unsigned num_items;
    struct UT_hash_handle *tail;
    ptrdiff_t hho;
    unsigned ideal_chain_maxlen;
    unsigned nonideal_items;
    unsigned ineff_expands, noexpand;
    uint32_t signature;
} UT_hash_table;
typedef struct UT_hash_handle {
    struct UT_hash_table *tbl;
    void *prev;
    void *next;
    struct UT_hash_handle *hh_prev;
    struct UT_hash_handle *hh_next;
    void *key;
    unsigned keylen;
    unsigned hashv;
} UT_hash_handle;
typedef struct {
    UT_hash_handle hh;
    void *pValue;
    char sKey[0];
} T_HASH_ENTRY;
struct T_HASH_TABLE {
    T_HASH_ENTRY *ptTable;
    int iInitSize;
    T_HASH_ENTRY *ptLast;
    T_HASH_FUNCS tFuncs;
};
static void *hashStringNew(T_HASH_FUNCS *ptFuncs,
                           const char *pKey, void *pValue);
static void *hashStructNew(T_HASH_FUNCS *ptFuncs,
                           const char *pKey, void *pValue);
static void hashComFree (T_HASH_FUNCS *ptFuncs,
                         const char *pKey, void *pValue);
T_HASH_TABLE *
hashNew(int iSize)
{
    T_HASH_TABLE *ptHashTable;
    ptHashTable = malloc(sizeof(T_HASH_TABLE));
    if (ptHashTable == NULL) {
        return NULL;
    }
    memset(ptHashTable, 0, sizeof(*ptHashTable));
    ptHashTable->ptTable = NULL;
    ptHashTable->iInitSize = iSize < 0 ? 0 : iSize;
    return ptHashTable;
}
int
hashAdd(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue)
{
    T_HASH_ENTRY *ptEntry;
    int iSize = 32, iLog2 = 5;
    int iRet = 0;
    if (!sKey) {
        return -4;
    }
    do {
        unsigned _hf_bkt,_hf_hashv;
        ptEntry=NULL;
        if (ptHashTable->ptTable) {
            do {
                unsigned _hj_i,_hj_j,_hj_k;
                char *_hj_key=(char*)(sKey);
                _hf_hashv = 0xfeedbeef;
                _hj_i = _hj_j = 0x9e3779b9;
                _hj_k = strlen(sKey);
                while (_hj_k >= 12) {
                    _hj_i += (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 ) + ( (unsigned)_hj_key[2] << 16 ) + ( (unsigned)_hj_key[3] << 24 ) );
                    _hj_j += (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 ) + ( (unsigned)_hj_key[6] << 16 ) + ( (unsigned)_hj_key[7] << 24 ) );
                    _hf_hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 ) + ( (unsigned)_hj_key[10] << 16 ) + ( (unsigned)_hj_key[11] << 24 ) );
                    do {
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 13 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 8 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 13 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 12 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 16 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 5 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 3 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 10 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 15 );
                    } while (0);
                    _hj_key += 12;
                    _hj_k -= 12;
                }
                _hf_hashv += strlen(sKey);
                switch ( _hj_k ) {
                case 11:
                    _hf_hashv += ( (unsigned)_hj_key[10] << 24 );
                case 10:
                    _hf_hashv += ( (unsigned)_hj_key[9] << 16 );
                case 9:
                    _hf_hashv += ( (unsigned)_hj_key[8] << 8 );
                case 8:
                    _hj_j += ( (unsigned)_hj_key[7] << 24 );
                case 7:
                    _hj_j += ( (unsigned)_hj_key[6] << 16 );
                case 6:
                    _hj_j += ( (unsigned)_hj_key[5] << 8 );
                case 5:
                    _hj_j += _hj_key[4];
                case 4:
                    _hj_i += ( (unsigned)_hj_key[3] << 24 );
                case 3:
                    _hj_i += ( (unsigned)_hj_key[2] << 16 );
                case 2:
                    _hj_i += ( (unsigned)_hj_key[1] << 8 );
                case 1:
                    _hj_i += _hj_key[0];
                }
                do {
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 13 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 8 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 13 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 12 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 16 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 5 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 3 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 10 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 15 );
                } while (0);
                _hf_bkt = _hf_hashv & ((ptHashTable->ptTable)->hh.tbl->num_buckets-1);
            } while (0);
            if ((1)) {
                do {
                    if ((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head) do {
                            char **_da_dst = (char**)(&(ptEntry));
                            *_da_dst = (char*)(((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                        } while (0);
                    else ptEntry=NULL;
                    while (ptEntry) {
                        if (ptEntry->hh.keylen == strlen(sKey)) {
                            if ((memcmp(ptEntry->hh.key,sKey,strlen(sKey))) == 0) break;
                        }
                        if (ptEntry->hh.hh_next) do {
                                char **_da_dst = (char**)(&(ptEntry));
                                *_da_dst = (char*)(((void*)(((char*)(ptEntry->hh.hh_next)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                            } while (0);
                        else ptEntry = NULL;
                    }
                } while (0);
            }
        }
    } while (0);
    if (ptEntry) {
        return -1;
    }
    ptEntry = malloc(sizeof(T_HASH_ENTRY) + strlen(sKey) + 1);
    if (ptEntry == NULL) {
        return -2;
    }
    strcpy(ptEntry->sKey, sKey);
    if (ptHashTable->tFuncs.fNew) {
        ptEntry->pValue = (ptHashTable)->tFuncs.fNew(&(ptHashTable)->tFuncs, (sKey), (pValue));
        if (ptEntry->pValue == NULL) {
            return -5;
        }
    } else {
        ptEntry->pValue = pValue;
    }
    if (!ptHashTable->ptTable) {
        if (ptHashTable->iInitSize > 0) {
            while (iSize < ptHashTable->iInitSize) {
                iSize *= 2;
                iLog2++;
            }
        }
    }
    do {
        unsigned _ha_bkt;
        (ptEntry)->hh.next = NULL;
        (ptEntry)->hh.key = (char*)&ptEntry->sKey;
        (ptEntry)->hh.keylen = strlen(ptEntry->sKey);
        if (!(ptHashTable->ptTable)) {
            ptHashTable->ptTable = (ptEntry);
            (ptHashTable->ptTable)->hh.prev = NULL;
            do {
                (ptHashTable->ptTable)->hh.tbl = (UT_hash_table*)malloc(sizeof(UT_hash_table));
                if (!((ptHashTable->ptTable)->hh.tbl)) {
                    (iRet = -1);
                }
                memset((ptHashTable->ptTable)->hh.tbl, 0, sizeof(UT_hash_table));
                (ptHashTable->ptTable)->hh.tbl->tail = &((ptHashTable->ptTable)->hh);
                (ptHashTable->ptTable)->hh.tbl->num_buckets = iSize;
                (ptHashTable->ptTable)->hh.tbl->log2_num_buckets = iLog2;
                (ptHashTable->ptTable)->hh.tbl->hho = (char*)(&(ptHashTable->ptTable)->hh) - (char*)(ptHashTable->ptTable);
                (ptHashTable->ptTable)->hh.tbl->buckets = (UT_hash_bucket*)malloc(iSize*sizeof(struct UT_hash_bucket));
                if (! (ptHashTable->ptTable)->hh.tbl->buckets) {
                    free((ptHashTable->ptTable)->hh.tbl);
                    (iRet = -1);
                }
                memset((ptHashTable->ptTable)->hh.tbl->buckets, 0, iSize*sizeof(struct UT_hash_bucket)); ;
                (ptHashTable->ptTable)->hh.tbl->signature = 0xa0111fe1;
            } while (0);
        } else {
            (ptHashTable->ptTable)->hh.tbl->tail->next = (ptEntry);
            (ptEntry)->hh.prev = ((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->tail)) - (((ptHashTable->ptTable)->hh.tbl)->hho)));
            (ptHashTable->ptTable)->hh.tbl->tail = &((ptEntry)->hh);
        } (ptHashTable->ptTable)->hh.tbl->num_items++;
        (ptEntry)->hh.tbl = (ptHashTable->ptTable)->hh.tbl;
        do {
            unsigned _hj_i,_hj_j,_hj_k;
            char *_hj_key=(char*)(&ptEntry->sKey);
            (ptEntry)->hh.hashv = 0xfeedbeef;
            _hj_i = _hj_j = 0x9e3779b9;
            _hj_k = strlen(ptEntry->sKey);
            while (_hj_k >= 12) {
                _hj_i += (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 ) + ( (unsigned)_hj_key[2] << 16 ) + ( (unsigned)_hj_key[3] << 24 ) );
                _hj_j += (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 ) + ( (unsigned)_hj_key[6] << 16 ) + ( (unsigned)_hj_key[7] << 24 ) );
                (ptEntry)->hh.hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 ) + ( (unsigned)_hj_key[10] << 16 ) + ( (unsigned)_hj_key[11] << 24 ) );
                do {
                    _hj_i -= _hj_j;
                    _hj_i -= (ptEntry)->hh.hashv;
                    _hj_i ^= ( (ptEntry)->hh.hashv >> 13 );
                    _hj_j -= (ptEntry)->hh.hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 8 );
                    (ptEntry)->hh.hashv -= _hj_i;
                    (ptEntry)->hh.hashv -= _hj_j;
                    (ptEntry)->hh.hashv ^= ( _hj_j >> 13 );
                    _hj_i -= _hj_j;
                    _hj_i -= (ptEntry)->hh.hashv;
                    _hj_i ^= ( (ptEntry)->hh.hashv >> 12 );
                    _hj_j -= (ptEntry)->hh.hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 16 );
                    (ptEntry)->hh.hashv -= _hj_i;
                    (ptEntry)->hh.hashv -= _hj_j;
                    (ptEntry)->hh.hashv ^= ( _hj_j >> 5 );
                    _hj_i -= _hj_j;
                    _hj_i -= (ptEntry)->hh.hashv;
                    _hj_i ^= ( (ptEntry)->hh.hashv >> 3 );
                    _hj_j -= (ptEntry)->hh.hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 10 );
                    (ptEntry)->hh.hashv -= _hj_i;
                    (ptEntry)->hh.hashv -= _hj_j;
                    (ptEntry)->hh.hashv ^= ( _hj_j >> 15 );
                } while (0);
                _hj_key += 12;
                _hj_k -= 12;
            } (ptEntry)->hh.hashv += strlen(ptEntry->sKey);
            switch ( _hj_k ) {
            case 11:
                (ptEntry)->hh.hashv += ( (unsigned)_hj_key[10] << 24 );
            case 10:
                (ptEntry)->hh.hashv += ( (unsigned)_hj_key[9] << 16 );
            case 9:
                (ptEntry)->hh.hashv += ( (unsigned)_hj_key[8] << 8 );
            case 8:
                _hj_j += ( (unsigned)_hj_key[7] << 24 );
            case 7:
                _hj_j += ( (unsigned)_hj_key[6] << 16 );
            case 6:
                _hj_j += ( (unsigned)_hj_key[5] << 8 );
            case 5:
                _hj_j += _hj_key[4];
            case 4:
                _hj_i += ( (unsigned)_hj_key[3] << 24 );
            case 3:
                _hj_i += ( (unsigned)_hj_key[2] << 16 );
            case 2:
                _hj_i += ( (unsigned)_hj_key[1] << 8 );
            case 1:
                _hj_i += _hj_key[0];
            }
            do {
                _hj_i -= _hj_j;
                _hj_i -= (ptEntry)->hh.hashv;
                _hj_i ^= ( (ptEntry)->hh.hashv >> 13 );
                _hj_j -= (ptEntry)->hh.hashv;
                _hj_j -= _hj_i;
                _hj_j ^= ( _hj_i << 8 );
                (ptEntry)->hh.hashv -= _hj_i;
                (ptEntry)->hh.hashv -= _hj_j;
                (ptEntry)->hh.hashv ^= ( _hj_j >> 13 );
                _hj_i -= _hj_j;
                _hj_i -= (ptEntry)->hh.hashv;
                _hj_i ^= ( (ptEntry)->hh.hashv >> 12 );
                _hj_j -= (ptEntry)->hh.hashv;
                _hj_j -= _hj_i;
                _hj_j ^= ( _hj_i << 16 );
                (ptEntry)->hh.hashv -= _hj_i;
                (ptEntry)->hh.hashv -= _hj_j;
                (ptEntry)->hh.hashv ^= ( _hj_j >> 5 );
                _hj_i -= _hj_j;
                _hj_i -= (ptEntry)->hh.hashv;
                _hj_i ^= ( (ptEntry)->hh.hashv >> 3 );
                _hj_j -= (ptEntry)->hh.hashv;
                _hj_j -= _hj_i;
                _hj_j ^= ( _hj_i << 10 );
                (ptEntry)->hh.hashv -= _hj_i;
                (ptEntry)->hh.hashv -= _hj_j;
                (ptEntry)->hh.hashv ^= ( _hj_j >> 15 );
            } while (0);
            _ha_bkt = (ptEntry)->hh.hashv & ((ptHashTable->ptTable)->hh.tbl->num_buckets-1);
        } while (0);
        do {
            (ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].count++;
            (&(ptEntry)->hh)->hh_next = (ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].hh_head;
            (&(ptEntry)->hh)->hh_prev = NULL;
            if ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].hh_head) {
                ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt]).hh_head->hh_prev = (&(ptEntry)->hh);
            } ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt]).hh_head=&(ptEntry)->hh;
            if ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].count >= (((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].expand_mult+1) * 10) && (&(ptEntry)->hh)->tbl->noexpand != 1) {
                do {
                    unsigned _he_bkt;
                    unsigned _he_bkt_i;
                    struct UT_hash_handle *_he_thh, *_he_hh_nxt;
                    UT_hash_bucket *_he_new_buckets, *_he_newbkt;
                    _he_new_buckets = (UT_hash_bucket*)malloc(2 * (&(ptEntry)->hh)->tbl->num_buckets * sizeof(struct UT_hash_bucket));
                    if (!_he_new_buckets) {
                        (iRet = -1);
                    }
                    memset(_he_new_buckets, 0, 2 * (&(ptEntry)->hh)->tbl->num_buckets * sizeof(struct UT_hash_bucket));
                    (&(ptEntry)->hh)->tbl->ideal_chain_maxlen = ((&(ptEntry)->hh)->tbl->num_items >> ((&(ptEntry)->hh)->tbl->log2_num_buckets+1)) + (((&(ptEntry)->hh)->tbl->num_items & (((&(ptEntry)->hh)->tbl->num_buckets*2)-1)) ? 1 : 0);
                    (&(ptEntry)->hh)->tbl->nonideal_items = 0;
                    for (_he_bkt_i = 0; _he_bkt_i < (&(ptEntry)->hh)->tbl->num_buckets; _he_bkt_i++) {
                        _he_thh = (&(ptEntry)->hh)->tbl->buckets[ _he_bkt_i ].hh_head;
                        while (_he_thh) {
                            _he_hh_nxt = _he_thh->hh_next;
                            do {
                                _he_bkt = ((_he_thh->hashv) & (((&(ptEntry)->hh)->tbl->num_buckets*2) - 1));
                            } while (0);
                            _he_newbkt = &(_he_new_buckets[ _he_bkt ]);
                            if (++(_he_newbkt->count) > (&(ptEntry)->hh)->tbl->ideal_chain_maxlen) {
                                (&(ptEntry)->hh)->tbl->nonideal_items++;
                                _he_newbkt->expand_mult = _he_newbkt->count / (&(ptEntry)->hh)->tbl->ideal_chain_maxlen;
                            }
                            _he_thh->hh_prev = NULL;
                            _he_thh->hh_next = _he_newbkt->hh_head;
                            if (_he_newbkt->hh_head) _he_newbkt->hh_head->hh_prev = _he_thh;
                            _he_newbkt->hh_head = _he_thh;
                            _he_thh = _he_hh_nxt;
                        }
                    }
                    free((&(ptEntry)->hh)->tbl->buckets);
                    (&(ptEntry)->hh)->tbl->num_buckets *= 2;
                    (&(ptEntry)->hh)->tbl->log2_num_buckets++;
                    (&(ptEntry)->hh)->tbl->buckets = _he_new_buckets;
                    (&(ptEntry)->hh)->tbl->ineff_expands = ((&(ptEntry)->hh)->tbl->nonideal_items > ((&(ptEntry)->hh)->tbl->num_items >> 1)) ? ((&(ptEntry)->hh)->tbl->ineff_expands+1) : 0;
                    if ((&(ptEntry)->hh)->tbl->ineff_expands > 1) {
                        (&(ptEntry)->hh)->tbl->noexpand=1; ;
                    } ;
                } while (0);
            }
        } while (0); ; ; ;
    } while (0);
    if (iRet < 0) {
        free(ptEntry);
        return -3;
    }
    return 0;
}
int
hashSet(T_HASH_TABLE *ptHashTable, const char *sKey, void *pValue)
{
    T_HASH_ENTRY *ptEntry;
    int iSize = 32, iLog2 = 5;
    int iRet = 0;
    if (!sKey) {
        return -4;
    }
    do {
        unsigned _hf_bkt,_hf_hashv;
        ptEntry=NULL;
        if (ptHashTable->ptTable) {
            do {
                unsigned _hj_i,_hj_j,_hj_k;
                char *_hj_key=(char*)(sKey);
                _hf_hashv = 0xfeedbeef;
                _hj_i = _hj_j = 0x9e3779b9;
                _hj_k = strlen(sKey);
                while (_hj_k >= 12) {
                    _hj_i += (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 ) + ( (unsigned)_hj_key[2] << 16 ) + ( (unsigned)_hj_key[3] << 24 ) );
                    _hj_j += (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 ) + ( (unsigned)_hj_key[6] << 16 ) + ( (unsigned)_hj_key[7] << 24 ) );
                    _hf_hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 ) + ( (unsigned)_hj_key[10] << 16 ) + ( (unsigned)_hj_key[11] << 24 ) );
                    do {
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 13 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 8 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 13 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 12 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 16 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 5 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 3 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 10 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 15 );
                    } while (0);
                    _hj_key += 12;
                    _hj_k -= 12;
                }
                _hf_hashv += strlen(sKey);
                switch ( _hj_k ) {
                case 11:
                    _hf_hashv += ( (unsigned)_hj_key[10] << 24 );
                case 10:
                    _hf_hashv += ( (unsigned)_hj_key[9] << 16 );
                case 9:
                    _hf_hashv += ( (unsigned)_hj_key[8] << 8 );
                case 8:
                    _hj_j += ( (unsigned)_hj_key[7] << 24 );
                case 7:
                    _hj_j += ( (unsigned)_hj_key[6] << 16 );
                case 6:
                    _hj_j += ( (unsigned)_hj_key[5] << 8 );
                case 5:
                    _hj_j += _hj_key[4];
                case 4:
                    _hj_i += ( (unsigned)_hj_key[3] << 24 );
                case 3:
                    _hj_i += ( (unsigned)_hj_key[2] << 16 );
                case 2:
                    _hj_i += ( (unsigned)_hj_key[1] << 8 );
                case 1:
                    _hj_i += _hj_key[0];
                }
                do {
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 13 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 8 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 13 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 12 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 16 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 5 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 3 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 10 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 15 );
                } while (0);
                _hf_bkt = _hf_hashv & ((ptHashTable->ptTable)->hh.tbl->num_buckets-1);
            } while (0);
            if ((1)) {
                do {
                    if ((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head) do {
                            char **_da_dst = (char**)(&(ptEntry));
                            *_da_dst = (char*)(((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                        } while (0);
                    else ptEntry=NULL;
                    while (ptEntry) {
                        if (ptEntry->hh.keylen == strlen(sKey)) {
                            if ((memcmp(ptEntry->hh.key,sKey,strlen(sKey))) == 0) break;
                        }
                        if (ptEntry->hh.hh_next) do {
                                char **_da_dst = (char**)(&(ptEntry));
                                *_da_dst = (char*)(((void*)(((char*)(ptEntry->hh.hh_next)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                            } while (0);
                        else ptEntry = NULL;
                    }
                } while (0);
            }
        }
    } while (0);
    if (ptEntry) {
        if (ptHashTable->tFuncs.fNew) {
            (ptHashTable)->tFuncs.fDelete(&(ptHashTable)->tFuncs, (ptEntry->sKey), (ptEntry->pValue));
            ptEntry->pValue = (ptHashTable)->tFuncs.fNew(&(ptHashTable)->tFuncs, (sKey), (pValue));
            if (ptEntry->pValue == NULL) {
                return -6;
            }
        } else {
            ptEntry->pValue = pValue;
        }
        return 0;
    }
    ptEntry = malloc(sizeof(T_HASH_ENTRY) + strlen(sKey) + 1);
    if (ptEntry == NULL) {
        return -2;
    }
    strcpy(ptEntry->sKey, sKey);
    if (ptHashTable->tFuncs.fNew) {
        ptEntry->pValue = (ptHashTable)->tFuncs.fNew(&(ptHashTable)->tFuncs, (sKey), (pValue));
        if (ptEntry->pValue == NULL) {
            return -5;
        }
    } else {
        ptEntry->pValue = pValue;
    }
    if (!ptHashTable->ptTable) {
        if (ptHashTable->iInitSize > 0) {
            while (iSize < ptHashTable->iInitSize) {
                iSize *= 2;
                iLog2++;
            }
        }
    }
    do {
        unsigned _ha_bkt;
        (ptEntry)->hh.next = NULL;
        (ptEntry)->hh.key = (char*)&ptEntry->sKey;
        (ptEntry)->hh.keylen = strlen(ptEntry->sKey);
        if (!(ptHashTable->ptTable)) {
            ptHashTable->ptTable = (ptEntry);
            (ptHashTable->ptTable)->hh.prev = NULL;
            do {
                (ptHashTable->ptTable)->hh.tbl = (UT_hash_table*)malloc(sizeof(UT_hash_table));
                if (!((ptHashTable->ptTable)->hh.tbl)) {
                    (iRet = -1);
                }
                memset((ptHashTable->ptTable)->hh.tbl, 0, sizeof(UT_hash_table));
                (ptHashTable->ptTable)->hh.tbl->tail = &((ptHashTable->ptTable)->hh);
                (ptHashTable->ptTable)->hh.tbl->num_buckets = iSize;
                (ptHashTable->ptTable)->hh.tbl->log2_num_buckets = iLog2;
                (ptHashTable->ptTable)->hh.tbl->hho = (char*)(&(ptHashTable->ptTable)->hh) - (char*)(ptHashTable->ptTable);
                (ptHashTable->ptTable)->hh.tbl->buckets = (UT_hash_bucket*)malloc(iSize*sizeof(struct UT_hash_bucket));
                if (! (ptHashTable->ptTable)->hh.tbl->buckets) {
                    free((ptHashTable->ptTable)->hh.tbl);
                    (iRet = -1);
                }
                memset((ptHashTable->ptTable)->hh.tbl->buckets, 0, iSize*sizeof(struct UT_hash_bucket)); ;
                (ptHashTable->ptTable)->hh.tbl->signature = 0xa0111fe1;
            } while (0);
        } else {
            (ptHashTable->ptTable)->hh.tbl->tail->next = (ptEntry);
            (ptEntry)->hh.prev = ((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->tail)) - (((ptHashTable->ptTable)->hh.tbl)->hho)));
            (ptHashTable->ptTable)->hh.tbl->tail = &((ptEntry)->hh);
        } (ptHashTable->ptTable)->hh.tbl->num_items++;
        (ptEntry)->hh.tbl = (ptHashTable->ptTable)->hh.tbl;
        do {
            unsigned _hj_i,_hj_j,_hj_k;
            char *_hj_key=(char*)(&ptEntry->sKey);
            (ptEntry)->hh.hashv = 0xfeedbeef;
            _hj_i = _hj_j = 0x9e3779b9;
            _hj_k = strlen(ptEntry->sKey);
            while (_hj_k >= 12) {
                _hj_i += (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 ) + ( (unsigned)_hj_key[2] << 16 ) + ( (unsigned)_hj_key[3] << 24 ) );
                _hj_j += (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 ) + ( (unsigned)_hj_key[6] << 16 ) + ( (unsigned)_hj_key[7] << 24 ) );
                (ptEntry)->hh.hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 ) + ( (unsigned)_hj_key[10] << 16 ) + ( (unsigned)_hj_key[11] << 24 ) );
                do {
                    _hj_i -= _hj_j;
                    _hj_i -= (ptEntry)->hh.hashv;
                    _hj_i ^= ( (ptEntry)->hh.hashv >> 13 );
                    _hj_j -= (ptEntry)->hh.hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 8 );
                    (ptEntry)->hh.hashv -= _hj_i;
                    (ptEntry)->hh.hashv -= _hj_j;
                    (ptEntry)->hh.hashv ^= ( _hj_j >> 13 );
                    _hj_i -= _hj_j;
                    _hj_i -= (ptEntry)->hh.hashv;
                    _hj_i ^= ( (ptEntry)->hh.hashv >> 12 );
                    _hj_j -= (ptEntry)->hh.hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 16 );
                    (ptEntry)->hh.hashv -= _hj_i;
                    (ptEntry)->hh.hashv -= _hj_j;
                    (ptEntry)->hh.hashv ^= ( _hj_j >> 5 );
                    _hj_i -= _hj_j;
                    _hj_i -= (ptEntry)->hh.hashv;
                    _hj_i ^= ( (ptEntry)->hh.hashv >> 3 );
                    _hj_j -= (ptEntry)->hh.hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 10 );
                    (ptEntry)->hh.hashv -= _hj_i;
                    (ptEntry)->hh.hashv -= _hj_j;
                    (ptEntry)->hh.hashv ^= ( _hj_j >> 15 );
                } while (0);
                _hj_key += 12;
                _hj_k -= 12;
            } (ptEntry)->hh.hashv += strlen(ptEntry->sKey);
            switch ( _hj_k ) {
            case 11:
                (ptEntry)->hh.hashv += ( (unsigned)_hj_key[10] << 24 );
            case 10:
                (ptEntry)->hh.hashv += ( (unsigned)_hj_key[9] << 16 );
            case 9:
                (ptEntry)->hh.hashv += ( (unsigned)_hj_key[8] << 8 );
            case 8:
                _hj_j += ( (unsigned)_hj_key[7] << 24 );
            case 7:
                _hj_j += ( (unsigned)_hj_key[6] << 16 );
            case 6:
                _hj_j += ( (unsigned)_hj_key[5] << 8 );
            case 5:
                _hj_j += _hj_key[4];
            case 4:
                _hj_i += ( (unsigned)_hj_key[3] << 24 );
            case 3:
                _hj_i += ( (unsigned)_hj_key[2] << 16 );
            case 2:
                _hj_i += ( (unsigned)_hj_key[1] << 8 );
            case 1:
                _hj_i += _hj_key[0];
            }
            do {
                _hj_i -= _hj_j;
                _hj_i -= (ptEntry)->hh.hashv;
                _hj_i ^= ( (ptEntry)->hh.hashv >> 13 );
                _hj_j -= (ptEntry)->hh.hashv;
                _hj_j -= _hj_i;
                _hj_j ^= ( _hj_i << 8 );
                (ptEntry)->hh.hashv -= _hj_i;
                (ptEntry)->hh.hashv -= _hj_j;
                (ptEntry)->hh.hashv ^= ( _hj_j >> 13 );
                _hj_i -= _hj_j;
                _hj_i -= (ptEntry)->hh.hashv;
                _hj_i ^= ( (ptEntry)->hh.hashv >> 12 );
                _hj_j -= (ptEntry)->hh.hashv;
                _hj_j -= _hj_i;
                _hj_j ^= ( _hj_i << 16 );
                (ptEntry)->hh.hashv -= _hj_i;
                (ptEntry)->hh.hashv -= _hj_j;
                (ptEntry)->hh.hashv ^= ( _hj_j >> 5 );
                _hj_i -= _hj_j;
                _hj_i -= (ptEntry)->hh.hashv;
                _hj_i ^= ( (ptEntry)->hh.hashv >> 3 );
                _hj_j -= (ptEntry)->hh.hashv;
                _hj_j -= _hj_i;
                _hj_j ^= ( _hj_i << 10 );
                (ptEntry)->hh.hashv -= _hj_i;
                (ptEntry)->hh.hashv -= _hj_j;
                (ptEntry)->hh.hashv ^= ( _hj_j >> 15 );
            } while (0);
            _ha_bkt = (ptEntry)->hh.hashv & ((ptHashTable->ptTable)->hh.tbl->num_buckets-1);
        } while (0);
        do {
            (ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].count++;
            (&(ptEntry)->hh)->hh_next = (ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].hh_head;
            (&(ptEntry)->hh)->hh_prev = NULL;
            if ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].hh_head) {
                ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt]).hh_head->hh_prev = (&(ptEntry)->hh);
            } ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt]).hh_head=&(ptEntry)->hh;
            if ((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].count >= (((ptHashTable->ptTable)->hh.tbl->buckets[_ha_bkt].expand_mult+1) * 10) && (&(ptEntry)->hh)->tbl->noexpand != 1) {
                do {
                    unsigned _he_bkt;
                    unsigned _he_bkt_i;
                    struct UT_hash_handle *_he_thh, *_he_hh_nxt;
                    UT_hash_bucket *_he_new_buckets, *_he_newbkt;
                    _he_new_buckets = (UT_hash_bucket*)malloc(2 * (&(ptEntry)->hh)->tbl->num_buckets * sizeof(struct UT_hash_bucket));
                    if (!_he_new_buckets) {
                        (iRet = -1);
                    }
                    memset(_he_new_buckets, 0, 2 * (&(ptEntry)->hh)->tbl->num_buckets * sizeof(struct UT_hash_bucket));
                    (&(ptEntry)->hh)->tbl->ideal_chain_maxlen = ((&(ptEntry)->hh)->tbl->num_items >> ((&(ptEntry)->hh)->tbl->log2_num_buckets+1)) + (((&(ptEntry)->hh)->tbl->num_items & (((&(ptEntry)->hh)->tbl->num_buckets*2)-1)) ? 1 : 0);
                    (&(ptEntry)->hh)->tbl->nonideal_items = 0;
                    for (_he_bkt_i = 0; _he_bkt_i < (&(ptEntry)->hh)->tbl->num_buckets; _he_bkt_i++) {
                        _he_thh = (&(ptEntry)->hh)->tbl->buckets[ _he_bkt_i ].hh_head;
                        while (_he_thh) {
                            _he_hh_nxt = _he_thh->hh_next;
                            do {
                                _he_bkt = ((_he_thh->hashv) & (((&(ptEntry)->hh)->tbl->num_buckets*2) - 1));
                            } while (0);
                            _he_newbkt = &(_he_new_buckets[ _he_bkt ]);
                            if (++(_he_newbkt->count) > (&(ptEntry)->hh)->tbl->ideal_chain_maxlen) {
                                (&(ptEntry)->hh)->tbl->nonideal_items++;
                                _he_newbkt->expand_mult = _he_newbkt->count / (&(ptEntry)->hh)->tbl->ideal_chain_maxlen;
                            }
                            _he_thh->hh_prev = NULL;
                            _he_thh->hh_next = _he_newbkt->hh_head;
                            if (_he_newbkt->hh_head) _he_newbkt->hh_head->hh_prev = _he_thh;
                            _he_newbkt->hh_head = _he_thh;
                            _he_thh = _he_hh_nxt;
                        }
                    }
                    free((&(ptEntry)->hh)->tbl->buckets);
                    (&(ptEntry)->hh)->tbl->num_buckets *= 2;
                    (&(ptEntry)->hh)->tbl->log2_num_buckets++;
                    (&(ptEntry)->hh)->tbl->buckets = _he_new_buckets;
                    (&(ptEntry)->hh)->tbl->ineff_expands = ((&(ptEntry)->hh)->tbl->nonideal_items > ((&(ptEntry)->hh)->tbl->num_items >> 1)) ? ((&(ptEntry)->hh)->tbl->ineff_expands+1) : 0;
                    if ((&(ptEntry)->hh)->tbl->ineff_expands > 1) {
                        (&(ptEntry)->hh)->tbl->noexpand=1; ;
                    } ;
                } while (0);
            }
        } while (0); ; ; ;
    } while (0);
    if (iRet < 0) {
        free(ptEntry);
        return -3;
    }
    return 0;
}
void *
hashGet(T_HASH_TABLE *ptHashTable, const char *sKey)
{
    T_HASH_ENTRY *ptEntry;
    if (!sKey) {
        return NULL;
    }
    do {
        unsigned _hf_bkt,_hf_hashv;
        ptEntry=NULL;
        if (ptHashTable->ptTable) {
            do {
                unsigned _hj_i,_hj_j,_hj_k;
                char *_hj_key=(char*)(sKey);
                _hf_hashv = 0xfeedbeef;
                _hj_i = _hj_j = 0x9e3779b9;
                _hj_k = strlen(sKey);
                while (_hj_k >= 12) {
                    _hj_i += (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 ) + ( (unsigned)_hj_key[2] << 16 ) + ( (unsigned)_hj_key[3] << 24 ) );
                    _hj_j += (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 ) + ( (unsigned)_hj_key[6] << 16 ) + ( (unsigned)_hj_key[7] << 24 ) );
                    _hf_hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 ) + ( (unsigned)_hj_key[10] << 16 ) + ( (unsigned)_hj_key[11] << 24 ) );
                    do {
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 13 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 8 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 13 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 12 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 16 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 5 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 3 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 10 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 15 );
                    } while (0);
                    _hj_key += 12;
                    _hj_k -= 12;
                }
                _hf_hashv += strlen(sKey);
                switch ( _hj_k ) {
                case 11:
                    _hf_hashv += ( (unsigned)_hj_key[10] << 24 );
                case 10:
                    _hf_hashv += ( (unsigned)_hj_key[9] << 16 );
                case 9:
                    _hf_hashv += ( (unsigned)_hj_key[8] << 8 );
                case 8:
                    _hj_j += ( (unsigned)_hj_key[7] << 24 );
                case 7:
                    _hj_j += ( (unsigned)_hj_key[6] << 16 );
                case 6:
                    _hj_j += ( (unsigned)_hj_key[5] << 8 );
                case 5:
                    _hj_j += _hj_key[4];
                case 4:
                    _hj_i += ( (unsigned)_hj_key[3] << 24 );
                case 3:
                    _hj_i += ( (unsigned)_hj_key[2] << 16 );
                case 2:
                    _hj_i += ( (unsigned)_hj_key[1] << 8 );
                case 1:
                    _hj_i += _hj_key[0];
                }
                do {
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 13 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 8 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 13 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 12 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 16 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 5 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 3 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 10 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 15 );
                } while (0);
                _hf_bkt = _hf_hashv & ((ptHashTable->ptTable)->hh.tbl->num_buckets-1);
            } while (0);
            if ((1)) {
                do {
                    if ((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head) do {
                            char **_da_dst = (char**)(&(ptEntry));
                            *_da_dst = (char*)(((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                        } while (0);
                    else ptEntry=NULL;
                    while (ptEntry) {
                        if (ptEntry->hh.keylen == strlen(sKey)) {
                            if ((memcmp(ptEntry->hh.key,sKey,strlen(sKey))) == 0) break;
                        }
                        if (ptEntry->hh.hh_next) do {
                                char **_da_dst = (char**)(&(ptEntry));
                                *_da_dst = (char*)(((void*)(((char*)(ptEntry->hh.hh_next)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                            } while (0);
                        else ptEntry = NULL;
                    }
                } while (0);
            }
        }
    } while (0);
    if (!ptEntry) {
        return NULL;
    }
    return ptEntry->pValue;
}
void *
hashDel(T_HASH_TABLE *ptHashTable, const char *sKey)
{
    T_HASH_ENTRY *ptEntry;
    void *pValue;
    if (!sKey) {
        return NULL;
    }
    do {
        unsigned _hf_bkt,_hf_hashv;
        ptEntry=NULL;
        if (ptHashTable->ptTable) {
            do {
                unsigned _hj_i,_hj_j,_hj_k;
                char *_hj_key=(char*)(sKey);
                _hf_hashv = 0xfeedbeef;
                _hj_i = _hj_j = 0x9e3779b9;
                _hj_k = strlen(sKey);
                while (_hj_k >= 12) {
                    _hj_i += (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 ) + ( (unsigned)_hj_key[2] << 16 ) + ( (unsigned)_hj_key[3] << 24 ) );
                    _hj_j += (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 ) + ( (unsigned)_hj_key[6] << 16 ) + ( (unsigned)_hj_key[7] << 24 ) );
                    _hf_hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 ) + ( (unsigned)_hj_key[10] << 16 ) + ( (unsigned)_hj_key[11] << 24 ) );
                    do {
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 13 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 8 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 13 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 12 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 16 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 5 );
                        _hj_i -= _hj_j;
                        _hj_i -= _hf_hashv;
                        _hj_i ^= ( _hf_hashv >> 3 );
                        _hj_j -= _hf_hashv;
                        _hj_j -= _hj_i;
                        _hj_j ^= ( _hj_i << 10 );
                        _hf_hashv -= _hj_i;
                        _hf_hashv -= _hj_j;
                        _hf_hashv ^= ( _hj_j >> 15 );
                    } while (0);
                    _hj_key += 12;
                    _hj_k -= 12;
                }
                _hf_hashv += strlen(sKey);
                switch ( _hj_k ) {
                case 11:
                    _hf_hashv += ( (unsigned)_hj_key[10] << 24 );
                case 10:
                    _hf_hashv += ( (unsigned)_hj_key[9] << 16 );
                case 9:
                    _hf_hashv += ( (unsigned)_hj_key[8] << 8 );
                case 8:
                    _hj_j += ( (unsigned)_hj_key[7] << 24 );
                case 7:
                    _hj_j += ( (unsigned)_hj_key[6] << 16 );
                case 6:
                    _hj_j += ( (unsigned)_hj_key[5] << 8 );
                case 5:
                    _hj_j += _hj_key[4];
                case 4:
                    _hj_i += ( (unsigned)_hj_key[3] << 24 );
                case 3:
                    _hj_i += ( (unsigned)_hj_key[2] << 16 );
                case 2:
                    _hj_i += ( (unsigned)_hj_key[1] << 8 );
                case 1:
                    _hj_i += _hj_key[0];
                }
                do {
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 13 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 8 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 13 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 12 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 16 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 5 );
                    _hj_i -= _hj_j;
                    _hj_i -= _hf_hashv;
                    _hj_i ^= ( _hf_hashv >> 3 );
                    _hj_j -= _hf_hashv;
                    _hj_j -= _hj_i;
                    _hj_j ^= ( _hj_i << 10 );
                    _hf_hashv -= _hj_i;
                    _hf_hashv -= _hj_j;
                    _hf_hashv ^= ( _hj_j >> 15 );
                } while (0);
                _hf_bkt = _hf_hashv & ((ptHashTable->ptTable)->hh.tbl->num_buckets-1);
            } while (0);
            if ((1)) {
                do {
                    if ((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head) do {
                            char **_da_dst = (char**)(&(ptEntry));
                            *_da_dst = (char*)(((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->buckets[ _hf_bkt ].hh_head)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                        } while (0);
                    else ptEntry=NULL;
                    while (ptEntry) {
                        if (ptEntry->hh.keylen == strlen(sKey)) {
                            if ((memcmp(ptEntry->hh.key,sKey,strlen(sKey))) == 0) break;
                        }
                        if (ptEntry->hh.hh_next) do {
                                char **_da_dst = (char**)(&(ptEntry));
                                *_da_dst = (char*)(((void*)(((char*)(ptEntry->hh.hh_next)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                            } while (0);
                        else ptEntry = NULL;
                    }
                } while (0);
            }
        }
    } while (0);
    if (!ptEntry) {
        return NULL;
    }
    pValue = ptEntry->pValue;
    if (ptHashTable->ptLast == ptEntry) {
        ptHashTable->ptLast = ptEntry->hh.prev;
    }
    do {
        unsigned _hd_bkt;
        struct UT_hash_handle *_hd_hh_del;
        if ( ((ptEntry)->hh.prev == NULL) && ((ptEntry)->hh.next == NULL) ) {
            free((ptHashTable->ptTable)->hh.tbl->buckets); ;
            free((ptHashTable->ptTable)->hh.tbl);
            ptHashTable->ptTable = NULL;
        } else {
            _hd_hh_del = &((ptEntry)->hh);
            if ((ptEntry) == ((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->tail)) - (((ptHashTable->ptTable)->hh.tbl)->hho)))) {
                (ptHashTable->ptTable)->hh.tbl->tail = (UT_hash_handle*)((char*)((ptEntry)->hh.prev) + (ptHashTable->ptTable)->hh.tbl->hho);
            }
            if ((ptEntry)->hh.prev) {
                ((UT_hash_handle*)((char*)((ptEntry)->hh.prev) + (ptHashTable->ptTable)->hh.tbl->hho))->next = (ptEntry)->hh.next;
            } else {
                do {
                    char **_da_dst = (char**)(&(ptHashTable->ptTable));
                    *_da_dst = (char*)((ptEntry)->hh.next);
                } while (0);
            }
            if (_hd_hh_del->next) {
                ((UT_hash_handle*)((char*)_hd_hh_del->next + (ptHashTable->ptTable)->hh.tbl->hho))->prev = _hd_hh_del->prev;
            }
            do {
                _hd_bkt = ((_hd_hh_del->hashv) & (((ptHashTable->ptTable)->hh.tbl->num_buckets) - 1));
            } while (0);
            ((ptHashTable->ptTable)->hh.tbl->buckets[_hd_bkt]).count--;
            if (((ptHashTable->ptTable)->hh.tbl->buckets[_hd_bkt]).hh_head == _hd_hh_del) {
                ((ptHashTable->ptTable)->hh.tbl->buckets[_hd_bkt]).hh_head = _hd_hh_del->hh_next;
            }
            if (_hd_hh_del->hh_prev) {
                _hd_hh_del->hh_prev->hh_next = _hd_hh_del->hh_next;
            }
            if (_hd_hh_del->hh_next) {
                _hd_hh_del->hh_next->hh_prev = _hd_hh_del->hh_prev;
            };
            (ptHashTable->ptTable)->hh.tbl->num_items--;
        } ;
    } while (0);
    if (ptHashTable->tFuncs.fDelete)
        (ptHashTable)->tFuncs.fDelete(&(ptHashTable)->tFuncs, (ptEntry->sKey), (ptEntry->pValue));
    free(ptEntry);
    return pValue;
}
int
hashFree(T_HASH_TABLE *ptHashTable)
{
    T_HASH_ENTRY *ptCur, *ptTmp;
    if (ptHashTable) {
        for ((ptCur)=(ptHashTable->ptTable), (*(char**)(&(ptTmp)))=(char*)((ptHashTable->ptTable)?(ptHashTable->ptTable)->hh.next:NULL); ptCur; (ptCur)=(ptTmp),(*(char**)(&(ptTmp)))=(char*)((ptTmp)?(ptTmp)->hh.next:NULL)) {
            do {
                unsigned _hd_bkt;
                struct UT_hash_handle *_hd_hh_del;
                if ( ((ptCur)->hh.prev == NULL) && ((ptCur)->hh.next == NULL) ) {
                    free((ptHashTable->ptTable)->hh.tbl->buckets); ;
                    free((ptHashTable->ptTable)->hh.tbl);
                    ptHashTable->ptTable = NULL;
                } else {
                    _hd_hh_del = &((ptCur)->hh);
                    if ((ptCur) == ((void*)(((char*)((ptHashTable->ptTable)->hh.tbl->tail)) - (((ptHashTable->ptTable)->hh.tbl)->hho)))) {
                        (ptHashTable->ptTable)->hh.tbl->tail = (UT_hash_handle*)((char*)((ptCur)->hh.prev) + (ptHashTable->ptTable)->hh.tbl->hho);
                    }
                    if ((ptCur)->hh.prev) {
                        ((UT_hash_handle*)((char*)((ptCur)->hh.prev) + (ptHashTable->ptTable)->hh.tbl->hho))->next = (ptCur)->hh.next;
                    } else {
                        do {
                            char **_da_dst = (char**)(&(ptHashTable->ptTable));
                            *_da_dst = (char*)((ptCur)->hh.next);
                        } while (0);
                    }
                    if (_hd_hh_del->next) {
                        ((UT_hash_handle*)((char*)_hd_hh_del->next + (ptHashTable->ptTable)->hh.tbl->hho))->prev = _hd_hh_del->prev;
                    }
                    do {
                        _hd_bkt = ((_hd_hh_del->hashv) & (((ptHashTable->ptTable)->hh.tbl->num_buckets) - 1));
                    } while (0);
                    ((ptHashTable->ptTable)->hh.tbl->buckets[_hd_bkt]).count--;
                    if (((ptHashTable->ptTable)->hh.tbl->buckets[_hd_bkt]).hh_head == _hd_hh_del) {
                        ((ptHashTable->ptTable)->hh.tbl->buckets[_hd_bkt]).hh_head = _hd_hh_del->hh_next;
                    }
                    if (_hd_hh_del->hh_prev) {
                        _hd_hh_del->hh_prev->hh_next = _hd_hh_del->hh_next;
                    }
                    if (_hd_hh_del->hh_next) {
                        _hd_hh_del->hh_next->hh_prev = _hd_hh_del->hh_prev;
                    };
                    (ptHashTable->ptTable)->hh.tbl->num_items--;
                } ;
            } while (0);
            if (ptHashTable->tFuncs.fDelete)
                (ptHashTable)->tFuncs.fDelete(&(ptHashTable)->tFuncs, (ptCur->sKey), (ptCur->pValue));
            free(ptCur);
        }
        free(ptHashTable->ptTable);
    }
    return 0;
}
int
hashCount(T_HASH_TABLE *ptHashTable)
{
    return ((ptHashTable->ptTable)?((ptHashTable->ptTable)->hh.tbl->num_items):0);
}
const char *
hashIter(T_HASH_TABLE *ptHashTable)
{
    ptHashTable->ptLast = ptHashTable->ptLast ? ptHashTable->ptLast->hh.next
                          : ptHashTable->ptTable;
    if (!ptHashTable->ptLast) {
        return NULL;
    }
    return ptHashTable->ptLast->sKey;
}
int
hashRewind(T_HASH_TABLE *ptHashTable)
{
    ptHashTable->ptLast = NULL;
    return 0;
}
int
hashSort(T_HASH_TABLE *ptHashTable, int (*fCompare)(void *, void *))
{
    do {
        unsigned _hs_i;
        unsigned _hs_looping,_hs_nmerges,_hs_insize,_hs_psize,_hs_qsize;
        struct UT_hash_handle *_hs_p, *_hs_q, *_hs_e, *_hs_list, *_hs_tail;
        if (ptHashTable->ptTable) {
            _hs_insize = 1;
            _hs_looping = 1;
            _hs_list = &((ptHashTable->ptTable)->hh);
            while (_hs_looping) {
                _hs_p = _hs_list;
                _hs_list = NULL;
                _hs_tail = NULL;
                _hs_nmerges = 0;
                while (_hs_p) {
                    _hs_nmerges++;
                    _hs_q = _hs_p;
                    _hs_psize = 0;
                    for ( _hs_i = 0; _hs_i < _hs_insize; _hs_i++ ) {
                        _hs_psize++;
                        _hs_q = (UT_hash_handle*)((_hs_q->next) ? ((void*)((char*)(_hs_q->next) + (ptHashTable->ptTable)->hh.tbl->hho)) : NULL);
                        if (! (_hs_q) ) break;
                    }
                    _hs_qsize = _hs_insize;
                    while ((_hs_psize > 0) || ((_hs_qsize > 0) && _hs_q )) {
                        if (_hs_psize == 0) {
                            _hs_e = _hs_q;
                            _hs_q = (UT_hash_handle*)((_hs_q->next) ? ((void*)((char*)(_hs_q->next) + (ptHashTable->ptTable)->hh.tbl->hho)) : NULL);
                            _hs_qsize--;
                        } else if ( (_hs_qsize == 0) || !(_hs_q) ) {
                            _hs_e = _hs_p;
                            _hs_p = (UT_hash_handle*)((_hs_p->next) ? ((void*)((char*)(_hs_p->next) + (ptHashTable->ptTable)->hh.tbl->hho)) : NULL);
                            _hs_psize--;
                        } else if (( fCompare((((void*)(((char*)(_hs_p)) - (((ptHashTable->ptTable)->hh.tbl)->hho)))), (((void*)(((char*)(_hs_q)) - (((ptHashTable->ptTable)->hh.tbl)->hho))))) ) <= 0) {
                            _hs_e = _hs_p;
                            _hs_p = (UT_hash_handle*)((_hs_p->next) ? ((void*)((char*)(_hs_p->next) + (ptHashTable->ptTable)->hh.tbl->hho)) : NULL);
                            _hs_psize--;
                        } else {
                            _hs_e = _hs_q;
                            _hs_q = (UT_hash_handle*)((_hs_q->next) ? ((void*)((char*)(_hs_q->next) + (ptHashTable->ptTable)->hh.tbl->hho)) : NULL);
                            _hs_qsize--;
                        }
                        if ( _hs_tail ) {
                            _hs_tail->next = ((_hs_e) ? ((void*)(((char*)(_hs_e)) - (((ptHashTable->ptTable)->hh.tbl)->hho))) : NULL);
                        } else {
                            _hs_list = _hs_e;
                        }
                        _hs_e->prev = ((_hs_tail) ? ((void*)(((char*)(_hs_tail)) - (((ptHashTable->ptTable)->hh.tbl)->hho))) : NULL);
                        _hs_tail = _hs_e;
                    }
                    _hs_p = _hs_q;
                }
                _hs_tail->next = NULL;
                if ( _hs_nmerges <= 1 ) {
                    _hs_looping=0;
                    (ptHashTable->ptTable)->hh.tbl->tail = _hs_tail;
                    do {
                        char **_da_dst = (char**)(&(ptHashTable->ptTable));
                        *_da_dst = (char*)(((void*)(((char*)(_hs_list)) - (((ptHashTable->ptTable)->hh.tbl)->hho))));
                    } while (0);
                }
                _hs_insize *= 2;
            } ;
        }
    } while (0);
    return 0;
}
int
hashSetFuncs(T_HASH_TABLE *ptHashTable, T_HASH_FUNCS tFuncs)
{
    if (((ptHashTable->ptTable)?((ptHashTable->ptTable)->hh.tbl->num_items):0) > 0) {
        return -1;
    }
    ptHashTable->tFuncs = tFuncs;
    return 0;
}
T_HASH_FUNCS
hashFuncString(void)
{
    T_HASH_FUNCS tFunc;
    memset(&tFunc, 0, sizeof(tFunc));
    tFunc.fNew = hashStringNew;
    tFunc.fDelete = hashComFree;
    return tFunc;
}
T_HASH_FUNCS
hashFuncStruct(int iSize)
{
    T_HASH_FUNCS tFunc;
    memset(&tFunc, 0, sizeof(tFunc));
    tFunc.fNew = hashStructNew;
    tFunc.fDelete = hashComFree;
    tFunc.pData = (void *)iSize;
    return tFunc;
}
static void *
hashStringNew(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
    return strdup(pValue);
}
static void *
hashStructNew(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
    void *pRet = malloc((int)ptFuncs->pData);
    if (pRet) memcpy(pRet, pValue, (int)ptFuncs->pData);
    return pRet;
}
static void
hashComFree(T_HASH_FUNCS *ptFuncs, const char *pKey, void *pValue)
{
    free(pValue);
    return;
}

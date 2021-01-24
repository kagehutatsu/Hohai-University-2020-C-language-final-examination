
#ifndef RATIONAL_H
#define RATIONAL_H

// -----------------------------------------------------------------------------

struct BigInt;

typedef struct BigInt BigInt;

// -----------------------------------------------------------------------------

BigInt* big_int_from_str(const char*); // accept
BigInt* big_int_from_ll(long long);  // accept
BigInt* big_int_copy(const BigInt*); // accept
void    big_int_free(BigInt*);       // accept

char*     big_int_to_str(const BigInt*);
long long big_int_to_ll(const BigInt*); // accept
int       big_int_cmp(const BigInt*, const BigInt*); // accept

BigInt* big_int_add(const BigInt*, const BigInt*);
BigInt* big_int_sub(const BigInt*, const BigInt*);
BigInt* big_int_mul(const BigInt*, const BigInt*);
BigInt* big_int_div(const BigInt*, const BigInt*, BigInt** rem);

// -----------------------------------------------------------------------------

#endif // ifndef RATIONAL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rational.h"

// -----------------------------------------------------------------------------

typedef long long ll;

ll big_int_test(ll, ll, ll, ll, int, int, int, int, int, int);

// -----------------------------------------------------------------------------

int main(void)
{
    ll errors = 0;
    errors += big_int_test(-200, 200, -10000, 10000, 1, 1, 1, 1, 1, 1);
    errors += big_int_test(-10000, 10000, -200, 200, 1, 1, 1, 1, 1, 1);
    errors += big_int_test(900, 1100, 8000, 10000, 1, 0, 0, 0, 1, 1);
    errors += big_int_test(900, 1100, -10000, -8000, 1, 0, 0, 0, 1, 1);
    errors += big_int_test(-1100, -900, 8000, 10000, 1, 0, 0, 0, 1, 1);
    errors += big_int_test(-1100, -900, -10000, -8000, 1, 0, 0, 0, 1, 1);
    errors += big_int_test(8000, 10000, 900, 1100, 1, 0, 0, 0, 1, 1);
    errors += big_int_test(-10000, -8000, 900, 1100, 1, 0, 0, 0, 1, 1);
    errors += big_int_test(8000, 10000, -1100, -900, 1, 0, 0, 0, 1, 1);
    errors += big_int_test(-10000, -8000, -1100, -900, 1, 0, 0, 0, 1, 1);

    printf("big int test errors: %lld\n\n", errors);
    return 0;
}

// -----------------------------------------------------------------------------

ll big_int_test(
    ll i_from, ll i_to, ll j_from, ll j_to,
    int test_cmp, int test_add, int test_sub,
    int test_mul, int test_div, int test_div_by_sub)
{
    ll errors = 0;
    for (ll i = i_from; i <= i_to; ++i)
        for (ll j = j_from; j <= j_to; ++j) {
            BigInt* a = big_int_from_ll(i);
            BigInt* b = big_int_from_ll(j);

            BigInt* s = 0;
            BigInt* d = 0;
            BigInt* p = 0;
            BigInt* q = 0;
            BigInt* r = 0;

            if (test_cmp) {
                int cmp = big_int_cmp(a, b);
                if (!((cmp == 1 && i > j) ||
                      (cmp == 0 && i == j) ||
                      (cmp == -1 && i < j)))
                {
                    printf("error comparing %lld and %lld\n", i, j);
                    ++errors;
                }
            }

            if (test_add) {
                s = big_int_add(a, b);
                if (big_int_to_ll(s) != i + j) {
                    printf("error adding %lld and %lld\n", i, j);
                    ++errors;
                }
            }

            if (test_sub) {
                d = big_int_sub(a, b);
                if (big_int_to_ll(d) != i - j) {
                    printf("error %lld minus %lld\n", i, j);
                    ++errors;
                }
            }

            if (test_mul) {
                p = big_int_mul(a, b);
                if (big_int_to_ll(p) != i * j) {
                    printf("error multiplying %lld by %lld\n", i, j);
                    ++errors;
                }
            }

            if (j) {
                if (test_div) {
                    q = big_int_div(a, b, &r);
                    if (big_int_to_ll(q) != i / j ||
                        big_int_to_ll(r) != i % j)
                    {
                        printf("error dividing %lld by %lld\n", i, j);
                        ++errors;
                    }
                }
            }

            big_int_free(a);
            big_int_free(b);
            big_int_free(s);
            big_int_free(d);
            big_int_free(p);
            big_int_free(q);
            big_int_free(r);
        }
    return errors;
}

// -----------------------------------------------------------------------------


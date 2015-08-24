#include <cassert>
#include "forward_to_member.hpp"

namespace detail
{

/**
 * Test all combinations of is_shared_ptr for int.
 */
#define TEST_IS_SHARED_PTR(t, exp) \
    static_assert(is_shared_ptr<t>::value == exp, "Unexpected is_shared_ptr result.");
TEST_IS_SHARED_PTR(int,                                                false);
TEST_IS_SHARED_PTR(const int,                                          false);
TEST_IS_SHARED_PTR(volatile int,                                       false);
TEST_IS_SHARED_PTR(const volatile int,                                 false);
TEST_IS_SHARED_PTR(int&,                                               false);
TEST_IS_SHARED_PTR(const int&,                                         false);
TEST_IS_SHARED_PTR(volatile int&,                                      false);
TEST_IS_SHARED_PTR(const volatile int&,                                false);
TEST_IS_SHARED_PTR(int*,                                               false);
TEST_IS_SHARED_PTR(const int*,                                         false);
TEST_IS_SHARED_PTR(volatile int*,                                      false);
TEST_IS_SHARED_PTR(const volatile int*,                                false);
TEST_IS_SHARED_PTR(std::shared_ptr<int>,                               true);
TEST_IS_SHARED_PTR(std::shared_ptr<const int>,                         true);
TEST_IS_SHARED_PTR(std::shared_ptr<volatile int>,                      true);
TEST_IS_SHARED_PTR(std::shared_ptr<const volatile int>,                true);
TEST_IS_SHARED_PTR(const std::shared_ptr<int>,                         true);
TEST_IS_SHARED_PTR(const std::shared_ptr<const int>,                   true);
TEST_IS_SHARED_PTR(const std::shared_ptr<volatile int>,                true);
TEST_IS_SHARED_PTR(const std::shared_ptr<const volatile int>,          true);
TEST_IS_SHARED_PTR(volatile std::shared_ptr<int>,                      true);
TEST_IS_SHARED_PTR(volatile std::shared_ptr<const int>,                true);
TEST_IS_SHARED_PTR(volatile std::shared_ptr<volatile int>,             true);
TEST_IS_SHARED_PTR(volatile std::shared_ptr<const volatile int>,       true);
TEST_IS_SHARED_PTR(const volatile std::shared_ptr<int>,                true);
TEST_IS_SHARED_PTR(const volatile std::shared_ptr<const int>,          true);
TEST_IS_SHARED_PTR(const volatile std::shared_ptr<volatile int>,       true);
TEST_IS_SHARED_PTR(const volatile std::shared_ptr<const volatile int>, true);

/**
 * Test all combinations of forward_member_underlying_type for int.
 */
#define TEST_FORWARD_MEMBER_UNDERLYING_TYPE(t)                                       \
    static_assert(std::is_same<int, forward_member_underlying_type<t>::type>::value, \
                  "Unexpected underlying type.")
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(int);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const int);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(volatile int);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const volatile int);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(int&);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const int&);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(volatile int&);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const volatile int&);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(int*);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const int*);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(volatile int*);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const volatile int*);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(std::shared_ptr<int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(std::shared_ptr<const int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(std::shared_ptr<volatile int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(std::shared_ptr<const volatile int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const std::shared_ptr<int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const std::shared_ptr<const int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const std::shared_ptr<volatile int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const std::shared_ptr<const volatile int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(volatile std::shared_ptr<int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(volatile std::shared_ptr<const int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(volatile std::shared_ptr<volatile int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(volatile std::shared_ptr<const volatile int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const volatile std::shared_ptr<int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const volatile std::shared_ptr<const int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const volatile std::shared_ptr<volatile int>);
TEST_FORWARD_MEMBER_UNDERLYING_TYPE(const volatile std::shared_ptr<const volatile int>);

} /* End of namespace detail. */

/**
 * Simple structure for testing containing a function that has all possible cv overloads.
 */
struct foo
{
    int func1(int i) { return i; }
    int func1(int i, int j) volatile { return i + j; }
    int func1(int i, int j, int k) const { return i + j + k; }
    int func1(int i, int j, int k, int l) const volatile { return i + j + k + l; }
};

/**
 * Simple structure containing foo members of various types (pointers, references, shared_ptr) and
 * cv qualifications. This structure will "forward" all function calls to the contained foo
 * structure.
 */
struct bar
{
    foo f;
    FORWARD_TO_MEMBER(f, func1);

    volatile foo fv;
    FORWARD_TO_MEMBER_AS(fv, func1, fv_func1);

    const foo fc;
    FORWARD_TO_MEMBER_AS(fc, func1, fc_func1);

    const volatile foo fcv;
    FORWARD_TO_MEMBER_AS(fcv, func1, fcv_func1);

    foo& fr;
    FORWARD_TO_MEMBER_AS(fr, func1, fr_func1);

    volatile foo& fvr;
    FORWARD_TO_MEMBER_AS(fvr, func1, fvr_func1);

    const foo& fcr;
    FORWARD_TO_MEMBER_AS(fcr, func1, fcr_func1);

    const volatile foo& fcvr;
    FORWARD_TO_MEMBER_AS(fcvr, func1, fcvr_func1);

    foo* fp;
    FORWARD_TO_MEMBER_AS(fp, func1, fp_func1);

    volatile foo* fvp;
    FORWARD_TO_MEMBER_AS(fvp, func1, fvp_func1);

    const foo* fcp;
    FORWARD_TO_MEMBER_AS(fcp, func1, fcp_func1);

    const volatile foo* fcvp;
    FORWARD_TO_MEMBER_AS(fcvp, func1, fcvp_func1);

    foo* const fpc;
    FORWARD_TO_MEMBER_AS(fpc, func1, fpc_func1);

    volatile foo* const fvpc;
    FORWARD_TO_MEMBER_AS(fvpc, func1, fvpc_func1);

    const foo* const fcpc;
    FORWARD_TO_MEMBER_AS(fcpc, func1, fcpc_func1);

    const volatile foo* const fcvpc;
    FORWARD_TO_MEMBER_AS(fcvpc, func1, fcvpc_func1);

    std::shared_ptr<foo> fsp;
    FORWARD_TO_MEMBER_AS(fsp, func1, fsp_func1);

    std::shared_ptr<volatile foo> fspv;
    FORWARD_TO_MEMBER_AS(fspv, func1, fspv_func1);

    std::shared_ptr<const foo> fspc;
    FORWARD_TO_MEMBER_AS(fspc, func1, fspc_func1);

    std::shared_ptr<const volatile foo> fspcv;
    FORWARD_TO_MEMBER_AS(fspcv, func1, fspcv_func1);

    const std::shared_ptr<foo> fcsp;
    FORWARD_TO_MEMBER_AS(fcsp, func1, fcsp_func1);

    const std::shared_ptr<volatile foo> fcspv;
    FORWARD_TO_MEMBER_AS(fcspv, func1, fcspv_func1);

    const std::shared_ptr<const foo> fcspc;
    FORWARD_TO_MEMBER_AS(fcspc, func1, fcspc_func1);

    const std::shared_ptr<const volatile foo> fcspcv;
    FORWARD_TO_MEMBER_AS(fcspcv, func1, fcspcv_func1);

    bar(foo& obj):
        f(obj),
        fv(obj),
        fc(obj),
        fcv(obj),
        fr(obj),
        fvr(obj),
        fcr(obj),
        fcvr(obj),
        fp(&obj),
        fvp(&obj),
        fcp(&obj),
        fcvp(&obj),
        fpc(&obj),
        fvpc(&obj),
        fcpc(&obj),
        fcvpc(&obj),
        fsp(std::make_shared<foo>(obj)),
        fspv(std::make_shared<volatile foo>(obj)),
        fspc(std::make_shared<const foo>(obj)),
        fspcv(std::make_shared<const volatile foo>(obj)),
        fcsp(std::make_shared<foo>(obj)),
        fcspv(std::make_shared<volatile foo>(obj)),
        fcspc(std::make_shared<const foo>(obj)),
        fcspcv(std::make_shared<const volatile foo>(obj))
    { }
};

int main()
{
    // Create bar objects of every possible cv qualification.
    foo f;
    bar b(f);
    volatile bar bv(f);
    const bar bc(f);
    const volatile bar bcv(f);

// Test all combinations of overload, foo, and bar types. Some combinations are expected to not
// compile and these are marked as invalid and explored in the "negative test" test script.
          assert(1 == b  .func1(1)               ); // Call plain          method on plain          bar with plain foo.
          assert(2 == b  .func1(1, 1)            ); // Call volatile       method on plain          bar with plain foo.
          assert(3 == b  .func1(1, 1, 1)         ); // Call const          method on plain          bar with plain foo.
          assert(4 == b  .func1(1, 1, 1, 1)      ); // Call const volatile method on plain          bar with plain foo.
//INVALID assert(1 == bv .func1(1)               ); // call plain          method on volatile       bar with plain foo.
          assert(2 == bv .func1(1, 1)            ); // Call volatile       method on volatile       bar with plain foo.
//INVALID assert(3 == bv .func1(1, 1, 1)         ); // Call const          method on volatile       bar with plain foo.
          assert(4 == bv .func1(1, 1, 1, 1)      ); // Call const volatile method on volatile       bar with plain foo.
//INVALID assert(1 == bc .func1(1)               ); // call plain          method on const          bar with plain foo.
//INVALID assert(2 == bc .func1(1, 1)            ); // Call volatile       method on const          bar with plain foo.
          assert(3 == bc .func1(1, 1, 1)         ); // Call const          method on const          bar with plain foo.
          assert(4 == bc .func1(1, 1, 1, 1)      ); // Call const volatile method on const          bar with plain foo.
//INVALID assert(1 == bcv.func1(1)               ); // call plain          method on const volatile bar with plain foo.
//INVALID assert(2 == bcv.func1(1, 1)            ); // Call volatile       method on const volatile bar with plain foo.
//INVALID assert(3 == bcv.func1(1, 1, 1)         ); // Call const          method on const volatile bar with plain foo.
          assert(4 == bcv.func1(1, 1, 1, 1)      ); // Call const volatile method on const volatile bar with plain foo.
//INVALID assert(1 == b  .fv_func1(1)            ); // Call plain          method on plain          bar with volatile foo.
          assert(2 == b  .fv_func1(1, 1)         ); // Call volatile       method on plain          bar with volatile foo.
//INVALID assert(3 == b  .fv_func1(1, 1, 1)      ); // Call const          method on plain          bar with volatile foo.
          assert(4 == b  .fv_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with volatile foo.
//INVALID assert(1 == bv .fv_func1(1)            ); // call plain          method on volatile       bar with volatile foo.
          assert(2 == bv .fv_func1(1, 1)         ); // Call volatile       method on volatile       bar with volatile foo.
//INVALID assert(3 == bv .fv_func1(1, 1, 1)      ); // Call const          method on volatile       bar with volatile foo.
          assert(4 == bv .fv_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with volatile foo.
//INVALID assert(1 == bc .fv_func1(1)            ); // call plain          method on const          bar with volatile foo.
//INVALID assert(2 == bc .fv_func1(1, 1)         ); // Call volatile       method on const          bar with volatile foo.
//INVALID assert(3 == bc .fv_func1(1, 1, 1)      ); // Call const          method on const          bar with volatile foo.
          assert(4 == bc .fv_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with volatile foo.
//INVALID assert(1 == bcv.fv_func1(1)            ); // call plain          method on const volatile bar with volatile foo.
//INVALID assert(2 == bcv.fv_func1(1, 1)         ); // Call volatile       method on const volatile bar with volatile foo.
//INVALID assert(3 == bcv.fv_func1(1, 1, 1)      ); // Call const          method on const volatile bar with volatile foo.
          assert(4 == bcv.fv_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with volatile foo.
//INVALID assert(1 == b  .fc_func1(1)            ); // Call plain          method on plain          bar with const foo.
//INVALID assert(2 == b  .fc_func1(1, 1)         ); // Call volatile       method on plain          bar with const foo.
          assert(3 == b  .fc_func1(1, 1, 1)      ); // Call const          method on plain          bar with const foo.
          assert(4 == b  .fc_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with const foo.
//INVALID assert(1 == bv .fc_func1(1)            ); // call plain          method on volatile       bar with const foo.
//INVALID assert(2 == bv .fc_func1(1, 1)         ); // Call volatile       method on volatile       bar with const foo.
//INVALID assert(3 == bv .fc_func1(1, 1, 1)      ); // Call const          method on volatile       bar with const foo.
          assert(4 == bv .fc_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with const foo.
//INVALID assert(1 == bc .fc_func1(1)            ); // call plain          method on const          bar with const foo.
//INVALID assert(2 == bc .fc_func1(1, 1)         ); // Call volatile       method on const          bar with const foo.
          assert(3 == bc .fc_func1(1, 1, 1)      ); // Call const          method on const          bar with const foo.
          assert(4 == bc .fc_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with const foo.
//INVALID assert(1 == bcv.fc_func1(1)            ); // call plain          method on const volatile bar with const foo.
//INVALID assert(2 == bcv.fc_func1(1, 1)         ); // Call volatile       method on const volatile bar with const foo.
//INVALID assert(3 == bcv.fc_func1(1, 1, 1)      ); // Call const          method on const volatile bar with const foo.
          assert(4 == bcv.fc_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with const foo.
//INVALID assert(1 == b  .fcv_func1(1)           ); // Call plain          method on plain          bar with const volatile foo.
//INVALID assert(2 == b  .fcv_func1(1, 1)        ); // Call volatile       method on plain          bar with const volatile foo.
//INVALID assert(3 == b  .fcv_func1(1, 1, 1)     ); // Call const          method on plain          bar with const volatile foo.
          assert(4 == b  .fcv_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with const volatile foo.
//INVALID assert(1 == bv .fcv_func1(1)           ); // call plain          method on volatile       bar with const volatile foo.
//INVALID assert(2 == bv .fcv_func1(1, 1)        ); // Call volatile       method on volatile       bar with const volatile foo.
//INVALID assert(3 == bv .fcv_func1(1, 1, 1)     ); // Call const          method on volatile       bar with const volatile foo.
          assert(4 == bv .fcv_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with const volatile foo.
//INVALID assert(1 == bc .fcv_func1(1)           ); // call plain          method on const          bar with const volatile foo.
//INVALID assert(2 == bc .fcv_func1(1, 1)        ); // Call volatile       method on const          bar with const volatile foo.
//INVALID assert(3 == bc .fcv_func1(1, 1, 1)     ); // Call const          method on const          bar with const volatile foo.
          assert(4 == bc .fcv_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with const volatile foo.
//INVALID assert(1 == bcv.fcv_func1(1)           ); // call plain          method on const volatile bar with const volatile foo.
//INVALID assert(2 == bcv.fcv_func1(1, 1)        ); // Call volatile       method on const volatile bar with const volatile foo.
//INVALID assert(3 == bcv.fcv_func1(1, 1, 1)     ); // Call const          method on const volatile bar with const volatile foo.
          assert(4 == bcv.fcv_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with const volatile foo.
          assert(1 == b  .fr_func1(1)            ); // Call plain          method on plain          bar with foo reference.
          assert(2 == b  .fr_func1(1, 1)         ); // Call volatile       method on plain          bar with foo reference.
          assert(3 == b  .fr_func1(1, 1, 1)      ); // Call const          method on plain          bar with foo reference.
          assert(4 == b  .fr_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with foo reference.
//INVALID assert(1 == bv .fr_func1(1)            ); // call plain          method on volatile       bar with foo reference.
          assert(2 == bv .fr_func1(1, 1)         ); // Call volatile       method on volatile       bar with foo reference.
//INVALID assert(3 == bv .fr_func1(1, 1, 1)      ); // Call const          method on volatile       bar with foo reference.
          assert(4 == bv .fr_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with foo reference.
//INVALID assert(1 == bc .fr_func1(1)            ); // call plain          method on const          bar with foo reference.
//INVALID assert(2 == bc .fr_func1(1, 1)         ); // Call volatile       method on const          bar with foo reference.
          assert(3 == bc .fr_func1(1, 1, 1)      ); // Call const          method on const          bar with foo reference.
          assert(4 == bc .fr_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with foo reference.
//INVALID assert(1 == bcv.fr_func1(1)            ); // call plain          method on const volatile bar with foo reference.
//INVALID assert(2 == bcv.fr_func1(1, 1)         ); // Call volatile       method on const volatile bar with foo reference.
//INVALID assert(3 == bcv.fr_func1(1, 1, 1)      ); // Call const          method on const volatile bar with foo reference.
          assert(4 == bcv.fr_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with foo reference.
//INVALID assert(1 == b  .fvr_func1(1)            ); // Call plain          method on plain          bar with volatile foo reference.
          assert(2 == b  .fvr_func1(1, 1)         ); // Call volatile       method on plain          bar with volatile foo reference.
//INVALID assert(3 == b  .fvr_func1(1, 1, 1)      ); // Call const          method on plain          bar with volatile foo reference.
          assert(4 == b  .fvr_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with volatile foo reference.
//INVALID assert(1 == bv .fvr_func1(1)            ); // call plain          method on volatile       bar with volatile foo reference.
          assert(2 == bv .fvr_func1(1, 1)         ); // Call volatile       method on volatile       bar with volatile foo reference.
//INVALID assert(3 == bv .fvr_func1(1, 1, 1)      ); // Call const          method on volatile       bar with volatile foo reference.
          assert(4 == bv .fvr_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with volatile foo reference.
//INVALID assert(1 == bc .fvr_func1(1)            ); // call plain          method on const          bar with volatile foo reference.
//INVALID assert(2 == bc .fvr_func1(1, 1)         ); // Call volatile       method on const          bar with volatile foo reference.
//INVALID assert(3 == bc .fvr_func1(1, 1, 1)      ); // Call const          method on const          bar with volatile foo reference.
          assert(4 == bc .fvr_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with volatile foo reference.
//INVALID assert(1 == bcv.fvr_func1(1)            ); // call plain          method on const volatile bar with volatile foo reference.
//INVALID assert(2 == bcv.fvr_func1(1, 1)         ); // Call volatile       method on const volatile bar with volatile foo reference.
//INVALID assert(3 == bcv.fvr_func1(1, 1, 1)      ); // Call const          method on const volatile bar with volatile foo reference.
          assert(4 == bcv.fvr_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with volatile foo reference.
//INVALID assert(1 == b  .fcr_func1(1)            ); // Call plain          method on plain          bar with const foo reference.
//INVALID assert(2 == b  .fcr_func1(1, 1)         ); // Call volatile       method on plain          bar with const foo reference.
          assert(3 == b  .fcr_func1(1, 1, 1)      ); // Call const          method on plain          bar with const foo reference.
          assert(4 == b  .fcr_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with const foo reference.
//INVALID assert(1 == bv .fcr_func1(1)            ); // call plain          method on volatile       bar with const foo reference.
//INVALID assert(2 == bv .fcr_func1(1, 1)         ); // Call volatile       method on volatile       bar with const foo reference.
//INVALID assert(3 == bv .fcr_func1(1, 1, 1)      ); // Call const          method on volatile       bar with const foo reference.
          assert(4 == bv .fcr_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with const foo reference.
//INVALID assert(1 == bc .fcr_func1(1)            ); // call plain          method on const          bar with const foo reference.
//INVALID assert(2 == bc .fcr_func1(1, 1)         ); // Call volatile       method on const          bar with const foo reference.
          assert(3 == bc .fcr_func1(1, 1, 1)      ); // Call const          method on const          bar with const foo reference.
          assert(4 == bc .fcr_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with const foo reference.
//INVALID assert(1 == bcv.fcr_func1(1)            ); // call plain          method on const volatile bar with const foo reference.
//INVALID assert(2 == bcv.fcr_func1(1, 1)         ); // Call volatile       method on const volatile bar with const foo reference.
//INVALID assert(3 == bcv.fcr_func1(1, 1, 1)      ); // Call const          method on const volatile bar with const foo reference.
          assert(4 == bcv.fcr_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with const foo reference.
//INVALID assert(1 == b  .fcvr_func1(1)           ); // Call plain          method on plain          bar with const volatile foo reference.
//INVALID assert(2 == b  .fcvr_func1(1, 1)        ); // Call volatile       method on plain          bar with const volatile foo reference.
//INVALID assert(3 == b  .fcvr_func1(1, 1, 1)     ); // Call const          method on plain          bar with const volatile foo reference.
          assert(4 == b  .fcvr_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with const volatile foo reference.
//INVALID assert(1 == bv .fcvr_func1(1)           ); // call plain          method on volatile       bar with const volatile foo reference.
//INVALID assert(2 == bv .fcvr_func1(1, 1)        ); // Call volatile       method on volatile       bar with const volatile foo reference.
//INVALID assert(3 == bv .fcvr_func1(1, 1, 1)     ); // Call const          method on volatile       bar with const volatile foo reference.
          assert(4 == bv .fcvr_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with const volatile foo reference.
//INVALID assert(1 == bc .fcvr_func1(1)           ); // call plain          method on const          bar with const volatile foo reference.
//INVALID assert(2 == bc .fcvr_func1(1, 1)        ); // Call volatile       method on const          bar with const volatile foo reference.
//INVALID assert(3 == bc .fcvr_func1(1, 1, 1)     ); // Call const          method on const          bar with const volatile foo reference.
          assert(4 == bc .fcvr_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with const volatile foo reference.
//INVALID assert(1 == bcv.fcvr_func1(1)           ); // call plain          method on const volatile bar with const volatile foo reference.
//INVALID assert(2 == bcv.fcvr_func1(1, 1)        ); // Call volatile       method on const volatile bar with const volatile foo reference.
//INVALID assert(3 == bcv.fcvr_func1(1, 1, 1)     ); // Call const          method on const volatile bar with const volatile foo reference.
          assert(4 == bcv.fcvr_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with const volatile foo reference.
          assert(1 == b  .fp_func1(1)             ); // Call plain          method on plain          bar with foo pointer.
          assert(2 == b  .fp_func1(1, 1)          ); // Call volatile       method on plain          bar with foo pointer.
          assert(3 == b  .fp_func1(1, 1, 1)       ); // Call const          method on plain          bar with foo pointer.
          assert(4 == b  .fp_func1(1, 1, 1, 1)    ); // Call const volatile method on plain          bar with foo pointer.
//INVALID assert(1 == bv .fp_func1(1)             ); // call plain          method on volatile       bar with foo pointer.
          assert(2 == bv .fp_func1(1, 1)          ); // Call volatile       method on volatile       bar with foo pointer.
//INVALID assert(3 == bv .fp_func1(1, 1, 1)       ); // Call const          method on volatile       bar with foo pointer.
          assert(4 == bv .fp_func1(1, 1, 1, 1)    ); // Call const volatile method on volatile       bar with foo pointer.
//INVALID assert(1 == bc .fp_func1(1)             ); // call plain          method on const          bar with foo pointer.
//INVALID assert(2 == bc .fp_func1(1, 1)          ); // Call volatile       method on const          bar with foo pointer.
          assert(3 == bc .fp_func1(1, 1, 1)       ); // Call const          method on const          bar with foo pointer.
          assert(4 == bc .fp_func1(1, 1, 1, 1)    ); // Call const volatile method on const          bar with foo pointer.
//INVALID assert(1 == bcv.fp_func1(1)             ); // call plain          method on const volatile bar with foo pointer.
//INVALID assert(2 == bcv.fp_func1(1, 1)          ); // Call volatile       method on const volatile bar with foo pointer.
//INVALID assert(3 == bcv.fp_func1(1, 1, 1)       ); // Call const          method on const volatile bar with foo pointer.
          assert(4 == bcv.fp_func1(1, 1, 1, 1)    ); // Call const volatile method on const volatile bar with foo pointer.
//INVALID assert(1 == b  .fvp_func1(1)            ); // Call plain          method on plain          bar with volatile foo pointer.
          assert(2 == b  .fvp_func1(1, 1)         ); // Call volatile       method on plain          bar with volatile foo pointer.
//INVALID assert(3 == b  .fvp_func1(1, 1, 1)      ); // Call const          method on plain          bar with volatile foo pointer.
          assert(4 == b  .fvp_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with volatile foo pointer.
//INVALID assert(1 == bv .fvp_func1(1)            ); // call plain          method on volatile       bar with volatile foo pointer.
          assert(2 == bv .fvp_func1(1, 1)         ); // Call volatile       method on volatile       bar with volatile foo pointer.
//INVALID assert(3 == bv .fvp_func1(1, 1, 1)      ); // Call const          method on volatile       bar with volatile foo pointer.
          assert(4 == bv .fvp_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with volatile foo pointer.
//INVALID assert(1 == bc .fvp_func1(1)            ); // call plain          method on const          bar with volatile foo pointer.
//INVALID assert(2 == bc .fvp_func1(1, 1)         ); // Call volatile       method on const          bar with volatile foo pointer.
//INVALID assert(3 == bc .fvp_func1(1, 1, 1)      ); // Call const          method on const          bar with volatile foo pointer.
          assert(4 == bc .fvp_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with volatile foo pointer.
//INVALID assert(1 == bcv.fvp_func1(1)            ); // call plain          method on const volatile bar with volatile foo pointer.
//INVALID assert(2 == bcv.fvp_func1(1, 1)         ); // Call volatile       method on const volatile bar with volatile foo pointer.
//INVALID assert(3 == bcv.fvp_func1(1, 1, 1)      ); // Call const          method on const volatile bar with volatile foo pointer.
          assert(4 == bcv.fvp_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with volatile foo pointer.
//INVALID assert(1 == b  .fcp_func1(1)            ); // Call plain          method on plain          bar with const foo pointer.
//INVALID assert(2 == b  .fcp_func1(1, 1)         ); // Call volatile       method on plain          bar with const foo pointer.
          assert(3 == b  .fcp_func1(1, 1, 1)      ); // Call const          method on plain          bar with const foo pointer.
          assert(4 == b  .fcp_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with const foo pointer.
//INVALID assert(1 == bv .fcp_func1(1)            ); // call plain          method on volatile       bar with const foo pointer.
//INVALID assert(2 == bv .fcp_func1(1, 1)         ); // Call volatile       method on volatile       bar with const foo pointer.
//INVALID assert(3 == bv .fcp_func1(1, 1, 1)      ); // Call const          method on volatile       bar with const foo pointer.
          assert(4 == bv .fcp_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with const foo pointer.
//INVALID assert(1 == bc .fcp_func1(1)            ); // call plain          method on const          bar with const foo pointer.
//INVALID assert(2 == bc .fcp_func1(1, 1)         ); // Call volatile       method on const          bar with const foo pointer.
          assert(3 == bc .fcp_func1(1, 1, 1)      ); // Call const          method on const          bar with const foo pointer.
          assert(4 == bc .fcp_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with const foo pointer.
//INVALID assert(1 == bcv.fcp_func1(1)            ); // call plain          method on const volatile bar with const foo pointer.
//INVALID assert(2 == bcv.fcp_func1(1, 1)         ); // Call volatile       method on const volatile bar with const foo pointer.
//INVALID assert(3 == bcv.fcp_func1(1, 1, 1)      ); // Call const          method on const volatile bar with const foo pointer.
          assert(4 == bcv.fcp_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with const foo pointer.
//INVALID assert(1 == b  .fcvp_func1(1)           ); // Call plain          method on plain          bar with const volatile foo pointer.
//INVALID assert(2 == b  .fcvp_func1(1, 1)        ); // Call volatile       method on plain          bar with const volatile foo pointer.
//INVALID assert(3 == b  .fcvp_func1(1, 1, 1)     ); // Call const          method on plain          bar with const volatile foo pointer.
          assert(4 == b  .fcvp_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with const volatile foo pointer.
//INVALID assert(1 == bv .fcvp_func1(1)           ); // call plain          method on volatile       bar with const volatile foo pointer.
//INVALID assert(2 == bv .fcvp_func1(1, 1)        ); // Call volatile       method on volatile       bar with const volatile foo pointer.
//INVALID assert(3 == bv .fcvp_func1(1, 1, 1)     ); // Call const          method on volatile       bar with const volatile foo pointer.
          assert(4 == bv .fcvp_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with const volatile foo pointer.
//INVALID assert(1 == bc .fcvp_func1(1)           ); // call plain          method on const          bar with const volatile foo pointer.
//INVALID assert(2 == bc .fcvp_func1(1, 1)        ); // Call volatile       method on const          bar with const volatile foo pointer.
//INVALID assert(3 == bc .fcvp_func1(1, 1, 1)     ); // Call const          method on const          bar with const volatile foo pointer.
          assert(4 == bc .fcvp_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with const volatile foo pointer.
//INVALID assert(1 == bcv.fcvp_func1(1)           ); // call plain          method on const volatile bar with const volatile foo pointer.
//INVALID assert(2 == bcv.fcvp_func1(1, 1)        ); // Call volatile       method on const volatile bar with const volatile foo pointer.
//INVALID assert(3 == bcv.fcvp_func1(1, 1, 1)     ); // Call const          method on const volatile bar with const volatile foo pointer.
          assert(4 == bcv.fcvp_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with const volatile foo pointer.
          assert(1 == b  .fpc_func1(1)            ); // Call plain          method on plain          bar with foo pointer const.
          assert(2 == b  .fpc_func1(1, 1)         ); // Call volatile       method on plain          bar with foo pointer const.
          assert(3 == b  .fpc_func1(1, 1, 1)      ); // Call const          method on plain          bar with foo pointer const.
          assert(4 == b  .fpc_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with foo pointer const.
//INVALID assert(1 == bv .fpc_func1(1)            ); // call plain          method on volatile       bar with foo pointer const.
          assert(2 == bv .fpc_func1(1, 1)         ); // Call volatile       method on volatile       bar with foo pointer const.
//INVALID assert(3 == bv .fpc_func1(1, 1, 1)      ); // Call const          method on volatile       bar with foo pointer const.
          assert(4 == bv .fpc_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with foo pointer const.
//INVALID assert(1 == bc .fpc_func1(1)            ); // call plain          method on const          bar with foo pointer const.
//INVALID assert(2 == bc .fpc_func1(1, 1)         ); // Call volatile       method on const          bar with foo pointer const.
          assert(3 == bc .fpc_func1(1, 1, 1)      ); // Call const          method on const          bar with foo pointer const.
          assert(4 == bc .fpc_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with foo pointer const.
//INVALID assert(1 == bcv.fpc_func1(1)            ); // call plain          method on const volatile bar with foo pointer const.
//INVALID assert(2 == bcv.fpc_func1(1, 1)         ); // Call volatile       method on const volatile bar with foo pointer const.
//INVALID assert(3 == bcv.fpc_func1(1, 1, 1)      ); // Call const          method on const volatile bar with foo pointer const.
          assert(4 == bcv.fpc_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with foo pointer const.
//INVALID assert(1 == b  .fvpc_func1(1)           ); // Call plain          method on plain          bar with volatile foo pointer const.
          assert(2 == b  .fvpc_func1(1, 1)        ); // Call volatile       method on plain          bar with volatile foo pointer const.
//INVALID assert(3 == b  .fvpc_func1(1, 1, 1)     ); // Call const          method on plain          bar with volatile foo pointer const.
          assert(4 == b  .fvpc_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with volatile foo pointer const.
//INVALID assert(1 == bv .fvpc_func1(1)           ); // call plain          method on volatile       bar with volatile foo pointer const.
          assert(2 == bv .fvpc_func1(1, 1)        ); // Call volatile       method on volatile       bar with volatile foo pointer const.
//INVALID assert(3 == bv .fvpc_func1(1, 1, 1)     ); // Call const          method on volatile       bar with volatile foo pointer const.
          assert(4 == bv .fvpc_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with volatile foo pointer const.
//INVALID assert(1 == bc .fvpc_func1(1)           ); // call plain          method on const          bar with volatile foo pointer const.
//INVALID assert(2 == bc .fvpc_func1(1, 1)        ); // Call volatile       method on const          bar with volatile foo pointer const.
//INVALID assert(3 == bc .fvpc_func1(1, 1, 1)     ); // Call const          method on const          bar with volatile foo pointer const.
          assert(4 == bc .fvpc_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with volatile foo pointer const.
//INVALID assert(1 == bcv.fvpc_func1(1)           ); // call plain          method on const volatile bar with volatile foo pointer const.
//INVALID assert(2 == bcv.fvpc_func1(1, 1)        ); // Call volatile       method on const volatile bar with volatile foo pointer const.
//INVALID assert(3 == bcv.fvpc_func1(1, 1, 1)     ); // Call const          method on const volatile bar with volatile foo pointer const.
          assert(4 == bcv.fvpc_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with volatile foo pointer const.
//INVALID assert(1 == b  .fcpc_func1(1)           ); // Call plain          method on plain          bar with const foo pointer const.
//INVALID assert(2 == b  .fcpc_func1(1, 1)        ); // Call volatile       method on plain          bar with const foo pointer const.
          assert(3 == b  .fcpc_func1(1, 1, 1)     ); // Call const          method on plain          bar with const foo pointer const.
          assert(4 == b  .fcpc_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with const foo pointer const.
//INVALID assert(1 == bv .fcpc_func1(1)           ); // call plain          method on volatile       bar with const foo pointer const.
//INVALID assert(2 == bv .fcpc_func1(1, 1)        ); // Call volatile       method on volatile       bar with const foo pointer const.
//INVALID assert(3 == bv .fcpc_func1(1, 1, 1)     ); // Call const          method on volatile       bar with const foo pointer const.
          assert(4 == bv .fcpc_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with const foo pointer const.
//INVALID assert(1 == bc .fcpc_func1(1)           ); // call plain          method on const          bar with const foo pointer const.
//INVALID assert(2 == bc .fcpc_func1(1, 1)        ); // Call volatile       method on const          bar with const foo pointer const.
          assert(3 == bc .fcpc_func1(1, 1, 1)     ); // Call const          method on const          bar with const foo pointer const.
          assert(4 == bc .fcpc_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with const foo pointer const.
//INVALID assert(1 == bcv.fcpc_func1(1)           ); // call plain          method on const volatile bar with const foo pointer const.
//INVALID assert(2 == bcv.fcpc_func1(1, 1)        ); // Call volatile       method on const volatile bar with const foo pointer const.
//INVALID assert(3 == bcv.fcpc_func1(1, 1, 1)     ); // Call const          method on const volatile bar with const foo pointer const.
          assert(4 == bcv.fcpc_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with const foo pointer const.
//INVALID assert(1 == b  .fcvpc_func1(1)          ); // Call plain          method on plain          bar with const volatile foo pointer const.
//INVALID assert(2 == b  .fcvpc_func1(1, 1)       ); // Call volatile       method on plain          bar with const volatile foo pointer const.
//INVALID assert(3 == b  .fcvpc_func1(1, 1, 1)    ); // Call const          method on plain          bar with const volatile foo pointer const.
          assert(4 == b  .fcvpc_func1(1, 1, 1, 1) ); // Call const volatile method on plain          bar with const volatile foo pointer const.
//INVALID assert(1 == bv .fcvpc_func1(1)          ); // call plain          method on volatile       bar with const volatile foo pointer const.
//INVALID assert(2 == bv .fcvpc_func1(1, 1)       ); // Call volatile       method on volatile       bar with const volatile foo pointer const.
//INVALID assert(3 == bv .fcvpc_func1(1, 1, 1)    ); // Call const          method on volatile       bar with const volatile foo pointer const.
          assert(4 == bv .fcvpc_func1(1, 1, 1, 1) ); // Call const volatile method on volatile       bar with const volatile foo pointer const.
//INVALID assert(1 == bc .fcvpc_func1(1)          ); // call plain          method on const          bar with const volatile foo pointer const.
//INVALID assert(2 == bc .fcvpc_func1(1, 1)       ); // Call volatile       method on const          bar with const volatile foo pointer const.
//INVALID assert(3 == bc .fcvpc_func1(1, 1, 1)    ); // Call const          method on const          bar with const volatile foo pointer const.
          assert(4 == bc .fcvpc_func1(1, 1, 1, 1) ); // Call const volatile method on const          bar with const volatile foo pointer const.
//INVALID assert(1 == bcv.fcvpc_func1(1)          ); // call plain          method on const volatile bar with const volatile foo pointer const.
//INVALID assert(2 == bcv.fcvpc_func1(1, 1)       ); // Call volatile       method on const volatile bar with const volatile foo pointer const.
//INVALID assert(3 == bcv.fcvpc_func1(1, 1, 1)    ); // Call const          method on const volatile bar with const volatile foo pointer const.
          assert(4 == bcv.fcvpc_func1(1, 1, 1, 1) ); // Call const volatile method on const volatile bar with const volatile foo pointer const.
          assert(1 == b  .fsp_func1(1)            ); // Call plain          method on plain          bar with foo shared pointer.
          assert(2 == b  .fsp_func1(1, 1)         ); // Call volatile       method on plain          bar with foo shared pointer.
          assert(3 == b  .fsp_func1(1, 1, 1)      ); // Call const          method on plain          bar with foo shared pointer.
          assert(4 == b  .fsp_func1(1, 1, 1, 1)   ); // Call const volatile method on plain          bar with foo shared pointer.
//INVALID assert(1 == bv .fsp_func1(1)            ); // Call plain          method on volatile       bar with foo shared pointer.
//INVALID assert(2 == bv .fsp_func1(1, 1)         ); // Call volatile       method on volatile       bar with foo shared pointer.
//INVALID assert(3 == bv .fsp_func1(1, 1, 1)      ); // Call const          method on volatile       bar with foo shared pointer.
//INVALID assert(4 == bv .fsp_func1(1, 1, 1, 1)   ); // Call const volatile method on volatile       bar with foo shared pointer.
//INVALID assert(1 == bc .fsp_func1(1)            ); // Call plain          method on const          bar with foo shared pointer.
//INVALID assert(2 == bc .fsp_func1(1, 1)         ); // Call volatile       method on const          bar with foo shared pointer.
          assert(3 == bc .fsp_func1(1, 1, 1)      ); // Call const          method on const          bar with foo shared pointer.
          assert(4 == bc .fsp_func1(1, 1, 1, 1)   ); // Call const volatile method on const          bar with foo shared pointer.
//INVALID assert(1 == bcv.fsp_func1(1)            ); // Call plain          method on const volatile bar with foo shared pointer.
//INVALID assert(2 == bcv.fsp_func1(1, 1)         ); // Call volatile       method on const volatile bar with foo shared pointer.
//INVALID assert(3 == bcv.fsp_func1(1, 1, 1)      ); // Call const          method on const volatile bar with foo shared pointer.
//INVALID assert(4 == bcv.fsp_func1(1, 1, 1, 1)   ); // Call const volatile method on const volatile bar with foo shared pointer.
//INVALID assert(1 == b  .fspv_func1(1)           ); // Call plain          method on plain          bar with volatile foo shared pointer.
          assert(2 == b  .fspv_func1(1, 1)        ); // Call volatile       method on plain          bar with volatile foo shared pointer.
//INVALID assert(3 == b  .fspv_func1(1, 1, 1)     ); // Call const          method on plain          bar with volatile foo shared pointer.
          assert(4 == b  .fspv_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with volatile foo shared pointer.
//INVALID assert(1 == bv .fspv_func1(1)           ); // Call plain          method on volatile       bar with volatile foo shared pointer.
//INVALID assert(2 == bv .fspv_func1(1, 1)        ); // Call volatile       method on volatile       bar with volatile foo shared pointer.
//INVALID assert(3 == bv .fspv_func1(1, 1, 1)     ); // Call const          method on volatile       bar with volatile foo shared pointer.
//INVALID assert(4 == bv .fspv_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with volatile foo shared pointer.
//INVALID assert(1 == bc .fspv_func1(1)           ); // Call plain          method on const          bar with volatile foo shared pointer.
//INVALID assert(2 == bc .fspv_func1(1, 1)        ); // Call volatile       method on const          bar with volatile foo shared pointer.
//INVALID assert(3 == bc .fspv_func1(1, 1, 1)     ); // Call const          method on const          bar with volatile foo shared pointer.
          assert(4 == bc .fspv_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with volatile foo shared pointer.
//INVALID assert(1 == bcv.fspv_func1(1)           ); // Call plain          method on const volatile bar with volatile foo shared pointer.
//INVALID assert(2 == bcv.fspv_func1(1, 1)        ); // Call volatile       method on const volatile bar with volatile foo shared pointer.
//INVALID assert(3 == bcv.fspv_func1(1, 1, 1)     ); // Call const          method on const volatile bar with volatile foo shared pointer.
//INVALID assert(4 == bcv.fspv_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with volatile foo shared pointer.
//INVALID assert(1 == b  .fspc_func1(1)           ); // Call plain          method on plain          bar with const foo shared pointer.
//INVALID assert(2 == b  .fspc_func1(1, 1)        ); // Call volatile       method on plain          bar with const foo shared pointer.
          assert(3 == b  .fspc_func1(1, 1, 1)     ); // Call const          method on plain          bar with const foo shared pointer.
          assert(4 == b  .fspc_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with const foo shared pointer.
//INVALID assert(1 == bv .fspc_func1(1)           ); // Call plain          method on volatile       bar with const foo shared pointer.
//INVALID assert(2 == bv .fspc_func1(1, 1)        ); // Call volatile       method on volatile       bar with const foo shared pointer.
//INVALID assert(3 == bv .fspc_func1(1, 1, 1)     ); // Call const          method on volatile       bar with const foo shared pointer.
//INVALID assert(4 == bv .fspc_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with const foo shared pointer.
//INVALID assert(1 == bc .fspc_func1(1)           ); // Call plain          method on const          bar with const foo shared pointer.
//INVALID assert(2 == bc .fspc_func1(1, 1)        ); // Call volatile       method on const          bar with const foo shared pointer.
          assert(3 == bc .fspc_func1(1, 1, 1)     ); // Call const          method on const          bar with const foo shared pointer.
          assert(4 == bc .fspc_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with const foo shared pointer.
//INVALID assert(1 == bcv.fspc_func1(1)           ); // Call plain          method on const volatile bar with const foo shared pointer.
//INVALID assert(2 == bcv.fspc_func1(1, 1)        ); // Call volatile       method on const volatile bar with const foo shared pointer.
//INVALID assert(3 == bcv.fspc_func1(1, 1, 1)     ); // Call const          method on const volatile bar with const foo shared pointer.
//INVALID assert(4 == bcv.fspc_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with const foo shared pointer.
//INVALID assert(1 == b  .fspcv_func1(1)          ); // Call plain          method on plain          bar with const volatile foo shared pointer.
//INVALID assert(2 == b  .fspcv_func1(1, 1)       ); // Call volatile       method on plain          bar with const volatile foo shared pointer.
//INVALID assert(3 == b  .fspcv_func1(1, 1, 1)    ); // Call const          method on plain          bar with const volatile foo shared pointer.
          assert(4 == b  .fspcv_func1(1, 1, 1, 1) ); // Call const volatile method on plain          bar with const volatile foo shared pointer.
//INVALID assert(1 == bv .fspcv_func1(1)          ); // Call plain          method on volatile       bar with const volatile foo shared pointer.
//INVALID assert(2 == bv .fspcv_func1(1, 1)       ); // Call volatile       method on volatile       bar with const volatile foo shared pointer.
//INVALID assert(3 == bv .fspcv_func1(1, 1, 1)    ); // Call const          method on volatile       bar with const volatile foo shared pointer.
//INVALID assert(4 == bv .fspcv_func1(1, 1, 1, 1) ); // Call const volatile method on volatile       bar with const volatile foo shared pointer.
//INVALID assert(1 == bc .fspcv_func1(1)          ); // Call plain          method on const          bar with const volatile foo shared pointer.
//INVALID assert(2 == bc .fspcv_func1(1, 1)       ); // Call volatile       method on const          bar with const volatile foo shared pointer.
//INVALID assert(3 == bc .fspcv_func1(1, 1, 1)    ); // Call const          method on const          bar with const volatile foo shared pointer.
          assert(4 == bc .fspcv_func1(1, 1, 1, 1) ); // Call const volatile method on const          bar with const volatile foo shared pointer.
//INVALID assert(1 == bcv.fspcv_func1(1)          ); // Call plain          method on const volatile bar with const volatile foo shared pointer.
//INVALID assert(2 == bcv.fspcv_func1(1, 1)       ); // Call volatile       method on const volatile bar with const volatile foo shared pointer.
//INVALID assert(3 == bcv.fspcv_func1(1, 1, 1)    ); // Call const          method on const volatile bar with const volatile foo shared pointer.
//INVALID assert(4 == bcv.fspcv_func1(1, 1, 1, 1) ); // Call const volatile method on const volatile bar with const volatile foo shared pointer.
          assert(1 == b  .fcsp_func1(1)           ); // Call plain          method on plain          bar with foo shared pointer const.
          assert(2 == b  .fcsp_func1(1, 1)        ); // Call volatile       method on plain          bar with foo shared pointer const.
          assert(3 == b  .fcsp_func1(1, 1, 1)     ); // Call const          method on plain          bar with foo shared pointer const.
          assert(4 == b  .fcsp_func1(1, 1, 1, 1)  ); // Call const volatile method on plain          bar with foo shared pointer const.
//INVALID assert(1 == bv .fcsp_func1(1)           ); // Call plain          method on volatile       bar with foo shared pointer const.
//INVALID assert(2 == bv .fcsp_func1(1, 1)        ); // Call volatile       method on volatile       bar with foo shared pointer const.
//INVALID assert(3 == bv .fcsp_func1(1, 1, 1)     ); // Call const          method on volatile       bar with foo shared pointer const.
//INVALID assert(4 == bv .fcsp_func1(1, 1, 1, 1)  ); // Call const volatile method on volatile       bar with foo shared pointer const.
//INVALID assert(1 == bc .fcsp_func1(1)           ); // Call plain          method on const          bar with foo shared pointer const.
//INVALID assert(2 == bc .fcsp_func1(1, 1)        ); // Call volatile       method on const          bar with foo shared pointer const.
          assert(3 == bc .fcsp_func1(1, 1, 1)     ); // Call const          method on const          bar with foo shared pointer const.
          assert(4 == bc .fcsp_func1(1, 1, 1, 1)  ); // Call const volatile method on const          bar with foo shared pointer const.
//INVALID assert(1 == bcv.fcsp_func1(1)           ); // Call plain          method on const volatile bar with foo shared pointer const.
//INVALID assert(2 == bcv.fcsp_func1(1, 1)        ); // Call volatile       method on const volatile bar with foo shared pointer const.
//INVALID assert(3 == bcv.fcsp_func1(1, 1, 1)     ); // Call const          method on const volatile bar with foo shared pointer const.
//INVALID assert(4 == bcv.fcsp_func1(1, 1, 1, 1)  ); // Call const volatile method on const volatile bar with foo shared pointer const.
//INVALID assert(1 == b  .fcspv_func1(1)          ); // Call plain          method on plain          bar with volatile foo shared pointer const.
          assert(2 == b  .fcspv_func1(1, 1)       ); // Call volatile       method on plain          bar with volatile foo shared pointer const.
//INVALID assert(3 == b  .fcspv_func1(1, 1, 1)    ); // Call const          method on plain          bar with volatile foo shared pointer const.
          assert(4 == b  .fcspv_func1(1, 1, 1, 1) ); // Call const volatile method on plain          bar with volatile foo shared pointer const.
//INVALID assert(1 == bv .fcspv_func1(1)          ); // Call plain          method on volatile       bar with volatile foo shared pointer const.
//INVALID assert(2 == bv .fcspv_func1(1, 1)       ); // Call volatile       method on volatile       bar with volatile foo shared pointer const.
//INVALID assert(3 == bv .fcspv_func1(1, 1, 1)    ); // Call const          method on volatile       bar with volatile foo shared pointer const.
//INVALID assert(4 == bv .fcspv_func1(1, 1, 1, 1) ); // Call const volatile method on volatile       bar with volatile foo shared pointer const.
//INVALID assert(1 == bc .fcspv_func1(1)          ); // Call plain          method on const          bar with volatile foo shared pointer const.
//INVALID assert(2 == bc .fcspv_func1(1, 1)       ); // Call volatile       method on const          bar with volatile foo shared pointer const.
//INVALID assert(3 == bc .fcspv_func1(1, 1, 1)    ); // Call const          method on const          bar with volatile foo shared pointer const.
          assert(4 == bc .fcspv_func1(1, 1, 1, 1) ); // Call const volatile method on const          bar with volatile foo shared pointer const.
//INVALID assert(1 == bcv.fcspv_func1(1)          ); // Call plain          method on const volatile bar with volatile foo shared pointer const.
//INVALID assert(2 == bcv.fcspv_func1(1, 1)       ); // Call volatile       method on const volatile bar with volatile foo shared pointer const.
//INVALID assert(3 == bcv.fcspv_func1(1, 1, 1)    ); // Call const          method on const volatile bar with volatile foo shared pointer const.
//INVALID assert(4 == bcv.fcspv_func1(1, 1, 1, 1) ); // Call const volatile method on const volatile bar with volatile foo shared pointer const.
//INVALID assert(1 == b  .fcspc_func1(1)          ); // Call plain          method on plain          bar with const foo shared pointer const.
//INVALID assert(2 == b  .fcspc_func1(1, 1)       ); // Call volatile       method on plain          bar with const foo shared pointer const.
          assert(3 == b  .fcspc_func1(1, 1, 1)    ); // Call const          method on plain          bar with const foo shared pointer const.
          assert(4 == b  .fcspc_func1(1, 1, 1, 1) ); // Call const volatile method on plain          bar with const foo shared pointer const.
//INVALID assert(1 == bv .fcspc_func1(1)          ); // Call plain          method on volatile       bar with const foo shared pointer const.
//INVALID assert(2 == bv .fcspc_func1(1, 1)       ); // Call volatile       method on volatile       bar with const foo shared pointer const.
//INVALID assert(3 == bv .fcspc_func1(1, 1, 1)    ); // Call const          method on volatile       bar with const foo shared pointer const.
//INVALID assert(4 == bv .fcspc_func1(1, 1, 1, 1) ); // Call const volatile method on volatile       bar with const foo shared pointer const.
//INVALID assert(1 == bc .fcspc_func1(1)          ); // Call plain          method on const          bar with const foo shared pointer const.
//INVALID assert(2 == bc .fcspc_func1(1, 1)       ); // Call volatile       method on const          bar with const foo shared pointer const.
          assert(3 == bc .fcspc_func1(1, 1, 1)    ); // Call const          method on const          bar with const foo shared pointer const.
          assert(4 == bc .fcspc_func1(1, 1, 1, 1) ); // Call const volatile method on const          bar with const foo shared pointer const.
//INVALID assert(1 == bcv.fcspc_func1(1)          ); // Call plain          method on const volatile bar with const foo shared pointer const.
//INVALID assert(2 == bcv.fcspc_func1(1, 1)       ); // Call volatile       method on const volatile bar with const foo shared pointer const.
//INVALID assert(3 == bcv.fcspc_func1(1, 1, 1)    ); // Call const          method on const volatile bar with const foo shared pointer const.
//INVALID assert(4 == bcv.fcspc_func1(1, 1, 1, 1) ); // Call const volatile method on const volatile bar with const foo shared pointer const.
//INVALID assert(1 == b  .fcspcv_func1(1)         ); // Call plain          method on plain          bar with const volatile foo shared pointer const.
//INVALID assert(2 == b  .fcspcv_func1(1, 1)      ); // Call volatile       method on plain          bar with const volatile foo shared pointer const.
//INVALID assert(3 == b  .fcspcv_func1(1, 1, 1)   ); // Call const          method on plain          bar with const volatile foo shared pointer const.
          assert(4 == b  .fcspcv_func1(1, 1, 1, 1)); // Call const volatile method on plain          bar with const volatile foo shared pointer const.
//INVALID assert(1 == bv .fcspcv_func1(1)         ); // Call plain          method on volatile       bar with const volatile foo shared pointer const.
//INVALID assert(2 == bv .fcspcv_func1(1, 1)      ); // Call volatile       method on volatile       bar with const volatile foo shared pointer const.
//INVALID assert(3 == bv .fcspcv_func1(1, 1, 1)   ); // Call const          method on volatile       bar with const volatile foo shared pointer const.
//INVALID assert(4 == bv .fcspcv_func1(1, 1, 1, 1)); // Call const volatile method on volatile       bar with const volatile foo shared pointer const.
//INVALID assert(1 == bc .fcspcv_func1(1)         ); // Call plain          method on const          bar with const volatile foo shared pointer const.
//INVALID assert(2 == bc .fcspcv_func1(1, 1)      ); // Call volatile       method on const          bar with const volatile foo shared pointer const.
//INVALID assert(3 == bc .fcspcv_func1(1, 1, 1)   ); // Call const          method on const          bar with const volatile foo shared pointer const.
          assert(4 == bc .fcspcv_func1(1, 1, 1, 1)); // Call const volatile method on const          bar with const volatile foo shared pointer const.
//INVALID assert(1 == bcv.fcspcv_func1(1)         ); // Call plain          method on const volatile bar with const volatile foo shared pointer const.
//INVALID assert(2 == bcv.fcspcv_func1(1, 1)      ); // Call volatile       method on const volatile bar with const volatile foo shared pointer const.
//INVALID assert(3 == bcv.fcspcv_func1(1, 1, 1)   ); // Call const          method on const volatile bar with const volatile foo shared pointer const.
//INVALID assert(4 == bcv.fcspcv_func1(1, 1, 1, 1)); // Call const volatile method on const volatile bar with const volatile foo shared pointer const.
}

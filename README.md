FORWARD_TO_MEMBER
=================
[![Build Status](https://travis-ci.org/jonesinator/forward-to-member.svg)](https://travis-ci.org/jonesinator/forward-to-member) [![Coverage Status](https://coveralls.io/repos/jonesinator/forward-to-member/badge.svg?branch=master&service=github)](https://coveralls.io/github/jonesinator/forward-to-member?branch=master)

This header-only library provides two macros FORWARD_TO_MEMBER and
FORWARD_TO_MEMBER_AS. The purpose of these macros is to simplify the fairly
common case below:

```cpp
#include <iostream>

class foo
{
public:
    void method()
    {
        std::cout << "METHOD" << std::endl;
    }
};

class bar
{
private:
    foo f;

public:
    void method()
    {
        f.method();
    }
};

int main(int, char**)
{
    bar b;
    b.method();
    return 0;
}
```

This pattern of exposing the public method of a member as a public method of
the containing class is a reasonably common occurence in certain systems. This
is a simple example, but things become more complicated as the method is
overloaded and has different const/volatile specifiers. It is the coders
responsibility to make sure that the signature of the method in bar matches the
signature of the method in foo. The FORWARD_TO_MEMBER macro simplifies this:

```cpp
#include <iostream>
#include "forward_to_member.hpp"

class foo
{
public:
    void method()
    {
        std::cout << "METHOD" << std::endl;
    }
};

class bar
{
private:
    foo f;

public:
    FORWARD_TO_MEMBER(f, method);
};

int main(int, char**)
{
    bar b;
    b.method();
    return 0;
}
```

Here FOWARD_TO_MEMBER is responsible for matching the signature in foo, and can
deal with all overloads of the method in foo. Unfortunately, given the amount
of variadic templates and SFINAE usage abuse of this macro could probably
produce some pretty bad compile times.

Another unfortunate aspect of this library is that the member must be declared
before invoking the FORWARD_TO_MEMBER macro. This disallows an often-used
convention where the public methods are at the top of the class and the members
are at the bottom.

Compilers
---------
This library uses features from c++11 and requires gcc-4.9 or greater. Also
tested under clang-3.6.2, but it probably works in older versions of clang as
well.

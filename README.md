FORWARD_TO_MEMBER
=================
[![Build Status](https://travis-ci.org/jonesinator/forward-to-member.svg)](https://travis-ci.org/jonesinator/forward-to-member)

This library provides two macros FORWARD_TO_MEMBER and FORWARD_TO_MEMBER_AS. The purpose of these
macros is to simplify the fairly common case below:

    class foo
    {
    public:
        void method()
        {
            do_stuff();
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

This pattern of exposing the public method of a member as a public method of the containing class
is a reasonably common occurence in certain systems. This is a simple example, but things become
more complicated as the method is overloaded and has different const/volatile specifiers. It is
the coders responsibility to make sure that the signature of the method in bar matches the
signature of the method in foo. The FORWARD_TO_MEMBER macro simplifies this:

    class foo
    {
    public:
        void method()
        {
            do_stuff();
        }
    };
    
    class bar
    {
    private:
        foo f;
    
    public:
        FORWARD_TO_MEMBER(f, method);
    };

Here FOWARD_TO_MEMBER is responsible for matching the signature in foo, and can deal with all
overloads of the method in foo. Unfortunately, given the amount of variadic templates and SFINAE
usage abuse of this macro could probably produce some pretty bad compile times.

Another unfortunate aspect of this library is that the member must be declared before invoking
the FORWARD_TO_MEMBER macro. This disallows an often-used convention where the public methods are
at the top of the class and the members are at the bottom.

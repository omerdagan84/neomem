

test scripts will go here, along with problem .neo files, etc

db tests and gui tests should be separate

gui tests could use perl's guitest library - 
* http://search.cpan.org/dist/Win32-GuiTest/GuiTest.pm




The goal of [unit testing] is to isolate each part of the program and show that the individual parts are correct. A unit test provides a strict, written contract that the piece of code must satisfy.
Good unit test design produces test cases that cover all paths through the unit with attention paid to loop conditions.
By testing the parts of a program first and then testing the sum of its parts, integration testing becomes much easier.
XP mandates a 'test everything that can possibly break' strategy, over the tradition 'test every execution path' method. 
Developers release unit testing code to the code repository in conjunction with the code it tests. 
**Unit testing is traditionally a motivator for programmers to create decoupled and cohesive code bodies. 

[Integration testing] takes as its input modules that have been unit tested, groups them in larger aggregates, applies tests defined in an integration test plan to those aggregates, and delivers as its output the integrated system ready for system testing.
Test cases are constructed to test that all components within assemblages interact correctly, for example across procedure calls or process activations, and this is done after testing individual modules, i.e. unit testing.

[System testing] of software or hardware is testing conducted on a complete, integrated system to evaluate the system's compliance with its specified requirements.
System testing is an investigatory testing phase, where the focus is to have almost a destructive attitude and test not only the design, but also the behaviour and even the believed expectations of the customer.

In object-oriented programming, [mock objects] are simulated objects that mimic the behavior of real objects in controlled ways. useful when a real object is difficult or impossible to incorporate into a unit test.
Many available mock object frameworks allow the programmer to specify which, and in what order, methods will be invoked on a mock object and what parameters will be passed to them, as well as what values will be returned.
Some authors[1] draw a distinction between fake and mock objects. Fakes are the simpler of the two, simply implementing the same interface as the object that they represent and returning pre-arranged responses. Mock objects in this sense do a little more: their method implementations contain assertions of their own. This means that a true mock, in this sense, will examine the context of each call—perhaps checking the order in which its methods are called, perhaps performing tests on the data passed into the method calls as arguments.


=Google Test
Google C++ Testing Framework works on different OSes, with different compilers (gcc, MSVC, and others), with or without exceptions.
Currently, the only Operating System
(OS) on which Google Test is known to build properly is Linux, but we are
actively working on Windows and Mac support as well. The source code itself is
already portable across many other platforms, but we are still developing
robust build systems for each.
### Windows Requirements ###
  * Microsoft Visual Studio 7.1 or newer. erghf. 
Google Test assertions are macros that resemble function calls. You test a class or function by making assertions about its behavior.
ASSERT_* versions generate fatal failures when they fail, and abort the current function. EXPECT_* versions generate nonfatal failures, which don't abort the current function. Usually EXPECT_* are preferred, as they allow more than one failures to be reported in a test. However, you should use ASSERT_* if it doesn't make sense to continue when the assertion in question fails. 
To provide a custom failure message, simply stream it into the macro using the << operator, or a sequence of such operators. An example:
ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

To create a test:
   1. Use the TEST() macro to define and name a test function, These are ordinary C++ functions that don't return a value. 
TEST(test_case_name, test_name) {
 ... test body ...
}
The first argument is the name of the test case, and the second argument is the test's name within the test case. 
The test case name and the test name should both be valid C++
identifiers.  And you should not use underscore (_) in the names.
// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(1, Factorial(0));
}

After defining your tests, you can run them with RUN_ALL_TESTS() , which returns 0 if all the tests are successful, or 1 otherwise.
// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//


### Windows ###
Open the gtest.sln file in the msvc/ folder using Visual Studio, and
you are ready to build Google Test the same way you build any Visual
Studio project.



=cppunit
In Win32 world, you can use CPPUnit under Visual C++ (6 and later), but as CPPUnit uses ANSI C++, there are few ports to other environments like C++Builder.



***In general, GuiTesting is difficult.
For several reasons, it is a good idea to design GUI applications to communicate with a back-end via some command language.
    * Doing so allows power users to easily automate tasks.
    * The command language can be used to automate integration testing.
    * The design of the command language creates a clear divide between the GUI code and the object model.
    * If a bug is discovered, then the command stream can be recorded, greatly simplifying fault analysis. 
My thoughts are that you need to design your gui software with the thought that it needs to run automated tests. Make sure that the classes (functions, whatever) that handle the user interface have the minimum of code. Delegate all of the work that actually runs the application to modules that are testable.
the main approach is to treat the GUI library like
any other 3rd party library - mainly, don't try and unit test it, unit
test your code that uses it.
By this, I mean: ensure all of your logic (business rules, gui
presentation rules, data retrival and updates, etc) are completed
separated from the gui code. In your gui's event handlers, don't do
anything except delegate to a plain old C++ Class.
By separating your code from the gui code, you can easily test your
code, without having to even link to the gui library.
In software, abstract is usually the way to solve a problem.
A unit test does NOT :
* Use any file IO
* Connect to a db
* Present anything on screen
* communicate across a network.
The rational for this, is many, but boils down to forcing us to create
a design that is decoupled from these areas, which nearly always
results in a good, clean & highly cohesive design. For example, how
many times have we in the past put logic directly into a dialog
class? Where as with this style of testing, we force ourselves to
separate out the UI aspect from the logic part, resulting in us using
the MVC or MVP or Humble Dialog design patterns, as they allow us to
unit test th elogic without going anywhere near the GUI.
The second major win is speed of test runs.



It turns out that C++ unit testing is nowhere near as standardized or easy as .Net or Java. Partly, it’s the lack of reflection/introspection, but I think it’s also because the language itself just doesn’t lend itself to it.


=unittest++
UnitTest++ is ANSI portable C++ and makes minimal use of advanced library and languages features, which means it should be easily portable to just about any platform. Out of the box, the following platforms are supported:
    * Win32
    * Linux
    * Mac OS X
UnitTest++ seems to be the best of all possible worlds. It’s extremely simple to add test cases. ~2008
#include "UnitTest++.h"
TEST(MyTest)
{
    CHECK(true);
}
int main(int, char const *[])
{
    return UnitTest::RunAllTests();
}

q.will it work with vs6 though? cppunit will. 
In the freebie realm, there's CppUnit and appears to have grown to include a "MFC TestRunner" unit tester. 
I just fired up VS 2008 Professional and there is a menu 
called "Test" which allows you to create unit tests.  I have no idea how 
good it is though - but I might try it out.  WinRunner and QuickTest are 
still the industry standard tools though for doing tests with GUIs from 
a "black box" perspective.


----

want a unit test framework that will carry over to wxwidgets, cross-platform, and c versions. 
cppunit and c++unit require duplication.? 
google test's use of macros looks better. but not ready for windows. or vs6. 
unittest++ looks good. but not for vs6?
maybe best to make a simple macro-based test suite thingy. 
  i mean, SIMPLE. 
so it's between unittest++ and cppunit
cppunit will do for now. meh. 




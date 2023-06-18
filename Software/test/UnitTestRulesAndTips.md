# Unit testing conventions and tips

## What is inside this directory?

This directory contains all unit tests that are made to verify project functionality.

## What is Test Case?

By convention, single `TEST` block inside test group is probably called a "Test case" and it should be exactly this.

The point is that one test case should cover exactly one functionality or a use case. Plz don't make 1000 line long `TEST` blocks which tests everything what you implemented in your module.

Also, it is really important to treat tests as code, plz make it readable and don't abuse comments.

## What is Test Suite?

Single .cpp file inside test directory is called "Test Suite". These files should contain all test cases for one module (exactly ONE module).

It could be used to test everything in one .cpp project file or test one "functional" module (prefer testing single .cpp file interface).

One test suite should not contain test, that was previously made somewhere else.

Example: If 1 functional module contains 2 files (`Module.cpp` and `ModuleHelper.cpp`), we can test `ModuleHelper.cpp` in one test suite `Module.cpp` in other one (using mocks to `ModuleHelper.cpp` or the production module, but using live module is encouraged to make sure everything works both separately and together)

## Adding new test in existing test suite

This is very easy, just create new `Test` block within existing test group

Example:

```cpp
TEST_GROUP(ExistingTestGroup)
{
}

TEST(ExistingTestGroup, MyNewTest)
{
    // Add tests here
    constexpr bool doesThisTestWork = true;
    CHECK(true);
}
```

## Adding new test suite

The best way to achive this is to copy one of the existing test directories and make your adjustments (Most basic candidate is TestExample). CMake will detect new directory and will try to compile your new test suite.

New test suite requires at least 1 source file and exactly 1 `TestMain.cpp` file with CppUnit test runner inside.

You will also have to compile and link libraries or modules you want to test.

# TestFwk

This is a minimalistic testing framework for C++, especially designed for Maike integration. It features IDE-friendly logging in `file:line: message` format.

## Features

TestFwk has the following features:

 * *Small*. This means that there are no advanced features such as death tests
 * file:line: message error output on stderr
 * An empty test suite will issue a warning
 * The REQUIRE macro (similar to gtest ASSERT) works from a non-void function, though it will terminate the testcase.

## How to use TestFwk

The recommended way of using TestFwk in your project is to keep it as a git submodule. If properly configured, Maike pickup the test driver, and testcases should run automatically during the build job. For usage example, see `test.test.cpp`. Notice that including `testfwk.hpp` will pull in an implementation of `main` and add it to the dependency list. Also, see `maikeconfig.json` for a build configuration. For reference, all test macros are available in `validation.hpp`.

## Sample output

### Stdout

```
✓ SuccessfulTestcase (test.test.cpp:5)
✗ FailingTestcase (test.test.cpp:10)
✗ RequireFromNonVoidFunctionA (test.test.cpp:15)
✓ RequireFromNonVoidFunctionB (test.test.cpp:29)
```

### Stderr

```
test.test.cpp:12: error: Required 1 == 2, but 1 == 1, and 2 == 2
test.test.cpp:10: error: Requirement not satisfied. Testcase aborted.
test.test.cpp:10: error: Testcase failed
test.test.cpp:19: error: Required val != 0, but val == 0, and 0 == 0
test.test.cpp:15: error: Requirement not satisfied. Testcase aborted.
test.test.cpp:15: error: Testcase failed
Error: 2 of 4 testcases failed
```
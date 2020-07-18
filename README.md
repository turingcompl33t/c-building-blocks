## C Building Blocks

Essential Building Blocks for Learning and Working with C.

### Contents

- [Data Structures](./data-structures)
- [Algorithms](./algorithms)

### Unit Testing

Each module within this project includes some pre-written unit tests to make checking the correctness of your implementation simpler. The unit test driver programs utilize the [Check](https://libcheck.github.io/check/index.html) unit testing library for C to make simplify the process of writing tests. Thus, in order to run the automated tests, you need to setup `Check` on the system on which you plan to develop. A description of the process for accomplishing this follows below.

On a Linux distribution such as Ubuntu that utilizes `apt` to manage packages, one may install the `Check` library with the command:

```
$ sudo apt install check
```

The process is similar for Mac users with `Homebrew`:

```
$ brew install check
```

Once check is installed by the package manager, the provided unit tests should work out of the box. To check your implementation for a specific module, simply run

```
$ make check
```

in the module's source directory. This command builds the test driver and runs the tests. If instead you want to build the test driver without running the tests immediately, you may do so by running the command:

```
$ make driver
```

**Troubleshooting**

Inevitably, different systems come with different requirements for properly configuring the build process for unit tests. I have attempted to setup the build in a relatively system-agnostic way, but I know that cases in which the current configuration fails will certainly arise. 

The first place one should look when encountering build errors with the test driver programs is the following line in the module Makefile:

```
CHECK_LIBS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit
```

This line lists all of the libraries against which the driver program must be linked in order to successfully run tests with `Check`. However, it appears that `Check`'s dependencies can vary based on the system on which it is installed. Some manual inspection of missing / out-of-order dependencies should fix most issues when building test drivers.
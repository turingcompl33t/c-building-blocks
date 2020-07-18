## C Building Blocks

Essential Building Blocks for Learning and Working with C.

### Overview

This project is intended to serve as an aid to those learning C programming. It is organized as a collection of modules (and submodules) containing the declarations of common C library functionality lacking a concrete implementation. That's where you come in.

The objective is to look over the module definitions and the expected functional semantics (typically described in comments) and implement the module accordingly. Most modules include some degree of automated testing that enables you to quickly check the correctness of your implementation.

### Contents

- [Data Structures](./data-structures)
- [Algorithms](./algorithms)

### Working in this Project

The easiest way to get started working in this project is to create your own fork of this repository and work from there. Creating a fork implies that you will be able to make arbitrary changes to the code in the repository without affecting the upstream repository (this repo) _but_ still retain the ability to pull in changes to the upstream repository to your local fork. This is an important ability to have so that when, for instance, new modules are added to the upstream, you will seamlessly be able to pull them into your local fork.

**Creating a Fork**

Create a new fork of this repository by clicking the `Fork` button on the [Github page for this project](https://github.com/turingcompl33t/c-building-blocks). In Github's current UI layout, this button is located at the top right of the browser window, but of course this is subject to change. 

Creating a fork essentially creates a complete copy of this repository under your personal Github account while retaining the connection with the source repository. Once you have created a fork, `clone` the remote repository to your development system. 

**Working in your Fork**

Now that you have your own local copy of the fork, you're ready to begin working in the repository. However, before doing so, I recommend switching a different git _branch_. After cloning, you will be located on the `master` branch, which is the default branch for the project. However, switching to a new branch to work on will make the process of pulling in upstream changes to the original repository cleaner.

To create a new branch, run the command:

```
git checkout -b <BRANCH NAME>
```

Where `<BRANCH NAME>` is replaced with whatever you want to call your new branch. Good options include `dev`, `development`, `local`, etc.

Now when you make changes to the repository, they will all be collected on your new branch rather than being applied to `master`. We'll see how this helps us in the workflow when we see how to pull in upstream changes (next).

**Pulling in Upstream Changes**

Suppose that a new module is released in the upstream repository and it contains something you really want to work on. How do you pull in this new module to your fork?

The first step is to synchronize your fork's `master` branch with the upstream's `master`. First, pull the new changes by running the following command from the project root directory:

```
$ git fetch upstream
```

Then merge these changes into your fork's `master` branch:

```
$ git checkout master
$ git merge upstream/master
```

This brings your fork's `master` up to date with the upstream's `master`. The final step is to make sure these updates are reflected on the branch on which you are doing all your work (`dev`, `development`, etc.). Accomplish this by running the following commands:

```
$ git checkout <BRANCH NAME>
$ git rebase master
```

This command _rebases_ your development branch on top of the updated `master` branch, essentially applying the changes to `master` and then replaying all of the commits you have made since the last rebase on top of that.

Congratulations! You should now be fully equipped to work with this project, and perhaps you even learned a little bit about `git` along the way.

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
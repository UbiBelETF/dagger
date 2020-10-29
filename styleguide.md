# Dagger Style Guide

## 1. Files and Inclusion

**a. Headers**: All header files should have the `.h` extension. Header files can have one or more attached implementation files. 
Headers should always contain the symbols that other header/implementation files can include.

**b. Implementations**: Implementation files should have the `.cpp` extension.

**c. Include what you use**: Always include whatever you use in the class itself. Do **not** rely on transitive inclusion.
This also applies to related headers: `foo.c` should include `bar.h` if it uses a symbol from it even if `foo.h` includes `bar.h` already.

**d. Include where you use**: If a class requires `#include <foo>` in the implementation file, do **not** put that include into the header.
Separate definitions and implementations so that cyclic dependencies don't happen.

**e. Forward declarations**: Try not to use forward declarations wherever possible. It won't always be possible, and that's alright.
It is always a better solution to include the headers you need directly, if possible.

**f. Inlining**: Inline only functions that have no branches, loops, and whose local variables can be easily optimized away by substitution.

**g. Order of Includes**: Use the following order whenever including headers:
  - (if in implementation) related header
  - other local headers
  - third-party headers
  - system/standard library headers

## 2. Preprocessing

**a. Conditional processing**: If using conditional processing (`#ifdef`, for example), try to always use `#if defined(...)` instead.
The `#if` conditional is much more variable than `#ifdef`, as it gives you boolean operations on the parameters, for example:
  - `#if defined(A) && !defined(B)`
  - `#if !defined(C)`
  - `#if defined(A) || defined(B)`
  
Be aware that even though the capability for stacking boolean expressions exists, readability should always win.

**b. Preprocessing Blocks**: Any preprocessing block has to be annotated with a comment at the end. For example:
```
#if defined(foo)
...
#end //defined(foo)
```

**a. Guards**: All header files should have `#pragma once` guards to prevent multiple inclusions. Old-school `#define` guards are discouraged.

## 3. Global Scope and Namespaces

Whenever working with the global scope, use namespaces to identify them. Your project should be set up in such a way that the only things
in the global scope are namespaces that you defined (ie. no functions, classes, variables, or constants etc. by themselves)

**a. Namespace Names**: Use simple names for your namespaces. All namespaces should be in `lowerCamelCase`.

**b. Namespaced Functions**: All global function names should be in `UpperCamelCase`.

## 4. Classes and Structs

Even though C++ doesn't recognize a true difference between structs and classes, 
within Dagger, we strictly distinguish between the two and use them for different purposes.

**a. Structs**: Use structs for outward-facing systems. This mostly means components in entity-component-system frameworks, 
as well as data-only types that are used to only move data around.

  - **1) Struct Names**: Struct names are in `PascalCase` or `UpperCamelCase`.

  - **2) Struct Data Members**: Structs shouldn't hold complex logic, ie. anything other than constructors, destructors, getters, and setters.
For structs, member names are always `lowerCamelCase`.

  - **3) Struct Function Members**: Struct function members (methods, if any) are always in `UpperCamelCase`. General function rules (5.x) apply.

**b. Classes**: Use classes whenever there is any need for complex functionalities that require complex state or ownership.

  - **1) Class Names**: Class names are in `PascalCase` or `UpperCamelCase`.
  
  - **2) Class Data Members**: Class data members (fields) are always in `prefixed_UpperCammelCase`. The prefix depends on the type of member:
    + use `m_` for non-static members
    + use `s_` for static members
  
  - **3) Class Function Members**: Class function members (methods) are always in `UpperCamelCase`. General function rules (5.x) apply.

## 5. Functions

**a. Names**: Functions are always in `UpperCamelCase`.

**b. Arguments**: Function arguments are written as `lowerCamelCase_`. The underscore at the end is mandatory. This is used to distinguish them from local variables.

**c. Local Variables**: Local variables within function scope are always `lowerCamelCase`.

## 6. Enums

Always use **enum classes** instead of C-style enums.

**a. Names**: Make sure the enum name acts as a namespace for its members, so make it distinct and descriptive. It should be in `EUpperCamelCase`. The prefix `E` is used to make it more distinct.

**b. Member Names**: Members in an enum are written in `UpperCamelCase`. There is no need to repeat the name of the enum within the member, as it is scoped.

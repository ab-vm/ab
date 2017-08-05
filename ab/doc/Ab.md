#+TITLE: AB

* Self
- http://journal.stuffwithstuff.com/2012/12/19/the-impoliteness-of-overriding-methods/
* Compilation
- http://docs.racket-lang.org/nanopass/index.html


* Spidermonkey
Write Barriers
Object Groups
Initial shape table

# Ab

## Memory Regions

0

|----------------|
| Native Memory  |
|----------------|
| ...            |
|----------------|
| Managed Memory |
|----------------|

### Native Memory


### Managed Memory

|----------------|
| Old Space      |
|----------------|
| New Space      |
|  |-------------|
|  | Nursery     |
   |-------------|
   | Free        |
|--|-------------|

## Extension Instructions
Extension

### Atomics

Atomics are implemented as a bytecode prefix.

/// Atomic locks
/// http://llvm.org/docs/Atomics.html

ATOMIC_NO
ATOMIC_MONO
ACQ
REL
ACQ_REL
/// Sequentially consistent
SEQ_CST

ModuleStore ->

ModuleReader (ModuleVisitor &v) {}
Module

#### Valid Atomic instructions

```
/// Load and store instructions
GET_LOCAL
SET_LOCAL
```

```
ATOMIC GET_LOCAL(1)
```

### Locking

In case you don't want to create

### Send

### Dynamic code generation and compilation

UNSAFE_CALL

### Native / Internal instructions

https://github.com/WebAssembly/binaryen/blob/master/src/wasm-traversal.h

https://github.com/WebAssembly/spec/blob/master/interpreter/spec/decode.ml
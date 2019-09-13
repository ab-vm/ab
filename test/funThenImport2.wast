(module
  (type (;0;) (func (param i32 i32 i32) (result i32)))
  (type (;1;) (func (param i32) (result i32)))
  (type (;2;) (func (result i32)))
  (type (;3;) (func (param i32)))
  (type (;4;) (func (param i32 i32) (result i32)))
  (type (;5;) (func (param i32 i32)))
  (type (;6;) (func (param i32 i32 i32 i32 i32) (result i32)))
  (type (;7;) (func (param i32 i32 i32)))
  (type (;8;) (func (param i64 i32 i32) (result i32)))
  (type (;9;) (func (param i64 i32) (result i32)))
  (type (;10;) (func (param i32 i32 i32 i32 i32)))
  (type (;11;) (func (param i32 f64 i32 i32 i32 i32) (result i32)))
  (type (;12;) (func (param f64) (result i64)))
  (type (;13;) (func (param f64 i32) (result f64)))
  (type (;14;) (func))
    (type (;14;) (func))
      (type (;14;) (func))
        (type (;14;) (func))
          (type (;14;) (func))
            (type (;14;) (func))
              (type (;14;) (func))
                (type (;14;) (func))
                  (type (;14;) (func))
  (type (;15;) (func (param i32 i32 i32 i32) (result i32)))
  (import "env" "DYNAMICTOP_PTR" (global (;0;) i32))
  (import "env" "tempDoublePtr" (global (;1;) i32))
  (import "env" "ABORT" (global (;2;) i32))
  (import "env" "STACKTOP" (global (;3;) i32))
  (import "env" "STACK_MAX" (global (;4;) i32))
  (import "global" "NaN" (global (;5;) f64))
  (import "global" "Infinity" (global (;6;) f64))
  (import "env" "enlargeMemory" (func (;0;) (type 2)))
  (import "env" "getTotalMemory" (func (;1;) (type 2)))
  (import "env" "abortOnCannotGrowMemory" (func (;2;) (type 2)))
  (import "env" "abortStackOverflow" (func (;3;) (type 3)))
  (import "env" "nullFunc_ii" (func (;4;) (type 3)))
  (import "env" "nullFunc_iiii" (func (;5;) (type 3)))
  (import "env" "___lock" (func (;6;) (type 3)))
  (import "env" "___syscall6" (func (;7;) (type 4)))
  (import "env" "___setErrNo" (func (;8;) (type 3)))
  (import "env" "tableBase" (global (;8;) i32)))
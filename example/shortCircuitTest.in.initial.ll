; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define internal i1 @func1() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @0, i32 1)
  ret i1 false
}

declare i32 @printf(ptr, ...)

define internal i1 @func2() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @1, i32 2)
  ret i1 true
}

define internal i1 @testShortCircuit() {
entry:
  %result = alloca i1, align 1
  %0 = call i32 (ptr, ...) @printf(ptr @2, i32 3)
  %func1 = call i1 @func1()
  br i1 %func1, label %and.rhs, label %and.end

and.rhs:                                          ; preds = %entry
  %func2 = call i1 @func2()
  br label %and.end

and.end:                                          ; preds = %and.rhs, %entry
  %1 = phi i1 [ false, %entry ], [ %func2, %and.rhs ]
  br i1 %1, label %and.rhs1, label %and.end3

and.rhs1:                                         ; preds = %and.end
  %func22 = call i1 @func2()
  br label %and.end3

and.end3:                                         ; preds = %and.rhs1, %and.end
  %2 = phi i1 [ false, %and.end ], [ %func22, %and.rhs1 ]
  store i1 %2, ptr %result, align 1
  %func24 = call i1 @func2()
  br i1 %func24, label %and.rhs5, label %and.end7

and.rhs5:                                         ; preds = %and.end3
  %func16 = call i1 @func1()
  br label %and.end7

and.end7:                                         ; preds = %and.rhs5, %and.end3
  %3 = phi i1 [ false, %and.end3 ], [ %func16, %and.rhs5 ]
  br i1 %3, label %and.rhs8, label %and.end10

and.rhs8:                                         ; preds = %and.end7
  %func29 = call i1 @func2()
  br label %and.end10

and.end10:                                        ; preds = %and.rhs8, %and.end7
  %4 = phi i1 [ false, %and.end7 ], [ %func29, %and.rhs8 ]
  store i1 %4, ptr %result, align 1
  %5 = call i32 (ptr, ...) @printf(ptr @3, i32 4)
  %func111 = call i1 @func1()
  br i1 %func111, label %or.end, label %or.rhs

or.rhs:                                           ; preds = %and.end10
  %func212 = call i1 @func2()
  br label %or.end

or.end:                                           ; preds = %or.rhs, %and.end10
  %6 = phi i1 [ true, %and.end10 ], [ %func212, %or.rhs ]
  store i1 %6, ptr %result, align 1
  br i1 true, label %and.rhs13, label %and.end14

and.rhs13:                                        ; preds = %or.end
  br label %and.end14

and.end14:                                        ; preds = %and.rhs13, %or.end
  %7 = phi i1 [ false, %or.end ], [ true, %and.rhs13 ]
  br i1 %7, label %then, label %join

then:                                             ; preds = %and.end14
  %8 = call i32 (ptr, ...) @printf(ptr @4, i32 5)
  br label %join

join:                                             ; preds = %then, %and.end14
  br i1 false, label %and.rhs15, label %and.end16

and.rhs15:                                        ; preds = %join
  br label %and.end16

and.end16:                                        ; preds = %and.rhs15, %join
  %9 = phi i1 [ false, %join ], [ true, %and.rhs15 ]
  br i1 %9, label %then17, label %join18

then17:                                           ; preds = %and.end16
  %10 = call i32 (ptr, ...) @printf(ptr @5, i32 6)
  br label %join18

join18:                                           ; preds = %then17, %and.end16
  %result19 = load i1, ptr %result, align 1
  ret i1 %result19
}

define i32 @main(i32 %0) {
entry:
  %testShortCircuit = call i1 @testShortCircuit()
  ret i32 0
}

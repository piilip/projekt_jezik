; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main(i32 %0) {
entry:
  %i = alloca i32, align 4
  %sum = alloca i32, align 4
  %y = alloca i32, align 4
  %a = alloca [4 x i32], align 4
  store i32 2, ptr %y, align 4
  %y1 = load i32, ptr %y, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 %y1)
  %y2 = load i32, ptr %y, align 4
  %2 = add i32 %y2, 1
  %3 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 %2
  store i32 2, ptr %3, align 4
  %4 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 3
  %load = load i32, ptr %4, align 4
  store i32 %load, ptr %y, align 4
  %y3 = load i32, ptr %y, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @1, i32 %y3)
  store i32 0, ptr %sum, align 4
  %y4 = load i32, ptr %y, align 4
  %sum5 = load i32, ptr %sum, align 4
  %6 = add i32 %y4, %sum5
  store i32 %6, ptr %i, align 4
  store i32 1, ptr %i, align 4
  %sum6 = load i32, ptr %sum, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @2, i32 %sum6)
  store i32 0, ptr %sum, align 4
  br label %loop

loop:                                             ; preds = %body, %entry
  %i7 = load i32, ptr %i, align 4
  %8 = icmp sle i32 %i7, %0
  br i1 %8, label %body, label %join

body:                                             ; preds = %loop
  %sum8 = load i32, ptr %sum, align 4
  %i9 = load i32, ptr %i, align 4
  %9 = add i32 %sum8, %i9
  store i32 %9, ptr %sum, align 4
  %i10 = load i32, ptr %i, align 4
  %10 = add i32 %i10, 1
  store i32 %10, ptr %i, align 4
  br label %loop

join:                                             ; preds = %loop
  %sum11 = load i32, ptr %sum, align 4
  %11 = call i32 (ptr, ...) @printf(ptr @3, i32 %sum11)
  %sum12 = load i32, ptr %sum, align 4
  ret i32 %sum12
}

declare i32 @printf(ptr, ...)

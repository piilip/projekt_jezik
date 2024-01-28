; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main(i32 %0) {
entry:
  %b = alloca i32, align 4
  %i = alloca i32, align 4
  %d = alloca i32, align 4
  %y = alloca i32, align 4
  %a = alloca [4 x i32], align 4
  store i32 2, ptr %y, align 4
  %y1 = load i32, ptr %y, align 4
  %1 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 %y1
  store i32 3, ptr %1, align 4
  %y2 = load i32, ptr %y, align 4
  %2 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 %y2
  %load = load i32, ptr %2, align 4
  store i32 %load, ptr %d, align 4
  store i32 1, ptr %i, align 4
  store i32 1, ptr %b, align 4
  %3 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 2
  %load3 = load i32, ptr %3, align 4
  %4 = call i32 (ptr, ...) @printf(ptr @0, i32 %load3)
  %y4 = load i32, ptr %y, align 4
  %5 = add i32 %y4, 0
  %6 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 %5
  %load5 = load i32, ptr %6, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @1, i32 %load5)
  %i6 = load i32, ptr %i, align 4
  %b7 = load i32, ptr %b, align 4
  %8 = add i32 %i6, %b7
  %9 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 %8
  %load8 = load i32, ptr %9, align 4
  %10 = call i32 (ptr, ...) @printf(ptr @2, i32 %load8)
  %d9 = load i32, ptr %d, align 4
  %11 = call i32 (ptr, ...) @printf(ptr @3, i32 %d9)
  %d10 = load i32, ptr %d, align 4
  ret i32 %d10
}

declare i32 @printf(ptr, ...)

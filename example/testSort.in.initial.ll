; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main(i32 %0) {
entry:
  %j = alloca i32, align 4
  %temp = alloca i32, align 4
  %i = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 16, ptr %n, align 4
  store i32 0, ptr %i, align 4
  %arr = alloca [16 x i32], align 4
  %i1 = load i32, ptr %i, align 4
  %i2 = load i32, ptr %i, align 4
  %1 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %i2
  store i32 %i1, ptr %1, align 4
  %2 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 0
  store i32 434, ptr %2, align 4
  %3 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 1
  store i32 34, ptr %3, align 4
  %4 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 2
  store i32 -3, ptr %4, align 4
  %5 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 3
  store i32 -155, ptr %5, align 4
  %6 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 4
  store i32 3434, ptr %6, align 4
  %7 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 5
  store i32 -34, ptr %7, align 4
  %8 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 6
  store i32 34, ptr %8, align 4
  %9 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 7
  store i32 -155, ptr %9, align 4
  %10 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 8
  store i32 434, ptr %10, align 4
  %11 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 9
  store i32 34, ptr %11, align 4
  %12 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 10
  store i32 -3, ptr %12, align 4
  %13 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 11
  store i32 -155, ptr %13, align 4
  %14 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 12
  store i32 3434, ptr %14, align 4
  %15 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 13
  store i32 -34, ptr %15, align 4
  %16 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 14
  store i32 34, ptr %16, align 4
  %17 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 15
  store i32 -155, ptr %17, align 4
  %i3 = load i32, ptr %i, align 4
  %18 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %i3
  %load = load i32, ptr %18, align 4
  %19 = call i32 (ptr, ...) @printf(ptr @0, i32 %load)
  store i32 0, ptr %temp, align 4
  store i32 0, ptr %j, align 4
  store i32 0, ptr %i, align 4
  br label %loop

loop:                                             ; preds = %join11, %entry
  %i4 = load i32, ptr %i, align 4
  %n5 = load i32, ptr %n, align 4
  %20 = sub i32 %n5, 1
  %21 = icmp slt i32 %i4, %20
  br i1 %21, label %body, label %join

body:                                             ; preds = %loop
  store i32 0, ptr %j, align 4
  br label %loop6

join:                                             ; preds = %loop
  store i32 0, ptr %i, align 4
  br label %loop26

loop6:                                            ; preds = %join16, %body
  %j7 = load i32, ptr %j, align 4
  %n8 = load i32, ptr %n, align 4
  %22 = sub i32 %n8, 1
  %i9 = load i32, ptr %i, align 4
  %23 = sub i32 %22, %i9
  %24 = icmp slt i32 %j7, %23
  br i1 %24, label %body10, label %join11

body10:                                           ; preds = %loop6
  %j12 = load i32, ptr %j, align 4
  %25 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %j12
  %load13 = load i32, ptr %25, align 4
  %j14 = load i32, ptr %j, align 4
  %26 = add i32 %j14, 1
  %27 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %26
  %load15 = load i32, ptr %27, align 4
  %28 = icmp sgt i32 %load13, %load15
  br i1 %28, label %then, label %join16

join11:                                           ; preds = %loop6
  %i25 = load i32, ptr %i, align 4
  %29 = add i32 %i25, 1
  store i32 %29, ptr %i, align 4
  br label %loop

then:                                             ; preds = %body10
  %j17 = load i32, ptr %j, align 4
  %30 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %j17
  %load18 = load i32, ptr %30, align 4
  store i32 %load18, ptr %temp, align 4
  %j19 = load i32, ptr %j, align 4
  %31 = add i32 %j19, 1
  %32 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %31
  %load20 = load i32, ptr %32, align 4
  %j21 = load i32, ptr %j, align 4
  %33 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %j21
  store i32 %load20, ptr %33, align 4
  %temp22 = load i32, ptr %temp, align 4
  %j23 = load i32, ptr %j, align 4
  %34 = add i32 %j23, 1
  %35 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %34
  store i32 %temp22, ptr %35, align 4
  br label %join16

join16:                                           ; preds = %then, %body10
  %j24 = load i32, ptr %j, align 4
  %36 = add i32 %j24, 1
  store i32 %36, ptr %j, align 4
  br label %loop6

loop26:                                           ; preds = %body29, %join
  %i27 = load i32, ptr %i, align 4
  %n28 = load i32, ptr %n, align 4
  %37 = icmp slt i32 %i27, %n28
  br i1 %37, label %body29, label %join30

body29:                                           ; preds = %loop26
  %i31 = load i32, ptr %i, align 4
  %38 = getelementptr inbounds [16 x i32], ptr %arr, i32 0, i32 %i31
  %load32 = load i32, ptr %38, align 4
  %39 = call i32 (ptr, ...) @printf(ptr @1, i32 %load32)
  %i33 = load i32, ptr %i, align 4
  %40 = add i32 %i33, 1
  store i32 %40, ptr %i, align 4
  br label %loop26

join30:                                           ; preds = %loop26
  ret i32 0
}

declare i32 @printf(ptr, ...)

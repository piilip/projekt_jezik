; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@7 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@8 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %y3 = alloca i32, align 4
  %istinaJe = alloca i1, align 1
  %y = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 3, ptr %i, align 4
  store i32 2, ptr %y, align 4
  store i1 false, ptr %istinaJe, align 1
  %istinaJe1 = load i1, ptr %istinaJe, align 1
  %isTrue = icmp ne i1 %istinaJe1, false
  %boolToStr = select i1 %isTrue, ptr @0, ptr @1
  %0 = call i32 (ptr, ...) @printf(ptr @2, ptr %boolToStr)
  %i2 = load i32, ptr %i, align 4
  %1 = icmp sgt i32 %i2, 1
  br i1 %1, label %then, label %join

then:                                             ; preds = %entry
  store i32 3, ptr %y3, align 4
  %y4 = load i32, ptr %y3, align 4
  %2 = call i32 (ptr, ...) @printf(ptr @3, i32 %y4)
  br label %join

join:                                             ; preds = %then, %entry
  %i5 = load i32, ptr %i, align 4
  %3 = icmp ne i32 %i5, 4
  br i1 %3, label %then6, label %join7

then6:                                            ; preds = %join
  %4 = call i32 (ptr, ...) @printf(ptr @4, i32 4)
  br label %join7

join7:                                            ; preds = %then6, %join
  %i8 = load i32, ptr %i, align 4
  %y9 = load i32, ptr %y, align 4
  %5 = icmp eq i32 %i8, %y9
  br i1 %5, label %then10, label %else

then10:                                           ; preds = %join7
  %6 = call i32 (ptr, ...) @printf(ptr @5, i32 1)
  br label %join11

else:                                             ; preds = %join7
  %i12 = load i32, ptr %i, align 4
  %y13 = load i32, ptr %y, align 4
  %7 = icmp sge i32 %i12, %y13
  br i1 %7, label %then14, label %else15

join11:                                           ; preds = %join16, %then10
  ret i32 0

then14:                                           ; preds = %else
  %8 = call i32 (ptr, ...) @printf(ptr @6, i32 2)
  br label %join16

else15:                                           ; preds = %else
  %i17 = load i32, ptr %i, align 4
  %y18 = load i32, ptr %y, align 4
  %9 = icmp sle i32 %i17, %y18
  br i1 %9, label %then19, label %else20

join16:                                           ; preds = %join21, %then14
  br label %join11

then19:                                           ; preds = %else15
  %10 = call i32 (ptr, ...) @printf(ptr @7, i32 3)
  br label %join21

else20:                                           ; preds = %else15
  %i22 = load i32, ptr %i, align 4
  %y23 = load i32, ptr %y, align 4
  %11 = icmp ne i32 %i22, %y23
  br i1 %11, label %then24, label %join25

join21:                                           ; preds = %join25, %then19
  br label %join16

then24:                                           ; preds = %else20
  %12 = call i32 (ptr, ...) @printf(ptr @8, i32 4)
  br label %join25

join25:                                           ; preds = %then24, %else20
  br label %join21
}

declare i32 @printf(ptr, ...)

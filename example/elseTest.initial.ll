; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %value = alloca i32, align 4
  store i32 5, ptr %value, align 4
  %value1 = load i32, ptr %value, align 4
  %0 = icmp eq i32 %value1, 1
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 1)
  br label %join

else:                                             ; preds = %entry
  %value2 = load i32, ptr %value, align 4
  %2 = icmp eq i32 %value2, 2
  br i1 %2, label %then3, label %else4

join:                                             ; preds = %join5, %then
  ret i32 0

then3:                                            ; preds = %else
  %3 = call i32 (ptr, ...) @printf(ptr @1, i32 2)
  br label %join5

else4:                                            ; preds = %else
  %value6 = load i32, ptr %value, align 4
  %4 = icmp eq i32 %value6, 3
  br i1 %4, label %then7, label %else8

join5:                                            ; preds = %join9, %then3
  br label %join

then7:                                            ; preds = %else4
  %5 = call i32 (ptr, ...) @printf(ptr @2, i32 3)
  br label %join9

else8:                                            ; preds = %else4
  %value10 = load i32, ptr %value, align 4
  %6 = icmp eq i32 %value10, 4
  br i1 %6, label %then11, label %else12

join9:                                            ; preds = %join13, %then7
  br label %join5

then11:                                           ; preds = %else8
  %7 = call i32 (ptr, ...) @printf(ptr @3, i32 4)
  br label %join13

else12:                                           ; preds = %else8
  %value14 = load i32, ptr %value, align 4
  %8 = icmp eq i32 %value14, 5
  br i1 %8, label %then15, label %else16

join13:                                           ; preds = %join17, %then11
  br label %join9

then15:                                           ; preds = %else12
  %9 = call i32 (ptr, ...) @printf(ptr @4, i32 5)
  br label %join17

else16:                                           ; preds = %else12
  %10 = call i32 (ptr, ...) @printf(ptr @5, i32 1886)
  br label %join17

join17:                                           ; preds = %else16, %then15
  br label %join13
}

declare i32 @printf(ptr, ...)

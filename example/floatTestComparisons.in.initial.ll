; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main(i32 %0) {
entry:
  %z = alloca i32, align 4
  %y = alloca i32, align 4
  %i = alloca float, align 4
  %j = alloca i32, align 4
  br i1 true, label %then, label %join

then:                                             ; preds = %entry
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 1)
  br label %join

join:                                             ; preds = %then, %entry
  store i32 3, ptr %j, align 4
  %j1 = load i32, ptr %j, align 4
  %2 = call i32 (ptr, ...) @printf(ptr @1, i32 %j1)
  %arr = alloca [5 x float], align 4
  store float 0x40099999A0000000, ptr %i, align 4
  store i32 2, ptr %y, align 4
  %i2 = load float, ptr %i, align 4
  %y3 = load i32, ptr %y, align 4
  %3 = getelementptr inbounds [5 x float], ptr %arr, i32 0, i32 %y3
  store float %i2, ptr %3, align 4
  %y4 = load i32, ptr %y, align 4
  %4 = getelementptr inbounds [5 x float], ptr %arr, i32 0, i32 %y4
  %load = load float, ptr %4, align 4
  %floatToDouble = fpext float %load to double
  %5 = call i32 (ptr, ...) @printf(ptr @2, double %floatToDouble)
  %y5 = load i32, ptr %y, align 4
  %6 = getelementptr inbounds [5 x float], ptr %arr, i32 0, i32 %y5
  %load6 = load float, ptr %6, align 4
  %i7 = load float, ptr %i, align 4
  %7 = fcmp ueq float %load6, %i7
  br i1 %7, label %then8, label %join9

then8:                                            ; preds = %join
  %y10 = load i32, ptr %y, align 4
  %8 = getelementptr inbounds [5 x float], ptr %arr, i32 0, i32 %y10
  %load11 = load float, ptr %8, align 4
  %floatToDouble12 = fpext float %load11 to double
  %9 = call i32 (ptr, ...) @printf(ptr @3, double %floatToDouble12)
  br label %join9

join9:                                            ; preds = %then8, %join
  store i32 0, ptr %z, align 4
  %i13 = load float, ptr %i, align 4
  %toint = fptosi float %i13 to i32
  store i32 %toint, ptr %z, align 4
  %i14 = load float, ptr %i, align 4
  %y15 = load i32, ptr %y, align 4
  %inttofloat = sitofp i32 %y15 to float
  %floatLT = fcmp olt float %i14, %inttofloat
  br i1 %floatLT, label %then16, label %join17

then16:                                           ; preds = %join9
  %10 = call i32 (ptr, ...) @printf(ptr @4, i32 1)
  br label %join17

join17:                                           ; preds = %then16, %join9
  %i18 = load float, ptr %i, align 4
  %y19 = load i32, ptr %y, align 4
  %inttofloat20 = sitofp i32 %y19 to float
  %11 = fcmp ugt float %i18, %inttofloat20
  br i1 %11, label %then21, label %join22

then21:                                           ; preds = %join17
  %12 = call i32 (ptr, ...) @printf(ptr @5, i32 2)
  br label %join22

join22:                                           ; preds = %then21, %join17
  %i23 = load float, ptr %i, align 4
  %13 = fcmp ugt float %i23, 2.000000e+00
  br i1 %13, label %then24, label %join25

then24:                                           ; preds = %join22
  %14 = call i32 (ptr, ...) @printf(ptr @6, i32 3)
  br label %join25

join25:                                           ; preds = %then24, %join22
  ret i32 0
}

declare i32 @printf(ptr, ...)

; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define i32 @main(i32 %0) {
entry:
  %i41 = alloca i32, align 4
  %j = alloca i32, align 4
  %i14 = alloca i32, align 4
  %temp = alloca float, align 4
  %i5 = alloca i32, align 4
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %arr = alloca [10 x float], align 4
  store i32 0, ptr %i, align 4
  br label %for.header

for.header:                                       ; preds = %for.update, %entry
  %i1 = load i32, ptr %i, align 4
  %1 = icmp slt i32 %i1, 10
  br i1 %1, label %for.body, label %for.exit

for.body:                                         ; preds = %for.header
  %i2 = load i32, ptr %i, align 4
  %inttofloat = sitofp i32 %i2 to float
  %2 = fsub float 0x4024333340000000, %inttofloat
  %i3 = load i32, ptr %i, align 4
  %3 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %i3
  store float %2, ptr %3, align 4
  br label %for.update

for.update:                                       ; preds = %for.body
  %i4 = load i32, ptr %i, align 4
  %4 = add i32 %i4, 1
  store i32 %4, ptr %i, align 4
  br label %for.header

for.exit:                                         ; preds = %for.header
  store i32 10, ptr %n, align 4
  store i32 0, ptr %i5, align 4
  br label %for.header6

for.header6:                                      ; preds = %for.update8, %for.exit
  %i10 = load i32, ptr %i5, align 4
  %n11 = load i32, ptr %n, align 4
  %5 = icmp slt i32 %i10, %n11
  br i1 %5, label %for.body7, label %for.exit9

for.body7:                                        ; preds = %for.header6
  %i12 = load i32, ptr %i5, align 4
  %6 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %i12
  %load = load float, ptr %6, align 4
  %floatToDouble = fpext float %load to double
  %7 = call i32 (ptr, ...) @printf(ptr @0, double %floatToDouble)
  br label %for.update8

for.update8:                                      ; preds = %for.body7
  %i13 = load i32, ptr %i5, align 4
  %8 = add i32 %i13, 1
  store i32 %8, ptr %i5, align 4
  br label %for.header6

for.exit9:                                        ; preds = %for.header6
  store float 0.000000e+00, ptr %temp, align 4
  store i32 0, ptr %i14, align 4
  br label %for.header15

for.header15:                                     ; preds = %for.update17, %for.exit9
  %i19 = load i32, ptr %i14, align 4
  %n20 = load i32, ptr %n, align 4
  %9 = sub i32 %n20, 1
  %10 = icmp slt i32 %i19, %9
  br i1 %10, label %for.body16, label %for.exit18

for.body16:                                       ; preds = %for.header15
  store i32 0, ptr %j, align 4
  br label %for.header21

for.update17:                                     ; preds = %for.exit24
  %i40 = load i32, ptr %i14, align 4
  %11 = add i32 %i40, 1
  store i32 %11, ptr %i14, align 4
  br label %for.header15

for.exit18:                                       ; preds = %for.header15
  store i32 0, ptr %i41, align 4
  br label %for.header42

for.header21:                                     ; preds = %for.update23, %for.body16
  %j25 = load i32, ptr %j, align 4
  %n26 = load i32, ptr %n, align 4
  %12 = sub i32 %n26, 1
  %i27 = load i32, ptr %i14, align 4
  %13 = sub i32 %12, %i27
  %14 = icmp slt i32 %j25, %13
  br i1 %14, label %for.body22, label %for.exit24

for.body22:                                       ; preds = %for.header21
  %j28 = load i32, ptr %j, align 4
  %15 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %j28
  %load29 = load float, ptr %15, align 4
  %j30 = load i32, ptr %j, align 4
  %16 = add i32 %j30, 1
  %17 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %16
  %load31 = load float, ptr %17, align 4
  %18 = fcmp ugt float %load29, %load31
  br i1 %18, label %then, label %join

for.update23:                                     ; preds = %join
  %j39 = load i32, ptr %j, align 4
  %19 = add i32 %j39, 1
  store i32 %19, ptr %j, align 4
  br label %for.header21

for.exit24:                                       ; preds = %for.header21
  br label %for.update17

then:                                             ; preds = %for.body22
  %j32 = load i32, ptr %j, align 4
  %20 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %j32
  %load33 = load float, ptr %20, align 4
  store float %load33, ptr %temp, align 4
  %j34 = load i32, ptr %j, align 4
  %21 = add i32 %j34, 1
  %22 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %21
  %load35 = load float, ptr %22, align 4
  %j36 = load i32, ptr %j, align 4
  %23 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %j36
  store float %load35, ptr %23, align 4
  %temp37 = load float, ptr %temp, align 4
  %j38 = load i32, ptr %j, align 4
  %24 = add i32 %j38, 1
  %25 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %24
  store float %temp37, ptr %25, align 4
  br label %join

join:                                             ; preds = %then, %for.body22
  br label %for.update23

for.header42:                                     ; preds = %for.update44, %for.exit18
  %i46 = load i32, ptr %i41, align 4
  %n47 = load i32, ptr %n, align 4
  %26 = icmp slt i32 %i46, %n47
  br i1 %26, label %for.body43, label %for.exit45

for.body43:                                       ; preds = %for.header42
  %i48 = load i32, ptr %i41, align 4
  %27 = getelementptr inbounds [10 x float], ptr %arr, i32 0, i32 %i48
  %load49 = load float, ptr %27, align 4
  %floatToDouble50 = fpext float %load49 to double
  %28 = call i32 (ptr, ...) @printf(ptr @1, double %floatToDouble50)
  br label %for.update44

for.update44:                                     ; preds = %for.body43
  %i51 = load i32, ptr %i41, align 4
  %29 = add i32 %i51, 1
  store i32 %29, ptr %i41, align 4
  br label %for.header42

for.exit45:                                       ; preds = %for.header42
  ret i32 0
}

declare i32 @printf(ptr, ...)

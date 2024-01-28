; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main(i32 %0) {
entry:
  %j = alloca i32, align 4
  %i1 = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 2, ptr %i, align 4
  store i32 0, ptr %i1, align 4
  br label %for.header

for.header:                                       ; preds = %for.update, %entry
  %i2 = load i32, ptr %i1, align 4
  %1 = icmp slt i32 %i2, 10
  br i1 %1, label %for.body, label %for.exit

for.body:                                         ; preds = %for.header
  store i32 0, ptr %j, align 4
  br label %for.header3

for.update:                                       ; preds = %for.exit6
  %i10 = load i32, ptr %i1, align 4
  %2 = add i32 %i10, 1
  store i32 %2, ptr %i1, align 4
  br label %for.header

for.exit:                                         ; preds = %for.header
  %i11 = load i32, ptr %i, align 4
  %3 = call i32 (ptr, ...) @printf(ptr @1, i32 %i11)
  ret i32 0

for.header3:                                      ; preds = %for.update5, %for.body
  %j7 = load i32, ptr %j, align 4
  %4 = icmp slt i32 %j7, 10
  br i1 %4, label %for.body4, label %for.exit6

for.body4:                                        ; preds = %for.header3
  %j8 = load i32, ptr %j, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0, i32 %j8)
  br label %for.update5

for.update5:                                      ; preds = %for.body4
  %j9 = load i32, ptr %j, align 4
  %6 = add i32 %j9, 1
  store i32 %6, ptr %j, align 4
  br label %for.header3

for.exit6:                                        ; preds = %for.header3
  br label %for.update
}

declare i32 @printf(ptr, ...)

; ModuleID = 'module'
source_filename = "module"

define i32 @main(i32 %0) {
entry:
  %i = alloca i32, align 4
  %d = alloca i32, align 4
  %y = alloca i32, align 4
  %a = alloca [4 x i32], align 4
  store i32 2, ptr %y, align 4
  %1 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 3
  store i32 2, ptr %1, align 4
  store i32 3, ptr %d, align 4
  %d1 = load i32, ptr %d, align 4
  %2 = getelementptr inbounds [4 x i32], ptr %a, i32 0, i32 %d1
  %load = load i32, ptr %2, align 4
  %3 = add i32 %load, 1
  store i32 %3, ptr %i, align 4
  ret i32 0
}

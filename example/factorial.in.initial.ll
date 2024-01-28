; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define internal i32 @fact(i32 %0) {
entry:
  %1 = icmp eq i32 %0, 1
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  ret i32 %0

else:                                             ; preds = %entry
  %2 = sub i32 %0, 1
  %fact = call i32 @fact(i32 %2)
  %3 = mul i32 %0, %fact
  ret i32 %3

join:                                             ; No predecessors!
  ret i32 0
}

define i32 @main(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 6, ptr %x, align 4
  %fact = call i32 @fact(i32 4)
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 %fact)
  ret i32 0
}

declare i32 @printf(ptr, ...)

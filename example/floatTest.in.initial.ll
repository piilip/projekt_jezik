; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define i32 @main(i32 %0) {
entry:
  %y = alloca float, align 4
  %i = alloca float, align 4
  store float 0x4009D70A40000000, ptr %i, align 4
  %i1 = load float, ptr %i, align 4
  store float %i1, ptr %y, align 4
  %y2 = load float, ptr %y, align 4
  %floatToDouble = fpext float %y2 to double
  %1 = call i32 (ptr, ...) @printf(ptr @0, double %floatToDouble)
  ret i32 0
}

declare i32 @printf(ptr, ...)

; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main(i32 %0) local_unnamed_addr #0 {
entry:
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 1)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 3)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x40099999A0000000)
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x40099999A0000000)
  %5 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 2)
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 3)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }

; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define i32 @main(i32 %0) local_unnamed_addr #0 {
entry:
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 3)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 1)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 2)
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 1)
  %5 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 4)
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 1)
  %7 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 2)
  %8 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 5)
  ret i32 0
}

attributes #0 = { nofree nounwind }

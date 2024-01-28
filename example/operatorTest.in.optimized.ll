; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @0)
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 3)
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 4)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }

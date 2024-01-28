; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main(i32 %0) local_unnamed_addr #0 {
entry:
  br label %for.header3.preheader

for.header3.preheader:                            ; preds = %entry, %for.header3.preheader
  %i1.016 = phi i32 [ 0, %entry ], [ %11, %for.header3.preheader ]
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 0)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 1)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 2)
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 3)
  %5 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 4)
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 5)
  %7 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 6)
  %8 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 7)
  %9 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 8)
  %10 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 9)
  %11 = add nuw nsw i32 %i1.016, 1
  %12 = icmp ult i32 %i1.016, 9
  br i1 %12, label %for.header3.preheader, label %for.exit

for.exit:                                         ; preds = %for.header3.preheader
  %13 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }

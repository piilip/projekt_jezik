; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main(i32 %0) local_unnamed_addr #0 {
entry:
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 2)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 2)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 0)
  %.not22 = icmp slt i32 %0, 1
  br i1 %.not22, label %join, label %body

body:                                             ; preds = %entry, %body
  %i.024 = phi i32 [ %5, %body ], [ 1, %entry ]
  %sum.023 = phi i32 [ %4, %body ], [ 0, %entry ]
  %4 = add i32 %i.024, %sum.023
  %5 = add i32 %i.024, 1
  %.not = icmp sgt i32 %5, %0
  br i1 %.not, label %join, label %body

join:                                             ; preds = %body, %entry
  %sum.0.lcssa = phi i32 [ 0, %entry ], [ %4, %body ]
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %sum.0.lcssa)
  ret i32 %sum.0.lcssa
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }

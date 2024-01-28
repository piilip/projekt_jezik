; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main(i32 %0) local_unnamed_addr #0 {
entry:
  %arr = alloca [10 x float], align 4
  store float 0x4024333340000000, ptr %arr, align 4
  %1 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 1
  store float 0x4022333340000000, ptr %1, align 4
  %2 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 2
  store float 0x4020333340000000, ptr %2, align 4
  %3 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 3
  store float 0x401C666680000000, ptr %3, align 4
  %4 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 4
  store float 0x4018666680000000, ptr %4, align 4
  %5 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 5
  store float 0x4014666680000000, ptr %5, align 4
  %6 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 6
  store float 0x4010666680000000, ptr %6, align 4
  %7 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 7
  store float 0x4008CCCD00000000, ptr %7, align 4
  %8 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 8
  store float 0x4000CCCD00000000, ptr %8, align 4
  %9 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 9
  store float 0x3FF1999A00000000, ptr %9, align 4
  %10 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4024333340000000)
  %11 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4022333340000000)
  %12 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4020333340000000)
  %13 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x401C666680000000)
  %14 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4018666680000000)
  %15 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4014666680000000)
  %16 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4010666680000000)
  %17 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4008CCCD00000000)
  %18 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x4000CCCD00000000)
  %19 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double 0x3FF1999A00000000)
  br label %for.body22.preheader

for.body22.preheader:                             ; preds = %for.exit24, %entry
  %i14.074 = phi i32 [ %25, %for.exit24 ], [ 0, %entry ]
  %20 = sub nuw nsw i32 9, %i14.074
  %load29.pre = load float, ptr %arr, align 4
  br label %for.body22

for.body22:                                       ; preds = %for.body22.preheader, %join
  %load29 = phi float [ %load2976, %join ], [ %load29.pre, %for.body22.preheader ]
  %j.073 = phi i32 [ %21, %join ], [ 0, %for.body22.preheader ]
  %21 = add nuw nsw i32 %j.073, 1
  %22 = zext i32 %21 to i64
  %23 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 %22
  %load31 = load float, ptr %23, align 4
  %24 = fcmp ugt float %load29, %load31
  br i1 %24, label %then, label %join

for.exit24:                                       ; preds = %join
  %25 = add nuw nsw i32 %i14.074, 1
  %26 = icmp ult i32 %i14.074, 8
  br i1 %26, label %for.body22.preheader, label %for.body43.preheader

for.body43.preheader:                             ; preds = %for.exit24
  %load49 = load float, ptr %arr, align 4
  %floatToDouble50 = fpext float %load49 to double
  %27 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50)
  %load49.1 = load float, ptr %1, align 4
  %floatToDouble50.1 = fpext float %load49.1 to double
  %28 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.1)
  %load49.2 = load float, ptr %2, align 4
  %floatToDouble50.2 = fpext float %load49.2 to double
  %29 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.2)
  %load49.3 = load float, ptr %3, align 4
  %floatToDouble50.3 = fpext float %load49.3 to double
  %30 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.3)
  %load49.4 = load float, ptr %4, align 4
  %floatToDouble50.4 = fpext float %load49.4 to double
  %31 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.4)
  %load49.5 = load float, ptr %5, align 4
  %floatToDouble50.5 = fpext float %load49.5 to double
  %32 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.5)
  %load49.6 = load float, ptr %6, align 4
  %floatToDouble50.6 = fpext float %load49.6 to double
  %33 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.6)
  %load49.7 = load float, ptr %7, align 4
  %floatToDouble50.7 = fpext float %load49.7 to double
  %34 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.7)
  %load49.8 = load float, ptr %8, align 4
  %floatToDouble50.8 = fpext float %load49.8 to double
  %35 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.8)
  %load49.9 = load float, ptr %9, align 4
  %floatToDouble50.9 = fpext float %load49.9 to double
  %36 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, double %floatToDouble50.9)
  ret i32 0

then:                                             ; preds = %for.body22
  %37 = zext i32 %j.073 to i64
  %38 = getelementptr inbounds [10 x float], ptr %arr, i64 0, i64 %37
  store float %load31, ptr %38, align 4
  store float %load29, ptr %23, align 4
  br label %join

join:                                             ; preds = %then, %for.body22
  %load2976 = phi float [ %load29, %then ], [ %load31, %for.body22 ]
  %39 = icmp ult i32 %21, %20
  br i1 %39, label %for.body22, label %for.exit24
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }

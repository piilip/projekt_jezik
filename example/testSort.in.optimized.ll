; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main(i32 %0) local_unnamed_addr #0 {
entry:
  %arr = alloca [16 x i32], align 4
  store i32 434, ptr %arr, align 4
  %1 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 1
  store i32 34, ptr %1, align 4
  %2 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 2
  store i32 -3, ptr %2, align 4
  %3 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 3
  store i32 -155, ptr %3, align 4
  %4 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 4
  store i32 3434, ptr %4, align 4
  %5 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 5
  store i32 -34, ptr %5, align 4
  %6 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 6
  store i32 34, ptr %6, align 4
  %7 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 7
  store i32 -155, ptr %7, align 4
  %8 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 8
  store i32 434, ptr %8, align 4
  %9 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 9
  store i32 34, ptr %9, align 4
  %10 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 10
  store i32 -3, ptr %10, align 4
  %11 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 11
  store i32 -155, ptr %11, align 4
  %12 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 12
  store i32 3434, ptr %12, align 4
  %13 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 13
  store i32 -34, ptr %13, align 4
  %14 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 14
  store i32 34, ptr %14, align 4
  %15 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 15
  store i32 -155, ptr %15, align 4
  %16 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 434)
  br label %body10.preheader

body10.preheader:                                 ; preds = %join11, %entry
  %i.052 = phi i32 [ 0, %entry ], [ %22, %join11 ]
  %17 = sub nuw nsw i32 15, %i.052
  %load13.pre = load i32, ptr %arr, align 4
  br label %body10

body10:                                           ; preds = %body10.preheader, %join16
  %load13 = phi i32 [ %load1354, %join16 ], [ %load13.pre, %body10.preheader ]
  %j.051 = phi i32 [ %18, %join16 ], [ 0, %body10.preheader ]
  %18 = add nuw nsw i32 %j.051, 1
  %19 = zext i32 %18 to i64
  %20 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 %19
  %load15 = load i32, ptr %20, align 4
  %21 = icmp sgt i32 %load13, %load15
  br i1 %21, label %then, label %join16

join11:                                           ; preds = %join16
  %22 = add nuw nsw i32 %i.052, 1
  %23 = icmp ult i32 %i.052, 14
  br i1 %23, label %body10.preheader, label %body29.preheader

body29.preheader:                                 ; preds = %join11
  %load32 = load i32, ptr %arr, align 4
  %24 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32)
  %load32.1 = load i32, ptr %1, align 4
  %25 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.1)
  %load32.2 = load i32, ptr %2, align 4
  %26 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.2)
  %load32.3 = load i32, ptr %3, align 4
  %27 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.3)
  %load32.4 = load i32, ptr %4, align 4
  %28 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.4)
  %load32.5 = load i32, ptr %5, align 4
  %29 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.5)
  %load32.6 = load i32, ptr %6, align 4
  %30 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.6)
  %load32.7 = load i32, ptr %7, align 4
  %31 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.7)
  %load32.8 = load i32, ptr %8, align 4
  %32 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.8)
  %load32.9 = load i32, ptr %9, align 4
  %33 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.9)
  %load32.10 = load i32, ptr %10, align 4
  %34 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.10)
  %load32.11 = load i32, ptr %11, align 4
  %35 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.11)
  %load32.12 = load i32, ptr %12, align 4
  %36 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.12)
  %load32.13 = load i32, ptr %13, align 4
  %37 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.13)
  %load32.14 = load i32, ptr %14, align 4
  %38 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.14)
  %load32.15 = load i32, ptr %15, align 4
  %39 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %load32.15)
  ret i32 0

then:                                             ; preds = %body10
  %40 = zext i32 %j.051 to i64
  %41 = getelementptr inbounds [16 x i32], ptr %arr, i64 0, i64 %40
  store i32 %load15, ptr %41, align 4
  store i32 %load13, ptr %20, align 4
  br label %join16

join16:                                           ; preds = %then, %body10
  %load1354 = phi i32 [ %load13, %then ], [ %load15, %body10 ]
  %42 = icmp ult i32 %18, %17
  br i1 %42, label %body10, label %join11
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }

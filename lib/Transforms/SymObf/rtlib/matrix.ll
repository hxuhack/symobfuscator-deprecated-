; ModuleID = 'matrix.c'
source_filename = "matrix.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i64** @MatrixMult(i64** %mat1, i64** %mat2, i64 %m1Height, i64 %m1Width, i64 %m2Height, i64 %m2Width) #0 {
entry:
  %retval = alloca i64**, align 8
  %mat1.addr = alloca i64**, align 8
  %mat2.addr = alloca i64**, align 8
  %m1Height.addr = alloca i64, align 8
  %m1Width.addr = alloca i64, align 8
  %m2Height.addr = alloca i64, align 8
  %m2Width.addr = alloca i64, align 8
  %saved_stack = alloca i8*, align 8
  %cleanup.dest.slot = alloca i32
  %i = alloca i64, align 8
  %j = alloca i64, align 8
  %ele = alloca i64, align 8
  %k = alloca i64, align 8
  store i64** %mat1, i64*** %mat1.addr, align 8
  store i64** %mat2, i64*** %mat2.addr, align 8
  store i64 %m1Height, i64* %m1Height.addr, align 8
  store i64 %m1Width, i64* %m1Width.addr, align 8
  store i64 %m2Height, i64* %m2Height.addr, align 8
  store i64 %m2Width, i64* %m2Width.addr, align 8
  %0 = load i64, i64* %m1Height.addr, align 8
  %1 = load i64, i64* %m2Width.addr, align 8
  %2 = call i8* @llvm.stacksave()
  store i8* %2, i8** %saved_stack, align 8
  %3 = mul nuw i64 %0, %1
  %vla = alloca i64, i64 %3, align 16
  %4 = load i64, i64* %m1Width.addr, align 8
  %5 = load i64, i64* %m2Height.addr, align 8
  %cmp = icmp ne i64 %4, %5
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %6 = bitcast i64* %vla to i64**
  store i64** %6, i64*** %retval, align 8
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

if.end:                                           ; preds = %entry
  store i64 0, i64* %i, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc17, %if.end
  %7 = load i64, i64* %i, align 8
  %8 = load i64, i64* %m1Height.addr, align 8
  %cmp1 = icmp slt i64 %7, %8
  br i1 %cmp1, label %for.body, label %for.end19

for.body:                                         ; preds = %for.cond
  store i64 0, i64* %j, align 8
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc14, %for.body
  %9 = load i64, i64* %j, align 8
  %10 = load i64, i64* %m2Width.addr, align 8
  %cmp3 = icmp slt i64 %9, %10
  br i1 %cmp3, label %for.body4, label %for.end16

for.body4:                                        ; preds = %for.cond2
  store i64 0, i64* %ele, align 8
  store i64 0, i64* %k, align 8
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc, %for.body4
  %11 = load i64, i64* %k, align 8
  %12 = load i64, i64* %m1Width.addr, align 8
  %cmp6 = icmp slt i64 %11, %12
  br i1 %cmp6, label %for.body7, label %for.end

for.body7:                                        ; preds = %for.cond5
  %13 = load i64, i64* %ele, align 8
  %14 = load i64**, i64*** %mat1.addr, align 8
  %15 = bitcast i64** %14 to i64*
  %16 = load i64, i64* %i, align 8
  %17 = load i64, i64* %m1Width.addr, align 8
  %mul = mul nsw i64 %16, %17
  %add.ptr = getelementptr inbounds i64, i64* %15, i64 %mul
  %18 = load i64, i64* %k, align 8
  %add.ptr8 = getelementptr inbounds i64, i64* %add.ptr, i64 %18
  %19 = load i64, i64* %add.ptr8, align 8
  %20 = load i64**, i64*** %mat2.addr, align 8
  %21 = bitcast i64** %20 to i64*
  %22 = load i64, i64* %k, align 8
  %23 = load i64, i64* %m2Width.addr, align 8
  %mul9 = mul nsw i64 %22, %23
  %add.ptr10 = getelementptr inbounds i64, i64* %21, i64 %mul9
  %24 = load i64, i64* %j, align 8
  %add.ptr11 = getelementptr inbounds i64, i64* %add.ptr10, i64 %24
  %25 = load i64, i64* %add.ptr11, align 8
  %mul12 = mul nsw i64 %19, %25
  %add = add nsw i64 %13, %mul12
  store i64 %add, i64* %ele, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body7
  %26 = load i64, i64* %k, align 8
  %inc = add nsw i64 %26, 1
  store i64 %inc, i64* %k, align 8
  br label %for.cond5

for.end:                                          ; preds = %for.cond5
  %27 = load i64, i64* %ele, align 8
  %28 = load i64, i64* %j, align 8
  %29 = load i64, i64* %i, align 8
  %30 = mul nsw i64 %29, %1
  %arrayidx = getelementptr inbounds i64, i64* %vla, i64 %30
  %arrayidx13 = getelementptr inbounds i64, i64* %arrayidx, i64 %28
  store i64 %27, i64* %arrayidx13, align 8
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %31 = load i64, i64* %j, align 8
  %inc15 = add nsw i64 %31, 1
  store i64 %inc15, i64* %j, align 8
  br label %for.cond2

for.end16:                                        ; preds = %for.cond2
  br label %for.inc17

for.inc17:                                        ; preds = %for.end16
  %32 = load i64, i64* %i, align 8
  %inc18 = add nsw i64 %32, 1
  store i64 %inc18, i64* %i, align 8
  br label %for.cond

for.end19:                                        ; preds = %for.cond
  %33 = bitcast i64* %vla to i64**
  store i64** %33, i64*** %retval, align 8
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

cleanup:                                          ; preds = %for.end19, %if.then
  %34 = load i8*, i8** %saved_stack, align 8
  call void @llvm.stackrestore(i8* %34)
  %35 = load i64**, i64*** %retval, align 8
  ret i64** %35
}

; Function Attrs: nounwind
declare i8* @llvm.stacksave() #1

; Function Attrs: nounwind
declare void @llvm.stackrestore(i8*) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"Symbolic Obfuscator clang version 3.9.0 (tags/RELEASE_390/final) (based on LLVM 3.9.0)"}

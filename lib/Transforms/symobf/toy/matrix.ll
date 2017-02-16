; ModuleID = 'matrix.c'
source_filename = "matrix.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32** @MM(i32** %mat1, i32** %mat2, i32 %m1Height, i32 %m1Width, i32 %m2Height, i32 %m2Width) #0 {
entry:
  %retval = alloca i32**, align 8
  %mat1.addr = alloca i32**, align 8
  %mat2.addr = alloca i32**, align 8
  %m1Height.addr = alloca i32, align 4
  %m1Width.addr = alloca i32, align 4
  %m2Height.addr = alloca i32, align 4
  %m2Width.addr = alloca i32, align 4
  %saved_stack = alloca i8*, align 8
  %cleanup.dest.slot = alloca i32
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %ele = alloca i32, align 4
  %k = alloca i32, align 4
  store i32** %mat1, i32*** %mat1.addr, align 8
  store i32** %mat2, i32*** %mat2.addr, align 8
  store i32 %m1Height, i32* %m1Height.addr, align 4
  store i32 %m1Width, i32* %m1Width.addr, align 4
  store i32 %m2Height, i32* %m2Height.addr, align 4
  store i32 %m2Width, i32* %m2Width.addr, align 4
  %0 = load i32, i32* %m1Height.addr, align 4
  %1 = zext i32 %0 to i64
  %2 = load i32, i32* %m2Width.addr, align 4
  %3 = zext i32 %2 to i64
  %4 = call i8* @llvm.stacksave()
  store i8* %4, i8** %saved_stack, align 8
  %5 = mul nuw i64 %1, %3
  %vla = alloca i32, i64 %5, align 16
  %6 = load i32, i32* %m1Width.addr, align 4
  %7 = load i32, i32* %m2Height.addr, align 4
  %cmp = icmp ne i32 %6, %7
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %8 = bitcast i32* %vla to i32**
  store i32** %8, i32*** %retval, align 8
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

if.end:                                           ; preds = %entry
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc35, %if.end
  %9 = load i32, i32* %i, align 4
  %10 = load i32, i32* %m1Height.addr, align 4
  %cmp1 = icmp slt i32 %9, %10
  br i1 %cmp1, label %for.body, label %for.end37

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc32, %for.body
  %11 = load i32, i32* %j, align 4
  %12 = load i32, i32* %m2Width.addr, align 4
  %cmp3 = icmp slt i32 %11, %12
  br i1 %cmp3, label %for.body4, label %for.end34

for.body4:                                        ; preds = %for.cond2
  store i32 0, i32* %ele, align 4
  store i32 0, i32* %k, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc, %for.body4
  %13 = load i32, i32* %k, align 4
  %14 = load i32, i32* %m1Width.addr, align 4
  %cmp6 = icmp slt i32 %13, %14
  br i1 %cmp6, label %for.body7, label %for.end

for.body7:                                        ; preds = %for.cond5
  %15 = load i32, i32* %k, align 4
  %cmp8 = icmp eq i32 %15, 0
  br i1 %cmp8, label %if.then9, label %if.else

if.then9:                                         ; preds = %for.body7
  %16 = load i32**, i32*** %mat1.addr, align 8
  %17 = bitcast i32** %16 to i32*
  %18 = load i32, i32* %i, align 4
  %19 = load i32, i32* %m1Width.addr, align 4
  %mul = mul nsw i32 %18, %19
  %idx.ext = sext i32 %mul to i64
  %add.ptr = getelementptr inbounds i32, i32* %17, i64 %idx.ext
  %20 = load i32, i32* %k, align 4
  %idx.ext10 = sext i32 %20 to i64
  %add.ptr11 = getelementptr inbounds i32, i32* %add.ptr, i64 %idx.ext10
  %21 = load i32, i32* %add.ptr11, align 4
  %22 = load i32**, i32*** %mat2.addr, align 8
  %23 = bitcast i32** %22 to i32*
  %24 = load i32, i32* %k, align 4
  %25 = load i32, i32* %m2Width.addr, align 4
  %mul12 = mul nsw i32 %24, %25
  %idx.ext13 = sext i32 %mul12 to i64
  %add.ptr14 = getelementptr inbounds i32, i32* %23, i64 %idx.ext13
  %26 = load i32, i32* %j, align 4
  %idx.ext15 = sext i32 %26 to i64
  %add.ptr16 = getelementptr inbounds i32, i32* %add.ptr14, i64 %idx.ext15
  %27 = load i32, i32* %add.ptr16, align 4
  %mul17 = mul nsw i32 %21, %27
  store i32 %mul17, i32* %ele, align 4
  br label %if.end29

if.else:                                          ; preds = %for.body7
  %28 = load i32, i32* %ele, align 4
  %29 = load i32**, i32*** %mat1.addr, align 8
  %30 = bitcast i32** %29 to i32*
  %31 = load i32, i32* %i, align 4
  %32 = load i32, i32* %m1Width.addr, align 4
  %mul18 = mul nsw i32 %31, %32
  %idx.ext19 = sext i32 %mul18 to i64
  %add.ptr20 = getelementptr inbounds i32, i32* %30, i64 %idx.ext19
  %33 = load i32, i32* %k, align 4
  %idx.ext21 = sext i32 %33 to i64
  %add.ptr22 = getelementptr inbounds i32, i32* %add.ptr20, i64 %idx.ext21
  %34 = load i32, i32* %add.ptr22, align 4
  %35 = load i32**, i32*** %mat2.addr, align 8
  %36 = bitcast i32** %35 to i32*
  %37 = load i32, i32* %k, align 4
  %38 = load i32, i32* %m2Width.addr, align 4
  %mul23 = mul nsw i32 %37, %38
  %idx.ext24 = sext i32 %mul23 to i64
  %add.ptr25 = getelementptr inbounds i32, i32* %36, i64 %idx.ext24
  %39 = load i32, i32* %j, align 4
  %idx.ext26 = sext i32 %39 to i64
  %add.ptr27 = getelementptr inbounds i32, i32* %add.ptr25, i64 %idx.ext26
  %40 = load i32, i32* %add.ptr27, align 4
  %mul28 = mul nsw i32 %34, %40
  %add = add nsw i32 %28, %mul28
  store i32 %add, i32* %ele, align 4
  br label %if.end29

if.end29:                                         ; preds = %if.else, %if.then9
  br label %for.inc

for.inc:                                          ; preds = %if.end29
  %41 = load i32, i32* %k, align 4
  %inc = add nsw i32 %41, 1
  store i32 %inc, i32* %k, align 4
  br label %for.cond5

for.end:                                          ; preds = %for.cond5
  %42 = load i32, i32* %ele, align 4
  %43 = load i32, i32* %j, align 4
  %idxprom = sext i32 %43 to i64
  %44 = load i32, i32* %i, align 4
  %idxprom30 = sext i32 %44 to i64
  %45 = mul nsw i64 %idxprom30, %3
  %arrayidx = getelementptr inbounds i32, i32* %vla, i64 %45
  %arrayidx31 = getelementptr inbounds i32, i32* %arrayidx, i64 %idxprom
  store i32 %42, i32* %arrayidx31, align 4
  br label %for.inc32

for.inc32:                                        ; preds = %for.end
  %46 = load i32, i32* %j, align 4
  %inc33 = add nsw i32 %46, 1
  store i32 %inc33, i32* %j, align 4
  br label %for.cond2

for.end34:                                        ; preds = %for.cond2
  br label %for.inc35

for.inc35:                                        ; preds = %for.end34
  %47 = load i32, i32* %i, align 4
  %inc36 = add nsw i32 %47, 1
  store i32 %inc36, i32* %i, align 4
  br label %for.cond

for.end37:                                        ; preds = %for.cond
  %48 = bitcast i32* %vla to i32**
  store i32** %48, i32*** %retval, align 8
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

cleanup:                                          ; preds = %for.end37, %if.then
  %49 = load i8*, i8** %saved_stack, align 8
  call void @llvm.stackrestore(i8* %49)
  %50 = load i32**, i32*** %retval, align 8
  ret i32** %50
}

; Function Attrs: nounwind
declare i8* @llvm.stacksave() #1

; Function Attrs: nounwind
declare void @llvm.stackrestore(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %mat1 = alloca [3 x [2 x i32]], align 16
  %mat2 = alloca [2 x [3 x i32]], align 16
  %mat3 = alloca i32**, align 8
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %arrayidx = getelementptr inbounds [3 x [2 x i32]], [3 x [2 x i32]]* %mat1, i64 0, i64 0
  %arrayidx1 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx, i64 0, i64 0
  store i32 0, i32* %arrayidx1, align 16
  %arrayidx2 = getelementptr inbounds [3 x [2 x i32]], [3 x [2 x i32]]* %mat1, i64 0, i64 0
  %arrayidx3 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx2, i64 0, i64 1
  store i32 1, i32* %arrayidx3, align 4
  %arrayidx4 = getelementptr inbounds [3 x [2 x i32]], [3 x [2 x i32]]* %mat1, i64 0, i64 1
  %arrayidx5 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx4, i64 0, i64 0
  store i32 0, i32* %arrayidx5, align 8
  %arrayidx6 = getelementptr inbounds [3 x [2 x i32]], [3 x [2 x i32]]* %mat1, i64 0, i64 1
  %arrayidx7 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx6, i64 0, i64 1
  store i32 1, i32* %arrayidx7, align 4
  %arrayidx8 = getelementptr inbounds [3 x [2 x i32]], [3 x [2 x i32]]* %mat1, i64 0, i64 2
  %arrayidx9 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx8, i64 0, i64 0
  store i32 0, i32* %arrayidx9, align 16
  %arrayidx10 = getelementptr inbounds [3 x [2 x i32]], [3 x [2 x i32]]* %mat1, i64 0, i64 2
  %arrayidx11 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx10, i64 0, i64 1
  store i32 1, i32* %arrayidx11, align 4
  %arrayidx12 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %mat2, i64 0, i64 0
  %arrayidx13 = getelementptr inbounds [3 x i32], [3 x i32]* %arrayidx12, i64 0, i64 0
  store i32 0, i32* %arrayidx13, align 16
  %arrayidx14 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %mat2, i64 0, i64 0
  %arrayidx15 = getelementptr inbounds [3 x i32], [3 x i32]* %arrayidx14, i64 0, i64 1
  store i32 1, i32* %arrayidx15, align 4
  %arrayidx16 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %mat2, i64 0, i64 0
  %arrayidx17 = getelementptr inbounds [3 x i32], [3 x i32]* %arrayidx16, i64 0, i64 2
  store i32 0, i32* %arrayidx17, align 8
  %arrayidx18 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %mat2, i64 0, i64 1
  %arrayidx19 = getelementptr inbounds [3 x i32], [3 x i32]* %arrayidx18, i64 0, i64 0
  store i32 1, i32* %arrayidx19, align 4
  %arrayidx20 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %mat2, i64 0, i64 1
  %arrayidx21 = getelementptr inbounds [3 x i32], [3 x i32]* %arrayidx20, i64 0, i64 1
  store i32 0, i32* %arrayidx21, align 4
  %arrayidx22 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %mat2, i64 0, i64 1
  %arrayidx23 = getelementptr inbounds [3 x i32], [3 x i32]* %arrayidx22, i64 0, i64 2
  store i32 1, i32* %arrayidx23, align 4
  %arraydecay = getelementptr inbounds [3 x [2 x i32]], [3 x [2 x i32]]* %mat1, i32 0, i32 0
  %0 = bitcast [2 x i32]* %arraydecay to i32**
  %arraydecay24 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %mat2, i32 0, i32 0
  %1 = bitcast [3 x i32]* %arraydecay24 to i32**
  %call = call i32** @MM(i32** %0, i32** %1, i32 3, i32 2, i32 2, i32 3)
  store i32** %call, i32*** %mat3, align 8
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.0 (tags/RELEASE_390/final)"}

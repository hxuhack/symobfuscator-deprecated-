; ModuleID = 'matrix.c'
source_filename = "matrix.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@main.initMat = private unnamed_addr constant [1 x [2 x i32]] [[2 x i32] [i32 1, i32 0]], align 4

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

for.cond:                                         ; preds = %for.inc21, %if.end
  %9 = load i32, i32* %i, align 4
  %10 = load i32, i32* %m1Height.addr, align 4
  %cmp1 = icmp slt i32 %9, %10
  br i1 %cmp1, label %for.body, label %for.end23

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc18, %for.body
  %11 = load i32, i32* %j, align 4
  %12 = load i32, i32* %m2Width.addr, align 4
  %cmp3 = icmp slt i32 %11, %12
  br i1 %cmp3, label %for.body4, label %for.end20

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
  %15 = load i32, i32* %ele, align 4
  %16 = load i32**, i32*** %mat1.addr, align 8
  %17 = bitcast i32** %16 to i32*
  %18 = load i32, i32* %i, align 4
  %19 = load i32, i32* %m1Width.addr, align 4
  %mul = mul nsw i32 %18, %19
  %idx.ext = sext i32 %mul to i64
  %add.ptr = getelementptr inbounds i32, i32* %17, i64 %idx.ext
  %20 = load i32, i32* %k, align 4
  %idx.ext8 = sext i32 %20 to i64
  %add.ptr9 = getelementptr inbounds i32, i32* %add.ptr, i64 %idx.ext8
  %21 = load i32, i32* %add.ptr9, align 4
  %22 = load i32**, i32*** %mat2.addr, align 8
  %23 = bitcast i32** %22 to i32*
  %24 = load i32, i32* %k, align 4
  %25 = load i32, i32* %m2Width.addr, align 4
  %mul10 = mul nsw i32 %24, %25
  %idx.ext11 = sext i32 %mul10 to i64
  %add.ptr12 = getelementptr inbounds i32, i32* %23, i64 %idx.ext11
  %26 = load i32, i32* %j, align 4
  %idx.ext13 = sext i32 %26 to i64
  %add.ptr14 = getelementptr inbounds i32, i32* %add.ptr12, i64 %idx.ext13
  %27 = load i32, i32* %add.ptr14, align 4
  %mul15 = mul nsw i32 %21, %27
  %add = add nsw i32 %15, %mul15
  store i32 %add, i32* %ele, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body7
  %28 = load i32, i32* %k, align 4
  %inc = add nsw i32 %28, 1
  store i32 %inc, i32* %k, align 4
  br label %for.cond5

for.end:                                          ; preds = %for.cond5
  %29 = load i32, i32* %ele, align 4
  %30 = load i32, i32* %j, align 4
  %idxprom = sext i32 %30 to i64
  %31 = load i32, i32* %i, align 4
  %idxprom16 = sext i32 %31 to i64
  %32 = mul nsw i64 %idxprom16, %3
  %arrayidx = getelementptr inbounds i32, i32* %vla, i64 %32
  %arrayidx17 = getelementptr inbounds i32, i32* %arrayidx, i64 %idxprom
  store i32 %29, i32* %arrayidx17, align 4
  %33 = load i32, i32* %ele, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %33)
  br label %for.inc18

for.inc18:                                        ; preds = %for.end
  %34 = load i32, i32* %j, align 4
  %inc19 = add nsw i32 %34, 1
  store i32 %inc19, i32* %j, align 4
  br label %for.cond2

for.end20:                                        ; preds = %for.cond2
  br label %for.inc21

for.inc21:                                        ; preds = %for.end20
  %35 = load i32, i32* %i, align 4
  %inc22 = add nsw i32 %35, 1
  store i32 %inc22, i32* %i, align 4
  br label %for.cond

for.end23:                                        ; preds = %for.cond
  %36 = bitcast i32* %vla to i32**
  store i32** %36, i32*** %retval, align 8
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

cleanup:                                          ; preds = %for.end23, %if.then
  %37 = load i8*, i8** %saved_stack, align 8
  call void @llvm.stackrestore(i8* %37)
  %38 = load i32**, i32*** %retval, align 8
  ret i32** %38
}

; Function Attrs: nounwind
declare i8* @llvm.stacksave() #1

declare i32 @printf(i8*, ...) #2

; Function Attrs: nounwind
declare void @llvm.stackrestore(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %len = alloca i32, align 4
  %mat01 = alloca [2 x [2 x i32]], align 16
  %mat00 = alloca [2 x [2 x i32]], align 16
  %mat11 = alloca [2 x [2 x i32]], align 16
  %mat10 = alloca [2 x [2 x i32]], align 16
  %mat = alloca [2 x [2 x i32**]], align 16
  %initMat = alloca [1 x [2 x i32]], align 4
  %inp = alloca i32, align 4
  %interMat = alloca i32**, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 2, i32* %len, align 4
  %arrayidx = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat01, i64 0, i64 0
  %arrayidx1 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx, i64 0, i64 0
  store i32 0, i32* %arrayidx1, align 16
  %arrayidx2 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat01, i64 0, i64 0
  %arrayidx3 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx2, i64 0, i64 1
  store i32 1, i32* %arrayidx3, align 4
  %arrayidx4 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat01, i64 0, i64 1
  %arrayidx5 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx4, i64 0, i64 1
  store i32 1, i32* %arrayidx5, align 4
  %arrayidx6 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat01, i64 0, i64 1
  %arrayidx7 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx6, i64 0, i64 0
  store i32 0, i32* %arrayidx7, align 8
  %arrayidx8 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat00, i64 0, i64 0
  %arrayidx9 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx8, i64 0, i64 0
  store i32 0, i32* %arrayidx9, align 16
  %arrayidx10 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat00, i64 0, i64 0
  %arrayidx11 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx10, i64 0, i64 1
  store i32 0, i32* %arrayidx11, align 4
  %arrayidx12 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat00, i64 0, i64 1
  %arrayidx13 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx12, i64 0, i64 1
  store i32 0, i32* %arrayidx13, align 4
  %arrayidx14 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat00, i64 0, i64 1
  %arrayidx15 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx14, i64 0, i64 0
  store i32 0, i32* %arrayidx15, align 8
  %arrayidx16 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat11, i64 0, i64 0
  %arrayidx17 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx16, i64 0, i64 0
  store i32 0, i32* %arrayidx17, align 16
  %arrayidx18 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat11, i64 0, i64 0
  %arrayidx19 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx18, i64 0, i64 1
  store i32 1, i32* %arrayidx19, align 4
  %arrayidx20 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat11, i64 0, i64 1
  %arrayidx21 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx20, i64 0, i64 1
  store i32 1, i32* %arrayidx21, align 4
  %arrayidx22 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat11, i64 0, i64 1
  %arrayidx23 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx22, i64 0, i64 0
  store i32 0, i32* %arrayidx23, align 8
  %arrayidx24 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat10, i64 0, i64 0
  %arrayidx25 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx24, i64 0, i64 0
  store i32 0, i32* %arrayidx25, align 16
  %arrayidx26 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat10, i64 0, i64 0
  %arrayidx27 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx26, i64 0, i64 1
  store i32 1, i32* %arrayidx27, align 4
  %arrayidx28 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat10, i64 0, i64 1
  %arrayidx29 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx28, i64 0, i64 0
  store i32 0, i32* %arrayidx29, align 8
  %arrayidx30 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat10, i64 0, i64 1
  %arrayidx31 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx30, i64 0, i64 1
  store i32 0, i32* %arrayidx31, align 4
  %arrayinit.begin = getelementptr inbounds [2 x [2 x i32**]], [2 x [2 x i32**]]* %mat, i64 0, i64 0
  %arrayinit.begin32 = getelementptr inbounds [2 x i32**], [2 x i32**]* %arrayinit.begin, i64 0, i64 0
  %arraydecay = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat00, i32 0, i32 0
  %0 = bitcast [2 x i32]* %arraydecay to i32**
  store i32** %0, i32*** %arrayinit.begin32, align 8
  %arrayinit.element = getelementptr inbounds i32**, i32*** %arrayinit.begin32, i64 1
  %arraydecay33 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat01, i32 0, i32 0
  %1 = bitcast [2 x i32]* %arraydecay33 to i32**
  store i32** %1, i32*** %arrayinit.element, align 8
  %arrayinit.element34 = getelementptr inbounds [2 x i32**], [2 x i32**]* %arrayinit.begin, i64 1
  %arrayinit.begin35 = getelementptr inbounds [2 x i32**], [2 x i32**]* %arrayinit.element34, i64 0, i64 0
  %arraydecay36 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat10, i32 0, i32 0
  %2 = bitcast [2 x i32]* %arraydecay36 to i32**
  store i32** %2, i32*** %arrayinit.begin35, align 8
  %arrayinit.element37 = getelementptr inbounds i32**, i32*** %arrayinit.begin35, i64 1
  %arraydecay38 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat11, i32 0, i32 0
  %3 = bitcast [2 x i32]* %arraydecay38 to i32**
  store i32** %3, i32*** %arrayinit.element37, align 8
  %4 = bitcast [1 x [2 x i32]]* %initMat to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* bitcast ([1 x [2 x i32]]* @main.initMat to i8*), i64 8, i32 4, i1 false)
  %5 = load i32, i32* %argc.addr, align 4
  %and = and i32 %5, 1
  store i32 %and, i32* %inp, align 4
  %arraydecay39 = getelementptr inbounds [1 x [2 x i32]], [1 x [2 x i32]]* %initMat, i32 0, i32 0
  %6 = bitcast [2 x i32]* %arraydecay39 to i32**
  %7 = load i32, i32* %inp, align 4
  %idxprom = sext i32 %7 to i64
  %arrayidx40 = getelementptr inbounds [2 x [2 x i32**]], [2 x [2 x i32**]]* %mat, i64 0, i64 %idxprom
  %arrayidx41 = getelementptr inbounds [2 x i32**], [2 x i32**]* %arrayidx40, i64 0, i64 0
  %8 = load i32**, i32*** %arrayidx41, align 16
  %call = call i32** @MM(i32** %6, i32** %8, i32 1, i32 2, i32 2, i32 2)
  store i32** %call, i32*** %interMat, align 8
  store i32 1, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %9 = load i32, i32* %i, align 4
  %10 = load i32, i32* %len, align 4
  %cmp = icmp slt i32 %9, %10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %11 = load i32, i32* %argc.addr, align 4
  %12 = load i32, i32* %i, align 4
  %shl = shl i32 1, %12
  %and42 = and i32 %11, %shl
  %13 = load i32, i32* %i, align 4
  %shr = ashr i32 %and42, %13
  store i32 %shr, i32* %inp, align 4
  %14 = load i32**, i32*** %interMat, align 8
  %15 = load i32, i32* %i, align 4
  %idxprom43 = sext i32 %15 to i64
  %16 = load i32, i32* %inp, align 4
  %idxprom44 = sext i32 %16 to i64
  %arrayidx45 = getelementptr inbounds [2 x [2 x i32**]], [2 x [2 x i32**]]* %mat, i64 0, i64 %idxprom44
  %arrayidx46 = getelementptr inbounds [2 x i32**], [2 x i32**]* %arrayidx45, i64 0, i64 %idxprom43
  %17 = load i32**, i32*** %arrayidx46, align 8
  %call47 = call i32** @MM(i32** %14, i32** %17, i32 1, i32 2, i32 2, i32 2)
  store i32** %call47, i32*** %interMat, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %18 = load i32, i32* %i, align 4
  %inc = add nsw i32 %18, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %19 = load i32, i32* %retval, align 4
  ret i32 %19
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #3

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.0 (tags/RELEASE_390/final)"}

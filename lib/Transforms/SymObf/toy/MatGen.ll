; ModuleID = 'MatGen.cpp'
source_filename = "MatGen.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [25 x i8] c"====Step: %d===========\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d,\00", align 1
@.str.2 = private unnamed_addr constant [5 x i8] c" || \00", align 1
@.str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@.str.4 = private unnamed_addr constant [7 x i8] c"dim : \00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.6 = private unnamed_addr constant [9 x i8] c"mat %d \0A\00", align 1
@.str.7 = private unnamed_addr constant [33 x i8] c"===========Identity Mat========\0A\00", align 1

; Function Attrs: nounwind uwtable
define i64 @_Z13MultIntMatrixPPlS0_S0_lllll(i64** %mat1, i64** %mat2, i64** %retMat, i64 %m1Height, i64 %m1Width, i64 %m2Height, i64 %m2Width, i64 %mod) #0 {
entry:
  %retval = alloca i64, align 8
  %mat1.addr = alloca i64**, align 8
  %mat2.addr = alloca i64**, align 8
  %retMat.addr = alloca i64**, align 8
  %m1Height.addr = alloca i64, align 8
  %m1Width.addr = alloca i64, align 8
  %m2Height.addr = alloca i64, align 8
  %m2Width.addr = alloca i64, align 8
  %mod.addr = alloca i64, align 8
  %i = alloca i64, align 8
  %j = alloca i64, align 8
  %ele = alloca i64, align 8
  %k = alloca i64, align 8
  store i64** %mat1, i64*** %mat1.addr, align 8
  store i64** %mat2, i64*** %mat2.addr, align 8
  store i64** %retMat, i64*** %retMat.addr, align 8
  store i64 %m1Height, i64* %m1Height.addr, align 8
  store i64 %m1Width, i64* %m1Width.addr, align 8
  store i64 %m2Height, i64* %m2Height.addr, align 8
  store i64 %m2Width, i64* %m2Width.addr, align 8
  store i64 %mod, i64* %mod.addr, align 8
  %0 = load i64, i64* %m1Width.addr, align 8
  %1 = load i64, i64* %m2Height.addr, align 8
  %cmp = icmp ne i64 %0, %1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i64 -1, i64* %retval, align 8
  br label %return

if.end:                                           ; preds = %entry
  store i64 0, i64* %i, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc16, %if.end
  %2 = load i64, i64* %i, align 8
  %3 = load i64, i64* %m1Height.addr, align 8
  %cmp1 = icmp slt i64 %2, %3
  br i1 %cmp1, label %for.body, label %for.end18

for.body:                                         ; preds = %for.cond
  store i64 0, i64* %j, align 8
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc13, %for.body
  %4 = load i64, i64* %j, align 8
  %5 = load i64, i64* %m2Width.addr, align 8
  %cmp3 = icmp slt i64 %4, %5
  br i1 %cmp3, label %for.body4, label %for.end15

for.body4:                                        ; preds = %for.cond2
  store i64 0, i64* %ele, align 8
  store i64 0, i64* %k, align 8
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc, %for.body4
  %6 = load i64, i64* %k, align 8
  %7 = load i64, i64* %m1Width.addr, align 8
  %cmp6 = icmp slt i64 %6, %7
  br i1 %cmp6, label %for.body7, label %for.end

for.body7:                                        ; preds = %for.cond5
  %8 = load i64, i64* %ele, align 8
  %9 = load i64, i64* %k, align 8
  %10 = load i64, i64* %i, align 8
  %11 = load i64**, i64*** %mat1.addr, align 8
  %arrayidx = getelementptr inbounds i64*, i64** %11, i64 %10
  %12 = load i64*, i64** %arrayidx, align 8
  %arrayidx8 = getelementptr inbounds i64, i64* %12, i64 %9
  %13 = load i64, i64* %arrayidx8, align 8
  %14 = load i64, i64* %j, align 8
  %15 = load i64, i64* %k, align 8
  %16 = load i64**, i64*** %mat2.addr, align 8
  %arrayidx9 = getelementptr inbounds i64*, i64** %16, i64 %15
  %17 = load i64*, i64** %arrayidx9, align 8
  %arrayidx10 = getelementptr inbounds i64, i64* %17, i64 %14
  %18 = load i64, i64* %arrayidx10, align 8
  %mul = mul nsw i64 %13, %18
  %add = add nsw i64 %8, %mul
  %19 = load i64, i64* %mod.addr, align 8
  %rem = srem i64 %add, %19
  store i64 %rem, i64* %ele, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body7
  %20 = load i64, i64* %k, align 8
  %inc = add nsw i64 %20, 1
  store i64 %inc, i64* %k, align 8
  br label %for.cond5

for.end:                                          ; preds = %for.cond5
  %21 = load i64, i64* %ele, align 8
  %22 = load i64, i64* %j, align 8
  %23 = load i64, i64* %i, align 8
  %24 = load i64**, i64*** %retMat.addr, align 8
  %arrayidx11 = getelementptr inbounds i64*, i64** %24, i64 %23
  %25 = load i64*, i64** %arrayidx11, align 8
  %arrayidx12 = getelementptr inbounds i64, i64* %25, i64 %22
  store i64 %21, i64* %arrayidx12, align 8
  br label %for.inc13

for.inc13:                                        ; preds = %for.end
  %26 = load i64, i64* %j, align 8
  %inc14 = add nsw i64 %26, 1
  store i64 %inc14, i64* %j, align 8
  br label %for.cond2

for.end15:                                        ; preds = %for.cond2
  br label %for.inc16

for.inc16:                                        ; preds = %for.end15
  %27 = load i64, i64* %i, align 8
  %inc17 = add nsw i64 %27, 1
  store i64 %inc17, i64* %i, align 8
  br label %for.cond

for.end18:                                        ; preds = %for.cond
  store i64 0, i64* %retval, align 8
  br label %return

return:                                           ; preds = %for.end18, %if.then
  %28 = load i64, i64* %retval, align 8
  ret i64 %28
}

; Function Attrs: uwtable
define i64 @_Z9PrintStepPPlS0_ii(i64** %mat, i64** %matInv, i32 %dim, i32 %step) #1 {
entry:
  %mat.addr = alloca i64**, align 8
  %matInv.addr = alloca i64**, align 8
  %dim.addr = alloca i32, align 4
  %step.addr = alloca i32, align 4
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  %n8 = alloca i32, align 4
  store i64** %mat, i64*** %mat.addr, align 8
  store i64** %matInv, i64*** %matInv.addr, align 8
  store i32 %dim, i32* %dim.addr, align 4
  store i32 %step, i32* %step.addr, align 4
  %0 = load i32, i32* %step.addr, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i32 0, i32 0), i32 %0)
  store i32 0, i32* %m, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc21, %entry
  %1 = load i32, i32* %m, align 4
  %2 = load i32, i32* %dim.addr, align 4
  %cmp = icmp slt i32 %1, %2
  br i1 %cmp, label %for.body, label %for.end23

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %n, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %3 = load i32, i32* %n, align 4
  %4 = load i32, i32* %dim.addr, align 4
  %cmp2 = icmp slt i32 %3, %4
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %5 = load i32, i32* %n, align 4
  %idxprom = sext i32 %5 to i64
  %6 = load i32, i32* %m, align 4
  %idxprom4 = sext i32 %6 to i64
  %7 = load i64**, i64*** %mat.addr, align 8
  %arrayidx = getelementptr inbounds i64*, i64** %7, i64 %idxprom4
  %8 = load i64*, i64** %arrayidx, align 8
  %arrayidx5 = getelementptr inbounds i64, i64* %8, i64 %idxprom
  %9 = load i64, i64* %arrayidx5, align 8
  %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i64 %9)
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %10 = load i32, i32* %n, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, i32* %n, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  %call7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.2, i32 0, i32 0))
  store i32 0, i32* %n8, align 4
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc17, %for.end
  %11 = load i32, i32* %n8, align 4
  %12 = load i32, i32* %dim.addr, align 4
  %cmp10 = icmp slt i32 %11, %12
  br i1 %cmp10, label %for.body11, label %for.end19

for.body11:                                       ; preds = %for.cond9
  %13 = load i32, i32* %n8, align 4
  %idxprom12 = sext i32 %13 to i64
  %14 = load i32, i32* %m, align 4
  %idxprom13 = sext i32 %14 to i64
  %15 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx14 = getelementptr inbounds i64*, i64** %15, i64 %idxprom13
  %16 = load i64*, i64** %arrayidx14, align 8
  %arrayidx15 = getelementptr inbounds i64, i64* %16, i64 %idxprom12
  %17 = load i64, i64* %arrayidx15, align 8
  %call16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i64 %17)
  br label %for.inc17

for.inc17:                                        ; preds = %for.body11
  %18 = load i32, i32* %n8, align 4
  %inc18 = add nsw i32 %18, 1
  store i32 %inc18, i32* %n8, align 4
  br label %for.cond9

for.end19:                                        ; preds = %for.cond9
  %call20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.3, i32 0, i32 0))
  br label %for.inc21

for.inc21:                                        ; preds = %for.end19
  %19 = load i32, i32* %m, align 4
  %inc22 = add nsw i32 %19, 1
  store i32 %inc22, i32* %m, align 4
  br label %for.cond

for.end23:                                        ; preds = %for.cond
  ret i64 0
}

declare i32 @printf(i8*, ...) #2

; Function Attrs: uwtable
define i32 @_Z13GenIntMatPairPPlS0_S0_ii(i64** %mat, i64** %matInv, i64** %iMat, i32 %dim, i32 %mod) #1 {
entry:
  %mat.addr = alloca i64**, align 8
  %matInv.addr = alloca i64**, align 8
  %iMat.addr = alloca i64**, align 8
  %dim.addr = alloca i32, align 4
  %mod.addr = alloca i32, align 4
  %saved_stack = alloca i8*, align 8
  %i = alloca i32, align 4
  %i12 = alloca i32, align 4
  %j = alloca i32, align 4
  %j78 = alloca i32, align 4
  %i118 = alloca i32, align 4
  %j123 = alloca i32, align 4
  %i210 = alloca i32, align 4
  %j215 = alloca i32, align 4
  %tmp = alloca i32, align 4
  %k = alloca i32, align 4
  store i64** %mat, i64*** %mat.addr, align 8
  store i64** %matInv, i64*** %matInv.addr, align 8
  store i64** %iMat, i64*** %iMat.addr, align 8
  store i32 %dim, i32* %dim.addr, align 4
  store i32 %mod, i32* %mod.addr, align 4
  %call = call i64 @time(i64* null) #4
  %conv = trunc i64 %call to i32
  call void @srand(i32 %conv) #4
  %0 = load i32, i32* %dim.addr, align 4
  %sub = sub nsw i32 %0, 1
  %1 = zext i32 %sub to i64
  %2 = call i8* @llvm.stacksave()
  store i8* %2, i8** %saved_stack, align 8
  %vla = alloca i64, i64 %1, align 16
  %3 = load i32, i32* %dim.addr, align 4
  %sub1 = sub nsw i32 %3, 1
  %4 = zext i32 %sub1 to i64
  %vla2 = alloca i64, i64 %4, align 16
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %5 = load i32, i32* %i, align 4
  %6 = load i32, i32* %dim.addr, align 4
  %sub3 = sub nsw i32 %6, 1
  %cmp = icmp slt i32 %5, %sub3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call4 = call i32 @rand() #4
  %rem = srem i32 %call4, 10
  %add = add nsw i32 %rem, 1
  %conv5 = sext i32 %add to i64
  %7 = load i32, i32* %i, align 4
  %idxprom = sext i32 %7 to i64
  %arrayidx = getelementptr inbounds i64, i64* %vla, i64 %idxprom
  store i64 %conv5, i64* %arrayidx, align 8
  %call6 = call i32 @rand() #4
  %8 = load i32, i32* %i, align 4
  %add7 = add nsw i32 %8, 1
  %rem8 = srem i32 %call6, %add7
  %conv9 = sext i32 %rem8 to i64
  %9 = load i32, i32* %i, align 4
  %idxprom10 = sext i32 %9 to i64
  %arrayidx11 = getelementptr inbounds i64, i64* %vla2, i64 %idxprom10
  store i64 %conv9, i64* %arrayidx11, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %10 = load i32, i32* %i, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 0, i32* %i12, align 4
  br label %for.cond13

for.cond13:                                       ; preds = %for.inc114, %for.end
  %11 = load i32, i32* %i12, align 4
  %12 = load i32, i32* %dim.addr, align 4
  %cmp14 = icmp slt i32 %11, %12
  br i1 %cmp14, label %for.body15, label %for.end116

for.body15:                                       ; preds = %for.cond13
  store i32 0, i32* %j, align 4
  br label %for.cond16

for.cond16:                                       ; preds = %for.inc73, %for.body15
  %13 = load i32, i32* %j, align 4
  %14 = load i32, i32* %dim.addr, align 4
  %cmp17 = icmp slt i32 %13, %14
  br i1 %cmp17, label %for.body18, label %for.end75

for.body18:                                       ; preds = %for.cond16
  %15 = load i32, i32* %j, align 4
  %idxprom19 = sext i32 %15 to i64
  %16 = load i32, i32* %i12, align 4
  %idxprom20 = sext i32 %16 to i64
  %17 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx21 = getelementptr inbounds i64*, i64** %17, i64 %idxprom20
  %18 = load i64*, i64** %arrayidx21, align 8
  %arrayidx22 = getelementptr inbounds i64, i64* %18, i64 %idxprom19
  store i64 0, i64* %arrayidx22, align 8
  %19 = load i32, i32* %i12, align 4
  %20 = load i32, i32* %j, align 4
  %cmp23 = icmp eq i32 %19, %20
  br i1 %cmp23, label %if.then, label %if.end

if.then:                                          ; preds = %for.body18
  %21 = load i32, i32* %j, align 4
  %idxprom24 = sext i32 %21 to i64
  %22 = load i32, i32* %i12, align 4
  %idxprom25 = sext i32 %22 to i64
  %23 = load i64**, i64*** %mat.addr, align 8
  %arrayidx26 = getelementptr inbounds i64*, i64** %23, i64 %idxprom25
  %24 = load i64*, i64** %arrayidx26, align 8
  %arrayidx27 = getelementptr inbounds i64, i64* %24, i64 %idxprom24
  store i64 1, i64* %arrayidx27, align 8
  %25 = load i32, i32* %j, align 4
  %idxprom28 = sext i32 %25 to i64
  %26 = load i32, i32* %i12, align 4
  %idxprom29 = sext i32 %26 to i64
  %27 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx30 = getelementptr inbounds i64*, i64** %27, i64 %idxprom29
  %28 = load i64*, i64** %arrayidx30, align 8
  %arrayidx31 = getelementptr inbounds i64, i64* %28, i64 %idxprom28
  store i64 1, i64* %arrayidx31, align 8
  %29 = load i32, i32* %j, align 4
  %idxprom32 = sext i32 %29 to i64
  %30 = load i32, i32* %i12, align 4
  %idxprom33 = sext i32 %30 to i64
  %31 = load i64**, i64*** %mat.addr, align 8
  %arrayidx34 = getelementptr inbounds i64*, i64** %31, i64 %idxprom33
  %32 = load i64*, i64** %arrayidx34, align 8
  %arrayidx35 = getelementptr inbounds i64, i64* %32, i64 %idxprom32
  %33 = load i64, i64* %arrayidx35, align 8
  %34 = load i32, i32* %j, align 4
  %idxprom36 = sext i32 %34 to i64
  %35 = load i32, i32* %i12, align 4
  %idxprom37 = sext i32 %35 to i64
  %36 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx38 = getelementptr inbounds i64*, i64** %36, i64 %idxprom37
  %37 = load i64*, i64** %arrayidx38, align 8
  %arrayidx39 = getelementptr inbounds i64, i64* %37, i64 %idxprom36
  store i64 %33, i64* %arrayidx39, align 8
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body18
  %38 = load i32, i32* %i12, align 4
  %39 = load i32, i32* %j, align 4
  %cmp40 = icmp slt i32 %38, %39
  br i1 %cmp40, label %if.then41, label %if.end57

if.then41:                                        ; preds = %if.end
  %call42 = call i32 @rand() #4
  %40 = load i32, i32* %mod.addr, align 4
  %rem43 = srem i32 %call42, %40
  %conv44 = sext i32 %rem43 to i64
  %41 = load i32, i32* %j, align 4
  %idxprom45 = sext i32 %41 to i64
  %42 = load i32, i32* %i12, align 4
  %idxprom46 = sext i32 %42 to i64
  %43 = load i64**, i64*** %mat.addr, align 8
  %arrayidx47 = getelementptr inbounds i64*, i64** %43, i64 %idxprom46
  %44 = load i64*, i64** %arrayidx47, align 8
  %arrayidx48 = getelementptr inbounds i64, i64* %44, i64 %idxprom45
  store i64 %conv44, i64* %arrayidx48, align 8
  %45 = load i32, i32* %j, align 4
  %idxprom49 = sext i32 %45 to i64
  %46 = load i32, i32* %i12, align 4
  %idxprom50 = sext i32 %46 to i64
  %47 = load i64**, i64*** %mat.addr, align 8
  %arrayidx51 = getelementptr inbounds i64*, i64** %47, i64 %idxprom50
  %48 = load i64*, i64** %arrayidx51, align 8
  %arrayidx52 = getelementptr inbounds i64, i64* %48, i64 %idxprom49
  %49 = load i64, i64* %arrayidx52, align 8
  %50 = load i32, i32* %j, align 4
  %idxprom53 = sext i32 %50 to i64
  %51 = load i32, i32* %i12, align 4
  %idxprom54 = sext i32 %51 to i64
  %52 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx55 = getelementptr inbounds i64*, i64** %52, i64 %idxprom54
  %53 = load i64*, i64** %arrayidx55, align 8
  %arrayidx56 = getelementptr inbounds i64, i64* %53, i64 %idxprom53
  store i64 %49, i64* %arrayidx56, align 8
  br label %if.end57

if.end57:                                         ; preds = %if.then41, %if.end
  %54 = load i32, i32* %i12, align 4
  %55 = load i32, i32* %j, align 4
  %cmp58 = icmp sgt i32 %54, %55
  br i1 %cmp58, label %if.then59, label %if.end72

if.then59:                                        ; preds = %if.end57
  %56 = load i32, i32* %j, align 4
  %idxprom60 = sext i32 %56 to i64
  %57 = load i32, i32* %i12, align 4
  %idxprom61 = sext i32 %57 to i64
  %58 = load i64**, i64*** %mat.addr, align 8
  %arrayidx62 = getelementptr inbounds i64*, i64** %58, i64 %idxprom61
  %59 = load i64*, i64** %arrayidx62, align 8
  %arrayidx63 = getelementptr inbounds i64, i64* %59, i64 %idxprom60
  store i64 0, i64* %arrayidx63, align 8
  %60 = load i32, i32* %j, align 4
  %idxprom64 = sext i32 %60 to i64
  %61 = load i32, i32* %i12, align 4
  %idxprom65 = sext i32 %61 to i64
  %62 = load i64**, i64*** %mat.addr, align 8
  %arrayidx66 = getelementptr inbounds i64*, i64** %62, i64 %idxprom65
  %63 = load i64*, i64** %arrayidx66, align 8
  %arrayidx67 = getelementptr inbounds i64, i64* %63, i64 %idxprom64
  %64 = load i64, i64* %arrayidx67, align 8
  %65 = load i32, i32* %j, align 4
  %idxprom68 = sext i32 %65 to i64
  %66 = load i32, i32* %i12, align 4
  %idxprom69 = sext i32 %66 to i64
  %67 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx70 = getelementptr inbounds i64*, i64** %67, i64 %idxprom69
  %68 = load i64*, i64** %arrayidx70, align 8
  %arrayidx71 = getelementptr inbounds i64, i64* %68, i64 %idxprom68
  store i64 %64, i64* %arrayidx71, align 8
  br label %if.end72

if.end72:                                         ; preds = %if.then59, %if.end57
  br label %for.inc73

for.inc73:                                        ; preds = %if.end72
  %69 = load i32, i32* %j, align 4
  %inc74 = add nsw i32 %69, 1
  store i32 %inc74, i32* %j, align 4
  br label %for.cond16

for.end75:                                        ; preds = %for.cond16
  %70 = load i32, i32* %i12, align 4
  %cmp76 = icmp sgt i32 %70, 0
  br i1 %cmp76, label %if.then77, label %if.end113

if.then77:                                        ; preds = %for.end75
  store i32 0, i32* %j78, align 4
  br label %for.cond79

for.cond79:                                       ; preds = %for.inc110, %if.then77
  %71 = load i32, i32* %j78, align 4
  %72 = load i32, i32* %dim.addr, align 4
  %cmp80 = icmp slt i32 %71, %72
  br i1 %cmp80, label %for.body81, label %for.end112

for.body81:                                       ; preds = %for.cond79
  %73 = load i32, i32* %j78, align 4
  %idxprom82 = sext i32 %73 to i64
  %74 = load i32, i32* %i12, align 4
  %idxprom83 = sext i32 %74 to i64
  %75 = load i64**, i64*** %mat.addr, align 8
  %arrayidx84 = getelementptr inbounds i64*, i64** %75, i64 %idxprom83
  %76 = load i64*, i64** %arrayidx84, align 8
  %arrayidx85 = getelementptr inbounds i64, i64* %76, i64 %idxprom82
  %77 = load i64, i64* %arrayidx85, align 8
  %78 = load i32, i32* %j78, align 4
  %idxprom86 = sext i32 %78 to i64
  %79 = load i32, i32* %i12, align 4
  %sub87 = sub nsw i32 %79, 1
  %idxprom88 = sext i32 %sub87 to i64
  %arrayidx89 = getelementptr inbounds i64, i64* %vla2, i64 %idxprom88
  %80 = load i64, i64* %arrayidx89, align 8
  %81 = load i64**, i64*** %mat.addr, align 8
  %arrayidx90 = getelementptr inbounds i64*, i64** %81, i64 %80
  %82 = load i64*, i64** %arrayidx90, align 8
  %arrayidx91 = getelementptr inbounds i64, i64* %82, i64 %idxprom86
  %83 = load i64, i64* %arrayidx91, align 8
  %84 = load i32, i32* %i12, align 4
  %sub92 = sub nsw i32 %84, 1
  %idxprom93 = sext i32 %sub92 to i64
  %arrayidx94 = getelementptr inbounds i64, i64* %vla, i64 %idxprom93
  %85 = load i64, i64* %arrayidx94, align 8
  %mul = mul nsw i64 %83, %85
  %add95 = add nsw i64 %77, %mul
  %86 = load i32, i32* %mod.addr, align 4
  %conv96 = sext i32 %86 to i64
  %rem97 = srem i64 %add95, %conv96
  %87 = load i32, i32* %j78, align 4
  %idxprom98 = sext i32 %87 to i64
  %88 = load i32, i32* %i12, align 4
  %idxprom99 = sext i32 %88 to i64
  %89 = load i64**, i64*** %mat.addr, align 8
  %arrayidx100 = getelementptr inbounds i64*, i64** %89, i64 %idxprom99
  %90 = load i64*, i64** %arrayidx100, align 8
  %arrayidx101 = getelementptr inbounds i64, i64* %90, i64 %idxprom98
  store i64 %rem97, i64* %arrayidx101, align 8
  %91 = load i32, i32* %j78, align 4
  %idxprom102 = sext i32 %91 to i64
  %92 = load i32, i32* %i12, align 4
  %idxprom103 = sext i32 %92 to i64
  %93 = load i64**, i64*** %mat.addr, align 8
  %arrayidx104 = getelementptr inbounds i64*, i64** %93, i64 %idxprom103
  %94 = load i64*, i64** %arrayidx104, align 8
  %arrayidx105 = getelementptr inbounds i64, i64* %94, i64 %idxprom102
  %95 = load i64, i64* %arrayidx105, align 8
  %96 = load i32, i32* %j78, align 4
  %idxprom106 = sext i32 %96 to i64
  %97 = load i32, i32* %i12, align 4
  %idxprom107 = sext i32 %97 to i64
  %98 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx108 = getelementptr inbounds i64*, i64** %98, i64 %idxprom107
  %99 = load i64*, i64** %arrayidx108, align 8
  %arrayidx109 = getelementptr inbounds i64, i64* %99, i64 %idxprom106
  store i64 %95, i64* %arrayidx109, align 8
  br label %for.inc110

for.inc110:                                       ; preds = %for.body81
  %100 = load i32, i32* %j78, align 4
  %inc111 = add nsw i32 %100, 1
  store i32 %inc111, i32* %j78, align 4
  br label %for.cond79

for.end112:                                       ; preds = %for.cond79
  br label %if.end113

if.end113:                                        ; preds = %for.end112, %for.end75
  br label %for.inc114

for.inc114:                                       ; preds = %if.end113
  %101 = load i32, i32* %i12, align 4
  %inc115 = add nsw i32 %101, 1
  store i32 %inc115, i32* %i12, align 4
  br label %for.cond13

for.end116:                                       ; preds = %for.cond13
  %102 = load i64**, i64*** %mat.addr, align 8
  %103 = load i64**, i64*** %matInv.addr, align 8
  %104 = load i32, i32* %dim.addr, align 4
  %call117 = call i64 @_Z9PrintStepPPlS0_ii(i64** %102, i64** %103, i32 %104, i32 0)
  %105 = load i32, i32* %dim.addr, align 4
  %sub119 = sub nsw i32 %105, 1
  store i32 %sub119, i32* %i118, align 4
  br label %for.cond120

for.cond120:                                      ; preds = %for.inc208, %for.end116
  %106 = load i32, i32* %i118, align 4
  %cmp121 = icmp sgt i32 %106, 0
  br i1 %cmp121, label %for.body122, label %for.end209

for.body122:                                      ; preds = %for.cond120
  store i32 0, i32* %j123, align 4
  br label %for.cond124

for.cond124:                                      ; preds = %for.inc203, %for.body122
  %107 = load i32, i32* %j123, align 4
  %108 = load i32, i32* %dim.addr, align 4
  %cmp125 = icmp slt i32 %107, %108
  br i1 %cmp125, label %for.body126, label %for.end205

for.body126:                                      ; preds = %for.cond124
  %109 = load i32, i32* %j123, align 4
  %idxprom127 = sext i32 %109 to i64
  %110 = load i32, i32* %i118, align 4
  %idxprom128 = sext i32 %110 to i64
  %111 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx129 = getelementptr inbounds i64*, i64** %111, i64 %idxprom128
  %112 = load i64*, i64** %arrayidx129, align 8
  %arrayidx130 = getelementptr inbounds i64, i64* %112, i64 %idxprom127
  %113 = load i64, i64* %arrayidx130, align 8
  %114 = load i32, i32* %j123, align 4
  %idxprom131 = sext i32 %114 to i64
  %115 = load i32, i32* %i118, align 4
  %sub132 = sub nsw i32 %115, 1
  %idxprom133 = sext i32 %sub132 to i64
  %arrayidx134 = getelementptr inbounds i64, i64* %vla2, i64 %idxprom133
  %116 = load i64, i64* %arrayidx134, align 8
  %117 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx135 = getelementptr inbounds i64*, i64** %117, i64 %116
  %118 = load i64*, i64** %arrayidx135, align 8
  %arrayidx136 = getelementptr inbounds i64, i64* %118, i64 %idxprom131
  %119 = load i64, i64* %arrayidx136, align 8
  %120 = load i32, i32* %i118, align 4
  %sub137 = sub nsw i32 %120, 1
  %idxprom138 = sext i32 %sub137 to i64
  %arrayidx139 = getelementptr inbounds i64, i64* %vla, i64 %idxprom138
  %121 = load i64, i64* %arrayidx139, align 8
  %mul140 = mul nsw i64 %119, %121
  %sub141 = sub nsw i64 %113, %mul140
  %122 = load i32, i32* %mod.addr, align 4
  %conv142 = sext i32 %122 to i64
  %rem143 = srem i64 %sub141, %conv142
  %123 = load i32, i32* %j123, align 4
  %idxprom144 = sext i32 %123 to i64
  %124 = load i32, i32* %i118, align 4
  %idxprom145 = sext i32 %124 to i64
  %125 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx146 = getelementptr inbounds i64*, i64** %125, i64 %idxprom145
  %126 = load i64*, i64** %arrayidx146, align 8
  %arrayidx147 = getelementptr inbounds i64, i64* %126, i64 %idxprom144
  store i64 %rem143, i64* %arrayidx147, align 8
  %127 = load i32, i32* %j123, align 4
  %idxprom148 = sext i32 %127 to i64
  %128 = load i32, i32* %i118, align 4
  %idxprom149 = sext i32 %128 to i64
  %129 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx150 = getelementptr inbounds i64*, i64** %129, i64 %idxprom149
  %130 = load i64*, i64** %arrayidx150, align 8
  %arrayidx151 = getelementptr inbounds i64, i64* %130, i64 %idxprom148
  %131 = load i64, i64* %arrayidx151, align 8
  %132 = load i32, i32* %j123, align 4
  %idxprom152 = sext i32 %132 to i64
  %133 = load i32, i32* %i118, align 4
  %sub153 = sub nsw i32 %133, 1
  %idxprom154 = sext i32 %sub153 to i64
  %arrayidx155 = getelementptr inbounds i64, i64* %vla2, i64 %idxprom154
  %134 = load i64, i64* %arrayidx155, align 8
  %135 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx156 = getelementptr inbounds i64*, i64** %135, i64 %134
  %136 = load i64*, i64** %arrayidx156, align 8
  %arrayidx157 = getelementptr inbounds i64, i64* %136, i64 %idxprom152
  %137 = load i64, i64* %arrayidx157, align 8
  %138 = load i32, i32* %i118, align 4
  %sub158 = sub nsw i32 %138, 1
  %idxprom159 = sext i32 %sub158 to i64
  %arrayidx160 = getelementptr inbounds i64, i64* %vla, i64 %idxprom159
  %139 = load i64, i64* %arrayidx160, align 8
  %mul161 = mul nsw i64 %137, %139
  %sub162 = sub nsw i64 %131, %mul161
  %140 = load i32, i32* %mod.addr, align 4
  %conv163 = sext i32 %140 to i64
  %rem164 = srem i64 %sub162, %conv163
  %141 = load i32, i32* %j123, align 4
  %idxprom165 = sext i32 %141 to i64
  %142 = load i32, i32* %i118, align 4
  %idxprom166 = sext i32 %142 to i64
  %143 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx167 = getelementptr inbounds i64*, i64** %143, i64 %idxprom166
  %144 = load i64*, i64** %arrayidx167, align 8
  %arrayidx168 = getelementptr inbounds i64, i64* %144, i64 %idxprom165
  store i64 %rem164, i64* %arrayidx168, align 8
  %145 = load i32, i32* %j123, align 4
  %idxprom169 = sext i32 %145 to i64
  %146 = load i32, i32* %i118, align 4
  %idxprom170 = sext i32 %146 to i64
  %147 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx171 = getelementptr inbounds i64*, i64** %147, i64 %idxprom170
  %148 = load i64*, i64** %arrayidx171, align 8
  %arrayidx172 = getelementptr inbounds i64, i64* %148, i64 %idxprom169
  %149 = load i64, i64* %arrayidx172, align 8
  %cmp173 = icmp slt i64 %149, 0
  br i1 %cmp173, label %if.then174, label %if.end185

if.then174:                                       ; preds = %for.body126
  %150 = load i32, i32* %j123, align 4
  %idxprom175 = sext i32 %150 to i64
  %151 = load i32, i32* %i118, align 4
  %idxprom176 = sext i32 %151 to i64
  %152 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx177 = getelementptr inbounds i64*, i64** %152, i64 %idxprom176
  %153 = load i64*, i64** %arrayidx177, align 8
  %arrayidx178 = getelementptr inbounds i64, i64* %153, i64 %idxprom175
  %154 = load i64, i64* %arrayidx178, align 8
  %155 = load i32, i32* %mod.addr, align 4
  %conv179 = sext i32 %155 to i64
  %add180 = add nsw i64 %154, %conv179
  %156 = load i32, i32* %j123, align 4
  %idxprom181 = sext i32 %156 to i64
  %157 = load i32, i32* %i118, align 4
  %idxprom182 = sext i32 %157 to i64
  %158 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx183 = getelementptr inbounds i64*, i64** %158, i64 %idxprom182
  %159 = load i64*, i64** %arrayidx183, align 8
  %arrayidx184 = getelementptr inbounds i64, i64* %159, i64 %idxprom181
  store i64 %add180, i64* %arrayidx184, align 8
  br label %if.end185

if.end185:                                        ; preds = %if.then174, %for.body126
  %160 = load i32, i32* %j123, align 4
  %idxprom186 = sext i32 %160 to i64
  %161 = load i32, i32* %i118, align 4
  %idxprom187 = sext i32 %161 to i64
  %162 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx188 = getelementptr inbounds i64*, i64** %162, i64 %idxprom187
  %163 = load i64*, i64** %arrayidx188, align 8
  %arrayidx189 = getelementptr inbounds i64, i64* %163, i64 %idxprom186
  %164 = load i64, i64* %arrayidx189, align 8
  %cmp190 = icmp slt i64 %164, 0
  br i1 %cmp190, label %if.then191, label %if.end202

if.then191:                                       ; preds = %if.end185
  %165 = load i32, i32* %j123, align 4
  %idxprom192 = sext i32 %165 to i64
  %166 = load i32, i32* %i118, align 4
  %idxprom193 = sext i32 %166 to i64
  %167 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx194 = getelementptr inbounds i64*, i64** %167, i64 %idxprom193
  %168 = load i64*, i64** %arrayidx194, align 8
  %arrayidx195 = getelementptr inbounds i64, i64* %168, i64 %idxprom192
  %169 = load i64, i64* %arrayidx195, align 8
  %170 = load i32, i32* %mod.addr, align 4
  %conv196 = sext i32 %170 to i64
  %add197 = add nsw i64 %169, %conv196
  %171 = load i32, i32* %j123, align 4
  %idxprom198 = sext i32 %171 to i64
  %172 = load i32, i32* %i118, align 4
  %idxprom199 = sext i32 %172 to i64
  %173 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx200 = getelementptr inbounds i64*, i64** %173, i64 %idxprom199
  %174 = load i64*, i64** %arrayidx200, align 8
  %arrayidx201 = getelementptr inbounds i64, i64* %174, i64 %idxprom198
  store i64 %add197, i64* %arrayidx201, align 8
  br label %if.end202

if.end202:                                        ; preds = %if.then191, %if.end185
  br label %for.inc203

for.inc203:                                       ; preds = %if.end202
  %175 = load i32, i32* %j123, align 4
  %inc204 = add nsw i32 %175, 1
  store i32 %inc204, i32* %j123, align 4
  br label %for.cond124

for.end205:                                       ; preds = %for.cond124
  %176 = load i64**, i64*** %iMat.addr, align 8
  %177 = load i64**, i64*** %matInv.addr, align 8
  %178 = load i32, i32* %dim.addr, align 4
  %179 = load i32, i32* %dim.addr, align 4
  %180 = load i32, i32* %i118, align 4
  %sub206 = sub nsw i32 %179, %180
  %call207 = call i64 @_Z9PrintStepPPlS0_ii(i64** %176, i64** %177, i32 %178, i32 %sub206)
  br label %for.inc208

for.inc208:                                       ; preds = %for.end205
  %181 = load i32, i32* %i118, align 4
  %dec = add nsw i32 %181, -1
  store i32 %dec, i32* %i118, align 4
  br label %for.cond120

for.end209:                                       ; preds = %for.cond120
  %182 = load i32, i32* %dim.addr, align 4
  %sub211 = sub nsw i32 %182, 1
  store i32 %sub211, i32* %i210, align 4
  br label %for.cond212

for.cond212:                                      ; preds = %for.inc306, %for.end209
  %183 = load i32, i32* %i210, align 4
  %cmp213 = icmp sgt i32 %183, 0
  br i1 %cmp213, label %for.body214, label %for.end308

for.body214:                                      ; preds = %for.cond212
  store i32 0, i32* %j215, align 4
  br label %for.cond216

for.cond216:                                      ; preds = %for.inc299, %for.body214
  %184 = load i32, i32* %j215, align 4
  %185 = load i32, i32* %i210, align 4
  %cmp217 = icmp slt i32 %184, %185
  br i1 %cmp217, label %for.body218, label %for.end301

for.body218:                                      ; preds = %for.cond216
  %186 = load i32, i32* %i210, align 4
  %idxprom219 = sext i32 %186 to i64
  %187 = load i32, i32* %j215, align 4
  %idxprom220 = sext i32 %187 to i64
  %188 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx221 = getelementptr inbounds i64*, i64** %188, i64 %idxprom220
  %189 = load i64*, i64** %arrayidx221, align 8
  %arrayidx222 = getelementptr inbounds i64, i64* %189, i64 %idxprom219
  %190 = load i64, i64* %arrayidx222, align 8
  %conv223 = trunc i64 %190 to i32
  store i32 %conv223, i32* %tmp, align 4
  store i32 0, i32* %k, align 4
  br label %for.cond225

for.cond225:                                      ; preds = %for.inc296, %for.body218
  %191 = load i32, i32* %k, align 4
  %192 = load i32, i32* %dim.addr, align 4
  %cmp226 = icmp slt i32 %191, %192
  br i1 %cmp226, label %for.body227, label %for.end298

for.body227:                                      ; preds = %for.cond225
  %193 = load i32, i32* %k, align 4
  %idxprom228 = sext i32 %193 to i64
  %194 = load i32, i32* %j215, align 4
  %idxprom229 = sext i32 %194 to i64
  %195 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx230 = getelementptr inbounds i64*, i64** %195, i64 %idxprom229
  %196 = load i64*, i64** %arrayidx230, align 8
  %arrayidx231 = getelementptr inbounds i64, i64* %196, i64 %idxprom228
  %197 = load i64, i64* %arrayidx231, align 8
  %198 = load i32, i32* %k, align 4
  %idxprom232 = sext i32 %198 to i64
  %199 = load i32, i32* %i210, align 4
  %idxprom233 = sext i32 %199 to i64
  %200 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx234 = getelementptr inbounds i64*, i64** %200, i64 %idxprom233
  %201 = load i64*, i64** %arrayidx234, align 8
  %arrayidx235 = getelementptr inbounds i64, i64* %201, i64 %idxprom232
  %202 = load i64, i64* %arrayidx235, align 8
  %203 = load i32, i32* %tmp, align 4
  %conv236 = sext i32 %203 to i64
  %mul237 = mul nsw i64 %202, %conv236
  %sub238 = sub nsw i64 %197, %mul237
  %204 = load i32, i32* %mod.addr, align 4
  %conv239 = sext i32 %204 to i64
  %rem240 = srem i64 %sub238, %conv239
  %205 = load i32, i32* %k, align 4
  %idxprom241 = sext i32 %205 to i64
  %206 = load i32, i32* %j215, align 4
  %idxprom242 = sext i32 %206 to i64
  %207 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx243 = getelementptr inbounds i64*, i64** %207, i64 %idxprom242
  %208 = load i64*, i64** %arrayidx243, align 8
  %arrayidx244 = getelementptr inbounds i64, i64* %208, i64 %idxprom241
  store i64 %rem240, i64* %arrayidx244, align 8
  %209 = load i32, i32* %k, align 4
  %idxprom245 = sext i32 %209 to i64
  %210 = load i32, i32* %j215, align 4
  %idxprom246 = sext i32 %210 to i64
  %211 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx247 = getelementptr inbounds i64*, i64** %211, i64 %idxprom246
  %212 = load i64*, i64** %arrayidx247, align 8
  %arrayidx248 = getelementptr inbounds i64, i64* %212, i64 %idxprom245
  %213 = load i64, i64* %arrayidx248, align 8
  %214 = load i32, i32* %k, align 4
  %idxprom249 = sext i32 %214 to i64
  %215 = load i32, i32* %i210, align 4
  %idxprom250 = sext i32 %215 to i64
  %216 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx251 = getelementptr inbounds i64*, i64** %216, i64 %idxprom250
  %217 = load i64*, i64** %arrayidx251, align 8
  %arrayidx252 = getelementptr inbounds i64, i64* %217, i64 %idxprom249
  %218 = load i64, i64* %arrayidx252, align 8
  %219 = load i32, i32* %tmp, align 4
  %conv253 = sext i32 %219 to i64
  %mul254 = mul nsw i64 %218, %conv253
  %sub255 = sub nsw i64 %213, %mul254
  %220 = load i32, i32* %mod.addr, align 4
  %conv256 = sext i32 %220 to i64
  %rem257 = srem i64 %sub255, %conv256
  %221 = load i32, i32* %k, align 4
  %idxprom258 = sext i32 %221 to i64
  %222 = load i32, i32* %j215, align 4
  %idxprom259 = sext i32 %222 to i64
  %223 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx260 = getelementptr inbounds i64*, i64** %223, i64 %idxprom259
  %224 = load i64*, i64** %arrayidx260, align 8
  %arrayidx261 = getelementptr inbounds i64, i64* %224, i64 %idxprom258
  store i64 %rem257, i64* %arrayidx261, align 8
  %225 = load i32, i32* %j215, align 4
  %idxprom262 = sext i32 %225 to i64
  %226 = load i32, i32* %i210, align 4
  %idxprom263 = sext i32 %226 to i64
  %227 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx264 = getelementptr inbounds i64*, i64** %227, i64 %idxprom263
  %228 = load i64*, i64** %arrayidx264, align 8
  %arrayidx265 = getelementptr inbounds i64, i64* %228, i64 %idxprom262
  %229 = load i64, i64* %arrayidx265, align 8
  %cmp266 = icmp slt i64 %229, 0
  br i1 %cmp266, label %if.then267, label %if.end278

if.then267:                                       ; preds = %for.body227
  %230 = load i32, i32* %j215, align 4
  %idxprom268 = sext i32 %230 to i64
  %231 = load i32, i32* %i210, align 4
  %idxprom269 = sext i32 %231 to i64
  %232 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx270 = getelementptr inbounds i64*, i64** %232, i64 %idxprom269
  %233 = load i64*, i64** %arrayidx270, align 8
  %arrayidx271 = getelementptr inbounds i64, i64* %233, i64 %idxprom268
  %234 = load i64, i64* %arrayidx271, align 8
  %235 = load i32, i32* %mod.addr, align 4
  %conv272 = sext i32 %235 to i64
  %add273 = add nsw i64 %234, %conv272
  %236 = load i32, i32* %j215, align 4
  %idxprom274 = sext i32 %236 to i64
  %237 = load i32, i32* %i210, align 4
  %idxprom275 = sext i32 %237 to i64
  %238 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx276 = getelementptr inbounds i64*, i64** %238, i64 %idxprom275
  %239 = load i64*, i64** %arrayidx276, align 8
  %arrayidx277 = getelementptr inbounds i64, i64* %239, i64 %idxprom274
  store i64 %add273, i64* %arrayidx277, align 8
  br label %if.end278

if.end278:                                        ; preds = %if.then267, %for.body227
  %240 = load i32, i32* %k, align 4
  %idxprom279 = sext i32 %240 to i64
  %241 = load i32, i32* %j215, align 4
  %idxprom280 = sext i32 %241 to i64
  %242 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx281 = getelementptr inbounds i64*, i64** %242, i64 %idxprom280
  %243 = load i64*, i64** %arrayidx281, align 8
  %arrayidx282 = getelementptr inbounds i64, i64* %243, i64 %idxprom279
  %244 = load i64, i64* %arrayidx282, align 8
  %cmp283 = icmp slt i64 %244, 0
  br i1 %cmp283, label %if.then284, label %if.end295

if.then284:                                       ; preds = %if.end278
  %245 = load i32, i32* %k, align 4
  %idxprom285 = sext i32 %245 to i64
  %246 = load i32, i32* %j215, align 4
  %idxprom286 = sext i32 %246 to i64
  %247 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx287 = getelementptr inbounds i64*, i64** %247, i64 %idxprom286
  %248 = load i64*, i64** %arrayidx287, align 8
  %arrayidx288 = getelementptr inbounds i64, i64* %248, i64 %idxprom285
  %249 = load i64, i64* %arrayidx288, align 8
  %250 = load i32, i32* %mod.addr, align 4
  %conv289 = sext i32 %250 to i64
  %add290 = add nsw i64 %249, %conv289
  %251 = load i32, i32* %k, align 4
  %idxprom291 = sext i32 %251 to i64
  %252 = load i32, i32* %j215, align 4
  %idxprom292 = sext i32 %252 to i64
  %253 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx293 = getelementptr inbounds i64*, i64** %253, i64 %idxprom292
  %254 = load i64*, i64** %arrayidx293, align 8
  %arrayidx294 = getelementptr inbounds i64, i64* %254, i64 %idxprom291
  store i64 %add290, i64* %arrayidx294, align 8
  br label %if.end295

if.end295:                                        ; preds = %if.then284, %if.end278
  br label %for.inc296

for.inc296:                                       ; preds = %if.end295
  %255 = load i32, i32* %k, align 4
  %inc297 = add nsw i32 %255, 1
  store i32 %inc297, i32* %k, align 4
  br label %for.cond225

for.end298:                                       ; preds = %for.cond225
  br label %for.inc299

for.inc299:                                       ; preds = %for.end298
  %256 = load i32, i32* %j215, align 4
  %inc300 = add nsw i32 %256, 1
  store i32 %inc300, i32* %j215, align 4
  br label %for.cond216

for.end301:                                       ; preds = %for.cond216
  %257 = load i64**, i64*** %iMat.addr, align 8
  %258 = load i64**, i64*** %matInv.addr, align 8
  %259 = load i32, i32* %dim.addr, align 4
  %260 = load i32, i32* %dim.addr, align 4
  %mul302 = mul nsw i32 2, %260
  %261 = load i32, i32* %i210, align 4
  %sub303 = sub nsw i32 %mul302, %261
  %sub304 = sub nsw i32 %sub303, 1
  %call305 = call i64 @_Z9PrintStepPPlS0_ii(i64** %257, i64** %258, i32 %259, i32 %sub304)
  br label %for.inc306

for.inc306:                                       ; preds = %for.end301
  %262 = load i32, i32* %i210, align 4
  %dec307 = add nsw i32 %262, -1
  store i32 %dec307, i32* %i210, align 4
  br label %for.cond212

for.end308:                                       ; preds = %for.cond212
  %263 = load i8*, i8** %saved_stack, align 8
  call void @llvm.stackrestore(i8* %263)
  ret i32 0
}

; Function Attrs: nounwind
declare void @srand(i32) #3

; Function Attrs: nounwind
declare i64 @time(i64*) #3

; Function Attrs: nounwind
declare i8* @llvm.stacksave() #4

; Function Attrs: nounwind
declare i32 @rand() #3

; Function Attrs: nounwind
declare void @llvm.stackrestore(i8*) #4

; Function Attrs: norecurse uwtable
define i32 @main(i32 %argc, i8** %argv) #5 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %dim = alloca i32, align 4
  %mat = alloca i64**, align 8
  %matInv = alloca i64**, align 8
  %iMat = alloca i64**, align 8
  %i = alloca i32, align 4
  %mod = alloca i64, align 8
  %i31 = alloca i32, align 4
  %j = alloca i32, align 4
  %i50 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 0, i32* %dim, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.4, i32 0, i32 0))
  %call1 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0), i32* %dim)
  %0 = load i32, i32* %dim, align 4
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.6, i32 0, i32 0), i32 %0)
  %1 = load i32, i32* %dim, align 4
  %conv = sext i32 %1 to i64
  %mul = mul i64 8, %conv
  %call3 = call noalias i8* @malloc(i64 %mul) #4
  %2 = bitcast i8* %call3 to i64**
  store i64** %2, i64*** %mat, align 8
  %3 = load i32, i32* %dim, align 4
  %conv4 = sext i32 %3 to i64
  %mul5 = mul i64 8, %conv4
  %call6 = call noalias i8* @malloc(i64 %mul5) #4
  %4 = bitcast i8* %call6 to i64**
  store i64** %4, i64*** %matInv, align 8
  %5 = load i32, i32* %dim, align 4
  %conv7 = sext i32 %5 to i64
  %mul8 = mul i64 8, %conv7
  %call9 = call noalias i8* @malloc(i64 %mul8) #4
  %6 = bitcast i8* %call9 to i64**
  store i64** %6, i64*** %iMat, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %7 = load i32, i32* %i, align 4
  %8 = load i32, i32* %dim, align 4
  %cmp = icmp slt i32 %7, %8
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %9 = load i32, i32* %dim, align 4
  %conv10 = sext i32 %9 to i64
  %mul11 = mul i64 8, %conv10
  %call12 = call noalias i8* @malloc(i64 %mul11) #4
  %10 = bitcast i8* %call12 to i64*
  %11 = load i32, i32* %i, align 4
  %idxprom = sext i32 %11 to i64
  %12 = load i64**, i64*** %mat, align 8
  %arrayidx = getelementptr inbounds i64*, i64** %12, i64 %idxprom
  store i64* %10, i64** %arrayidx, align 8
  %13 = load i32, i32* %dim, align 4
  %conv13 = sext i32 %13 to i64
  %mul14 = mul i64 8, %conv13
  %call15 = call noalias i8* @malloc(i64 %mul14) #4
  %14 = bitcast i8* %call15 to i64*
  %15 = load i32, i32* %i, align 4
  %idxprom16 = sext i32 %15 to i64
  %16 = load i64**, i64*** %matInv, align 8
  %arrayidx17 = getelementptr inbounds i64*, i64** %16, i64 %idxprom16
  store i64* %14, i64** %arrayidx17, align 8
  %17 = load i32, i32* %dim, align 4
  %conv18 = sext i32 %17 to i64
  %mul19 = mul i64 8, %conv18
  %call20 = call noalias i8* @malloc(i64 %mul19) #4
  %18 = bitcast i8* %call20 to i64*
  %19 = load i32, i32* %i, align 4
  %idxprom21 = sext i32 %19 to i64
  %20 = load i64**, i64*** %iMat, align 8
  %arrayidx22 = getelementptr inbounds i64*, i64** %20, i64 %idxprom21
  store i64* %18, i64** %arrayidx22, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %21 = load i32, i32* %i, align 4
  %inc = add nsw i32 %21, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i64 1024, i64* %mod, align 8
  %22 = load i64**, i64*** %mat, align 8
  %23 = load i64**, i64*** %matInv, align 8
  %24 = load i64**, i64*** %iMat, align 8
  %25 = load i32, i32* %dim, align 4
  %26 = load i64, i64* %mod, align 8
  %conv23 = trunc i64 %26 to i32
  %call24 = call i32 @_Z13GenIntMatPairPPlS0_S0_ii(i64** %22, i64** %23, i64** %24, i32 %25, i32 %conv23)
  %27 = load i64**, i64*** %mat, align 8
  %28 = load i64**, i64*** %matInv, align 8
  %29 = load i64**, i64*** %iMat, align 8
  %30 = load i32, i32* %dim, align 4
  %conv25 = sext i32 %30 to i64
  %31 = load i32, i32* %dim, align 4
  %conv26 = sext i32 %31 to i64
  %32 = load i32, i32* %dim, align 4
  %conv27 = sext i32 %32 to i64
  %33 = load i32, i32* %dim, align 4
  %conv28 = sext i32 %33 to i64
  %34 = load i64, i64* %mod, align 8
  %call29 = call i64 @_Z13MultIntMatrixPPlS0_S0_lllll(i64** %27, i64** %28, i64** %29, i64 %conv25, i64 %conv26, i64 %conv27, i64 %conv28, i64 %34)
  %call30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7, i32 0, i32 0))
  store i32 0, i32* %i31, align 4
  br label %for.cond32

for.cond32:                                       ; preds = %for.inc47, %for.end
  %35 = load i32, i32* %i31, align 4
  %36 = load i32, i32* %dim, align 4
  %cmp33 = icmp slt i32 %35, %36
  br i1 %cmp33, label %for.body34, label %for.end49

for.body34:                                       ; preds = %for.cond32
  store i32 0, i32* %j, align 4
  br label %for.cond35

for.cond35:                                       ; preds = %for.inc43, %for.body34
  %37 = load i32, i32* %j, align 4
  %38 = load i32, i32* %dim, align 4
  %cmp36 = icmp slt i32 %37, %38
  br i1 %cmp36, label %for.body37, label %for.end45

for.body37:                                       ; preds = %for.cond35
  %39 = load i32, i32* %j, align 4
  %idxprom38 = sext i32 %39 to i64
  %40 = load i32, i32* %i31, align 4
  %idxprom39 = sext i32 %40 to i64
  %41 = load i64**, i64*** %iMat, align 8
  %arrayidx40 = getelementptr inbounds i64*, i64** %41, i64 %idxprom39
  %42 = load i64*, i64** %arrayidx40, align 8
  %arrayidx41 = getelementptr inbounds i64, i64* %42, i64 %idxprom38
  %43 = load i64, i64* %arrayidx41, align 8
  %call42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i64 %43)
  br label %for.inc43

for.inc43:                                        ; preds = %for.body37
  %44 = load i32, i32* %j, align 4
  %inc44 = add nsw i32 %44, 1
  store i32 %inc44, i32* %j, align 4
  br label %for.cond35

for.end45:                                        ; preds = %for.cond35
  %call46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.3, i32 0, i32 0))
  br label %for.inc47

for.inc47:                                        ; preds = %for.end45
  %45 = load i32, i32* %i31, align 4
  %inc48 = add nsw i32 %45, 1
  store i32 %inc48, i32* %i31, align 4
  br label %for.cond32

for.end49:                                        ; preds = %for.cond32
  store i32 0, i32* %i50, align 4
  br label %for.cond51

for.cond51:                                       ; preds = %for.inc58, %for.end49
  %46 = load i32, i32* %i50, align 4
  %47 = load i32, i32* %dim, align 4
  %cmp52 = icmp slt i32 %46, %47
  br i1 %cmp52, label %for.body53, label %for.end60

for.body53:                                       ; preds = %for.cond51
  %48 = load i32, i32* %i50, align 4
  %idxprom54 = sext i32 %48 to i64
  %49 = load i64**, i64*** %matInv, align 8
  %arrayidx55 = getelementptr inbounds i64*, i64** %49, i64 %idxprom54
  %50 = load i64*, i64** %arrayidx55, align 8
  %51 = bitcast i64* %50 to i8*
  call void @free(i8* %51) #4
  %52 = load i32, i32* %i50, align 4
  %idxprom56 = sext i32 %52 to i64
  %53 = load i64**, i64*** %mat, align 8
  %arrayidx57 = getelementptr inbounds i64*, i64** %53, i64 %idxprom56
  %54 = load i64*, i64** %arrayidx57, align 8
  %55 = bitcast i64* %54 to i8*
  call void @free(i8* %55) #4
  br label %for.inc58

for.inc58:                                        ; preds = %for.body53
  %56 = load i32, i32* %i50, align 4
  %inc59 = add nsw i32 %56, 1
  store i32 %inc59, i32* %i50, align 4
  br label %for.cond51

for.end60:                                        ; preds = %for.cond51
  %57 = load i64**, i64*** %matInv, align 8
  %58 = bitcast i64** %57 to i8*
  call void @free(i8* %58) #4
  %59 = load i64**, i64*** %mat, align 8
  %60 = bitcast i64** %59 to i8*
  call void @free(i8* %60) #4
  ret i32 0
}

declare i32 @scanf(i8*, ...) #2

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare void @free(i8*) #3

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }
attributes #5 = { norecurse uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Symbolic Obfuscator clang version 3.9.0 (tags/RELEASE_390/final) (based on LLVM 3.9.0)"}

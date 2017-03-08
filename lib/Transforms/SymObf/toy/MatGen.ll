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
define i64 @_Z9PrintStepPPlS0_ll(i64** %mat, i64** %matInv, i64 %dim, i64 %step) #1 {
entry:
  %mat.addr = alloca i64**, align 8
  %matInv.addr = alloca i64**, align 8
  %dim.addr = alloca i64, align 8
  %step.addr = alloca i64, align 8
  %m = alloca i64, align 8
  %n = alloca i64, align 8
  %n7 = alloca i64, align 8
  store i64** %mat, i64*** %mat.addr, align 8
  store i64** %matInv, i64*** %matInv.addr, align 8
  store i64 %dim, i64* %dim.addr, align 8
  store i64 %step, i64* %step.addr, align 8
  %0 = load i64, i64* %step.addr, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i32 0, i32 0), i64 %0)
  store i64 0, i64* %m, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc18, %entry
  %1 = load i64, i64* %m, align 8
  %2 = load i64, i64* %dim.addr, align 8
  %cmp = icmp slt i64 %1, %2
  br i1 %cmp, label %for.body, label %for.end20

for.body:                                         ; preds = %for.cond
  store i64 0, i64* %n, align 8
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %3 = load i64, i64* %n, align 8
  %4 = load i64, i64* %dim.addr, align 8
  %cmp2 = icmp slt i64 %3, %4
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %5 = load i64, i64* %n, align 8
  %6 = load i64, i64* %m, align 8
  %7 = load i64**, i64*** %mat.addr, align 8
  %arrayidx = getelementptr inbounds i64*, i64** %7, i64 %6
  %8 = load i64*, i64** %arrayidx, align 8
  %arrayidx4 = getelementptr inbounds i64, i64* %8, i64 %5
  %9 = load i64, i64* %arrayidx4, align 8
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i64 %9)
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %10 = load i64, i64* %n, align 8
  %inc = add nsw i64 %10, 1
  store i64 %inc, i64* %n, align 8
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.2, i32 0, i32 0))
  store i64 0, i64* %n7, align 8
  br label %for.cond8

for.cond8:                                        ; preds = %for.inc14, %for.end
  %11 = load i64, i64* %n7, align 8
  %12 = load i64, i64* %dim.addr, align 8
  %cmp9 = icmp slt i64 %11, %12
  br i1 %cmp9, label %for.body10, label %for.end16

for.body10:                                       ; preds = %for.cond8
  %13 = load i64, i64* %n7, align 8
  %14 = load i64, i64* %m, align 8
  %15 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx11 = getelementptr inbounds i64*, i64** %15, i64 %14
  %16 = load i64*, i64** %arrayidx11, align 8
  %arrayidx12 = getelementptr inbounds i64, i64* %16, i64 %13
  %17 = load i64, i64* %arrayidx12, align 8
  %call13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i64 %17)
  br label %for.inc14

for.inc14:                                        ; preds = %for.body10
  %18 = load i64, i64* %n7, align 8
  %inc15 = add nsw i64 %18, 1
  store i64 %inc15, i64* %n7, align 8
  br label %for.cond8

for.end16:                                        ; preds = %for.cond8
  %call17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.3, i32 0, i32 0))
  br label %for.inc18

for.inc18:                                        ; preds = %for.end16
  %19 = load i64, i64* %m, align 8
  %inc19 = add nsw i64 %19, 1
  store i64 %inc19, i64* %m, align 8
  br label %for.cond

for.end20:                                        ; preds = %for.cond
  ret i64 0
}

declare i32 @printf(i8*, ...) #2

; Function Attrs: uwtable
define i64 @_Z13GenIntMatPairPPlS0_S0_ll(i64** %mat, i64** %matInv, i64** %iMat, i64 %dim, i64 %mod) #1 {
entry:
  %mat.addr = alloca i64**, align 8
  %matInv.addr = alloca i64**, align 8
  %iMat.addr = alloca i64**, align 8
  %dim.addr = alloca i64, align 8
  %mod.addr = alloca i64, align 8
  %saved_stack = alloca i8*, align 8
  %i = alloca i64, align 8
  %i11 = alloca i64, align 8
  %j = alloca i64, align 8
  %j55 = alloca i64, align 8
  %i83 = alloca i64, align 8
  %j88 = alloca i64, align 8
  %i145 = alloca i64, align 8
  %j150 = alloca i64, align 8
  %tmp = alloca i64, align 8
  %k = alloca i64, align 8
  store i64** %mat, i64*** %mat.addr, align 8
  store i64** %matInv, i64*** %matInv.addr, align 8
  store i64** %iMat, i64*** %iMat.addr, align 8
  store i64 %dim, i64* %dim.addr, align 8
  store i64 %mod, i64* %mod.addr, align 8
  %call = call i64 @time(i64* null) #4
  %conv = trunc i64 %call to i32
  call void @srand(i32 %conv) #4
  %0 = load i64, i64* %dim.addr, align 8
  %sub = sub nsw i64 %0, 1
  %1 = call i8* @llvm.stacksave()
  store i8* %1, i8** %saved_stack, align 8
  %vla = alloca i64, i64 %sub, align 16
  %2 = load i64, i64* %dim.addr, align 8
  %sub1 = sub nsw i64 %2, 1
  %vla2 = alloca i64, i64 %sub1, align 16
  store i64 0, i64* %i, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %3 = load i64, i64* %i, align 8
  %4 = load i64, i64* %dim.addr, align 8
  %sub3 = sub nsw i64 %4, 1
  %cmp = icmp slt i64 %3, %sub3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call4 = call i32 @rand() #4
  %rem = srem i32 %call4, 10
  %add = add nsw i32 %rem, 1
  %conv5 = sext i32 %add to i64
  %5 = load i64, i64* %i, align 8
  %arrayidx = getelementptr inbounds i64, i64* %vla, i64 %5
  store i64 %conv5, i64* %arrayidx, align 8
  %call6 = call i32 @rand() #4
  %conv7 = sext i32 %call6 to i64
  %6 = load i64, i64* %i, align 8
  %add8 = add nsw i64 %6, 1
  %rem9 = srem i64 %conv7, %add8
  %7 = load i64, i64* %i, align 8
  %arrayidx10 = getelementptr inbounds i64, i64* %vla2, i64 %7
  store i64 %rem9, i64* %arrayidx10, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %8 = load i64, i64* %i, align 8
  %inc = add nsw i64 %8, 1
  store i64 %inc, i64* %i, align 8
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i64 0, i64* %i11, align 8
  br label %for.cond12

for.cond12:                                       ; preds = %for.inc79, %for.end
  %9 = load i64, i64* %i11, align 8
  %10 = load i64, i64* %dim.addr, align 8
  %cmp13 = icmp slt i64 %9, %10
  br i1 %cmp13, label %for.body14, label %for.end81

for.body14:                                       ; preds = %for.cond12
  store i64 0, i64* %j, align 8
  br label %for.cond15

for.cond15:                                       ; preds = %for.inc50, %for.body14
  %11 = load i64, i64* %j, align 8
  %12 = load i64, i64* %dim.addr, align 8
  %cmp16 = icmp slt i64 %11, %12
  br i1 %cmp16, label %for.body17, label %for.end52

for.body17:                                       ; preds = %for.cond15
  %13 = load i64, i64* %j, align 8
  %14 = load i64, i64* %i11, align 8
  %15 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx18 = getelementptr inbounds i64*, i64** %15, i64 %14
  %16 = load i64*, i64** %arrayidx18, align 8
  %arrayidx19 = getelementptr inbounds i64, i64* %16, i64 %13
  store i64 0, i64* %arrayidx19, align 8
  %17 = load i64, i64* %i11, align 8
  %18 = load i64, i64* %j, align 8
  %cmp20 = icmp eq i64 %17, %18
  br i1 %cmp20, label %if.then, label %if.end

if.then:                                          ; preds = %for.body17
  %19 = load i64, i64* %j, align 8
  %20 = load i64, i64* %i11, align 8
  %21 = load i64**, i64*** %mat.addr, align 8
  %arrayidx21 = getelementptr inbounds i64*, i64** %21, i64 %20
  %22 = load i64*, i64** %arrayidx21, align 8
  %arrayidx22 = getelementptr inbounds i64, i64* %22, i64 %19
  store i64 1, i64* %arrayidx22, align 8
  %23 = load i64, i64* %j, align 8
  %24 = load i64, i64* %i11, align 8
  %25 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx23 = getelementptr inbounds i64*, i64** %25, i64 %24
  %26 = load i64*, i64** %arrayidx23, align 8
  %arrayidx24 = getelementptr inbounds i64, i64* %26, i64 %23
  store i64 1, i64* %arrayidx24, align 8
  %27 = load i64, i64* %j, align 8
  %28 = load i64, i64* %i11, align 8
  %29 = load i64**, i64*** %mat.addr, align 8
  %arrayidx25 = getelementptr inbounds i64*, i64** %29, i64 %28
  %30 = load i64*, i64** %arrayidx25, align 8
  %arrayidx26 = getelementptr inbounds i64, i64* %30, i64 %27
  %31 = load i64, i64* %arrayidx26, align 8
  %32 = load i64, i64* %j, align 8
  %33 = load i64, i64* %i11, align 8
  %34 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx27 = getelementptr inbounds i64*, i64** %34, i64 %33
  %35 = load i64*, i64** %arrayidx27, align 8
  %arrayidx28 = getelementptr inbounds i64, i64* %35, i64 %32
  store i64 %31, i64* %arrayidx28, align 8
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body17
  %36 = load i64, i64* %i11, align 8
  %37 = load i64, i64* %j, align 8
  %cmp29 = icmp slt i64 %36, %37
  br i1 %cmp29, label %if.then30, label %if.end40

if.then30:                                        ; preds = %if.end
  %call31 = call i32 @rand() #4
  %conv32 = sext i32 %call31 to i64
  %38 = load i64, i64* %mod.addr, align 8
  %rem33 = srem i64 %conv32, %38
  %39 = load i64, i64* %j, align 8
  %40 = load i64, i64* %i11, align 8
  %41 = load i64**, i64*** %mat.addr, align 8
  %arrayidx34 = getelementptr inbounds i64*, i64** %41, i64 %40
  %42 = load i64*, i64** %arrayidx34, align 8
  %arrayidx35 = getelementptr inbounds i64, i64* %42, i64 %39
  store i64 %rem33, i64* %arrayidx35, align 8
  %43 = load i64, i64* %j, align 8
  %44 = load i64, i64* %i11, align 8
  %45 = load i64**, i64*** %mat.addr, align 8
  %arrayidx36 = getelementptr inbounds i64*, i64** %45, i64 %44
  %46 = load i64*, i64** %arrayidx36, align 8
  %arrayidx37 = getelementptr inbounds i64, i64* %46, i64 %43
  %47 = load i64, i64* %arrayidx37, align 8
  %48 = load i64, i64* %j, align 8
  %49 = load i64, i64* %i11, align 8
  %50 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx38 = getelementptr inbounds i64*, i64** %50, i64 %49
  %51 = load i64*, i64** %arrayidx38, align 8
  %arrayidx39 = getelementptr inbounds i64, i64* %51, i64 %48
  store i64 %47, i64* %arrayidx39, align 8
  br label %if.end40

if.end40:                                         ; preds = %if.then30, %if.end
  %52 = load i64, i64* %i11, align 8
  %53 = load i64, i64* %j, align 8
  %cmp41 = icmp sgt i64 %52, %53
  br i1 %cmp41, label %if.then42, label %if.end49

if.then42:                                        ; preds = %if.end40
  %54 = load i64, i64* %j, align 8
  %55 = load i64, i64* %i11, align 8
  %56 = load i64**, i64*** %mat.addr, align 8
  %arrayidx43 = getelementptr inbounds i64*, i64** %56, i64 %55
  %57 = load i64*, i64** %arrayidx43, align 8
  %arrayidx44 = getelementptr inbounds i64, i64* %57, i64 %54
  store i64 0, i64* %arrayidx44, align 8
  %58 = load i64, i64* %j, align 8
  %59 = load i64, i64* %i11, align 8
  %60 = load i64**, i64*** %mat.addr, align 8
  %arrayidx45 = getelementptr inbounds i64*, i64** %60, i64 %59
  %61 = load i64*, i64** %arrayidx45, align 8
  %arrayidx46 = getelementptr inbounds i64, i64* %61, i64 %58
  %62 = load i64, i64* %arrayidx46, align 8
  %63 = load i64, i64* %j, align 8
  %64 = load i64, i64* %i11, align 8
  %65 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx47 = getelementptr inbounds i64*, i64** %65, i64 %64
  %66 = load i64*, i64** %arrayidx47, align 8
  %arrayidx48 = getelementptr inbounds i64, i64* %66, i64 %63
  store i64 %62, i64* %arrayidx48, align 8
  br label %if.end49

if.end49:                                         ; preds = %if.then42, %if.end40
  br label %for.inc50

for.inc50:                                        ; preds = %if.end49
  %67 = load i64, i64* %j, align 8
  %inc51 = add nsw i64 %67, 1
  store i64 %inc51, i64* %j, align 8
  br label %for.cond15

for.end52:                                        ; preds = %for.cond15
  %68 = load i64, i64* %i11, align 8
  %cmp53 = icmp sgt i64 %68, 0
  br i1 %cmp53, label %if.then54, label %if.end78

if.then54:                                        ; preds = %for.end52
  store i64 0, i64* %j55, align 8
  br label %for.cond56

for.cond56:                                       ; preds = %for.inc75, %if.then54
  %69 = load i64, i64* %j55, align 8
  %70 = load i64, i64* %dim.addr, align 8
  %cmp57 = icmp slt i64 %69, %70
  br i1 %cmp57, label %for.body58, label %for.end77

for.body58:                                       ; preds = %for.cond56
  %71 = load i64, i64* %j55, align 8
  %72 = load i64, i64* %i11, align 8
  %73 = load i64**, i64*** %mat.addr, align 8
  %arrayidx59 = getelementptr inbounds i64*, i64** %73, i64 %72
  %74 = load i64*, i64** %arrayidx59, align 8
  %arrayidx60 = getelementptr inbounds i64, i64* %74, i64 %71
  %75 = load i64, i64* %arrayidx60, align 8
  %76 = load i64, i64* %j55, align 8
  %77 = load i64, i64* %i11, align 8
  %sub61 = sub nsw i64 %77, 1
  %arrayidx62 = getelementptr inbounds i64, i64* %vla2, i64 %sub61
  %78 = load i64, i64* %arrayidx62, align 8
  %79 = load i64**, i64*** %mat.addr, align 8
  %arrayidx63 = getelementptr inbounds i64*, i64** %79, i64 %78
  %80 = load i64*, i64** %arrayidx63, align 8
  %arrayidx64 = getelementptr inbounds i64, i64* %80, i64 %76
  %81 = load i64, i64* %arrayidx64, align 8
  %82 = load i64, i64* %i11, align 8
  %sub65 = sub nsw i64 %82, 1
  %arrayidx66 = getelementptr inbounds i64, i64* %vla, i64 %sub65
  %83 = load i64, i64* %arrayidx66, align 8
  %mul = mul nsw i64 %81, %83
  %add67 = add nsw i64 %75, %mul
  %84 = load i64, i64* %mod.addr, align 8
  %rem68 = srem i64 %add67, %84
  %85 = load i64, i64* %j55, align 8
  %86 = load i64, i64* %i11, align 8
  %87 = load i64**, i64*** %mat.addr, align 8
  %arrayidx69 = getelementptr inbounds i64*, i64** %87, i64 %86
  %88 = load i64*, i64** %arrayidx69, align 8
  %arrayidx70 = getelementptr inbounds i64, i64* %88, i64 %85
  store i64 %rem68, i64* %arrayidx70, align 8
  %89 = load i64, i64* %j55, align 8
  %90 = load i64, i64* %i11, align 8
  %91 = load i64**, i64*** %mat.addr, align 8
  %arrayidx71 = getelementptr inbounds i64*, i64** %91, i64 %90
  %92 = load i64*, i64** %arrayidx71, align 8
  %arrayidx72 = getelementptr inbounds i64, i64* %92, i64 %89
  %93 = load i64, i64* %arrayidx72, align 8
  %94 = load i64, i64* %j55, align 8
  %95 = load i64, i64* %i11, align 8
  %96 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx73 = getelementptr inbounds i64*, i64** %96, i64 %95
  %97 = load i64*, i64** %arrayidx73, align 8
  %arrayidx74 = getelementptr inbounds i64, i64* %97, i64 %94
  store i64 %93, i64* %arrayidx74, align 8
  br label %for.inc75

for.inc75:                                        ; preds = %for.body58
  %98 = load i64, i64* %j55, align 8
  %inc76 = add nsw i64 %98, 1
  store i64 %inc76, i64* %j55, align 8
  br label %for.cond56

for.end77:                                        ; preds = %for.cond56
  br label %if.end78

if.end78:                                         ; preds = %for.end77, %for.end52
  br label %for.inc79

for.inc79:                                        ; preds = %if.end78
  %99 = load i64, i64* %i11, align 8
  %inc80 = add nsw i64 %99, 1
  store i64 %inc80, i64* %i11, align 8
  br label %for.cond12

for.end81:                                        ; preds = %for.cond12
  %100 = load i64**, i64*** %mat.addr, align 8
  %101 = load i64**, i64*** %matInv.addr, align 8
  %102 = load i64, i64* %dim.addr, align 8
  %call82 = call i64 @_Z9PrintStepPPlS0_ll(i64** %100, i64** %101, i64 %102, i64 0)
  %103 = load i64, i64* %dim.addr, align 8
  %sub84 = sub nsw i64 %103, 1
  store i64 %sub84, i64* %i83, align 8
  br label %for.cond85

for.cond85:                                       ; preds = %for.inc143, %for.end81
  %104 = load i64, i64* %i83, align 8
  %cmp86 = icmp sgt i64 %104, 0
  br i1 %cmp86, label %for.body87, label %for.end144

for.body87:                                       ; preds = %for.cond85
  store i64 0, i64* %j88, align 8
  br label %for.cond89

for.cond89:                                       ; preds = %for.inc138, %for.body87
  %105 = load i64, i64* %j88, align 8
  %106 = load i64, i64* %dim.addr, align 8
  %cmp90 = icmp slt i64 %105, %106
  br i1 %cmp90, label %for.body91, label %for.end140

for.body91:                                       ; preds = %for.cond89
  %107 = load i64, i64* %j88, align 8
  %108 = load i64, i64* %i83, align 8
  %109 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx92 = getelementptr inbounds i64*, i64** %109, i64 %108
  %110 = load i64*, i64** %arrayidx92, align 8
  %arrayidx93 = getelementptr inbounds i64, i64* %110, i64 %107
  %111 = load i64, i64* %arrayidx93, align 8
  %112 = load i64, i64* %j88, align 8
  %113 = load i64, i64* %i83, align 8
  %sub94 = sub nsw i64 %113, 1
  %arrayidx95 = getelementptr inbounds i64, i64* %vla2, i64 %sub94
  %114 = load i64, i64* %arrayidx95, align 8
  %115 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx96 = getelementptr inbounds i64*, i64** %115, i64 %114
  %116 = load i64*, i64** %arrayidx96, align 8
  %arrayidx97 = getelementptr inbounds i64, i64* %116, i64 %112
  %117 = load i64, i64* %arrayidx97, align 8
  %118 = load i64, i64* %i83, align 8
  %sub98 = sub nsw i64 %118, 1
  %arrayidx99 = getelementptr inbounds i64, i64* %vla, i64 %sub98
  %119 = load i64, i64* %arrayidx99, align 8
  %mul100 = mul nsw i64 %117, %119
  %sub101 = sub nsw i64 %111, %mul100
  %120 = load i64, i64* %mod.addr, align 8
  %rem102 = srem i64 %sub101, %120
  %121 = load i64, i64* %j88, align 8
  %122 = load i64, i64* %i83, align 8
  %123 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx103 = getelementptr inbounds i64*, i64** %123, i64 %122
  %124 = load i64*, i64** %arrayidx103, align 8
  %arrayidx104 = getelementptr inbounds i64, i64* %124, i64 %121
  store i64 %rem102, i64* %arrayidx104, align 8
  %125 = load i64, i64* %j88, align 8
  %126 = load i64, i64* %i83, align 8
  %127 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx105 = getelementptr inbounds i64*, i64** %127, i64 %126
  %128 = load i64*, i64** %arrayidx105, align 8
  %arrayidx106 = getelementptr inbounds i64, i64* %128, i64 %125
  %129 = load i64, i64* %arrayidx106, align 8
  %130 = load i64, i64* %j88, align 8
  %131 = load i64, i64* %i83, align 8
  %sub107 = sub nsw i64 %131, 1
  %arrayidx108 = getelementptr inbounds i64, i64* %vla2, i64 %sub107
  %132 = load i64, i64* %arrayidx108, align 8
  %133 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx109 = getelementptr inbounds i64*, i64** %133, i64 %132
  %134 = load i64*, i64** %arrayidx109, align 8
  %arrayidx110 = getelementptr inbounds i64, i64* %134, i64 %130
  %135 = load i64, i64* %arrayidx110, align 8
  %136 = load i64, i64* %i83, align 8
  %sub111 = sub nsw i64 %136, 1
  %arrayidx112 = getelementptr inbounds i64, i64* %vla, i64 %sub111
  %137 = load i64, i64* %arrayidx112, align 8
  %mul113 = mul nsw i64 %135, %137
  %sub114 = sub nsw i64 %129, %mul113
  %138 = load i64, i64* %mod.addr, align 8
  %rem115 = srem i64 %sub114, %138
  %139 = load i64, i64* %j88, align 8
  %140 = load i64, i64* %i83, align 8
  %141 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx116 = getelementptr inbounds i64*, i64** %141, i64 %140
  %142 = load i64*, i64** %arrayidx116, align 8
  %arrayidx117 = getelementptr inbounds i64, i64* %142, i64 %139
  store i64 %rem115, i64* %arrayidx117, align 8
  %143 = load i64, i64* %j88, align 8
  %144 = load i64, i64* %i83, align 8
  %145 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx118 = getelementptr inbounds i64*, i64** %145, i64 %144
  %146 = load i64*, i64** %arrayidx118, align 8
  %arrayidx119 = getelementptr inbounds i64, i64* %146, i64 %143
  %147 = load i64, i64* %arrayidx119, align 8
  %cmp120 = icmp slt i64 %147, 0
  br i1 %cmp120, label %if.then121, label %if.end127

if.then121:                                       ; preds = %for.body91
  %148 = load i64, i64* %j88, align 8
  %149 = load i64, i64* %i83, align 8
  %150 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx122 = getelementptr inbounds i64*, i64** %150, i64 %149
  %151 = load i64*, i64** %arrayidx122, align 8
  %arrayidx123 = getelementptr inbounds i64, i64* %151, i64 %148
  %152 = load i64, i64* %arrayidx123, align 8
  %153 = load i64, i64* %mod.addr, align 8
  %add124 = add nsw i64 %152, %153
  %154 = load i64, i64* %j88, align 8
  %155 = load i64, i64* %i83, align 8
  %156 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx125 = getelementptr inbounds i64*, i64** %156, i64 %155
  %157 = load i64*, i64** %arrayidx125, align 8
  %arrayidx126 = getelementptr inbounds i64, i64* %157, i64 %154
  store i64 %add124, i64* %arrayidx126, align 8
  br label %if.end127

if.end127:                                        ; preds = %if.then121, %for.body91
  %158 = load i64, i64* %j88, align 8
  %159 = load i64, i64* %i83, align 8
  %160 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx128 = getelementptr inbounds i64*, i64** %160, i64 %159
  %161 = load i64*, i64** %arrayidx128, align 8
  %arrayidx129 = getelementptr inbounds i64, i64* %161, i64 %158
  %162 = load i64, i64* %arrayidx129, align 8
  %cmp130 = icmp slt i64 %162, 0
  br i1 %cmp130, label %if.then131, label %if.end137

if.then131:                                       ; preds = %if.end127
  %163 = load i64, i64* %j88, align 8
  %164 = load i64, i64* %i83, align 8
  %165 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx132 = getelementptr inbounds i64*, i64** %165, i64 %164
  %166 = load i64*, i64** %arrayidx132, align 8
  %arrayidx133 = getelementptr inbounds i64, i64* %166, i64 %163
  %167 = load i64, i64* %arrayidx133, align 8
  %168 = load i64, i64* %mod.addr, align 8
  %add134 = add nsw i64 %167, %168
  %169 = load i64, i64* %j88, align 8
  %170 = load i64, i64* %i83, align 8
  %171 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx135 = getelementptr inbounds i64*, i64** %171, i64 %170
  %172 = load i64*, i64** %arrayidx135, align 8
  %arrayidx136 = getelementptr inbounds i64, i64* %172, i64 %169
  store i64 %add134, i64* %arrayidx136, align 8
  br label %if.end137

if.end137:                                        ; preds = %if.then131, %if.end127
  br label %for.inc138

for.inc138:                                       ; preds = %if.end137
  %173 = load i64, i64* %j88, align 8
  %inc139 = add nsw i64 %173, 1
  store i64 %inc139, i64* %j88, align 8
  br label %for.cond89

for.end140:                                       ; preds = %for.cond89
  %174 = load i64**, i64*** %iMat.addr, align 8
  %175 = load i64**, i64*** %matInv.addr, align 8
  %176 = load i64, i64* %dim.addr, align 8
  %177 = load i64, i64* %dim.addr, align 8
  %178 = load i64, i64* %i83, align 8
  %sub141 = sub nsw i64 %177, %178
  %call142 = call i64 @_Z9PrintStepPPlS0_ll(i64** %174, i64** %175, i64 %176, i64 %sub141)
  br label %for.inc143

for.inc143:                                       ; preds = %for.end140
  %179 = load i64, i64* %i83, align 8
  %dec = add nsw i64 %179, -1
  store i64 %dec, i64* %i83, align 8
  br label %for.cond85

for.end144:                                       ; preds = %for.cond85
  %180 = load i64, i64* %dim.addr, align 8
  %sub146 = sub nsw i64 %180, 1
  store i64 %sub146, i64* %i145, align 8
  br label %for.cond147

for.cond147:                                      ; preds = %for.inc208, %for.end144
  %181 = load i64, i64* %i145, align 8
  %cmp148 = icmp sgt i64 %181, 0
  br i1 %cmp148, label %for.body149, label %for.end210

for.body149:                                      ; preds = %for.cond147
  store i64 0, i64* %j150, align 8
  br label %for.cond151

for.cond151:                                      ; preds = %for.inc201, %for.body149
  %182 = load i64, i64* %j150, align 8
  %183 = load i64, i64* %i145, align 8
  %cmp152 = icmp slt i64 %182, %183
  br i1 %cmp152, label %for.body153, label %for.end203

for.body153:                                      ; preds = %for.cond151
  %184 = load i64, i64* %i145, align 8
  %185 = load i64, i64* %j150, align 8
  %186 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx154 = getelementptr inbounds i64*, i64** %186, i64 %185
  %187 = load i64*, i64** %arrayidx154, align 8
  %arrayidx155 = getelementptr inbounds i64, i64* %187, i64 %184
  %188 = load i64, i64* %arrayidx155, align 8
  store i64 %188, i64* %tmp, align 8
  store i64 0, i64* %k, align 8
  br label %for.cond157

for.cond157:                                      ; preds = %for.inc198, %for.body153
  %189 = load i64, i64* %k, align 8
  %190 = load i64, i64* %dim.addr, align 8
  %cmp158 = icmp slt i64 %189, %190
  br i1 %cmp158, label %for.body159, label %for.end200

for.body159:                                      ; preds = %for.cond157
  %191 = load i64, i64* %k, align 8
  %192 = load i64, i64* %j150, align 8
  %193 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx160 = getelementptr inbounds i64*, i64** %193, i64 %192
  %194 = load i64*, i64** %arrayidx160, align 8
  %arrayidx161 = getelementptr inbounds i64, i64* %194, i64 %191
  %195 = load i64, i64* %arrayidx161, align 8
  %196 = load i64, i64* %k, align 8
  %197 = load i64, i64* %i145, align 8
  %198 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx162 = getelementptr inbounds i64*, i64** %198, i64 %197
  %199 = load i64*, i64** %arrayidx162, align 8
  %arrayidx163 = getelementptr inbounds i64, i64* %199, i64 %196
  %200 = load i64, i64* %arrayidx163, align 8
  %201 = load i64, i64* %tmp, align 8
  %mul164 = mul nsw i64 %200, %201
  %sub165 = sub nsw i64 %195, %mul164
  %202 = load i64, i64* %mod.addr, align 8
  %rem166 = srem i64 %sub165, %202
  %203 = load i64, i64* %k, align 8
  %204 = load i64, i64* %j150, align 8
  %205 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx167 = getelementptr inbounds i64*, i64** %205, i64 %204
  %206 = load i64*, i64** %arrayidx167, align 8
  %arrayidx168 = getelementptr inbounds i64, i64* %206, i64 %203
  store i64 %rem166, i64* %arrayidx168, align 8
  %207 = load i64, i64* %k, align 8
  %208 = load i64, i64* %j150, align 8
  %209 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx169 = getelementptr inbounds i64*, i64** %209, i64 %208
  %210 = load i64*, i64** %arrayidx169, align 8
  %arrayidx170 = getelementptr inbounds i64, i64* %210, i64 %207
  %211 = load i64, i64* %arrayidx170, align 8
  %212 = load i64, i64* %k, align 8
  %213 = load i64, i64* %i145, align 8
  %214 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx171 = getelementptr inbounds i64*, i64** %214, i64 %213
  %215 = load i64*, i64** %arrayidx171, align 8
  %arrayidx172 = getelementptr inbounds i64, i64* %215, i64 %212
  %216 = load i64, i64* %arrayidx172, align 8
  %217 = load i64, i64* %tmp, align 8
  %mul173 = mul nsw i64 %216, %217
  %sub174 = sub nsw i64 %211, %mul173
  %218 = load i64, i64* %mod.addr, align 8
  %rem175 = srem i64 %sub174, %218
  %219 = load i64, i64* %k, align 8
  %220 = load i64, i64* %j150, align 8
  %221 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx176 = getelementptr inbounds i64*, i64** %221, i64 %220
  %222 = load i64*, i64** %arrayidx176, align 8
  %arrayidx177 = getelementptr inbounds i64, i64* %222, i64 %219
  store i64 %rem175, i64* %arrayidx177, align 8
  %223 = load i64, i64* %j150, align 8
  %224 = load i64, i64* %i145, align 8
  %225 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx178 = getelementptr inbounds i64*, i64** %225, i64 %224
  %226 = load i64*, i64** %arrayidx178, align 8
  %arrayidx179 = getelementptr inbounds i64, i64* %226, i64 %223
  %227 = load i64, i64* %arrayidx179, align 8
  %cmp180 = icmp slt i64 %227, 0
  br i1 %cmp180, label %if.then181, label %if.end187

if.then181:                                       ; preds = %for.body159
  %228 = load i64, i64* %j150, align 8
  %229 = load i64, i64* %i145, align 8
  %230 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx182 = getelementptr inbounds i64*, i64** %230, i64 %229
  %231 = load i64*, i64** %arrayidx182, align 8
  %arrayidx183 = getelementptr inbounds i64, i64* %231, i64 %228
  %232 = load i64, i64* %arrayidx183, align 8
  %233 = load i64, i64* %mod.addr, align 8
  %add184 = add nsw i64 %232, %233
  %234 = load i64, i64* %j150, align 8
  %235 = load i64, i64* %i145, align 8
  %236 = load i64**, i64*** %iMat.addr, align 8
  %arrayidx185 = getelementptr inbounds i64*, i64** %236, i64 %235
  %237 = load i64*, i64** %arrayidx185, align 8
  %arrayidx186 = getelementptr inbounds i64, i64* %237, i64 %234
  store i64 %add184, i64* %arrayidx186, align 8
  br label %if.end187

if.end187:                                        ; preds = %if.then181, %for.body159
  %238 = load i64, i64* %k, align 8
  %239 = load i64, i64* %j150, align 8
  %240 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx188 = getelementptr inbounds i64*, i64** %240, i64 %239
  %241 = load i64*, i64** %arrayidx188, align 8
  %arrayidx189 = getelementptr inbounds i64, i64* %241, i64 %238
  %242 = load i64, i64* %arrayidx189, align 8
  %cmp190 = icmp slt i64 %242, 0
  br i1 %cmp190, label %if.then191, label %if.end197

if.then191:                                       ; preds = %if.end187
  %243 = load i64, i64* %k, align 8
  %244 = load i64, i64* %j150, align 8
  %245 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx192 = getelementptr inbounds i64*, i64** %245, i64 %244
  %246 = load i64*, i64** %arrayidx192, align 8
  %arrayidx193 = getelementptr inbounds i64, i64* %246, i64 %243
  %247 = load i64, i64* %arrayidx193, align 8
  %248 = load i64, i64* %mod.addr, align 8
  %add194 = add nsw i64 %247, %248
  %249 = load i64, i64* %k, align 8
  %250 = load i64, i64* %j150, align 8
  %251 = load i64**, i64*** %matInv.addr, align 8
  %arrayidx195 = getelementptr inbounds i64*, i64** %251, i64 %250
  %252 = load i64*, i64** %arrayidx195, align 8
  %arrayidx196 = getelementptr inbounds i64, i64* %252, i64 %249
  store i64 %add194, i64* %arrayidx196, align 8
  br label %if.end197

if.end197:                                        ; preds = %if.then191, %if.end187
  br label %for.inc198

for.inc198:                                       ; preds = %if.end197
  %253 = load i64, i64* %k, align 8
  %inc199 = add nsw i64 %253, 1
  store i64 %inc199, i64* %k, align 8
  br label %for.cond157

for.end200:                                       ; preds = %for.cond157
  br label %for.inc201

for.inc201:                                       ; preds = %for.end200
  %254 = load i64, i64* %j150, align 8
  %inc202 = add nsw i64 %254, 1
  store i64 %inc202, i64* %j150, align 8
  br label %for.cond151

for.end203:                                       ; preds = %for.cond151
  %255 = load i64**, i64*** %iMat.addr, align 8
  %256 = load i64**, i64*** %matInv.addr, align 8
  %257 = load i64, i64* %dim.addr, align 8
  %258 = load i64, i64* %dim.addr, align 8
  %mul204 = mul nsw i64 2, %258
  %259 = load i64, i64* %i145, align 8
  %sub205 = sub nsw i64 %mul204, %259
  %sub206 = sub nsw i64 %sub205, 1
  %call207 = call i64 @_Z9PrintStepPPlS0_ll(i64** %255, i64** %256, i64 %257, i64 %sub206)
  br label %for.inc208

for.inc208:                                       ; preds = %for.end203
  %260 = load i64, i64* %i145, align 8
  %dec209 = add nsw i64 %260, -1
  store i64 %dec209, i64* %i145, align 8
  br label %for.cond147

for.end210:                                       ; preds = %for.cond147
  %261 = load i8*, i8** %saved_stack, align 8
  call void @llvm.stackrestore(i8* %261)
  ret i64 0
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
  %dim = alloca i64, align 8
  %mat = alloca i64**, align 8
  %matInv = alloca i64**, align 8
  %iMat = alloca i64**, align 8
  %i = alloca i64, align 8
  %mod = alloca i64, align 8
  %i19 = alloca i64, align 8
  %j = alloca i64, align 8
  %i36 = alloca i64, align 8
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i64 0, i64* %dim, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.4, i32 0, i32 0))
  %call1 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0), i64* %dim)
  %0 = load i64, i64* %dim, align 8
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.6, i32 0, i32 0), i64 %0)
  %1 = load i64, i64* %dim, align 8
  %mul = mul i64 8, %1
  %call3 = call noalias i8* @malloc(i64 %mul) #4
  %2 = bitcast i8* %call3 to i64**
  store i64** %2, i64*** %mat, align 8
  %3 = load i64, i64* %dim, align 8
  %mul4 = mul i64 8, %3
  %call5 = call noalias i8* @malloc(i64 %mul4) #4
  %4 = bitcast i8* %call5 to i64**
  store i64** %4, i64*** %matInv, align 8
  %5 = load i64, i64* %dim, align 8
  %mul6 = mul i64 8, %5
  %call7 = call noalias i8* @malloc(i64 %mul6) #4
  %6 = bitcast i8* %call7 to i64**
  store i64** %6, i64*** %iMat, align 8
  store i64 0, i64* %i, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %7 = load i64, i64* %i, align 8
  %8 = load i64, i64* %dim, align 8
  %cmp = icmp slt i64 %7, %8
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %9 = load i64, i64* %dim, align 8
  %mul8 = mul i64 8, %9
  %call9 = call noalias i8* @malloc(i64 %mul8) #4
  %10 = bitcast i8* %call9 to i64*
  %11 = load i64, i64* %i, align 8
  %12 = load i64**, i64*** %mat, align 8
  %arrayidx = getelementptr inbounds i64*, i64** %12, i64 %11
  store i64* %10, i64** %arrayidx, align 8
  %13 = load i64, i64* %dim, align 8
  %mul10 = mul i64 8, %13
  %call11 = call noalias i8* @malloc(i64 %mul10) #4
  %14 = bitcast i8* %call11 to i64*
  %15 = load i64, i64* %i, align 8
  %16 = load i64**, i64*** %matInv, align 8
  %arrayidx12 = getelementptr inbounds i64*, i64** %16, i64 %15
  store i64* %14, i64** %arrayidx12, align 8
  %17 = load i64, i64* %dim, align 8
  %mul13 = mul i64 8, %17
  %call14 = call noalias i8* @malloc(i64 %mul13) #4
  %18 = bitcast i8* %call14 to i64*
  %19 = load i64, i64* %i, align 8
  %20 = load i64**, i64*** %iMat, align 8
  %arrayidx15 = getelementptr inbounds i64*, i64** %20, i64 %19
  store i64* %18, i64** %arrayidx15, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %21 = load i64, i64* %i, align 8
  %inc = add nsw i64 %21, 1
  store i64 %inc, i64* %i, align 8
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i64 1024, i64* %mod, align 8
  %22 = load i64**, i64*** %mat, align 8
  %23 = load i64**, i64*** %matInv, align 8
  %24 = load i64**, i64*** %iMat, align 8
  %25 = load i64, i64* %dim, align 8
  %26 = load i64, i64* %mod, align 8
  %call16 = call i64 @_Z13GenIntMatPairPPlS0_S0_ll(i64** %22, i64** %23, i64** %24, i64 %25, i64 %26)
  %27 = load i64**, i64*** %mat, align 8
  %28 = load i64**, i64*** %matInv, align 8
  %29 = load i64**, i64*** %iMat, align 8
  %30 = load i64, i64* %dim, align 8
  %31 = load i64, i64* %dim, align 8
  %32 = load i64, i64* %dim, align 8
  %33 = load i64, i64* %dim, align 8
  %34 = load i64, i64* %mod, align 8
  %call17 = call i64 @_Z13MultIntMatrixPPlS0_S0_lllll(i64** %27, i64** %28, i64** %29, i64 %30, i64 %31, i64 %32, i64 %33, i64 %34)
  %call18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7, i32 0, i32 0))
  store i64 0, i64* %i19, align 8
  br label %for.cond20

for.cond20:                                       ; preds = %for.inc33, %for.end
  %35 = load i64, i64* %i19, align 8
  %36 = load i64, i64* %dim, align 8
  %cmp21 = icmp slt i64 %35, %36
  br i1 %cmp21, label %for.body22, label %for.end35

for.body22:                                       ; preds = %for.cond20
  store i64 0, i64* %j, align 8
  br label %for.cond23

for.cond23:                                       ; preds = %for.inc29, %for.body22
  %37 = load i64, i64* %j, align 8
  %38 = load i64, i64* %dim, align 8
  %cmp24 = icmp slt i64 %37, %38
  br i1 %cmp24, label %for.body25, label %for.end31

for.body25:                                       ; preds = %for.cond23
  %39 = load i64, i64* %j, align 8
  %40 = load i64, i64* %i19, align 8
  %41 = load i64**, i64*** %iMat, align 8
  %arrayidx26 = getelementptr inbounds i64*, i64** %41, i64 %40
  %42 = load i64*, i64** %arrayidx26, align 8
  %arrayidx27 = getelementptr inbounds i64, i64* %42, i64 %39
  %43 = load i64, i64* %arrayidx27, align 8
  %call28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i64 %43)
  br label %for.inc29

for.inc29:                                        ; preds = %for.body25
  %44 = load i64, i64* %j, align 8
  %inc30 = add nsw i64 %44, 1
  store i64 %inc30, i64* %j, align 8
  br label %for.cond23

for.end31:                                        ; preds = %for.cond23
  %call32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.3, i32 0, i32 0))
  br label %for.inc33

for.inc33:                                        ; preds = %for.end31
  %45 = load i64, i64* %i19, align 8
  %inc34 = add nsw i64 %45, 1
  store i64 %inc34, i64* %i19, align 8
  br label %for.cond20

for.end35:                                        ; preds = %for.cond20
  store i64 0, i64* %i36, align 8
  br label %for.cond37

for.cond37:                                       ; preds = %for.inc42, %for.end35
  %46 = load i64, i64* %i36, align 8
  %47 = load i64, i64* %dim, align 8
  %cmp38 = icmp slt i64 %46, %47
  br i1 %cmp38, label %for.body39, label %for.end44

for.body39:                                       ; preds = %for.cond37
  %48 = load i64, i64* %i36, align 8
  %49 = load i64**, i64*** %matInv, align 8
  %arrayidx40 = getelementptr inbounds i64*, i64** %49, i64 %48
  %50 = load i64*, i64** %arrayidx40, align 8
  %51 = bitcast i64* %50 to i8*
  call void @free(i8* %51) #4
  %52 = load i64, i64* %i36, align 8
  %53 = load i64**, i64*** %mat, align 8
  %arrayidx41 = getelementptr inbounds i64*, i64** %53, i64 %52
  %54 = load i64*, i64** %arrayidx41, align 8
  %55 = bitcast i64* %54 to i8*
  call void @free(i8* %55) #4
  br label %for.inc42

for.inc42:                                        ; preds = %for.body39
  %56 = load i64, i64* %i36, align 8
  %inc43 = add nsw i64 %56, 1
  store i64 %inc43, i64* %i36, align 8
  br label %for.cond37

for.end44:                                        ; preds = %for.cond37
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

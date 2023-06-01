; ModuleID = 'support.ll'
source_filename = "support.c"
target datalayout = "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-unknown-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%struct.stat = type { i64, i16, i32, i32, i32, i32, i32, i64, i16, i32, i32, i32, %struct.timespec, %struct.timespec, %struct.timespec, i32, i32 }
%struct.timespec = type { i32, i32 }

@.str.1 = private unnamed_addr constant [34 x i8] c"fd>1 && \22Invalid file descriptor\22\00", align 1
@.str.2 = private unnamed_addr constant [10 x i8] c"support.c\00", align 1
@__PRETTY_FUNCTION__.readfile = private unnamed_addr constant [20 x i8] c"char *readfile(int)\00", align 1
@.str.4 = private unnamed_addr constant [51 x i8] c"0==fstat(fd, &s) && \22Couldn't determine file size\22\00", align 1
@.str.6 = private unnamed_addr constant [25 x i8] c"len>0 && \22File is empty\22\00", align 1
@.str.8 = private unnamed_addr constant [29 x i8] c"status>=0 && \22read() failed\22\00", align 1
@.str.10 = private unnamed_addr constant [33 x i8] c"n>=0 && \22Invalid section number\22\00", align 1
@__PRETTY_FUNCTION__.find_section_start = private unnamed_addr constant [38 x i8] c"char *find_section_start(char *, int)\00", align 1
@.str.12 = private unnamed_addr constant [34 x i8] c"s!=NULL && \22Invalid input string\22\00", align 1
@__PRETTY_FUNCTION__.parse_string = private unnamed_addr constant [38 x i8] c"int parse_string(char *, char *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_uint8_t_array = private unnamed_addr constant [48 x i8] c"int parse_uint8_t_array(char *, uint8_t *, int)\00", align 1
@.str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@stderr = external dso_local local_unnamed_addr global %struct._IO_FILE*, align 4
@.str.14 = private unnamed_addr constant [35 x i8] c"Invalid input: line %d of section\0A\00", align 1
@__PRETTY_FUNCTION__.parse_uint16_t_array = private unnamed_addr constant [50 x i8] c"int parse_uint16_t_array(char *, uint16_t *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_uint32_t_array = private unnamed_addr constant [50 x i8] c"int parse_uint32_t_array(char *, uint32_t *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_uint64_t_array = private unnamed_addr constant [50 x i8] c"int parse_uint64_t_array(char *, uint64_t *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_int8_t_array = private unnamed_addr constant [46 x i8] c"int parse_int8_t_array(char *, int8_t *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_int16_t_array = private unnamed_addr constant [48 x i8] c"int parse_int16_t_array(char *, int16_t *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_int32_t_array = private unnamed_addr constant [48 x i8] c"int parse_int32_t_array(char *, int32_t *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_int64_t_array = private unnamed_addr constant [48 x i8] c"int parse_int64_t_array(char *, int64_t *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_float_array = private unnamed_addr constant [44 x i8] c"int parse_float_array(char *, float *, int)\00", align 1
@__PRETTY_FUNCTION__.parse_double_array = private unnamed_addr constant [46 x i8] c"int parse_double_array(char *, double *, int)\00", align 1
@__PRETTY_FUNCTION__.write_string = private unnamed_addr constant [35 x i8] c"int write_string(int, char *, int)\00", align 1
@.str.16 = private unnamed_addr constant [28 x i8] c"status>=0 && \22Write failed\22\00", align 1
@__PRETTY_FUNCTION__.write_uint8_t_array = private unnamed_addr constant [45 x i8] c"int write_uint8_t_array(int, uint8_t *, int)\00", align 1
@.str.17 = private unnamed_addr constant [4 x i8] c"%u\0A\00", align 1
@__PRETTY_FUNCTION__.write_uint16_t_array = private unnamed_addr constant [47 x i8] c"int write_uint16_t_array(int, uint16_t *, int)\00", align 1
@__PRETTY_FUNCTION__.write_uint32_t_array = private unnamed_addr constant [47 x i8] c"int write_uint32_t_array(int, uint32_t *, int)\00", align 1
@__PRETTY_FUNCTION__.write_uint64_t_array = private unnamed_addr constant [47 x i8] c"int write_uint64_t_array(int, uint64_t *, int)\00", align 1
@.str.18 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 1
@__PRETTY_FUNCTION__.write_int8_t_array = private unnamed_addr constant [43 x i8] c"int write_int8_t_array(int, int8_t *, int)\00", align 1
@.str.19 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@__PRETTY_FUNCTION__.write_int16_t_array = private unnamed_addr constant [45 x i8] c"int write_int16_t_array(int, int16_t *, int)\00", align 1
@__PRETTY_FUNCTION__.write_int32_t_array = private unnamed_addr constant [45 x i8] c"int write_int32_t_array(int, int32_t *, int)\00", align 1
@__PRETTY_FUNCTION__.write_int64_t_array = private unnamed_addr constant [45 x i8] c"int write_int64_t_array(int, int64_t *, int)\00", align 1
@.str.20 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@__PRETTY_FUNCTION__.write_float_array = private unnamed_addr constant [41 x i8] c"int write_float_array(int, float *, int)\00", align 1
@.str.21 = private unnamed_addr constant [7 x i8] c"%.16f\0A\00", align 1
@__PRETTY_FUNCTION__.write_double_array = private unnamed_addr constant [43 x i8] c"int write_double_array(int, double *, int)\00", align 1
@__PRETTY_FUNCTION__.write_section_header = private unnamed_addr constant [30 x i8] c"int write_section_header(int)\00", align 1
@.str.22 = private unnamed_addr constant [6 x i8] c"%%%%\0A\00", align 1
@.str.24 = private unnamed_addr constant [90 x i8] c"buffered<SUFFICIENT_SPRINTF_SPACE && \22Overran fd_printf buffer---output possibly corrupt\22\00", align 1
@__PRETTY_FUNCTION__.fd_printf = private unnamed_addr constant [38 x i8] c"int fd_printf(int, const char *, ...)\00", align 1
@.str.26 = private unnamed_addr constant [50 x i8] c"written==buffered && \22Wrote more data than given\22\00", align 1

; Function Attrs: nounwind
define dso_local noalias i8* @readfile(i32 %fd) local_unnamed_addr #0 {
entry:
  %s = alloca %struct.stat, align 4
  %0 = bitcast %struct.stat* %s to i8*
  call void @llvm.lifetime.start.p0i8(i64 88, i8* nonnull %0) #9
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %if.end, label %if.else

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 40, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @__PRETTY_FUNCTION__.readfile, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

if.end:                                           ; preds = %entry
  %call = call i32 @fstat(i32 %fd, %struct.stat* %s) #9
  %cmp1 = icmp eq i32 %call, 0
  br i1 %cmp1, label %if.end5, label %if.else4

if.else4:                                         ; preds = %if.end
  call void @__assert_fail(i8* getelementptr inbounds ([51 x i8], [51 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 41, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @__PRETTY_FUNCTION__.readfile, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

if.end5:                                          ; preds = %if.end
  %st_size = getelementptr inbounds %struct.stat, %struct.stat* %s, i32 0, i32 9
  %1 = load i32, i32* %st_size, align 4, !tbaa !3
  %cmp6 = icmp sgt i32 %1, 0
  br i1 %cmp6, label %if.end10, label %if.else9

if.else9:                                         ; preds = %if.end5
  call void @__assert_fail(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @.str.6, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 43, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @__PRETTY_FUNCTION__.readfile, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

if.end10:                                         ; preds = %if.end5
  %add = add nsw i32 %1, 1
  %call11 = call noalias i8* @malloc(i32 %add) #9
  br label %while.body

while.cond:                                       ; preds = %while.body
  %cmp12 = icmp sgt i32 %1, %add19
  br i1 %cmp12, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond, %if.end10
  %bytes_read.040 = phi i32 [ 0, %if.end10 ], [ %add19, %while.cond ]
  %arrayidx = getelementptr inbounds i8, i8* %call11, i32 %bytes_read.040
  %sub = sub nsw i32 %1, %bytes_read.040
  %call13 = call i32 @read(i32 %fd, i8* %arrayidx, i32 %sub) #9
  %cmp14 = icmp sgt i32 %call13, -1
  %add19 = add nsw i32 %call13, %bytes_read.040
  br i1 %cmp14, label %while.cond, label %if.else17

if.else17:                                        ; preds = %while.body
  call void @__assert_fail(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.8, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 48, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @__PRETTY_FUNCTION__.readfile, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

while.end:                                        ; preds = %while.cond
  %arrayidx20 = getelementptr inbounds i8, i8* %call11, i32 %1
  store i8 0, i8* %arrayidx20, align 1, !tbaa !12
  %call21 = call i32 @close(i32 %fd) #9
  call void @llvm.lifetime.end.p0i8(i64 88, i8* nonnull %0) #9
  ret i8* %call11

UnifiedUnreachableBlock:                          ; preds = %if.else17, %if.else9, %if.else4, %if.else
  unreachable
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: noreturn nounwind
declare dso_local void @__assert_fail(i8*, i8*, i32, i8*) local_unnamed_addr #2

; Function Attrs: inlinehint nounwind
define available_externally dso_local i32 @fstat(i32 %__fd, %struct.stat* nonnull %__statbuf) local_unnamed_addr #3 {
entry:
  %call = call i32 @__fxstat(i32 3, i32 %__fd, %struct.stat* nonnull %__statbuf) #9
  ret i32 %call
}

; Function Attrs: nofree nounwind
declare dso_local noalias i8* @malloc(i32) local_unnamed_addr #4

; Function Attrs: nofree
declare dso_local i32 @read(i32, i8* nocapture, i32) local_unnamed_addr #5

declare dso_local i32 @close(i32) local_unnamed_addr #6

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nounwind
define dso_local i8* @find_section_start(i8* readonly %s, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %n, -1
  br i1 %cmp, label %if.end, label %if.else

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.10, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 59, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @__PRETTY_FUNCTION__.find_section_start, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %cmp1 = icmp eq i32 %n, 0
  br i1 %cmp1, label %cleanup, label %while.cond.preheader

while.cond.preheader:                             ; preds = %if.end
  br label %while.cond

while.cond:                                       ; preds = %while.cond.preheader, %if.end21
  %s.addr.0 = phi i8* [ %incdec.ptr, %if.end21 ], [ %s, %while.cond.preheader ]
  %i.0 = phi i32 [ %i.1, %if.end21 ], [ 0, %while.cond.preheader ]
  %cmp4 = icmp slt i32 %i.0, %n
  %0 = load i8, i8* %s.addr.0, align 1, !tbaa !12
  %cmp5 = icmp ne i8 %0, 0
  %1 = select i1 %cmp4, i1 %cmp5, i1 false
  br i1 %1, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cmp8 = icmp eq i8 %0, 37
  br i1 %cmp8, label %land.lhs.true10, label %if.end21

land.lhs.true10:                                  ; preds = %while.body
  %arrayidx11 = getelementptr inbounds i8, i8* %s.addr.0, i32 1
  %2 = load i8, i8* %arrayidx11, align 1, !tbaa !12
  %cmp13 = icmp eq i8 %2, 37
  br i1 %cmp13, label %land.lhs.true15, label %if.end21

land.lhs.true15:                                  ; preds = %land.lhs.true10
  %arrayidx16 = getelementptr inbounds i8, i8* %s.addr.0, i32 2
  %3 = load i8, i8* %arrayidx16, align 1, !tbaa !12
  %cmp18 = icmp eq i8 %3, 10
  %inc = zext i1 %cmp18 to i32
  %spec.select = add nsw i32 %i.0, %inc
  br label %if.end21

if.end21:                                         ; preds = %land.lhs.true15, %land.lhs.true10, %while.body
  %i.1 = phi i32 [ %i.0, %land.lhs.true10 ], [ %i.0, %while.body ], [ %spec.select, %land.lhs.true15 ]
  %incdec.ptr = getelementptr inbounds i8, i8* %s.addr.0, i32 1
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %.lcssa = phi i8 [ %0, %while.cond ]
  %s.addr.0.lcssa = phi i8* [ %s.addr.0, %while.cond ]
  %cmp23 = icmp eq i8 %.lcssa, 0
  %add.ptr = getelementptr inbounds i8, i8* %s.addr.0.lcssa, i32 2
  %spec.select37 = select i1 %cmp23, i8* %s.addr.0.lcssa, i8* %add.ptr
  br label %UnifiedReturnBlock

cleanup:                                          ; preds = %if.end
  br label %UnifiedReturnBlock

UnifiedReturnBlock:                               ; preds = %cleanup, %while.end
  %UnifiedRetVal = phi i8* [ %spec.select37, %while.end ], [ %s, %cleanup ]
  ret i8* %UnifiedRetVal
}

; Function Attrs: nounwind
define dso_local i32 @parse_string(i8* readonly %s, i8* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 79, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @__PRETTY_FUNCTION__.parse_string, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %cmp1 = icmp slt i32 %n, 0
  br i1 %cmp1, label %if.end.while.cond_crit_edge, label %if.end34

if.end.while.cond_crit_edge:                      ; preds = %if.end
  %.pre = load i8, i8* %s, align 1, !tbaa !12
  br label %while.cond

while.cond:                                       ; preds = %if.end.while.cond_crit_edge, %land.rhs
  %0 = phi i8 [ %1, %land.rhs ], [ %.pre, %if.end.while.cond_crit_edge ]
  %k.0 = phi i32 [ %add, %land.rhs ], [ 0, %if.end.while.cond_crit_edge ]
  %cmp3 = icmp eq i8 %0, 0
  br i1 %cmp3, label %if.end34.loopexit, label %land.lhs.true5

land.lhs.true5:                                   ; preds = %while.cond
  %add = add nuw nsw i32 %k.0, 1
  %arrayidx6 = getelementptr inbounds i8, i8* %s, i32 %add
  %1 = load i8, i8* %arrayidx6, align 1, !tbaa !12
  %cmp8 = icmp eq i8 %1, 0
  br i1 %cmp8, label %if.end34.loopexit, label %land.lhs.true10

land.lhs.true10:                                  ; preds = %land.lhs.true5
  %add11 = add nuw nsw i32 %k.0, 2
  %arrayidx12 = getelementptr inbounds i8, i8* %s, i32 %add11
  %2 = load i8, i8* %arrayidx12, align 1, !tbaa !12
  %cmp14 = icmp eq i8 %2, 0
  br i1 %cmp14, label %if.end34.loopexit, label %land.rhs

land.rhs:                                         ; preds = %land.lhs.true10
  %cmp18 = icmp eq i8 %0, 10
  %cmp24 = icmp eq i8 %1, 37
  %or.cond = and i1 %cmp18, %cmp24
  %cmp30 = icmp eq i8 %2, 37
  %or.cond61 = and i1 %or.cond, %cmp30
  br i1 %or.cond61, label %if.end34.loopexit, label %while.cond

if.end34.loopexit:                                ; preds = %while.cond, %land.lhs.true5, %land.lhs.true10, %land.rhs
  %k.0.lcssa = phi i32 [ %k.0, %while.cond ], [ %k.0, %land.lhs.true5 ], [ %k.0, %land.lhs.true10 ], [ %k.0, %land.rhs ]
  br label %if.end34

if.end34:                                         ; preds = %if.end34.loopexit, %if.end
  %k.1 = phi i32 [ %n, %if.end ], [ %k.0.lcssa, %if.end34.loopexit ]
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* align 1 %arr, i8* nonnull align 1 %s, i32 %k.1, i1 false)
  br i1 %cmp1, label %if.then37, label %if.end39

if.then37:                                        ; preds = %if.end34
  %arrayidx38 = getelementptr inbounds i8, i8* %arr, i32 %k.1
  store i8 0, i8* %arrayidx38, align 1, !tbaa !12
  br label %if.end39

if.end39:                                         ; preds = %if.then37, %if.end34
  ret i32 0
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i32, i1 immarg) #1

; Function Attrs: nounwind
define dso_local i32 @parse_uint8_t_array(i8* %s, i8* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 132, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @__PRETTY_FUNCTION__.parse_uint8_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp131 = icmp ne i8* %call, null
  %cmp232 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp232, %cmp131
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end9
  %line.034 = phi i8* [ %call12, %if.end9 ], [ %call, %while.body.preheader ]
  %i.033 = phi i32 [ %inc, %if.end9 ], [ 0, %while.body.preheader ]
  store i8* %line.034, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.034, i8** nonnull %endptr, i32 10) #9
  %conv = trunc i32 %call3 to i8
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp5 = icmp eq i8 %3, 0
  br i1 %cmp5, label %if.end9, label %if.then7

if.then7:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call8 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.033) #11
  br label %if.end9

if.end9:                                          ; preds = %if.then7, %while.body
  %arrayidx = getelementptr inbounds i8, i8* %arr, i32 %i.033
  store i8 %conv, i8* %arrayidx, align 1, !tbaa !12
  %inc = add nuw nsw i32 %i.033, 1
  %call10 = call i32 @strlen(i8* nonnull %line.034) #12
  %arrayidx11 = getelementptr inbounds i8, i8* %line.034, i32 %call10
  store i8 10, i8* %arrayidx11, align 1, !tbaa !12
  %call12 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call12, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end9
  %call12.lcssa = phi i8* [ %call12, %if.end9 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end9 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call12.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp131, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then15, label %if.end18

if.then15:                                        ; preds = %while.end
  %call16 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx17 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call16
  store i8 10, i8* %arrayidx17, align 1, !tbaa !12
  br label %if.end18

if.end18:                                         ; preds = %if.then15, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local i8* @strtok(i8*, i8* nocapture readonly) local_unnamed_addr #4

; Function Attrs: nofree nounwind
declare dso_local i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #4

; Function Attrs: nofree nounwind
declare dso_local i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #4

; Function Attrs: argmemonly nofree nounwind readonly
declare dso_local i32 @strlen(i8* nocapture) local_unnamed_addr #7

; Function Attrs: nounwind
define dso_local i32 @parse_uint16_t_array(i8* %s, i16* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 133, i8* getelementptr inbounds ([50 x i8], [50 x i8]* @__PRETTY_FUNCTION__.parse_uint16_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp132 = icmp ne i8* %call, null
  %cmp233 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp233, %cmp132
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end9
  %line.035 = phi i8* [ %call12, %if.end9 ], [ %call, %while.body.preheader ]
  %i.034 = phi i32 [ %inc, %if.end9 ], [ 0, %while.body.preheader ]
  store i8* %line.035, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.035, i8** nonnull %endptr, i32 10) #9
  %conv = trunc i32 %call3 to i16
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp5 = icmp eq i8 %3, 0
  br i1 %cmp5, label %if.end9, label %if.then7

if.then7:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call8 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.034) #11
  br label %if.end9

if.end9:                                          ; preds = %if.then7, %while.body
  %arrayidx = getelementptr inbounds i16, i16* %arr, i32 %i.034
  store i16 %conv, i16* %arrayidx, align 2, !tbaa !15
  %inc = add nuw nsw i32 %i.034, 1
  %call10 = call i32 @strlen(i8* nonnull %line.035) #12
  %arrayidx11 = getelementptr inbounds i8, i8* %line.035, i32 %call10
  store i8 10, i8* %arrayidx11, align 1, !tbaa !12
  %call12 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call12, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end9
  %call12.lcssa = phi i8* [ %call12, %if.end9 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end9 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call12.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp132, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then15, label %if.end18

if.then15:                                        ; preds = %while.end
  %call16 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx17 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call16
  store i8 10, i8* %arrayidx17, align 1, !tbaa !12
  br label %if.end18

if.end18:                                         ; preds = %if.then15, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @parse_uint32_t_array(i8* %s, i32* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 134, i8* getelementptr inbounds ([50 x i8], [50 x i8]* @__PRETTY_FUNCTION__.parse_uint32_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp131 = icmp ne i8* %call, null
  %cmp232 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp232, %cmp131
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end8
  %line.034 = phi i8* [ %call11, %if.end8 ], [ %call, %while.body.preheader ]
  %i.033 = phi i32 [ %inc, %if.end8 ], [ 0, %while.body.preheader ]
  store i8* %line.034, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.034, i8** nonnull %endptr, i32 10) #9
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp4 = icmp eq i8 %3, 0
  br i1 %cmp4, label %if.end8, label %if.then6

if.then6:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call7 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.033) #11
  br label %if.end8

if.end8:                                          ; preds = %if.then6, %while.body
  %arrayidx = getelementptr inbounds i32, i32* %arr, i32 %i.033
  store i32 %call3, i32* %arrayidx, align 4, !tbaa !16
  %inc = add nuw nsw i32 %i.033, 1
  %call9 = call i32 @strlen(i8* nonnull %line.034) #12
  %arrayidx10 = getelementptr inbounds i8, i8* %line.034, i32 %call9
  store i8 10, i8* %arrayidx10, align 1, !tbaa !12
  %call11 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call11, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end8
  %call11.lcssa = phi i8* [ %call11, %if.end8 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end8 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call11.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp131, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then14, label %if.end17

if.then14:                                        ; preds = %while.end
  %call15 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx16 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call15
  store i8 10, i8* %arrayidx16, align 1, !tbaa !12
  br label %if.end17

if.end17:                                         ; preds = %if.then14, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @parse_uint64_t_array(i8* %s, i64* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 135, i8* getelementptr inbounds ([50 x i8], [50 x i8]* @__PRETTY_FUNCTION__.parse_uint64_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp132 = icmp ne i8* %call, null
  %cmp233 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp233, %cmp132
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end9
  %line.035 = phi i8* [ %call12, %if.end9 ], [ %call, %while.body.preheader ]
  %i.034 = phi i32 [ %inc, %if.end9 ], [ 0, %while.body.preheader ]
  store i8* %line.035, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.035, i8** nonnull %endptr, i32 10) #9
  %conv = sext i32 %call3 to i64
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp5 = icmp eq i8 %3, 0
  br i1 %cmp5, label %if.end9, label %if.then7

if.then7:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call8 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.034) #11
  br label %if.end9

if.end9:                                          ; preds = %if.then7, %while.body
  %arrayidx = getelementptr inbounds i64, i64* %arr, i32 %i.034
  store i64 %conv, i64* %arrayidx, align 4, !tbaa !17
  %inc = add nuw nsw i32 %i.034, 1
  %call10 = call i32 @strlen(i8* nonnull %line.035) #12
  %arrayidx11 = getelementptr inbounds i8, i8* %line.035, i32 %call10
  store i8 10, i8* %arrayidx11, align 1, !tbaa !12
  %call12 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call12, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end9
  %call12.lcssa = phi i8* [ %call12, %if.end9 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end9 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call12.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp132, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then15, label %if.end18

if.then15:                                        ; preds = %while.end
  %call16 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx17 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call16
  store i8 10, i8* %arrayidx17, align 1, !tbaa !12
  br label %if.end18

if.end18:                                         ; preds = %if.then15, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @parse_int8_t_array(i8* %s, i8* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 136, i8* getelementptr inbounds ([46 x i8], [46 x i8]* @__PRETTY_FUNCTION__.parse_int8_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp131 = icmp ne i8* %call, null
  %cmp232 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp232, %cmp131
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end9
  %line.034 = phi i8* [ %call12, %if.end9 ], [ %call, %while.body.preheader ]
  %i.033 = phi i32 [ %inc, %if.end9 ], [ 0, %while.body.preheader ]
  store i8* %line.034, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.034, i8** nonnull %endptr, i32 10) #9
  %conv = trunc i32 %call3 to i8
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp5 = icmp eq i8 %3, 0
  br i1 %cmp5, label %if.end9, label %if.then7

if.then7:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call8 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.033) #11
  br label %if.end9

if.end9:                                          ; preds = %if.then7, %while.body
  %arrayidx = getelementptr inbounds i8, i8* %arr, i32 %i.033
  store i8 %conv, i8* %arrayidx, align 1, !tbaa !12
  %inc = add nuw nsw i32 %i.033, 1
  %call10 = call i32 @strlen(i8* nonnull %line.034) #12
  %arrayidx11 = getelementptr inbounds i8, i8* %line.034, i32 %call10
  store i8 10, i8* %arrayidx11, align 1, !tbaa !12
  %call12 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call12, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end9
  %call12.lcssa = phi i8* [ %call12, %if.end9 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end9 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call12.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp131, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then15, label %if.end18

if.then15:                                        ; preds = %while.end
  %call16 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx17 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call16
  store i8 10, i8* %arrayidx17, align 1, !tbaa !12
  br label %if.end18

if.end18:                                         ; preds = %if.then15, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @parse_int16_t_array(i8* %s, i16* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 137, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @__PRETTY_FUNCTION__.parse_int16_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp132 = icmp ne i8* %call, null
  %cmp233 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp233, %cmp132
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end9
  %line.035 = phi i8* [ %call12, %if.end9 ], [ %call, %while.body.preheader ]
  %i.034 = phi i32 [ %inc, %if.end9 ], [ 0, %while.body.preheader ]
  store i8* %line.035, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.035, i8** nonnull %endptr, i32 10) #9
  %conv = trunc i32 %call3 to i16
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp5 = icmp eq i8 %3, 0
  br i1 %cmp5, label %if.end9, label %if.then7

if.then7:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call8 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.034) #11
  br label %if.end9

if.end9:                                          ; preds = %if.then7, %while.body
  %arrayidx = getelementptr inbounds i16, i16* %arr, i32 %i.034
  store i16 %conv, i16* %arrayidx, align 2, !tbaa !15
  %inc = add nuw nsw i32 %i.034, 1
  %call10 = call i32 @strlen(i8* nonnull %line.035) #12
  %arrayidx11 = getelementptr inbounds i8, i8* %line.035, i32 %call10
  store i8 10, i8* %arrayidx11, align 1, !tbaa !12
  %call12 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call12, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end9
  %call12.lcssa = phi i8* [ %call12, %if.end9 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end9 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call12.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp132, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then15, label %if.end18

if.then15:                                        ; preds = %while.end
  %call16 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx17 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call16
  store i8 10, i8* %arrayidx17, align 1, !tbaa !12
  br label %if.end18

if.end18:                                         ; preds = %if.then15, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @parse_int32_t_array(i8* %s, i32* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 138, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @__PRETTY_FUNCTION__.parse_int32_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp131 = icmp ne i8* %call, null
  %cmp232 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp232, %cmp131
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end8
  %line.034 = phi i8* [ %call11, %if.end8 ], [ %call, %while.body.preheader ]
  %i.033 = phi i32 [ %inc, %if.end8 ], [ 0, %while.body.preheader ]
  store i8* %line.034, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.034, i8** nonnull %endptr, i32 10) #9
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp4 = icmp eq i8 %3, 0
  br i1 %cmp4, label %if.end8, label %if.then6

if.then6:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call7 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.033) #11
  br label %if.end8

if.end8:                                          ; preds = %if.then6, %while.body
  %arrayidx = getelementptr inbounds i32, i32* %arr, i32 %i.033
  store i32 %call3, i32* %arrayidx, align 4, !tbaa !16
  %inc = add nuw nsw i32 %i.033, 1
  %call9 = call i32 @strlen(i8* nonnull %line.034) #12
  %arrayidx10 = getelementptr inbounds i8, i8* %line.034, i32 %call9
  store i8 10, i8* %arrayidx10, align 1, !tbaa !12
  %call11 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call11, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end8
  %call11.lcssa = phi i8* [ %call11, %if.end8 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end8 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call11.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp131, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then14, label %if.end17

if.then14:                                        ; preds = %while.end
  %call15 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx16 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call15
  store i8 10, i8* %arrayidx16, align 1, !tbaa !12
  br label %if.end17

if.end17:                                         ; preds = %if.then14, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @parse_int64_t_array(i8* %s, i64* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 139, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @__PRETTY_FUNCTION__.parse_int64_t_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp132 = icmp ne i8* %call, null
  %cmp233 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp233, %cmp132
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end9
  %line.035 = phi i8* [ %call12, %if.end9 ], [ %call, %while.body.preheader ]
  %i.034 = phi i32 [ %inc, %if.end9 ], [ 0, %while.body.preheader ]
  store i8* %line.035, i8** %endptr, align 4, !tbaa !13
  %call3 = call i32 @strtol(i8* nonnull %line.035, i8** nonnull %endptr, i32 10) #9
  %conv = sext i32 %call3 to i64
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp5 = icmp eq i8 %3, 0
  br i1 %cmp5, label %if.end9, label %if.then7

if.then7:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call8 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.034) #11
  br label %if.end9

if.end9:                                          ; preds = %if.then7, %while.body
  %arrayidx = getelementptr inbounds i64, i64* %arr, i32 %i.034
  store i64 %conv, i64* %arrayidx, align 4, !tbaa !17
  %inc = add nuw nsw i32 %i.034, 1
  %call10 = call i32 @strlen(i8* nonnull %line.035) #12
  %arrayidx11 = getelementptr inbounds i8, i8* %line.035, i32 %call10
  store i8 10, i8* %arrayidx11, align 1, !tbaa !12
  %call12 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call12, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end9
  %call12.lcssa = phi i8* [ %call12, %if.end9 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end9 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call12.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp132, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then15, label %if.end18

if.then15:                                        ; preds = %while.end
  %call16 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx17 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call16
  store i8 10, i8* %arrayidx17, align 1, !tbaa !12
  br label %if.end18

if.end18:                                         ; preds = %if.then15, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @parse_float_array(i8* %s, float* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 141, i8* getelementptr inbounds ([44 x i8], [44 x i8]* @__PRETTY_FUNCTION__.parse_float_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp131 = icmp ne i8* %call, null
  %cmp232 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp232, %cmp131
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end8
  %line.034 = phi i8* [ %call11, %if.end8 ], [ %call, %while.body.preheader ]
  %i.033 = phi i32 [ %inc, %if.end8 ], [ 0, %while.body.preheader ]
  store i8* %line.034, i8** %endptr, align 4, !tbaa !13
  %call3 = call float @strtof(i8* nonnull %line.034, i8** nonnull %endptr) #9
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp4 = icmp eq i8 %3, 0
  br i1 %cmp4, label %if.end8, label %if.then6

if.then6:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call7 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.033) #11
  br label %if.end8

if.end8:                                          ; preds = %if.then6, %while.body
  %arrayidx = getelementptr inbounds float, float* %arr, i32 %i.033
  store float %call3, float* %arrayidx, align 4, !tbaa !18
  %inc = add nuw nsw i32 %i.033, 1
  %call9 = call i32 @strlen(i8* nonnull %line.034) #12
  %arrayidx10 = getelementptr inbounds i8, i8* %line.034, i32 %call9
  store i8 10, i8* %arrayidx10, align 1, !tbaa !12
  %call11 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call11, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end8
  %call11.lcssa = phi i8* [ %call11, %if.end8 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end8 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call11.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp131, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then14, label %if.end17

if.then14:                                        ; preds = %while.end
  %call15 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx16 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call15
  store i8 10, i8* %arrayidx16, align 1, !tbaa !12
  br label %if.end17

if.end17:                                         ; preds = %if.then14, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local float @strtof(i8* readonly, i8** nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define dso_local i32 @parse_double_array(i8* %s, double* nocapture %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %endptr = alloca i8*, align 4
  %0 = bitcast i8** %endptr to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %cmp = icmp eq i8* %s, null
  br i1 %cmp, label %if.else, label %if.end

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.12, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 142, i8* getelementptr inbounds ([46 x i8], [46 x i8]* @__PRETTY_FUNCTION__.parse_double_array, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  %call = call i8* @strtok(i8* nonnull %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp131 = icmp ne i8* %call, null
  %cmp232 = icmp sgt i32 %n, 0
  %1 = and i1 %cmp232, %cmp131
  br i1 %1, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %if.end
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %if.end8
  %line.034 = phi i8* [ %call11, %if.end8 ], [ %call, %while.body.preheader ]
  %i.033 = phi i32 [ %inc, %if.end8 ], [ 0, %while.body.preheader ]
  store i8* %line.034, i8** %endptr, align 4, !tbaa !13
  %call3 = call double @strtod(i8* nonnull %line.034, i8** nonnull %endptr) #9
  %2 = load i8*, i8** %endptr, align 4, !tbaa !13
  %3 = load i8, i8* %2, align 1, !tbaa !12
  %cmp4 = icmp eq i8 %3, 0
  br i1 %cmp4, label %if.end8, label %if.then6

if.then6:                                         ; preds = %while.body
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !13
  %call7 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.14, i32 0, i32 0), i32 %i.033) #11
  br label %if.end8

if.end8:                                          ; preds = %if.then6, %while.body
  %arrayidx = getelementptr inbounds double, double* %arr, i32 %i.033
  store double %call3, double* %arrayidx, align 4, !tbaa !20
  %inc = add nuw nsw i32 %i.033, 1
  %call9 = call i32 @strlen(i8* nonnull %line.034) #12
  %arrayidx10 = getelementptr inbounds i8, i8* %line.034, i32 %call9
  store i8 10, i8* %arrayidx10, align 1, !tbaa !12
  %call11 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0)) #9
  %cmp1 = icmp ne i8* %call11, null
  %cmp2 = icmp slt i32 %inc, %n
  %5 = and i1 %cmp2, %cmp1
  br i1 %5, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end8
  %call11.lcssa = phi i8* [ %call11, %if.end8 ]
  %cmp1.lcssa1 = phi i1 [ %cmp1, %if.end8 ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %if.end
  %line.0.lcssa = phi i8* [ %call, %if.end ], [ %call11.lcssa, %while.end.loopexit ]
  %cmp1.lcssa = phi i1 [ %cmp131, %if.end ], [ %cmp1.lcssa1, %while.end.loopexit ]
  br i1 %cmp1.lcssa, label %if.then14, label %if.end17

if.then14:                                        ; preds = %while.end
  %call15 = call i32 @strlen(i8* nonnull %line.0.lcssa) #12
  %arrayidx16 = getelementptr inbounds i8, i8* %line.0.lcssa, i32 %call15
  store i8 10, i8* %arrayidx16, align 1, !tbaa !12
  br label %if.end17

if.end17:                                         ; preds = %if.then14, %while.end
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local double @strtod(i8* readonly, i8** nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define dso_local i32 @write_string(i32 %fd, i8* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %if.end, label %if.else

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 147, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @__PRETTY_FUNCTION__.write_string, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

if.end:                                           ; preds = %entry
  %cmp1 = icmp slt i32 %n, 0
  br i1 %cmp1, label %if.then2, label %if.end3

if.then2:                                         ; preds = %if.end
  %call = call i32 @strlen(i8* nonnull dereferenceable(1) %arr) #12
  br label %if.end3

if.end3:                                          ; preds = %if.then2, %if.end
  %n.addr.0 = phi i32 [ %call, %if.then2 ], [ %n, %if.end ]
  %cmp431 = icmp sgt i32 %n.addr.0, 0
  br i1 %cmp431, label %while.body.preheader, label %do.body.preheader

while.body.preheader:                             ; preds = %if.end3
  br label %while.body

do.body.preheader.loopexit:                       ; preds = %while.cond
  br label %do.body.preheader

do.body.preheader:                                ; preds = %do.body.preheader.loopexit, %if.end3
  br label %do.body

while.cond:                                       ; preds = %while.body
  %cmp4 = icmp sgt i32 %n.addr.0, %add
  br i1 %cmp4, label %while.body, label %do.body.preheader.loopexit

while.body:                                       ; preds = %while.body.preheader, %while.cond
  %written.032 = phi i32 [ %add, %while.cond ], [ 0, %while.body.preheader ]
  %arrayidx = getelementptr inbounds i8, i8* %arr, i32 %written.032
  %sub = sub nsw i32 %n.addr.0, %written.032
  %call5 = call i32 @write(i32 %fd, i8* %arrayidx, i32 %sub) #9
  %cmp6 = icmp sgt i32 %call5, -1
  %add = add nsw i32 %call5, %written.032
  br i1 %cmp6, label %while.cond, label %if.else9

if.else9:                                         ; preds = %while.body
  call void @__assert_fail(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.16, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 154, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @__PRETTY_FUNCTION__.write_string, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

do.body:                                          ; preds = %do.cond, %do.body.preheader
  %call11 = call i32 @write(i32 %fd, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0), i32 1) #9
  %cmp12 = icmp sgt i32 %call11, -1
  br i1 %cmp12, label %do.cond, label %if.else15

if.else15:                                        ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.16, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 160, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @__PRETTY_FUNCTION__.write_string, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

do.cond:                                          ; preds = %do.body
  %cmp17 = icmp eq i32 %call11, 0
  br i1 %cmp17, label %do.body, label %do.end

do.end:                                           ; preds = %do.cond
  ret i32 0

UnifiedUnreachableBlock:                          ; preds = %if.else15, %if.else9, %if.else
  unreachable
}

; Function Attrs: nofree
declare dso_local i32 @write(i32, i8* nocapture readonly, i32) local_unnamed_addr #5

; Function Attrs: nounwind
define dso_local i32 @write_uint8_t_array(i32 %fd, i8* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 177, i8* getelementptr inbounds ([45 x i8], [45 x i8]* @__PRETTY_FUNCTION__.write_uint8_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i8, i8* %arr, i32 %i.07
  %0 = load i8, i8* %arrayidx, align 1, !tbaa !12
  %conv = zext i8 %0 to i32
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.17, i32 0, i32 0), i32 %conv)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: inlinehint nounwind
define internal void @fd_printf(i32 %fd, i8* nocapture readonly %format, ...) unnamed_addr #3 {
entry:
  %args = alloca i8*, align 4
  %buffer = alloca [256 x i8], align 1
  %0 = bitcast i8** %args to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #9
  %1 = getelementptr inbounds [256 x i8], [256 x i8]* %buffer, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %1) #9
  call void @llvm.va_start(i8* %0)
  %2 = load i8*, i8** %args, align 4, !tbaa !13
  %call = call i32 @vsnprintf(i8* nonnull %1, i32 256, i8* %format, i8* %2) #9
  call void @llvm.va_end(i8* nonnull %0)
  %cmp = icmp slt i32 %call, 256
  br i1 %cmp, label %while.cond.preheader, label %if.else

while.cond.preheader:                             ; preds = %entry
  %cmp32 = icmp sgt i32 %call, 0
  br i1 %cmp32, label %while.body.preheader, label %while.end

while.body.preheader:                             ; preds = %while.cond.preheader
  br label %while.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([90 x i8], [90 x i8]* @.str.24, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 22, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @__PRETTY_FUNCTION__.fd_printf, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

while.cond:                                       ; preds = %while.body
  %cmp3 = icmp sgt i32 %call, %add
  br i1 %cmp3, label %while.body, label %while.end.loopexit

while.body:                                       ; preds = %while.body.preheader, %while.cond
  %written.03 = phi i32 [ %add, %while.cond ], [ 0, %while.body.preheader ]
  %arrayidx = getelementptr inbounds [256 x i8], [256 x i8]* %buffer, i32 0, i32 %written.03
  %sub = sub nsw i32 %call, %written.03
  %call4 = call i32 @write(i32 %fd, i8* nonnull %arrayidx, i32 %sub) #9
  %cmp5 = icmp sgt i32 %call4, -1
  %add = add nsw i32 %call4, %written.03
  br i1 %cmp5, label %while.cond, label %if.else8

if.else8:                                         ; preds = %while.body
  call void @__assert_fail(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.16, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 26, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @__PRETTY_FUNCTION__.fd_printf, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

while.end.loopexit:                               ; preds = %while.cond
  %add.lcssa1 = phi i32 [ %add, %while.cond ]
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %while.cond.preheader
  %written.0.lcssa = phi i32 [ 0, %while.cond.preheader ], [ %add.lcssa1, %while.end.loopexit ]
  %cmp10 = icmp eq i32 %call, %written.0.lcssa
  br i1 %cmp10, label %if.end14, label %if.else13

if.else13:                                        ; preds = %while.end
  call void @__assert_fail(i8* getelementptr inbounds ([50 x i8], [50 x i8]* @.str.26, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 29, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @__PRETTY_FUNCTION__.fd_printf, i32 0, i32 0)) #10
  br label %UnifiedUnreachableBlock

if.end14:                                         ; preds = %while.end
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %1) #9
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #9
  ret void

UnifiedUnreachableBlock:                          ; preds = %if.else13, %if.else8, %if.else
  unreachable
}

; Function Attrs: nounwind
define dso_local i32 @write_uint16_t_array(i32 %fd, i16* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 178, i8* getelementptr inbounds ([47 x i8], [47 x i8]* @__PRETTY_FUNCTION__.write_uint16_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i16, i16* %arr, i32 %i.07
  %0 = load i16, i16* %arrayidx, align 2, !tbaa !15
  %conv = zext i16 %0 to i32
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.17, i32 0, i32 0), i32 %conv)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_uint32_t_array(i32 %fd, i32* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 179, i8* getelementptr inbounds ([47 x i8], [47 x i8]* @__PRETTY_FUNCTION__.write_uint32_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i32, i32* %arr, i32 %i.07
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !16
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.17, i32 0, i32 0), i32 %0)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_uint64_t_array(i32 %fd, i64* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 180, i8* getelementptr inbounds ([47 x i8], [47 x i8]* @__PRETTY_FUNCTION__.write_uint64_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i64, i64* %arr, i32 %i.07
  %0 = load i64, i64* %arrayidx, align 4, !tbaa !17
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.18, i32 0, i32 0), i64 %0)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_int8_t_array(i32 %fd, i8* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 181, i8* getelementptr inbounds ([43 x i8], [43 x i8]* @__PRETTY_FUNCTION__.write_int8_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i8, i8* %arr, i32 %i.07
  %0 = load i8, i8* %arrayidx, align 1, !tbaa !12
  %conv = sext i8 %0 to i32
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.19, i32 0, i32 0), i32 %conv)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_int16_t_array(i32 %fd, i16* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 182, i8* getelementptr inbounds ([45 x i8], [45 x i8]* @__PRETTY_FUNCTION__.write_int16_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i16, i16* %arr, i32 %i.07
  %0 = load i16, i16* %arrayidx, align 2, !tbaa !15
  %conv = sext i16 %0 to i32
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.19, i32 0, i32 0), i32 %conv)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_int32_t_array(i32 %fd, i32* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 183, i8* getelementptr inbounds ([45 x i8], [45 x i8]* @__PRETTY_FUNCTION__.write_int32_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i32, i32* %arr, i32 %i.07
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !16
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.19, i32 0, i32 0), i32 %0)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_int64_t_array(i32 %fd, i64* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 184, i8* getelementptr inbounds ([45 x i8], [45 x i8]* @__PRETTY_FUNCTION__.write_int64_t_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds i64, i64* %arr, i32 %i.07
  %0 = load i64, i64* %arrayidx, align 4, !tbaa !17
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.20, i32 0, i32 0), i64 %0)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_float_array(i32 %fd, float* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 186, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @__PRETTY_FUNCTION__.write_float_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds float, float* %arr, i32 %i.07
  %0 = load float, float* %arrayidx, align 4, !tbaa !18
  %conv = fpext float %0 to double
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.21, i32 0, i32 0), double %conv)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_double_array(i32 %fd, double* nocapture readonly %arr, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %for.cond.preheader, label %if.else

for.cond.preheader:                               ; preds = %entry
  %cmp16 = icmp sgt i32 %n, 0
  br i1 %cmp16, label %for.body.preheader, label %for.end

for.body.preheader:                               ; preds = %for.cond.preheader
  br label %for.body

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 187, i8* getelementptr inbounds ([43 x i8], [43 x i8]* @__PRETTY_FUNCTION__.write_double_array, i32 0, i32 0)) #10
  unreachable

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.07 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %arrayidx = getelementptr inbounds double, double* %arr, i32 %i.07
  %0 = load double, double* %arrayidx, align 4, !tbaa !20
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.21, i32 0, i32 0), double %0)
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.end.loopexit, label %for.body

for.end.loopexit:                                 ; preds = %for.body
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %for.cond.preheader
  ret i32 0
}

; Function Attrs: nounwind
define dso_local i32 @write_section_header(i32 %fd) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %fd, 1
  br i1 %cmp, label %if.end, label %if.else

if.else:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i32 190, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @__PRETTY_FUNCTION__.write_section_header, i32 0, i32 0)) #10
  unreachable

if.end:                                           ; preds = %entry
  call void (i32, i8*, ...) @fd_printf(i32 %fd, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.22, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local i32 @__fxstat(i32, i32, %struct.stat*) local_unnamed_addr #8

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #9

; Function Attrs: nofree nounwind
declare dso_local i32 @vsnprintf(i8* nocapture, i32, i8* nocapture readonly, i8*) local_unnamed_addr #4

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #9

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { inlinehint nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nofree "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { argmemonly nofree nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { nounwind }
attributes #10 = { noreturn nounwind }
attributes #11 = { cold }
attributes #12 = { nounwind readonly }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 10.0.0 "}
!3 = !{!4, !9, i64 44}
!4 = !{!"stat", !5, i64 0, !8, i64 8, !9, i64 12, !10, i64 16, !10, i64 20, !10, i64 24, !10, i64 28, !5, i64 32, !8, i64 40, !9, i64 44, !9, i64 48, !9, i64 52, !11, i64 56, !11, i64 64, !11, i64 72, !9, i64 80, !9, i64 84}
!5 = !{!"long long", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = !{!"short", !6, i64 0}
!9 = !{!"long", !6, i64 0}
!10 = !{!"int", !6, i64 0}
!11 = !{!"timespec", !9, i64 0, !9, i64 4}
!12 = !{!6, !6, i64 0}
!13 = !{!14, !14, i64 0}
!14 = !{!"any pointer", !6, i64 0}
!15 = !{!8, !8, i64 0}
!16 = !{!10, !10, i64 0}
!17 = !{!5, !5, i64 0}
!18 = !{!19, !19, i64 0}
!19 = !{!"float", !6, i64 0}
!20 = !{!21, !21, i64 0}
!21 = !{!"double", !6, i64 0}

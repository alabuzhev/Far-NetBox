//---------------------------------------------------------------------------
#ifndef ScpFileSystemH
#define ScpFileSystemH

#include <FileSystems.h>
//---------------------------------------------------------------------------
class TCommandSet;
class TSecureShell;
//---------------------------------------------------------------------------
class TSCPFileSystem : public TCustomFileSystem
{
public:
    explicit TSCPFileSystem(TTerminal *ATerminal);
    virtual void __fastcall Init(TSecureShell *SecureShell);
    virtual ~TSCPFileSystem();

    virtual void __fastcall Open();
    virtual void __fastcall Close();
    virtual bool __fastcall GetActive();
    virtual void __fastcall Idle();
    virtual UnicodeString __fastcall AbsolutePath(const UnicodeString Path, bool Local);
    virtual void __fastcall AnyCommand(const UnicodeString Command,
                            const TCaptureOutputEvent *OutputEvent);
    virtual void __fastcall ChangeDirectory(const UnicodeString Directory);
    virtual void __fastcall CachedChangeDirectory(const UnicodeString Directory);
    virtual void __fastcall AnnounceFileListOperation();
    virtual void __fastcall ChangeFileProperties(const UnicodeString FileName,
                                      const TRemoteFile *File, const TRemoteProperties *Properties,
                                      TChmodSessionAction &Action);
    virtual bool __fastcall LoadFilesProperties(TStrings *FileList);
    virtual void __fastcall CalculateFilesChecksum(const UnicodeString Alg,
                                        TStrings *FileList, TStrings *Checksums,
                                        TCalculatedChecksumEvent *OnCalculatedChecksum);
    virtual void __fastcall CopyToLocal(TStrings *FilesToCopy,
                             const UnicodeString TargetDir, const TCopyParamType *CopyParam,
                             int Params, TFileOperationProgressType *OperationProgress,
                             TOnceDoneOperation &OnceDoneOperation);
    virtual void __fastcall CopyToRemote(TStrings *FilesToCopy,
                              const UnicodeString TargetDir, const TCopyParamType *CopyParam,
                              int Params, TFileOperationProgressType *OperationProgress,
                              TOnceDoneOperation &OnceDoneOperation);
    virtual void __fastcall CreateDirectory(const UnicodeString DirName);
    virtual void __fastcall CreateLink(const UnicodeString FileName, const UnicodeString PointTo, bool Symbolic);
    virtual void __fastcall DeleteFile(const UnicodeString FileName,
                            const TRemoteFile *File, int Params, TRmSessionAction &Action);
    virtual void __fastcall CustomCommandOnFile(const UnicodeString FileName,
                                     const TRemoteFile *File, const UnicodeString Command, int Params, const TCaptureOutputEvent &OutputEvent);
    virtual void __fastcall DoStartup();
    virtual void __fastcall HomeDirectory();
    virtual bool __fastcall IsCapable(int Capability) const;
    virtual void __fastcall LookupUsersGroups();
    virtual void __fastcall ReadCurrentDirectory();
    virtual void __fastcall ReadDirectory(TRemoteFileList *FileList);
    virtual void __fastcall ReadFile(const UnicodeString FileName,
                          TRemoteFile *& File);
    virtual void __fastcall ReadSymlink(TRemoteFile *SymlinkFile,
                             TRemoteFile *& File);
    virtual void __fastcall RenameFile(const UnicodeString FileName,
                            const UnicodeString NewName);
    virtual void __fastcall CopyFile(const UnicodeString FileName,
                          const UnicodeString NewName);
    virtual UnicodeString __fastcall FileUrl(const UnicodeString FileName);
    virtual TStrings * __fastcall GetFixedPaths();
    virtual void __fastcall SpaceAvailable(const UnicodeString Path,
                                TSpaceAvailable &ASpaceAvailable);
    virtual const TSessionInfo & __fastcall GetSessionInfo();
    virtual const TFileSystemInfo & __fastcall GetFileSystemInfo(bool Retrieve);
    virtual bool __fastcall TemporaryTransferFile(const UnicodeString FileName);
    virtual bool __fastcall GetStoredCredentialsTried();
    virtual UnicodeString __fastcall GetUserName();

protected:
    TStrings * __fastcall GetOutput() { return FOutput; };
    int __fastcall GetReturnCode() { return FReturnCode; }

    virtual UnicodeString __fastcall GetCurrentDirectory();

private:
    TSecureShell *FSecureShell;
    TCommandSet *FCommandSet;
    TFileSystemInfo FFileSystemInfo;
    UnicodeString FCurrentDirectory;
    TStrings *FOutput;
    int FReturnCode;
    UnicodeString FCachedDirectoryChange;
    bool FProcessingCommand;
    int FLsFullTime;
    captureoutput_signal_type FOnCaptureOutput;
    bool FUtfStrings;
    bool FUtfNever;
    TSCPFileSystem *Self;

    void __fastcall ClearAliases();
    void __fastcall ClearAlias(const UnicodeString Alias);
    void __fastcall CustomReadFile(const UnicodeString FileName,
                        TRemoteFile *& File, TRemoteFile *ALinkedByFile);
    static UnicodeString __fastcall DelimitStr(const UnicodeString Str);
    void __fastcall DetectReturnVar();
    bool __fastcall IsLastLine(UnicodeString &Line);
    static bool __fastcall IsTotalListingLine(const UnicodeString Line);
    void __fastcall EnsureLocation();
    void __fastcall ExecCommand(const UnicodeString Cmd, int Params,
                     const UnicodeString CmdString);
    void __fastcall ExecCommand(TFSCommand Cmd, int Params = -1, ...);
    void __fastcall ReadCommandOutput(int Params, const UnicodeString *Cmd = NULL);
    void __fastcall SCPResponse(bool *GotLastLine = NULL);
    void __fastcall SCPDirectorySource(const UnicodeString DirectoryName,
                            const UnicodeString TargetDir, const TCopyParamType *CopyParam, int Params,
                            TFileOperationProgressType *OperationProgress, int Level);
    void __fastcall SCPError(const UnicodeString Message, bool Fatal);
    void __fastcall SCPSendError(const UnicodeString Message, bool Fatal);
    void __fastcall SCPSink(const UnicodeString TargetDir,
                 const UnicodeString FileName, const UnicodeString SourceDir,
                 const TCopyParamType *CopyParam, bool &Success,
                 TFileOperationProgressType *OperationProgress, int Params, int Level);
    void __fastcall SCPSource(const UnicodeString FileName,
                   const UnicodeString TargetDir, const TCopyParamType *CopyParam, int Params,
                   TFileOperationProgressType *OperationProgress, int Level);
    void __fastcall SendCommand(const UnicodeString Cmd);
    void __fastcall SkipFirstLine();
    void __fastcall SkipStartupMessage();
    void __fastcall UnsetNationalVars();
    TRemoteFile * __fastcall CreateRemoteFile(const UnicodeString ListingStr,
                                  TRemoteFile *LinkedByFile = NULL);
    void CaptureOutput(const UnicodeString AddedLine, bool StdError);
    void __fastcall ChangeFileToken(const UnicodeString DelimitedName,
                         const TRemoteToken &Token, TFSCommand Cmd, const UnicodeString RecursiveStr);

    static bool __fastcall RemoveLastLine(UnicodeString &Line,
                               int &ReturnCode, UnicodeString LastLine = L"");
private:
    TSCPFileSystem(const TSCPFileSystem &);
    void operator=(const TSCPFileSystem &);
};
//---------------------------------------------------------------------------
#endif // ScpFileSystemH

; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#include 'include\sapi\sapi.iss'
#define MyAppName "快眼看图王"
#define MyEnAppName "ImageView"
#define MyAppVersion "1.0.2.5"
#define PacketID  	  1024 ;包ID

#define MyAppPublisher "极客工作室."
#define MyAppURL "http://www.kantuwang.wang"
#define MyAppExeName "ImageView.exe"
#define MyDefOper    "0"          ;默认渠道ID


#define ProID		5;产品ID
#define ApiHost "http://update.jicer.cn"
#define InstallUri  "install"
#define UninstallUri "uninstall"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{A95B3BFD-ED8E-4C36-A14E-D853E03F17BA}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
VersionInfoVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}

UsePreviousAppDir=yes
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}

;LicenseFile=License_cn.txt
OutputDir=bin
OutputBaseFilename={#MyEnAppName}_{#MyDefOper}_{#MyAppVersion}
SetupIconFile=Logo.ico
UninstallDisplayIcon   ={app}\{#MyAppExeName}
Compression=lzma
SolidCompression=yes

DisableWelcomePage=yes
DisableFinishedPage=yes

DisableReadyPage=yes
DisableStartupPrompt=yes
DisableReadyMemo=yes
DisableDirPage=no
DisableProgramGroupPage=yes
PrivilegesRequired=admin


[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Languages\ChineseSimp.isl"



[Files]
Source: "..\bin\ImageView.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\icon\*"; DestDir: "{app}\icon"; Flags: ignoreversion recursesubdirs
;Source: "..\bin\skinScreen\*"; DestDir: "{app}\skinScreen"; Flags: ignoreversion recursesubdirs
Source: "..\bin\skin\imageview.zip"; DestDir: "{app}\skin"; Flags: ignoreversion
Source: "..\bin\cximagecrtu.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\directui license.txt"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\bin\duilib license.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\soui license.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\FreeImage.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\bin\imScreen.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\msvcp100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\msvcr100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\cximagecrtu.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\stdlog.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\syslay.dll"; DestDir: "{app}"; Flags: ignoreversion
[Icons]
Name: "{group}\卸载"; Filename: "{app}\unins000.exe"
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

Name: "{userdesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: runascurrentuser nowait postinstall
[InstallDelete]
Name: {app}; Type: filesandordirs

[UninstallDelete]
Name: {app}; Type: filesandordirs


[Registry]
Root: HKCR; Subkey: {#MyAppName}; ValueData: {#MyAppName}; ValueType: string; ValueName: ; Flags: uninsdeletekey
Root: HKCR; Subkey: {#MyAppName}\DefaultIcon; ValueData: {app}\icon\iso.ico; ValueType: string; ValueName: ; Flags: uninsdeletevalue
Root: HKCR; Subkey: {#MyAppName}\shell\open\command; ValueData: """{app}\{#MyAppExeName}"" ""%1"""; ValueType: string; ValueName: ; Flags: uninsdeletevalue

Root:HKCR; Subkey: Directory\shell\ImageView ; ValueName:; ValueData:"快眼看图王-穿透文件夹";  ValueType: string;Flags: uninsdeletekey 
Root:HKCR; Subkey: Directory\shell\ImageView\command ; ValueName:; ValueData:"""{app}\{#MyAppExeName}"" ""%1""";  ValueType: string;Flags: uninsdeletekey 
Root:HKCR; Subkey: Directory\shell\ImageView ; ValueName:Icon; ValueData:{app}\{#MyAppExeName};  ValueType: string;Flags: uninsdeletekey 

[code]
var oper:String;

procedure SetDefaultOpen();
begin
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.BMP', '{#MyAppName}', ' ' , ExpandConstant('{app}\icon\BMP.ico'));
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.WBMP', '{#MyAppName}', ' ',ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.JPG', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\JPG.ico'));
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.JPEG', '{#MyAppName}', ' ',ExpandConstant('{app}\icon\JPG.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.JPE', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\JPG.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.PNG', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\PNG.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.JP2', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\JPG.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.PSD', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\PSD.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.GIF', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\GIF.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.DDS', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.PCX', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.XPM', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.TGA', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.XBM', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.TIF', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\TIF.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.PNM', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.SKA', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.WEBP', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.CUT', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.ICO', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.HDR', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.JNG', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.ARW', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.TIFF', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.KOA', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.MNG', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.PCD', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.RAS', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.J2K', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.JPC', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.IFF', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.G3', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));		
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.SGI', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.EXR', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.WDP', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.PFM', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.PCT', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
	SetDefProgram('{#MyAppName}',ExpandConstant('{app}\{#MyAppExeName}'), '.RAW', '{#MyAppName}', ' ', ExpandConstant('{app}\icon\COMMON.ico'));	
end;

procedure SetOper();
var
  strArray: TArrayOfString;
  strFileName : String;
  strExeName:String;
begin
    oper:= '{#MyDefOper}';
	strFileName := '{#MyAppName}' + '{#MyAppVersion}';
	strExeName := ExtractFileName(ParamStr(0));
	if strFileName = strExeName then
	begin
		oper := '360';
	end
	else
	begin	
		Explode(strArray, ExtractFileName(ParamStr(0)), '_');
		if GetArrayLength(strArray) > 2 then
		begin
		oper:= strArray[1];
		end;
	end;
    SetIniString('setting','oper',oper,ExpandConstant('{app}/setting.ini'));
end;

procedure ReportInstall();
var 
    sUserGuid : String;
	strUrl:String;
begin
   sUserGuid := GetUserGuid();
   strUrl:= Format('%s/%s?uguid=%s&prod=%s&packet=%d&prodver=%s&oper=%s&wow64=%d&osver=%d&ant=%s', \
   ['{#ApiHost}','{#InstallUri}', sUserGuid, '{#ProID}', {#PacketID}, '{#MyAppVersion}', oper,IsWindowsX64(), GetMicroVersion(),GetAnt()]);
  
   log(strUrl);
   HttpGET( strUrl );
end;


procedure ReportUnInstall();
var 
    sUserGuid : String;
	strUrl:String;

begin
	
	log('Uninstall Oper' + oper);
   sUserGuid := GetUserGuid();
   strUrl:= Format('%s/%s?uguid=%s&prod=%s&packet=%d&prodver=%s&oper=%s&wow64=%d&osver=%d&ant=%s', \
   ['{#ApiHost}','{#UninstallUri}', sUserGuid, '{#ProID}', {#PacketID}, '{#MyAppVersion}', oper,IsWindowsX64(), GetMicroVersion(),GetAnt()]);
  
   log(strUrl);
   HttpGET( strUrl );
end;

procedure OnInstallFinesh();
begin
	SetIniString('setting','version','{#MyAppVersion}',ExpandConstant('{app}/setting.ini'));
	SetOper();
	SetDefaultOpen();
	ReportInstall();
end;

procedure CurStepChanged (CurStep: TSetupStep ) ;

begin
  if CurStep = ssInstall then
  begin
     TerminateProcessAsName('{#MyAppExeName}');
  end;
  if CurStep = ssDone  then
  begin
      OnInstallFinesh();
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var winHwnd:  HWND;
begin
	if CurUninstallStep = usUninstall then
	begin
		TerminateProcessAsName('{#MyAppExeName}');
		DelTree(ExpandConstant('{app}'), True, True, True);
	end;
	if CurUninstallStep = usUninstall then
	begin
		ReportUnInstall();
	end;
end;

procedure InitializeWizard();
begin
	
end;

function InitializeUninstall(): boolean;
var
  url:string;
begin
  result:=true;
  FileCopy(ExpandConstant('{app}\sapi.dll'),ExpandConstant('{tmp}\sapi.dll'),False);
  oper := GetIniString('setting','oper','0',ExpandConstant('{app}\setting.ini'));
end;

function InitializeSetup(): boolean;
begin
   result:=true;
	ExtractTemporaryFile('sapi.dll');
end;





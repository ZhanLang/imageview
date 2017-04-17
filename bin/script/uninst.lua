
local searchPath = tostring(winapi.GetModuleFilePath(winapi.GetModuleHandle("lua-5.1.4.dll")));

package.path = package.path .. ";" .. searchPath .. "\\script\\?.lua"
package.cpath = package.cpath .. ";" .. searchPath .. "\\script\\?.dll"

require "common"

local function GetStrDate( _strTime)
	local ta  = LuaSplit(_strTime,' ');
	local size = table.getn(ta);
	if( size == 0 ) then
		return "";
	end
	
	
	local t = LuaReomveChar(ta[1], '-');
	print(t);
	return t;
end

local function GetStrTime( _strTime )
	local ta  = LuaSplit(_strTime,' ');
	local size = table.getn(ta);
	local t = LuaReomveChar(ta[2], ':');
	return t;
end

local function booltoint(_boolen)
	if _boolen == true then
		return 1;
	else
		return 0;
	end
end



_g_operid = 0;
_g_prodname="";
_g_idata = "";	--����İ�װ����
_g_itime = "";	--����İ�װʱ��


--lua ����C++�ӿ�
_g_LuaVM=nil; 		--ȫ��CLuaVM����
function main(_luaVm) --�ڽű���ʼ����ʱ�����
	_g_LuaVM = LuaVM.ConvertToLuaVM(_luaVm);
	
	_g_prodname = tostring(_g_LuaVM.GetEnvParamString("productname"));	--��ƷӢ������
	_g_operid = msapp.CApp(_g_prodname).GetSetting("oper", 0);
	
	_g_idata = msapp.CApp(_g_prodname).GetSetting("idata", tostring(mstime.GetCurrentDateStr(false)));
	_g_itime = msapp.CApp(_g_prodname).GetSetting("idata", tostring(mstime.GetCurrentTimeStr(false)));
	
end

--��ȡ��װ·��
local function GetProgramPosition()
	local path = _g_LuaVM.LuaCallFunction("Setup", "GetProgramPosition", "");
	local json = cjson.decode(tostring(path));
	return tostring(json["path"]);
end

function LuaGetSilent()
	local path = _g_LuaVM.LuaCallFunction("Setup", "GetSilent", "");
	local json = cjson.decode(tostring(path));
	return json["silent"];
end

local function DeleteProgramsShrotcut()
	local prodName = tostring(_g_LuaVM.GetEnvParamString("friendprodname"));
	local DeskTopPos = string.format("%s\\%s\\", tostring(quicklink.GetStartMenuPos()), prodName);
	file.DeletePath(DeskTopPos,true,false,true);
	return true;
end

local function DeleteDisktopShrotcut()
	local prodName = tostring(_g_LuaVM.GetEnvParamString("friendprodname"));
	local DeskTopPos = string.format("%s\\%s.lnk", tostring(quicklink.GetDesktopPos()), prodName);
	print(DeskTopPos);
	file.DeletePath(DeskTopPos,true,false,true);
end

local function DeleteTaskbarShrotcut()
	local prodName = tostring(_g_LuaVM.GetEnvParamString("friendprodname"));
	local DeskTopPos = string.format("\\%s.lnk", prodName);
	quicklink.DeleteTaskBar(DeskTopPos);
	return true;
end

local function DeleteUninstallReg()
	local regPath = nil
	if(winapi.IsWindowsX64()) then
		regPath =  "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
	else
		regPath =  "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
	end
	
	local regObj = winfunc.WinRegKey();
	
	local bRet = regObj.Open("HKEY_LOCAL_MACHINE", regPath , "KEY_READ_WRITE");
	if not bRet then
		return false;
	end
	
	regObj.DeleteSubKey(tostring(_g_LuaVM.GetEnvParamString("productname")));
end

local function TerminateProductProcess()
	local procName = string.format("%s.exe", tostring(_g_LuaVM.GetEnvParamString("productname")));
	winapi.TerminateProcessAsName(procName);
	return true;
end

local function Report()
  local _host = "update.kyan.com.cn";
  local _path = "uninstall";
  local _param = 
    {
      ["apiver"]      = 1;--�ӿڰ汾��
      ["uguid"]       = winapi.GetUserGuid(),
	  ["mainbord"]	  = winapi.GetMainBoardSerialNumber(),
	  ["hdd"]	  	  = winapi.GetDiskSerialNumber(),
	  ["mac"]		  = winapi.GetNetAdapter(),
      ["prod"]        = _g_LuaVM.GetEnvParamString("prouctid"),
	  ["packet"]      = _g_LuaVM.GetEnvParamString("packetid"),
      ["prodver"]     = _g_LuaVM.GetEnvParamString("version"),
	  ["idate"]		  = _g_idata,
      ["itime"]		  = _g_itime,
      ["oper"]        = _g_operid,
      ["wow64"]       = winapi.IsWindowsX64(),
      ["osver"]          = winapi.GetMicroVersion(),
	  ["vm"]          = booltoint(osinfo.IsVM()),
	  ["type"]        = booltoint(LuaGetSilent()),
    };
   
  local _retcode , _context  = HttpGet(_host, 80, _path, _param);
  return _retcode;
end



function Do( param )

	log(tostring(GetProgramPosition()));
	local result = {};
	result["result"] = 0;
	
	
	
	--��������
	winapi.TerminateProcessAsName("ImageView.exe");
	
	--ɾ���ļ���������Ŀ¼
	local AppPath = msapp.CApp(_g_prodname).GetDataPath();
	file.DeletePathEx(tostring(AppPath) .. "fscache\\" , true, false, true);
	
	DeleteProgramsShrotcut();
	DeleteDisktopShrotcut();
	DeleteTaskbarShrotcut();
	DeleteUninstallReg();
	Report();
	
	msapi.RegDeleteKey(msapi.HCR, "Directory\\shell\\ImageView");
	--ɾ�������ļ�
	local settingFile = tostring(msapp.CApp((_g_prodname)).GetSettingFile());
	file.DeleteFileForce(settingFile);
	
	result["result"] = 1; --��װ�ɹ�
	return cjson.encode(result);
end






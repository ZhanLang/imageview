package.path = package.path .. ";" .. lfs.currentdir() .. "\\script\\?.lua"
package.cpath = package.cpath .. ";" .. lfs.currentdir() .. "\\script\\?.dll"

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
--lua ����C++�ӿ�
_g_LuaVM=nil; 		--ȫ��CLuaVM����

_g_operid = 0;
_g_prodname="";


function main(_luaVm) --�ڽű���ʼ����ʱ�����
	_g_LuaVM = LuaVM.ConvertToLuaVM(_luaVm);
	
	_g_prodname = tostring(_g_LuaVM.GetEnvParamString("productname"));	--��ƷӢ������
	_g_operid = msapi.CApp(_g_prodname).GetSetting("oper", 0);
	
	
end	

local function Report()
  local _host = "update.kyan.com.cn";
  local _path = "active";
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
	  ["idate"]		  = GetStrDate(GetCurrentTimeStr());
      ["itime"]		  = GetStrTime(GetCurrentTimeStr());
      ["oper"]        = _g_operid,
      ["wow64"]       = winapi.IsWindowsX64(),
      ["os"]          = winapi.GetMicroVersion(),
	  ["vm"]          = booltoint(osinfo.IsVM()),
    };
   
  local _retcode , _context  = HttpGet(_host, 80, _path, _param);
  return _retcode;
end

function DoThread()
	Report();
	
	return 0;
end
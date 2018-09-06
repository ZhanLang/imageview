#pragma once

#include <util/str_encode_embed.h>
#include <AppDefiner.h>

#define _Def_Version	L"1.0.1.7"
#define _Def_Packet		1017

//update.jicer.cn
#define _Def_Prod		AppProd_imaveview

#define nDefNum 12
#define UPDATE_URL str_encode_embed<nDefNum,'u','p','d','a','t','e','.','j','i','c','e','r','.','c','n'>().c_str()

#define _Def_Oper		0
//api.jicer.cn
#define _Def_ApiHost	str_encode_embed<nDefNum,'a','p','i','.','j','i','c','e','r','.','c','n'>()

//imageview/update
#define _Def_Update_NetApi str_encode_embed<nDefNum,'i','m','a','g','e','v','i','e','w','/','u','p','d','a','t','e'>()

#define _Def_AesKey str_encode_embed<nDefNum,'a','s','d','f','w','e','t','y','h','j','u','y','t','r','f','d'>()
#define _Def_AesVi  str_encode_embed<nDefNum,'g','f','d','e','r','t','f','g','h','j','k','u','y','r','t','g'>()
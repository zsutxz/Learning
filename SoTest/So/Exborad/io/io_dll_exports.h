/********************************************************************
	created:	2014/05/19
	file base:	io_dll_exports
	file ext:	h
	author:		zz
	
	purpose:	
*********************************************************************/
#ifndef INCLUDED_IO_DLL_EXPORTS_H
#define INCLUDED_IO_DLL_EXPORTS_H

// #include "..\common\dll_define.h"
#define _EXPORT   _declspec(dllexport)


#ifdef __cplusplus
extern "C" {
#endif

	int		_EXPORT		ExboardMgr_InitEx();
	int		_EXPORT		ExboardMgr_UpdateEx();
	unsigned char 		_EXPORT	ExboardMgr_GetCoinInEx();
	int		_EXPORT		ExboardMgr_IsConnectedEx();
	int		_EXPORT		ExboardMgr_GetVersionEx();

	void	_EXPORT		RelateKeyAndKeyidLowEx(int key,int keyid);
	void	_EXPORT		RelateKeyboardKeyAndKeyidEx();
	void	_EXPORT		Key_KeyScanEx();
	int		_EXPORT		Key_IsKeyDownEx(int keyid);
	int		_EXPORT		Key_IsKeyOnEx(int keyid);
	int		_EXPORT		Key_IsKeyPressEx(int keyid);
	int		_EXPORT		Key_IsKeyLongPressEx(int keyid);
	void	_EXPORT		Key_ClearKeyEx(int keyid);
	void	_EXPORT		Key_ClearAllKeyEx();
	void	_EXPORT		Key_CardOutEx(unsigned int cardnum);
	unsigned int		_EXPORT		Key_GetCardOutEx();
	void	_EXPORT		Key_ClearCardOutEx();

	void	_EXPORT		Light_ExboardExcuteEx(unsigned char idnum,unsigned char type,unsigned short data,unsigned short speed);


#ifdef __cplusplus
};
#endif

#endif


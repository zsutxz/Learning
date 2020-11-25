/********************************************************************
	created:	2014/05/19
	file base:	sram_dll_exports
	file ext:	h
	author:		ÖÓÕ÷
	
	purpose:	
*********************************************************************/

#ifndef INCLUDED_SRAM_DLL_EXPORTS_H
#define INCLUDED_SRAM_DLL_EXPORTS_H

//#include "dll_define.h"

#define _EXPORT   _declspec(dllexport)


#ifdef __cplusplus
extern "C" {
#endif

	// ----------------Sram---------------

	int				_EXPORT 		SramInitEx		(int max_id,int length[][3]);
	unsigned int	_EXPORT			SramSafeReadEx	(unsigned int id,int len);
	unsigned int	_EXPORT			SramSafeWriteEx	(unsigned int id,unsigned char op,int len,unsigned int data);
	void			_EXPORT			SramSafeChangeAfrterServalSecEx();
	void			_EXPORT			SramSafeClearEx	();
	void			_EXPORT			SramCheckEx		();
	void			_EXPORT			SramSafeFlgEx	();
	int				_EXPORT			SramDetectEx	();
	void			_EXPORT			SramTestEx		();
	void			_EXPORT			SramCheckFixRecordEx();

	void			_EXPORT			SramExitEx		();

	// ----------------Spi--------------

	void			_EXPORT			Sl05InitEx		(int max_id,int length[][3]);
	unsigned int	_EXPORT			Sl05SafeReadEx	(int id,int len);
	unsigned int	_EXPORT			Sl05SafeWriteEx	(unsigned int id,unsigned char op,int len,unsigned int data);
	void			_EXPORT			Sl05SafeChangeEx();
	void			_EXPORT			SL05SafeClearEx	();
	void			_EXPORT			Sl05CheckEx		();
	int				_EXPORT			Sl05CheckProductVersionEx(unsigned int product,char version[],int flg);
	void			_EXPORT			Sl05TestEx		();
	void			_EXPORT			Sl05ExitEx		();


#ifdef __cplusplus
};
#endif

#endif

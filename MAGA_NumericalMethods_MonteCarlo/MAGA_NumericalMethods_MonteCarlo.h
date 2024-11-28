
// MAGA_NumericalMethods_MonteCarlo.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMAGANumericalMethodsMonteCarloApp:
// Сведения о реализации этого класса: MAGA_NumericalMethods_MonteCarlo.cpp
//

class CMAGANumericalMethodsMonteCarloApp : public CWinApp
{
public:
	CMAGANumericalMethodsMonteCarloApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMAGANumericalMethodsMonteCarloApp theApp;

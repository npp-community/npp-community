/* This file was originally hacked into winmain.cpp to run the tests
   Hopefully it'll be converted to whatever unit testing framework is in place, at some point
*/

void testPathRemoveFileSpec(TCHAR* charPath, generic_string strPath)
{
	PathRemoveFileSpec(strPath);
	PathRemoveFileSpec(charPath);
	assert(strPath == charPath);
}

void testPathFunctions()
{
	generic_string strPath;
	TCHAR charPath[MAX_PATH];

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\bar\\test.php"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);


	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\\\test"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);


	_tcscpy_s(charPath, MAX_PATH, _T("C:"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\test\\foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\foo\\"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("foo.exe"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("f"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://foo//test.exe"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("//foo//test.exe"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);


	_tcscpy_s(charPath, MAX_PATH, _T("C://foo//test"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://foo//"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T(""));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

}
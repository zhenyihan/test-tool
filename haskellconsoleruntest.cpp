/*
 * This source code was published under GPL v3
 *
 * Copyright (C) 2016 Too-Naive E-mail:sometimes.naive@hotmail.com
 *
 */

#include <dirent.h>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>

/*
bool stringchecksub(char const * str,char const *sub){
	size_t slen=strlen(str);
	size_t sublen=strlen(sub);
	if (slen<sublen) return false;
	if (!strcmp(str+slen-sublen,sub)) return true;
	return false;
}*/

bool stringchecksub(char const * str){
	size_t slen=strlen(str);
	if (slen<4) return false;
	if (!strcmp(str+slen-4,".exe")) return true;
	return false;
}
struct _{
	int w;
	_(){this->w=0;}
	_ & operator = (int n){
		if (!this->w) this->w=n;
		return *this;
	}
} nerror;

inline bool checkisnotfolder(TCHAR const *_in){
	return (*_in==_T('.') && ((*(_in+1)==_T('.')) || (*(_in+1)==_T('\0'))))?true:false;
}

char buffer[1000];

void printfile(char const *n){
	FILE *fp=fopen(n,"r");
	if (!fp) printf("Error in fopen()!\n"),abort();
	size_t readbyte=0;
	while ((readbyte=fread(buffer,sizeof(char),1000,fp)))
		fwrite(buffer,sizeof(char),readbyte,stdout);
	fclose(fp);
}


void GetAndPrintAllFile(TCHAR const * path){
	TCHAR str[512]=_T(""),tmp[512]=_T("");
	_stprintf(tmp,_T("%s\\*"),path);
//	_tprintf(_T("Now path is: %s \n"),path);
	{
		WIN32_FIND_DATA wfd={0};
		HANDLE hdFile=FindFirstFile(tmp,&wfd);
		if (hdFile==INVALID_HANDLE_VALUE && GetLastError()!=ERROR_FILE_NOT_FOUND)
			_tprintf(TEXT("FindFirstFile() Error!(%ld)\n"),GetLastError()),
		MessageBox(NULL,TEXT("FindFirstFile() Error!"),TEXT("Error!"),MB_ICONSTOP|MB_SETFOREGROUND),abort();
//		if (GetLastError()==ERROR_FILE_NOT_FOUND) goto closef;
		do {
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
//			printf("%s\n",wfd.cFileName);
			if (checkisnotfolder(wfd.cFileName)) continue;
//			GetCurrentDirectory(1000,tmp);
			_stprintf(str,_T("%s\\%s"),path,wfd.cFileName);
			GetAndPrintAllFile(str);
		} while (FindNextFile(hdFile,&wfd));
		FindClose(hdFile);
	}
	SetCurrentDirectory(path);
	DIR * currentdir=opendir(".");
	dirent *nex=NULL;
	if (!currentdir) _tprintf(("opendir() Error!\n"));
	while ((nex=readdir(currentdir))!=NULL){
//		_tprintf(_T("Current File Name:%s\n"),nex->d_name);
		if (!stringchecksub(nex->d_name)) continue;
//		strlwr(nex->d_name);
//		if (!strstr("haskell-console",nex->d_name)) continue;
#ifndef ONLINERUN
		_tprintf(_T("%s\\%s\n"),path,nex->d_name);
#else
		_stprintf(tmp,_T("%s\\%s > out.tmp"),path,nex->d_name);
		_tprintf(_T("%s(%d)\n"),tmp,(nerror=system(tmp)));
		printfile("out.tmp");
#endif
	}
	closedir(currentdir);
	return ;
}

TCHAR a[122];

int _tmain(int argc, TCHAR const *argv[]){
//	printf("%d %d",checkexe("exe"),checkexe("aisndf.exe"));
//	GetAndPrintAllFile(_T("E:\\xdfj"));
	if (argc==1)	GetCurrentDirectory(122,a);
//	_tprintf(_T("Current Dircetory:%s\n"),a);
//	if (argc!=1) _tfreopen(_T(argv[1]),_T("w"),stdout);
	GetAndPrintAllFile(argc==1?a:argv[1]);
	return nerror.w;
}


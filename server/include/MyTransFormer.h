#ifndef __MYTRANSFORMER_H__
#define __MYTRANSFORMER_H__
#include <string.h>
#include <iconv.h>
#include <iostream>
class MyTransFormer 
{
	private:
		iconv_t cd;
	public:
		//  构造
		MyTransFormer(const char *from_charset,const char *to_charset) {
			cd = iconv_open(to_charset,from_charset);
		}
		// // 析构
		~MyTransFormer() {
			iconv_close(cd);
		}

		// 转换输出
		int convert(char *inbuf,int inlen,char *outbuf,int outlen) {
			char **pin = &inbuf;
			char **pout = &outbuf;

			memset(outbuf,0,outlen);
			return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
		}
};
#endif
